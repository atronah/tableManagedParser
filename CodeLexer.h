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

#ifndef CODELEXER_H
#define CODELEXER_H

#include <Lexer.h>
#include <QStack>


//! Лексический анализатор для программного кода
/*!
    Считывает токены из входного потока, объединяя в лексемы целые и дробные
    числа (токен с псевдонимом \a num) и идентификаторы (токен с псевдонимом
    \a id). Отдельной лексемой считывает конец потока. Отслеживает позицию
    считывания во входном потоке.
  */
class CodeLexer : public Lexer
{
    public: //members
        QSet<QString> reservedWords;    //!< Список зарезервированных слов

    public: //methods
        explicit CodeLexer(QTextStream &strm);
        virtual Token nextToken();

    private: //members
        int m_currentLine;              //!< номер обрабатываемой строки
        int m_pos;                      //!< позиция токена в строке
        QStack<QChar> m_buffer;         //!< хранение считанных символов
};

#endif // CODELEXER_H
