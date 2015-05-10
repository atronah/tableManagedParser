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
/*
  File   : main.cpp
  Created: 20.01.2012
  Reason : Курсовой проект по дисциплине Лингвистическое и программное обеспечение
           автоматизированных систем.
  Product: tableManagedParser, Таблично-управляемый парсер входных текстов.
  Author : Ануфриев Артем, студент 4884 группы.



  Purpose:
      Основной модуль программы. Реализация диалога с пользователем.
*/

#include <QtCore/QCoreApplication>

#include <QFile>
#include <Lexer.h>
#include <QTextStream>
#include <QStringList>
#include <QTextCodec>
#include <QString>
#include <TableManagedParser.h>

bool userBinaryRequest(QTextStream &out,
                       QTextStream &in,
                       const QString &message,
                       const QString & trueResponse,
                       Qt::CaseSensitivity caseSens = Qt::CaseInsensitive){
    out << message << endl;
    QString userResponse;
    in >> userResponse;
    // Если пользователь ответил отказом
    if(QString::compare(userResponse, trueResponse, caseSens)){
        return false;
    }

    return true;
}


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

//    QString test;
//    QTextStream inD(stdin);
//    QTextStream outD(stdout);

//    test = inD.readLine();

//    int i = 0;
//    return 0;

    /*/debug code---------#################################
    QTextStream outD(stdout);
    outD.setCodec(QTextCodec::codecForName("IBM 866"));
    TableManagedParser parserD(&outD);
//    QString srcStrD("(55)/12 * 3 + 213*12.5 / (18-2.5)");
//    QString srcStrD("55/123 + 213*12.5 / (18-2.5)");
//    QString srcStrD("(55)/123 + 213*12.5 / 18");
//    QString srcStrD("(23+4)*5");
    QString srcStrD("(22+55) + 17");
    QTextStream srcStreamD(&srcStrD);
    parserD.loadGrammar("d:\\test_eng");
    QDomDocument blabla("ParsingResult");
//    parserD.debug();
    CodeLexer lexer(srcStreamD);
    parserD.analyse(lexer, blabla);
//    outD << endl << endl << blabla.toString() << endl;
    QFile xmlFile("d:\\test.xml");
    if (xmlFile.open(QFile::WriteOnly)){
        QTextStream xmlFileStream(&xmlFile);
        blabla.save(xmlFileStream, 4);
        xmlFile.close();
    }


    return 0;
    //end debug code-----#################################*/

    const QString defaultGrammarFileName = QString::fromUtf8("grammar.txt");
    const QString defaultSourceFileName = QString::fromUtf8("source.txt");
    const QString defaultParseTreeFileName = QString::fromUtf8("parseTree.xml");
    const QString appVersion("1.0");
    const QString appTitle = QObject::trUtf8("Table driven parser v%1")
                             .arg(appVersion);

    const QString yesResponseChar = QObject::trUtf8("y");
    const QString yesResponseWord = QObject::trUtf8("Yes");
    const QString noResponseChar = QObject::trUtf8("n");
    const QString noResponseWord = QObject::trUtf8("No");
    const QString yesNoMsg = QObject::trUtf8("%1/%2 = %3, %4/%5 = %6 (default)")
                             .arg(yesResponseChar.toLower())
                             .arg(yesResponseChar.toUpper())
                             .arg(yesResponseWord)
                             .arg(noResponseChar.toLower())
                             .arg(noResponseChar.toUpper())
                             .arg(noResponseWord);
    const QString promptEnterPath = QObject::trUtf8("Enter path to file %1\ndefault: %2");
    const QString grammarFileDescription =
            QObject::trUtf8("with grammar description.");
    const QString sourceFileDescription =
            QObject::trUtf8("with text for analyse.");
    const QString loadingFileMsg = QObject::trUtf8("Loading file: %1");
    const QString loadingCompleteMsg = QObject::trUtf8("Loading file complete.");
    const QString promptReEnterFileName =
            QObject::trUtf8("re-enter file name? %1")
            .arg(yesNoMsg);
    const QString goodLuckMsg = QObject::trUtf8("Good luck...");
    const QString analyseSuccessfulMsg = QObject::trUtf8("Analyse Successful.");
    const QString analyseFailMsg = QObject::trUtf8("Analyse fail.");
    const QString promptContinueWork =
            QObject::trUtf8("Continue work with program? %1")
            .arg(yesNoMsg);
    const QString promptSourceFromFile =
            QObject::trUtf8("Load source data from file (Otherwise from standart input)?\n %1")
            .arg(yesNoMsg);
    const QString promptEnterSourceData =
            QObject::trUtf8("Enter source data for analyse:");
    const QString promptSaveParseResult =
            QObject::trUtf8("Save parse tree in file? %1")
            .arg(yesNoMsg);
    const QString parseTreeFileDescripion =
            QObject::trUtf8("for saving parse tree.");
    const QString errorFileOpening =
            QObject::trUtf8("Error opepening file %1");
    const QString saveParseTreeComplete =
            QObject::trUtf8("Parse tree writed in file %1");


    bool isAppExit = false;
    bool isSrcFromFile = false;

    QTextStream out(stdout);
    QTextStream in(stdin);
    out.setCodec(QTextCodec::codecForName("IBM 866"));
    in.setCodec(QTextCodec::codecForName("IBM 866"));

    out << appTitle << endl;

    // Создание объекта парсера
    TableManagedParser parser(&out);

    // загрузка гармматики
    do{
        out <<  promptEnterPath
                .arg(grammarFileDescription)
                .arg(defaultGrammarFileName)
             << endl;

        QString grammarFileName;
        in >> grammarFileName;

        if (grammarFileName.isEmpty()){
            grammarFileName = defaultGrammarFileName;
        }

        out << loadingFileMsg.arg(grammarFileName) << endl;
        // Если произошла ошибка при загрузке грамматики
        if (!parser.loadGrammar(grammarFileName)){
            // Попробовать загрузить грамматику из файла по умолчанию
            out << loadingFileMsg.arg(defaultGrammarFileName) << endl;
            if (!parser.loadGrammar(defaultGrammarFileName)){
                //Вывести предложение ввести новое имя файла
                isAppExit = !userBinaryRequest(out, in, promptReEnterFileName, yesResponseChar);
                if (isAppExit){
                    break;
                }
            // Если загрузка грамматики прошла успешно
            }else{
                out << loadingCompleteMsg << endl;
                break;
            }
        // Если загрузка грамматики прошла успешно
        }else{
            out << loadingCompleteMsg << endl;
            // Выйти из цикла запроса имени файла
            break;
        }
    }while (true);


    if(!isAppExit){
        isSrcFromFile = userBinaryRequest(out, in, promptSourceFromFile, yesResponseChar);
    }


    while(!isAppExit){
        QTextStream srcStream;
        // Если пользователь решит вводить данные с файла
        QFile srcFile;
        // Если пользователь решит вводить данные с клавиатуры
        QString srcData;
        if(isSrcFromFile){
            out << promptEnterPath
                   .arg(sourceFileDescription)
                   .arg(defaultSourceFileName)
                <<endl;
            QString sourceFileName;
            in >> sourceFileName;

            srcFile.setFileName(sourceFileName);
            // Если не удалось открыть файл
            if(!srcFile.open(QFile::ReadOnly)){
                sourceFileName = defaultSourceFileName;
                srcFile.setFileName(sourceFileName);
                if(!srcFile.open(QFile::ReadOnly)){
                    //Вывести предложение ввести новое имя файла
                    isAppExit = !userBinaryRequest(out, in, promptReEnterFileName, yesResponseChar);
                    if (isAppExit){
                        break;
                    }
                }
            }
            // Если файл успешно открыт
            if (srcFile.isReadable()){
                //Создать поток на основе файла для передачи его парсеру
                srcStream.setDevice(&srcFile);
            }
        }else{
            out << promptEnterSourceData << endl;
            // Считывание строки из одного символа конца строки (из буфера)
            in.readLine();
            // Считывание обрабатываемое строки
            srcData = in.readLine();
            srcStream.setString(&srcData);
        }

        // --- Анализ входных данных ---
        QDomDocument analyseResult("ParsingResult");
        // Если анализ прошел успешно
        CodeLexer lexer(srcStream);
        if (parser.analyse(lexer, analyseResult)){
            // Вывести сообщение об успехе
            out << analyseSuccessfulMsg << endl;
            if(userBinaryRequest(out, in, promptSaveParseResult, yesResponseChar)){
                out << promptEnterPath
                       .arg(parseTreeFileDescripion)
                       .arg(defaultParseTreeFileName)
                    << endl;
                QString parseTreeFileName;
                in >> parseTreeFileName;
                QFile parseTreeFile(parseTreeFileName);

                out << loadingFileMsg
                       .arg(parseTreeFileName)
                    <<endl;
                if(!parseTreeFile.open(QFile::WriteOnly)){
                    out << errorFileOpening
                           .arg(parseTreeFileName)
                        << endl;
                    parseTreeFile.setFileName(defaultParseTreeFileName);
                    out << loadingFileMsg
                           .arg(defaultParseTreeFileName)
                        <<endl;
                    if(!parseTreeFile.open(QFile::WriteOnly)){
                        out << errorFileOpening
                               .arg(defaultParseTreeFileName)
                            <<endl;
                    }
                }
                if (parseTreeFile.isOpen()){
                    QTextStream xmlResultStream(&parseTreeFile);
                    analyseResult.save(xmlResultStream, 4);
                    parseTreeFile.close();
                    out << saveParseTreeComplete
                           .arg(parseTreeFile.fileName())
                        <<endl;
                }
            }
        }else{
            // Вывести сообщение об ошибке
            out << analyseFailMsg << endl;
        }

        if (isSrcFromFile && srcFile.isOpen()){
            srcFile.close();
        }
        // --- Конец анализа ---

        // Спросить у пользователя, не желает ли он продолжить работу
        isAppExit = !userBinaryRequest(out, in, promptContinueWork, yesResponseChar);
    }

    out << goodLuckMsg << endl;

    return app.exec();
}
