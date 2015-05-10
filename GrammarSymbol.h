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

#ifndef GRAMMARSYMBOL_H
#define GRAMMARSYMBOL_H

#include <QList>
#include <QString>

class GrammarSymbol;
typedef QList<GrammarSymbol*> SymbolsSequence;

//! Описывает символ грамматики
class GrammarSymbol
{
    public: //members
        QString alias;              //!< псевдоним грамматического символа
        QString description;        //!< описание грамматического символа
        QList<SymbolsSequence> products;    //!< продукции грамматического символа

    public: //methids
        GrammarSymbol();
        explicit GrammarSymbol(const QString &alias,
                               const QString &description  = QString());

        bool isTerminal() const;
};

#endif // GRAMMARSYMBOL_H
