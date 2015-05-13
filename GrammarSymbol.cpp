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
  File   : GrammarSymbol.cpp
  Created: 20.01.2012
  Reason : Курсовой проект по дисциплине Лингвистическое и программное обеспечение
           автоматизированных систем.
  Product: tableManagedParser, Таблично-управляемый парсер входных текстов.
  Author : Ануфриев Артем, студент 4884 группы.



  Purpose:
      Символ грамматики.
*/

#include "GrammarSymbol.h"

//! Создает грамматический пустой символ
GrammarSymbol::GrammarSymbol(){
    alias = "empty";
    description = QString();
}

//! Создает грамматический символ с указаннами псевдонимом и описанием
/*!
  \param alias псевдоним грамматического символа.
  \param description описание грамматического символа
  */
GrammarSymbol::GrammarSymbol(const QString &alias, const QString &description){
    this->alias = alias;
    this->description = description;
}

//! Проверяет, является ли символ терминальным
/*!
  \return \c true, если символ терминальный (не содержит продукций), иначе \c false
  */
bool GrammarSymbol::isTerminal() const{
    return this->products.isEmpty();
}
