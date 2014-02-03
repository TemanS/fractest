/******************************************************************************
**
**  fractest - an application exercising fraction arithmetic.
**
**  Tony Camuso
**  November, 2011
**
**  Version 0.1
**
**    fractest is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**    This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**  GNU General Public License http://www.gnu.org/licenses/gpl.html
**
**  Copyright 2011 by Tony Camuso.
**
******************************************************************************/

#ifndef FRACTEST_H
#define FRACTEST_H

#include <QDialog>
#include <QCheckBox>
#include <QPushButton>
#include "ftnamespace.h"

QT_BEGIN_NAMESPACE
class QWidget;
class QDesktopWidget;
class QMenu;
class QMenuBar;
class QPoint;
class QGroupBox;
class QGroupBox;
class QCheckBox;
class QLineEdit;
class QRadioButton;
class QButtonGroup;
class QTextEdit;
class QTextStream;
QT_END_NAMESPACE

class Msg;
class RandOp;
class QpFile;
class TestParmManager;
class ResultFileManager;

class FracTest : public QDialog
{
    Q_OBJECT

public:
    FracTest(QPoint origin);//, QWidget *parent = 0);
    ~FracTest();
    //void setUserName(QString& name);
    int getProblemCount();
    QString getFinalScore();
    void setUserName(const QString& name);

signals:

private slots:
    void ftExit();
    void ftSave();
    void ftSaveAs();
    void onNameBox();
    void onPBclicked(int index);
    void onAnswer();

private:
    void initDisplay();
    void createMenu();
    void createTestGroup();
    void createNameGroup();
    void startTest();
    void stopTest();
    void openResultFile();
    void getDefaults();
    void setDefaults(QTextStream& in);
    int  openDefaults(QpFile& inFile, QTextStream& stream, bool builtin=false);
    void getMaxops();
    void setEditable(bool state);
    void buffToInt(QTextStream &str, QList<int>& list);
    void runTest();
    void genScore();
    void doLcm();
    void doReduce();
    void doCombine();
    void showProblem(const QString& problem);
    QVector<int> extractNumbers(const QString& string);
    QString doLcmAnswer();
    QString doReduceAnswer();

    QString userName;
    QButtonGroup* pbGroup;
    QButtonGroup* levelGroup;

    QMenuBar* menuBar;
    QMenu* fileMenu;
    QAction* saveAction;
    QAction* saveAsAction;
    QAction* exitAction;

    QLineEdit* nameBox;
    QTextEdit* messageArea;
    QList<QCheckBox*> testChkBoxList;
    QList<QLineEdit*> quanEditList;
    QList<QRadioButton*> levelList;
    QList<QPushButton*> pbList;
    QList<QLineEdit*> problemList;

    QGroupBox* testGroupBox;
    QGroupBox* nameGroupBox;
    QStringList testNames;
    QStringList inputMasks;

    Msg* msgHandler;
    RandOp* rnd;
    TestParmManager* testParmManager;
    ResultFileManager *resultFileManager;
};

#endif // FRACTEST_H
