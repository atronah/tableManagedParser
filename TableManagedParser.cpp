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
  File   : main.cpp
  Created: 20.01.2012
  Reason : Курсовой проект по дисциплине Лингвистическое и программное обеспечение
           автоматизированных систем.
  Product: tableManagedParser, Таблично-управляемый парсер входных текстов.
  Author : Ануфриев Артем, студент 4884 группы.



  Purpose:
      Синтаксический таблично-управляемый анализатор LL(1) грамматик.
*/




#include "TableManagedParser.h"

#include <GrammarLexer.h>
#include <QDebug>
#include <QStringList>


//TODO: Создать константы для символов "empty", "endLine", "error"

//! Создает экземпляр синтаксического анализатора
/*!
  \param errorOut указатель на поток для вывода сообщений об ошибках анализа.
  */
TableManagedParser::TableManagedParser(QTextStream *errorOut)
    :m_errorOutStream(errorOut){
    m_reservedTerminals.insert("id");
    m_reservedTerminals.insert("num");
    m_reservedTerminals.insert("empty");
    m_reservedTerminals.insert("endLine");
    m_reservedTerminals.insert("endStream");
}

//! Поиск всех терминальных символов, с которых может начинаться продукция
/*!
    Подробное описание и назначение функции описано в книге
    "Компиляторы: принципы, технологии и инструментарий" Альфред В. Ахо,
    Моника С. Лам и др.
    \param symbols анализируемая последовательность символов грамматики.
    \param isNoError флаг наличия/отсутствия ошибки в работе функции.
    \return множество символов грамматики, с которых может начинаться \a symbols
  */
StringSet TableManagedParser::first(const SymbolsSequence &symbols, bool *isNoError){
    StringSet result;
    if (isNoError != 0){
        *isNoError = true;
    }
    // Если первый символ последовательности - терминал, то вернуть его
    // псевдоним в качестве результирующего множества
    if (symbols.at(0)->isTerminal()){
        result.insert(symbols.at(0)->alias);

        return result;
    }

    // Если переданная последовательность состоит из одного символа грамматики
    // и этот символ не терминальный
    if(symbols.count() == 1){
        // Для каждой продукции нетерминала
        foreach(const SymbolsSequence &product, symbols.at(0)->products){
            StringSet subResult;
            int lenProduct = product.count();
            int index = 0;

            // Добавить множество его возможных начальных терминалов, а так же
            // начальные терминалы всех предшествующих ему символов.
            for (index = 0; index < lenProduct; ++index){
                // Если очередной символ продукции равен символу, чьи продукции
                // просматриваются (левая рекурсия)
                if(product.at(index) == symbols.at(0)){
                    if (isNoError != 0){
                        *isNoError = false;
                    }
                    qDebug() << "Left recursion in first() function";
                    // выйти из функции во избежании зацикливания
                    return result;
                }
                subResult = first(product.mid(index, 1), isNoError);
                result.unite(subResult);
                if(!subResult.contains("empty")){
                    break;
                }
            }
            // Если в продукции не все символы порождают пустой терминал
            if(index != lenProduct){
                // Удалить пустой терминал из результата
                result.remove("empty");
            }
        }
    // Если последовательность состоит более чем из одного символа
    }else{
        StringSet subResult;
        int lenSequnce = symbols.count();
        int index = 0;

        // найти первый символ в продукции, не порождающий пустой терминал
        // Добавить множество его возможных начальных терминалов, а так же
        // начальные терминалы всех предшествующих ему символов.
        for (index = 0; index < lenSequnce; ++index){
            subResult = first(symbols.mid(index, 1), isNoError);
            result.unite(subResult);
            if(!subResult.contains("empty")){
                break;
            }
        }
        // Если в продукции не все символы порождают пустой терминал
        if(index != lenSequnce - 1){
            // Удалить пустой терминал из результата
            result.remove("empty");
        }
    }

    return result;
}


