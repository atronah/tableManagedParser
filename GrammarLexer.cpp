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
  File   : GrammarLexer.cpp
  Created: 20.01.2012
  Reason : Курсовой проект по дисциплине Лингвистическое и программное обеспечение
           автоматизированных систем.
  Product: tableManagedParser, Таблично-управляемый парсер входных текстов.
  Author : Ануфриев Артем, студент 4884 группы.



  Purpose:
      Лексического анализатор для файла с описанием КС-грамматики
*/

#include "GrammarLexer.h"

#include <QRegExp>
#include <QString>

const QString GrammarLexer::deffLexem = "::=";
const SurroundChars GrammarLexer::surroundChars = SurroundChars('<', '>');

//! Создает лексический анализатор для указанного потока \a strm
GrammarLexer::GrammarLexer(QTextStream &strm)
    : Lexer(strm)
{
    ignoredChars.remove('\n');
    ignoredChars.remove('\r');
    m_currentLine = 0;

    m_token = Token("startStream", 0, 0, 0, true);
}


//! Считывает новую лексему из потока преобразует в токен и возвращает этот токен
/*!
    Анализирует символы входного потока построчно.
    В каждой строке пытается найти последовательность \b NsM, где
    \b N - нетерминальный символ, окруженный символами из surroundChars (угловыми
    скобками в системе БНФ);
    \b s - символ определения порождения (\a "::=" в системе БНФ)
    \b M - множество терминальных, нетерминальных символов и символа "|".

    Если строка входного потока не начинается с пары \b Ns, то возвращает токен
    ошибки (\a alias = \c "error", \a isSystemToken = \c true.

    Каждая строка преобразуется в очередь токенов, откуда и происходит считывание
    очередного токена до тех пор, пока очередь не опустеет, после чего считывается
    новая строка и формируется новая очередь.

    \return сформированный токен.
  */
Token GrammarLexer::nextToken(){

    if(m_tokenBuffers.isEmpty()){
        if(m_strm.atEnd()){
            m_token = Token("endStream", 0, m_currentLine + 1, -1, true);
            return token();
        }

        QString line = m_strm.readLine();
        ++m_currentLine;

        //Удаление из строки всех символов из списка игнорируемых
        foreach(QChar c, ignoredChars){
            QRegExp forDel(QString::fromUtf8("(%1*)").arg(c));
            line.remove(forDel);
        }

        QRegExp finder;
        int delPartLen = 0;  // длина удаленной части строки
        // Регулярное выражение для символа определения (в БНФ это '::=')
        const QString deffSymbRegExp(QString("(%1)").arg(deffLexem));
        // Регулярное выражение для нетерминала (в БНФ это <Имя>)
        const QString nonTermRegExp = QString::fromUtf8("%1([a-zA-Zа-яА-Я]\\w*)%2")
                                      .arg(surroundChars.left)
                                      .arg(surroundChars.right);
        const QString identificatorRegExp = QString::fromUtf8("([a-zA-Zа-яА-Я]\\w*)");
        const QString otherSymbols = QString::fromUtf8("([^a-zA-Zа-яА-Я])");

        // Шаблон поиска левой части порождения и знака определения
        finder.setPattern(nonTermRegExp + deffSymbRegExp);
        finder.indexIn(line);
        // Если одна из частей не найдена, то вернуть токен ошибки
        if(finder.cap(1).isEmpty() ||  finder.cap(2).isEmpty()){
            m_token = Token("error", 0, m_currentLine, delPartLen, true);
            return token();
        }
        // Поместить в очередь считанные токены (нетерминал и символ определения)
        m_tokenBuffers.enqueue(Token(finder.cap(1), 0,
                                     m_currentLine,
                                     delPartLen + finder.pos(1)));
        m_tokenBuffers.enqueue(Token(finder.cap(2), 0,
                                     m_currentLine,
                                     delPartLen + finder.pos(2)));
        // Удалить найденную подстроку
        line.remove(finder);
        delPartLen += finder.matchedLength();

        finder.setPattern(QString("%1|%2|%3")
                          .arg(nonTermRegExp)
                          .arg(otherSymbols)
                          .arg(identificatorRegExp));

        int lexemPos = 0;
        // Разбиение строки на состовляющие лексемы
        while((lexemPos = finder.indexIn(line)) != -1){
            int capIndex = 0;
            QString lexem;

            //перебор найденных подстрок в поисках ненулевой
            do{
                ++capIndex;
                lexem = finder.cap(capIndex);
            }while (lexem.isEmpty());

            line.remove(0, finder.matchedLength());

            m_tokenBuffers.enqueue(Token(lexem, 0, m_currentLine, delPartLen));
            delPartLen += finder.matchedLength();
        }
        // Добавление токена конца строки в конец очереди
        m_tokenBuffers.enqueue(Token("endLine", 0, m_currentLine, delPartLen, true));
    }

    m_token = m_tokenBuffers.dequeue();
    return token();
}


SurroundChars::SurroundChars(char l, char r)
    :left(l), right(r)
{
    //nothing;
}

