/**************************************************************************
**  Copyright 2012-2015 atronah.
**
**  This file is part of the tableManagedParser program.
**
**  tableManagedParser is free software: you can redistribute it and/or modify
**  it under the terms of the GNU Lesser General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  tableManagedParser is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU Lesser General Public License for more details.
**
**  You should have received a copy of the GNU Lesser General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
**  (Это свободная программа: вы можете перераспространять ее и/или изменять
**  ее на условиях GNU Lesser General Public License в том виде, в каком
**  она была опубликована Фондом свободного программного обеспечения; либо
**  версии 3 лицензии, либо (по вашему выбору) любой более поздней версии.
**
**  Эта программа распространяется в надежде, что она будет полезной,
**  но БЕЗО ВСЯКИХ ГАРАНТИЙ; даже без неявной гарантии ТОВАРНОГО ВИДА
**  или ПРИГОДНОСТИ ДЛЯ ОПРЕДЕЛЕННЫХ ЦЕЛЕЙ. Подробнее см. в GNU Lesser General Public License.
**
**  Вы должны были получить копию GNU Lesser General Public License
**  вместе с этой программой. Если это не так, см.
**  <http://www.gnu.org/licenses/>.)
**************************************************************************/
/*
  File   : CodeLexer.cpp
  Created: 20.01.2012
  Reason : Курсовой проект по дисциплине Лингвистическое и программное обеспечение
           автоматизированных систем.
  Product: tableManagedParser, Таблично-управляемый парсер входных текстов.
  Author : Ануфриев Артем, студент 4884 группы.



  Purpose:
      Лексического анализатор для программного кода
*/

#include "CodeLexer.h"

#ifndef CHECK_LEXER_READ_STATUS
#define CHECK_LEXER_READ_STATUS(stream, retValue) \
    if(stream.status() != QTextStream::Ok){\
        m_token = retValue;\
        return retValue;\
    }
#endif

#ifndef CHECK_FOR_END_STREAM
#define CHECK_FOR_END_STREAM(stream, retValue) \
    if (stream.atEnd()){\
        m_token = retValue;\
        return retValue; \
    }
#endif

//! Создает экземпляр лексического анализатора
/*!
    \param strm ссылка на входной поток, из которого будут считываться лексемы.
  */
CodeLexer::CodeLexer(QTextStream &strm)
    : Lexer(strm), m_currentLine(1), m_pos(0)
{
    m_token = Token("startStream", 0, 0, 0, true);
}


//! Считывает новую лексему из потока, преобразует ее в токен и возвращает этот токен
/*!
    Анализирует символы входного потока.

    Если поток содержит последовательность цифр (возможно разделенных точкой),
    то формируется токен числа с псевдонимом \c "num", в поле \a value которого
    значение числа.

    Если поток содержит последовательность букв и цифр, начинающуюся с буквы, то
    формируется токен идентификатора. Далее проверяется наличие имени
    идентификатора (лексемы) в списке зарезервированных слов, если оно там
    имеется, то псевдонимом токена является сама лексема, иначе \c "id",
    хранящий в поле \a value имя идентификатора (лексему).

    \return сформированный токен.
  */
Token CodeLexer::nextToken(){    
    Token errToken("error", "error", m_currentLine, m_pos, true);
    Token endStreamToken("endStream", "endStream", m_currentLine, m_pos, true);

    QChar c;
    if(m_buffer.isEmpty()){
        CHECK_FOR_END_STREAM(m_strm, endStreamToken);
        m_strm >> c;
        CHECK_LEXER_READ_STATUS(m_strm, errToken);
    }else{
        c = m_buffer.pop();
    }

    // Проверка на ошибки чтения


    if(c == '\r' || c == '\n'){
        ++m_currentLine;
        m_pos = 0;
    }

    while(ignoredChars.contains(c)){
        if(m_buffer.isEmpty()){
            CHECK_FOR_END_STREAM(m_strm, endStreamToken);
            m_strm >> c;
            CHECK_LEXER_READ_STATUS(m_strm, errToken);
        }else{
            c = m_buffer.pop();
        }
        CHECK_LEXER_READ_STATUS(m_strm, errToken);
        ++m_pos;
    }


    // Если считанный символ - цифра, то делается предположение, что это число
    if (c.isDigit()){
        QString numberStr;
        bool isDotFind = false;     //< флаг обнаружения точки в числе
        //Заполнение строки последовательностью цифр с одной точкой из потока
        while(c.isDigit()){
            numberStr.append(c);
            if(m_strm.atEnd()){
                break;
            }
            if(m_buffer.isEmpty()){
                m_strm >> c;
                CHECK_LEXER_READ_STATUS(m_strm, errToken);
            }else{
                c = m_buffer.pop();
            }


            // Если считанный символ - точка и до этого точек считано не было
            if(c == '.' && !isDotFind){
                // обработка точки отдельно, чтобы цикл мог дальше считывать цифры
                isDotFind = true;
                numberStr.append(c);
                if(m_buffer.isEmpty()){
                    CHECK_FOR_END_STREAM(m_strm, endStreamToken);
                    m_strm >> c;
                    CHECK_LEXER_READ_STATUS(m_strm, errToken);
                }else{
                    c = m_buffer.pop();
                }

            }
        }
        // Возвращает обратно в поток последний считанный символ (не цифру)
        if(!c.isDigit()){
            m_buffer.push(c);
        }


        // Если точка была обнаружена
        if(isDotFind){
            // то привести число к вещественному типу
            m_token = Token("num", numberStr.toDouble(), m_currentLine, m_pos);
        }else{
            // иначе, привести число к целму типу
            m_token = Token("num", numberStr.toInt(), m_currentLine, m_pos);
        }

        // сместить позицию на количество считанных/обработанных символов
        m_pos += numberStr.length();

    //Если первый символ не цифра, а буквы, то предположить идентификатор
    }else if (c.isLetter()){
        QString id;
        // Заполнение лексемы всеми подряд идущими цифро-буквенными символами
        while(c.isLetterOrNumber()){
            id.append(c);
            if (m_strm.atEnd()){
                break;
            }
            if(m_buffer.isEmpty()){
                m_strm >> c;
                CHECK_LEXER_READ_STATUS(m_strm, errToken);
            }else{
                c = m_buffer.pop();
            }
        }
        // Вернуть в исходный поток последний символ, остановивший цикл
        if (!c.isLetterOrNumber()){
            m_buffer.push(c);
        }


        // Если данный идентификатор имеется в списке зарезервированных слов
        if (reservedWords.contains(id)){
            // то использовать в качестве псевдонима токена саму лексему
            m_token = Token(id, id, m_currentLine, m_pos);
        }else{
            // иначе псевдоним "id"
            m_token = Token("id", id, m_currentLine, m_pos);
        }

        m_pos += id.length();
    }else{
        m_token = Token(c, c, m_currentLine, m_pos);
        ++m_pos;
    }

    return token();
}