//! Поиск всех терминальных символов, которые могут следовать сразу за указанным
/*!
    Подробное описание и назначение функции описано в книге
    "Компиляторы: принципы, технологии и инструментарий" Альфред В. Ахо,
    Моника С. Лам и др.
    \param symbol указанный символ.
    \return {множество символов грамматики, которые могут идти непосредственно за
    \a symbol}
  */
StringSet TableManagedParser::follow(const GrammarSymbol *symbol){
    StringSet result;

    if (symbol->isTerminal()){
        return result;
    }

    if (symbol == m_startSymbol){
        result.insert("endStream");
    }

    // Для каждого символа грамматики
    foreach(const QString &key, m_symbols.keys()){
        // Если он нетерминальный и не равен исследуемому
        if(!m_symbols[key].isTerminal() && (&m_symbols[key] != symbol)){
            // для каждой его продукции
            foreach(const SymbolsSequence &product, m_symbols[key].products){
                // Найти в продукции индекс последнего вхождения
                // исследуемого символа
                int index = product.lastIndexOf(const_cast<GrammarSymbol*>(symbol));
                // Если исследуемый символ имеется в продукции
                if (index != -1){
                    // Если символ стоит последним в продукции
                    if (index == product.count() - 1){
                        // добавить к результату все конечные символы нетерминала
                        // чья продукция просматривается
                        result.unite(follow(&m_symbols[key]));
                    }else{
                        // Если после символа в продукии есть последовательность
                        StringSet subResult;
                        // Найти начальные символы этой последовательности
                        subResult = first(product.mid(index + 1));
                        // Если среди найденых начальных символов есть пустой
                        // символ
                        if (subResult.contains("empty")){
                            // добавить к результату все конечные символы нетерминала
                            result.unite(follow(&m_symbols[key]));
                            // удалить пустой символ из найденных начальных
                            subResult.remove("empty");
                        }
                        // добавить к результату найденные начальные символы
                        result.unite(subResult);
                    }
                }
            }
        }
    }
    return result;
}


//! Заполнение таблицы анализа с использованием вспомогательных функций first/folow
/*!
    Заполнение таблицы происходит по алгоритму, написанному в книге
    "Компиляторы: принципы, технологии и инструментарий" Альфред В. Ахо,
    Моника С. Лам и др.
    Там же описано назначение таблицы.
    \return \c true, если заполнение таблицы прошло успешно, иначе \c false
  */
bool TableManagedParser::fillAnalyseTable(){
    bool isNoError = true;
    // Заполнение таблицы анализа (M)
    // Для каждого нетерминального символа грамматики (А)
    foreach (const QString &key, m_symbols.keys()){
        GrammarSymbol *symbol = &m_symbols[key];
        if (!symbol->isTerminal()){
            // для каждой продукции нетерминала А (p)
            foreach (const SymbolsSequence &product, symbol->products){
                StringSet startTerminals = first(product, &isNoError);
                // Если была ошибка при вычислении first()
                if (!isNoError){
                    return isNoError;
                }
                // для каждого терминала из first (f)
                foreach (const QString &startTermAlias, startTerminals){
                    // Добавляем в таблицу продукцию M[A, f] = p
                    m_analyseTable[symbol->alias][startTermAlias] = product;
                }
                // Если среди f есть пустой терминал
                if (startTerminals.contains("empty")){
                    // То для каждого терминала (b) из follow(A)
                    StringSet endTerminals = follow(symbol);
                    foreach (QString startTermAlias, endTerminals){
                        // Добавляем в таблицу продукцию M[A, b] = p
                        m_analyseTable[symbol->alias][startTermAlias] = product;
                    }
                }
            }
        }
    }
    return isNoError;
}


//! Очищает список символов грамматики и добавляет в него зарезервированные.
void TableManagedParser::clearSymbos(){
    m_symbols.clear();
    foreach (QString alias,  m_reservedTerminals){
        m_symbols.insert(alias, GrammarSymbol(alias));
    }
}

//! Выводит ошибку в указанный при создании анализатора поток
/*!
  \param errorMsg текст сообщения об ошибке.
  \param title префикс сообщения об ошибке.
  */
