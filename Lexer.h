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

#ifndef LEXER_H
#define LEXER_H


#include <QString>
#include <QVariant>
#include <QList>
#include <QSet>
#include <QTextStream>


//! Токен полученный из лексемы входного потока
struct Token
{
        QString alias;      //!< псевдоним для лексемы
        QVariant value;     //!< значение лексемы
        int line;           //!< номер строки входного потока, содержащей лексему
        int posInLine;      //!< позиция лексемы в строке входного потока
        bool isSystemToken; //!< флаг системного токена (ошибки, конец потока)
        Token(QString a = QString("empty"),
              QVariant v = QVariant(),
              int l = -1, int p = -1,
              bool isSys = false);
};

//! Абстрактный лексический анализатор
/*!
    Описывает минимальный набор функционала для лексического анализатора
  */
class Lexer
{
    public: //members
        QSet<QChar> ignoredChars; //!< Список игнорируемых символов

    public: //methods
        explicit Lexer(QTextStream &strm);

        Token token() const;
        //! Возвращает следующую лексему в виде токена.
        virtual Token nextToken() = 0;
        const QTextStream& stream();

    protected: //members
        QTextStream & m_strm;   //!< Поток для обработки
        Token m_token;          //!< Текущий, уже считанный токен

    private: //methods
        //Запрет на использование пустого конструктора
        Lexer();
};

#endif // LEXER_H
