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
**  или ПРИГОДНОСТИ ДЛЯ ОПРЕДЕЛЕННЫХ ЦЕЛЕЙ. Подробнее см. в СGNU Lesser General Public License.
**
**  Вы должны были получить копию GNU Lesser General Public License
**  вместе с этой программой. Если это не так, см.
**  <http://www.gnu.org/licenses/>.)
**************************************************************************/

#ifndef GRAMMARLEXER_H
#define GRAMMARLEXER_H

#include <Lexer.h>
#include <QQueue>

//! Описание символов, окружающих лексему
struct SurroundChars{
        char left;      //!< левый обрамляющий символ
        char right;     //!< правый обрамляющий символ
        SurroundChars(char l, char r);
};

//! Лексический анализатор для работы с описанием КС-грамматики
/*!
    Обрабатывает файлы и прочие текстовые потоки построчно. Выделяя в строке
    отдельные лексемы левую (с одним нетерминалом) и правую (с одним и более
    символов грамматики) часть правила, символ их разделяющий. Правая часть
    разбивается на лексемы, соответствующие терминальным, нетерминальным символам
    и символу "|", разделяющему несколько продукций для единой левой части.
  */
class GrammarLexer : public Lexer
{
    public: //members
        //! лексема, соединяющая левую и правую часть правила грамматики
        static const QString deffLexem;
        //! символы, в которые заключается имя неТерминала
        static const SurroundChars surroundChars;

    public: //methods
        GrammarLexer(QTextStream &strm);

        virtual Token nextToken();

    private: //members
        int m_currentLine;      //!< хранит номер обрабатываемой строки
        QQueue<Token> m_tokenBuffers; //!< очередь считанный токенов

    private: //methods
        GrammarLexer();
};

#endif // GRAMMARLEXER_H