void TableManagedParser::printError(const QString &errorMsg, const QString &title){
    QTextStream *errorOut = m_errorOutStream;

    QFile stdErrOut;
    if(errorOut == 0 || !errorOut->device()->isOpen()){
        stdErrOut.open(stderr, QFile::WriteOnly);
        errorOut->setDevice(&stdErrOut);
    }

    *errorOut << QString("%1: %2")
                 .arg(title)
                 .arg(errorMsg)
              << endl;

    if (stdErrOut.isOpen()){
        stdErrOut.close();
    }
}



//! Загружает описание LL(1) грамматики из файла для конфигурирования парсера
/*!
    Файл должен содержать описание по системе БНФ. При описании разрешено
    использовать как латиницу, так и кириллицу.
    Для грамматики предопределены такие терминалы, как \c "id" для идентификаторов
    и \c "num" для вещественных и целых чисел, а так же \c "empty" для пустого
    символа.

  \param filePathName путь к файлу с описанием грамматики.
  \return \c true, если грамматика успешно загружена, иначе false.
  */
bool TableManagedParser::loadGrammar(const QString &filePathName){
    const QString errorTitle =
            QObject::trUtf8("Loading grammar");
    const QString invalidLeftPartErrorMsg =
            QObject::trUtf8("Invalid left part in line %1");
    const QString invalidLeftPart2ErrorMsg =
            QObject::trUtf8("Contain one of (%1) in left part of line %2");
    const QString errorOpeningFile =
            QObject::trUtf8("Unable to open file %1\n(%2)");

    QFile fInp(filePathName);
    if (!fInp.open(QFile::ReadOnly)){
        printError(errorOpeningFile
                   .arg(filePathName)
                   .arg(fInp.errorString()), errorTitle);
        return false;
    }

    clearSymbos();

    bool isNoError = true;
    QTextStream strm(&fInp);
    GrammarLexer lexer(strm);

    // Считывание определяемого (порождающего) символа
    Token tkn = lexer.nextToken();
    // Проверка на конец потока
    bool isEndStream = tkn.isSystemToken && tkn.alias == "endStream";
    bool isStartSymbolPassed = false;
    while (!isEndStream && isNoError){
        // Если результатом считывания стала ошибка (начало строки не соответствует
        // регламентированному шаблону)
        isNoError = !(tkn.isSystemToken && tkn.alias == "error");
        if (!isNoError){
            printError(invalidLeftPartErrorMsg.arg(tkn.line), errorTitle);
            // то прервать считывание настроек
            break;
        }

        // Проверка, не содержатся ли предопределенные имена в левой части
        if (m_reservedTerminals.contains(tkn.alias)){
            printError(invalidLeftPart2ErrorMsg
                       .arg(QStringList(m_reservedTerminals.toList()).join(", "))
                       .arg(tkn.line),
                       errorTitle);
            isNoError = false;
            break;
        }

        // Проверить, встречается ли токен впервые
        if (!m_symbols.contains(tkn.alias)){
            m_symbols[tkn.alias] = GrammarSymbol(tkn.alias);
            if (!isStartSymbolPassed){
                m_startSymbol =  &m_symbols[tkn.alias];
                isStartSymbolPassed = true;
            }
        }

        // Сохранения ссылки на текущий нетерминал, для которого будут
        // формироваться продукции
        GrammarSymbol *currentNonTerm = &m_symbols[tkn.alias];

        // Пропуск символа определения
        lexer.nextToken();

        // Формирование продукции
        tkn = lexer.nextToken();

        bool isEndLine = tkn.isSystemToken && tkn.alias == "endLine";
        SymbolsSequence product;
        while (!isEndLine){
            // Если считанный токен не является разделителем продукций
            if(tkn.alias != "|"){
                // Проверить, встречается ли токен впервые
                if (!m_symbols.contains(tkn.alias)){
                    // если да, то добавить его к списку символов грамматики
                    m_symbols[tkn.alias] = GrammarSymbol(tkn.alias);
                }
                // Добавить символ грамматики, соответсвующий считанному токену
                // в последовательность символов текущей продукции
                product.append(&m_symbols[tkn.alias]);

            }else{ // Если считан разделитель продукций
                // добавить продукции к списку продукций текущего нетерминала
                currentNonTerm->products.append(product);
                product.clear();
            }
            // Считать следующий токен
            tkn = lexer.nextToken();

            isEndLine = tkn.isSystemToken && tkn.alias == "endLine";
        }
        // Добавить в список продукций последнюю
        currentNonTerm->products.append(product);
        // Считать первый токен следующей строки
        tkn = lexer.nextToken();
        isEndStream = tkn.isSystemToken && tkn.alias == "endStream";
    }


    fInp.close();

    if(isNoError){
        isNoError = fillAnalyseTable();
    }

    return isNoError;
}

