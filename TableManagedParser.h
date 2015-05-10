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

#ifndef TABLEMANAGEDPARSER_H
#define TABLEMANAGEDPARSER_H

#include <GrammarSymbol.h>
#include <CodeLexer.h>

#include <QList>
#include <QHash>
#include <QStack>
#include <QString>
#include <QTextStream>
#include <QFile>
#include <QtXml/QDomDocument>


typedef QSet<QString> StringSet;


//! Синтаксический нерекурсивный предиктивный анализатор LL(1)-грамматик
/*!
    По указанному в виде файла описанию LL(1)-грамматики производит синтаксический
    разбор входного потока с использованнием указанного лексического анализатора.

    Формирует дерево разбора в виде XML-документа.
  */
class TableManagedParser{
    public: //methods
        TableManagedParser(QTextStream *errorOut = 0);

        bool loadGrammar(const QString &filePathName);
        void setGrammar(const QList<GrammarSymbol> &symbols);

        bool analyse(Lexer &lexer,
                     QDomDocument &result,
                     bool isParseTreeInResult = true);

#ifdef QT_DEBUG
        void debug();
#endif

    private: //members
        //! Описание грамматики
        QHash<QString, GrammarSymbol> m_symbols;
        //! Стартовый символ грамматики
        GrammarSymbol *m_startSymbol;
        //! Таблица анализа [NonTerminalAlias][TerminalAlias] = SymbolSequence
        QHash<QString, QHash<QString, SymbolsSequence> > m_analyseTable;
        //! Список зарезервированных терминалов
        StringSet m_reservedTerminals;

        //! Поток для вывода сообщений об ошибках анализа
        QTextStream * m_errorOutStream;

    private: //methods
        TableManagedParser();

        StringSet first(const SymbolsSequence &symbols, bool *isNoError = 0);
        StringSet follow(const GrammarSymbol *symbol);
        bool fillAnalyseTable();

        void clearSymbos();
        void printError(const QString &errorMsg,
                        const QString &title = QString());
};

#endif // TABLEMANAGEDPARSER_H