//! Создает внутреннее описание грамматики на основе переданого списка
/*!
  \param symbols список символов грамматики и их продукций.
  */
void TableManagedParser::setGrammar(const QList<GrammarSymbol> &symbols){
    clearSymbos();
    foreach(const GrammarSymbol &symbol, symbols){
        m_symbols.insert(symbol.alias, symbol);
    }
    fillAnalyseTable();
}

//! Анализирует входной поток \a stream на соответствие грамматике
/*!
    Производит анализ входного потока \a stream на соответствие грамматике,
    установленной для парсера с помощью loadGrammar() или setGrammar().
    Все ошибки, возникающие во время анализа выводятся либо в поток, заданный при
    создании объекта парсера, либо в stderr, если поток не был задан.

    В случае успешного анализа функция возрващает \c true и заполняет переданную
    по ссылке конструкцию QDomDocument деревом разбора (при
    \a isParseTreeInResult = \c true, по умолчанию) или ситаксическим деревом (при
    \a isParseTreeInResult = \c false).

    \param lexer лексический анализатор, из которого будут получаться лексемы.
    \param result дерево разбора или синтаксическое дерево, в зависимости от
    \a isSyntaxTreeInResult
    \param isParseTreeInResult флаг, указывающий, что нужно строить
    дерево разбора, а не синтаксическое дерево
  */
bool TableManagedParser::analyse(Lexer &lexer,
                                QDomDocument &result,
                                bool isParseTreeInResult){
    const QString errorTitle =
            QObject::trUtf8("Analyse");
    const QString streamErrorMsg =
            QObject::trUtf8("Error input stream");
    const QString unknownTokenErrorMsg =
            QObject::trUtf8("Unknown token \"%1\" in line %2 in pos %3");
    const QString expectedTokenErrorMsg =
            QObject::trUtf8("Token \"%1\" expected, but \"%2\" found in line %3 in pos %4");
    const QString emptyAnalyseTable =
            QObject::trUtf8("Error. Analyse table is empty. Possible error in loaded grammar.");
    const QString emptyTableRecordErrorMsg =
            QObject::trUtf8("Empty record in analyse table for nonTerm \"%1\" ") +
            QObject::trUtf8("and Term \"%2\"\n\t(line %3, pos %4)");
    const QString noErrorMsg =
            QObject::trUtf8("No error");

    if (lexer.stream().status() != QTextStream::Ok){
        printError(streamErrorMsg);
        return false;
    }

    if (m_analyseTable.isEmpty()){
        fillAnalyseTable();
        if (m_analyseTable.isEmpty()){
            printError(emptyAnalyseTable, errorTitle);
            return false;
        }

    }

    QStack<GrammarSymbol*> stack;
    stack.push(&m_symbols["endStream"]);
    stack.push(m_startSymbol);

    QDomImplementation xmlImpl;
    QDomDocument resultDoc;
    if(isParseTreeInResult){
        resultDoc = xmlImpl.createDocument("", "ParseTree",
                                                       result.doctype());
    }else{
        resultDoc = xmlImpl.createDocument("", "SyntaxTree",
                                                       result.doctype());
    }

    QStack<QDomElement> parentElementsStack;
    parentElementsStack.push(resultDoc.firstChildElement());

    lexer.nextToken();
    while (!stack.isEmpty()){
        Token currToken = lexer.token();
        if (currToken.isSystemToken && currToken.alias == "error"){
            // Сообщить об ошибке
            printError(streamErrorMsg, errorTitle);
            return false;
        }
        // Если считанного токена нет среди символов грамматики
        if (!m_symbols.contains(currToken.alias)){
            // Сообщить об ошибке
            printError(unknownTokenErrorMsg
                       .arg(currToken.alias)
                       .arg(currToken.line)
                       .arg(currToken.posInLine),
                       errorTitle);
            return false;
        }
        GrammarSymbol *stackSymbol = stack.pop();
        QDomElement treeNode;

        GrammarSymbol *inputSymbol = &m_symbols[currToken.alias];
        // Если символ на вершине стека терминальный
        if (stackSymbol->isTerminal()){
            treeNode = resultDoc.createElement("Terminal");
            treeNode.setAttribute("alias", stackSymbol->alias);
            treeNode.setAttribute("lineInSrc", currToken.line);
            treeNode.setAttribute("posInLine", currToken.posInLine);
            QDomText textNode = (stackSymbol->alias == "empty")
                                ? resultDoc.createTextNode(stackSymbol->alias)
                                : resultDoc.createTextNode(currToken.value.toString());
            treeNode.appendChild(textNode);


            if (stackSymbol->alias == "empty"){
                parentElementsStack.pop().appendChild(treeNode);
            } else if (stackSymbol == inputSymbol){
                if(stackSymbol->alias != "endStream"){
                    parentElementsStack.pop().appendChild(treeNode);
                }
                // Считать новый символ
                lexer.nextToken();
            // Если символ на стеке не пустой и не равен символу входного потока
            }else{
                // Вывести ошибку в случае неравенства символов
                printError(expectedTokenErrorMsg
                           .arg(stackSymbol->alias)
                           .arg(currToken.alias)
                           .arg(currToken.line)
                           .arg(currToken.posInLine),
                           errorTitle);
                return false;
            }

        // Если символ на вершине стека не терминальный
        }else{
            treeNode = resultDoc.createElement(stackSymbol->alias);
            parentElementsStack.pop().appendChild(treeNode);
            // То заменить его на последовательность символов согласно таблице
            // или вывести ошибку в случае отсутствия соответствующей последовательности
            if (m_analyseTable[stackSymbol->alias].contains(inputSymbol->alias)){
                SymbolsSequence product =
                        m_analyseTable[stackSymbol->alias][inputSymbol->alias];

                QListIterator<GrammarSymbol*> i(product);
                i.toBack();
                while(i.hasPrevious()){
                    stack.push(i.previous());
                    parentElementsStack.push(treeNode);
                }

            // Если нет записи в таблице
            }else{
                // сообщить об ошибке
                printError(emptyTableRecordErrorMsg
                           .arg(stackSymbol->alias)
                           .arg(currToken.alias)
                           .arg(currToken.line)
                           .arg(currToken.posInLine),
                           errorTitle);
                return false;
            }
        }
    }

    result.clear();
    result = resultDoc;
    printError(noErrorMsg, errorTitle);

    return true;
}

#ifdef QT_DEBUG
void TableManagedParser::debug(){
    foreach (const QString &key, m_symbols.keys()){
        SymbolsSequence seq;
        seq.append(&m_symbols[key]);
        QString str;
        if (!m_symbols[key].isTerminal()){
            qDebug() << QObject::trUtf8("Symbol: %1\n%2")
                        .arg(key)
                        .arg(str);
            qDebug() << QObject::trUtf8("First: ") << first(seq);
            qDebug() << QObject::trUtf8("Follow: ") << follow(&m_symbols[key]);
            qDebug() << "------" << endl;
        }

    }
}
#endif
