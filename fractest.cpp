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

/******************************************************************************
**
**    APPLICATION WINDOW LAYOUT
**    =========================
**
**
**    Select Test                 How Many        Level
**                               +----------+
**    [X] Least Common Multiple  |   Edit   |     ( ) 1
**                               +----------+
**                               +----------+
**    [X] Reduce to LCM          |   Edit   |     (*) 2
**                               +----------+
**                               +----------+
**    [X] Combine Terms          |   Edit   |     ( ) 3
**                               +----------+
**
**    +--------------+  +--------------+  +--------------+
**    |   Start PB   |  |   Stop PB    |  |    Dlg PB    |
**    +--------------+  +--------------+  +--------------+
**
**    +--------------------------------------------------+
**    |    Username                                      |
**    +--------------------------------------------------+
**
**    Problem                                  Your
**    Number         Problem               Answer
**    +----------+  +--------------------+  +------------+
**    | R/O Edit |  |    R/O Edit        |  |    Edit    |
**    +----------+  +--------------------+  +------------+
**
**    +--------------------------------------------------+
**    |                                                  |
**    |                                                  |
**    |           Messages  R/O                          |
**    |                                                  |
**    |                                                  |
**    |                                                  |
**    |                                                  |
**    +--------------------------------------------------+
**
******************************************************************************/

#include <time.h>
#include <QtCore>
#include <QtGui>
#include <QPoint>
#include <QMenu>
#include <QMenuBar>
#include <QLineEdit>
#include <QMessageBox>
#include <QVBoxLayout>

#include <qpgui.h>
#include <mathpack.h>
#include <randomop.h>
#include <randmanager.h>
#include <testparm.h>
#include <testparmmanager.h>
#include <fractest.h>

using namespace ft;

/*********************************************************************
 * FracTest(QPoint origin) - constructor
 *
 * Create an instance of the factest class at the origin defined by
 * the QPoint.
 */
FracTest::FracTest(QPoint origin)//, QWidget *parent)
//    : QDialog(parent)
{
    setWindowFlags(Qt::Window);
    setWindowTitle("Fractions Test");
    move(origin.x(), origin.y());
    QpAppStyle style;
    setStyleSheet(*style.getAppStyle());

    testNames << "Lowest Common Multiple"
              << "Reduce Fraction"
              << "Combine Fractions";
    inputMasks << "0000" << "00 00 / 000" << "00 00 / 000";

    createMenu();
    createTestGroup();
    createNameGroup();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setMenuBar(menuBar);
    mainLayout->addWidget(testGroupBox);
    mainLayout->addWidget(nameGroupBox);

    msgHandler = new Msg(messageArea);
    testParmManager = new TestParmManager(ft_tests_end);
    resultFileManager = new ResultFileManager;
    rnd = new RandOp;
    getDefaults();
}


/*********************************************************************
 * FracTest deconstructor
 *
 */
FracTest::~FracTest()
{
}

/*********************************************************************
 * createMenu - create the menu bar and the options droplist.
 *
 */
void FracTest::createMenu()
{
    menuBar = new QMenuBar;

    fileMenu = new QMenu(tr("&File"), this);
    saveAction   = fileMenu->addAction("&Save Settings");
    saveAsAction = fileMenu->addAction("Save Settings &As...");
    exitAction   = fileMenu->addAction("E&xit");
    menuBar->addMenu(fileMenu);

    connect(saveAction,   SIGNAL(triggered()), this, SLOT(ftSave()));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(ftSaveAs()));
    connect(exitAction,   SIGNAL(triggered()), this, SLOT(ftExit()));
}

/*********************************************************************
 *
 */
 void FracTest::createTestGroup()
{
    int wid = 200;
    testGroupBox = new QGroupBox;
    testGroupBox->setAlignment(Qt::AlignCenter);
    testGroupBox->setMinimumWidth(wid);
    QGridLayout* layout = new QGridLayout;
    QStringList qsTest;
    qsTest << "Select Problem Type" << "Quantity" << "Level";

    for(int i = 0; i < ft_tests_end; ++i) {
        layout->addWidget(new QLabel(qsTest[i]), 0, i);
    }

    for(int i = 0; i < 3; ++i) {
        testChkBoxList << new QCheckBox(testNames[i]);
        testChkBoxList[i]->setMinimumWidth(wid-10);
        layout->addWidget(testChkBoxList[i], i+1, 0);
    }

    wid = 60;

    for(int i = 0; i < ft_tests_end; ++i) {
        quanEditList << new QLineEdit("");
        quanEditList[i]->setFixedWidth(wid);
        quanEditList[i]->setAlignment(Qt::AlignCenter);
        layout->addWidget(quanEditList[i], i+1, 1);
    }

    levelGroup = new QButtonGroup;

    for(int i = 0; i < ft_lvl_end; ++i) {
        levelList << new QRadioButton(QString("%1").arg(i+1));
        levelGroup->addButton(levelList[i], i);
        layout->addWidget(levelList[i], i+1, 2);
    }
    testGroupBox->setLayout(layout);
    levelList[ft_lvl_1]->setChecked(true);
}

 /*********************************************************************
  *
  */
void FracTest::createNameGroup()
{
    const int wid = 140;
    nameGroupBox = new QGroupBox;
    nameGroupBox->setMinimumWidth(wid);
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(new QLabel("Your Name: "), 0, 0);
    nameBox = new QLineEdit("");
    layout->addWidget(nameBox, 0, 1, 1, 2);
    connect(nameBox, SIGNAL(returnPressed()), this, SLOT(onNameBox()));

    pbGroup = new QButtonGroup;
    connect(pbGroup, SIGNAL(buttonClicked(int)), this, SLOT(onPBclicked(int)));

    QStringList pbStrings;
    pbStrings << "Start" << "Stop" << "Set Defaults";

    for(int i = 0; i < ft_pb_end; ++i) {
        pbList << new QPushButton(pbStrings[i]);
        pbList[i]->setAutoDefault(false);
        pbList[i]->setDefault(false);
        pbList[i]->setFixedSize(100,24);
        layout->addWidget(pbList[i], 1, i, Qt::AlignRight);
        pbGroup->addButton(pbList[i], i);
    }

    QList<QLabel*> labelList;
    QStringList stringList;

    stringList << "" << "Problem      " << "Answer      ";

    for(int i = 0; i < ft_prob_end; ++i) {
        labelList << new QLabel(stringList[i]);
        layout->addWidget(labelList[i], 2, i, Qt::AlignRight);
        problemList << new QLineEdit;
        problemList[i]->setReadOnly(true);
        layout->addWidget(problemList[i], 3, i, Qt::AlignRight);
    }
    problemList[ft_pnum]->setMaximumWidth(50);
    problemList[ft_pnum]->setAlignment(Qt::AlignRight);
    problemList[ft_answer]->setReadOnly(false);
    problemList[ft_answer]->setInputMask("0000");

    connect(problemList[ft_answer], SIGNAL(returnPressed()), this,
            SLOT(onAnswer()));

    layout->addWidget(new QLabel("Messages"), 4, 1, Qt::AlignCenter);
    messageArea = new QTextEdit;
    messageArea->setReadOnly(true);
    layout->addWidget(messageArea, 5, 0, 1, 3);
    nameGroupBox->setLayout(layout);
}

/*********************************************************************
 *
 */
void FracTest::ftExit()
{
    QMessageBox mbox;
    mbox.setWindowTitle("Fractions Test");
    mbox.setText("Do you want to exit now?");
    mbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    mbox.setDefaultButton(QMessageBox::Yes);
    int status = mbox.exec();
    switch(status) {
        case QMessageBox::Yes : this->done(0); break;
        case QMessageBox::No  : break;
    }
}

/*********************************************************************
 *
 */
void FracTest::ftSave()
{
    QMessageBox mbox;
    mbox.setText("Save");
    mbox.exec();
}

/*********************************************************************
 *
 */
void FracTest::ftSaveAs()
{
    QMessageBox mbox;
    mbox.setText("Save as ...");
    mbox.exec();
}

/*********************************************************************
 *
 */
void FracTest::onNameBox()
{
    if((nameBox->text() != userName) && (nameBox->text() != "")) {
        openResultFile();
        getDefaults();
    }
}

/*********************************************************************
 *
 */
void FracTest::onPBclicked(int index)
{
    switch(index) {
    case ft_start : startTest(); break;
    case ft_stop  :
        msgHandler->sendAlert("\"Stop\" Button Pressed - Test Stopped.");
        stopTest();
        break;
    }
}

/*********************************************************************
 *
 */
void FracTest::onAnswer()
{
    TestParmManager* ptm = testParmManager;

    if( ! ptm->isRunning())
        return;

    ptm->getElapsedTime();
    ptm->setUserAnswer(problemList[ft_answer]->text());
    QString text;
    msg_t msgLevel;
    int test = ptm->getIndex();

    switch(test) {
    case ft_lcm: text = doLcmAnswer(); break;
    case ft_reduce: text = doReduceAnswer(); break;
    case ft_combine: break;
    }

    msgLevel = (ptm->isCorrect()) ? msg_notify : msg_alert;
    text += ptm->getCorrectAnswer()
            % ".  You said: " % problemList[ft_answer]->text() % " in "
            % QString("%1").arg(ptm->getUserTime()) % " sec";

    msgHandler->sendMessage(text, msgLevel);
    resultFileManager->updateTest(ptm);
    ptm->updatePass();
    runTest();
}

/*********************************************************************
 *
 */
QString FracTest::doLcmAnswer()
{
    TestParmManager* ptm = testParmManager;
    QString qsAnswer = ptm->getCorrectAnswer();
    int correctAnswer = qsAnswer.toInt();
    int userAnswer = problemList[ft_answer]->text().toInt();
    ptm->setUserAnswer(problemList[ft_answer]->text());

    if(userAnswer == correctAnswer)
        ptm->bumpCorrect();

    QString text = "LCM( " % ptm->getProblem() % " ) is ";
    return text;
}

/*********************************************************************
 *
 */
QString FracTest::doReduceAnswer()
{
    TestParmManager* ptm = testParmManager;
    QVector<int> correctAnswer = extractNumbers(ptm->getCorrectAnswer());

    ptm->setUserAnswer(problemList[ft_answer]->text());
    QVector<int> userAnswer = extractNumbers(ptm->getUserAnswer());

    // Check for /1
    //
    if(correctAnswer.last() == 1)
        correctAnswer.resize(userAnswer.size());

    if(userAnswer == correctAnswer)
        ptm->bumpCorrect();

    QString text = ptm->getProblem() % " reduced is ";
    return text;
}

/*********************************************************************
 * startTest - Begin the testing
 *
 */
void FracTest::startTest()
{
    if(testParmManager->isRunning()) {
        msgHandler->sendAlert("Tests are already running");
        return;
    }

    if(testParmManager->isFirstRun() || (nameBox->text() != userName))
        openResultFile();

    testParmManager->initTotalCount();
    testParmManager->initTotalCorrect();
    for(int i = 0; i < ft_tests_end; ++i) {
        testParmManager->initTest(
             i,                                 // index into the test params
             quanEditList[i]->text().toInt(),   // number of problems
             -1,                                // no timeout
             levelGroup->checkedId(),           // level of difficulty
             testChkBoxList[i]->isChecked() ? true : false, // enabled?
             inputMasks[i],
             testNames[i]);
    }

    if(testParmManager->getTotalCount() == 0) {
        msgHandler->sendAlert("No tests selected.\nPlease select a test.");
        testParmManager->stopTest();
        return;
    }

    if(testParmManager->isMaxopsLoaded() == false) {
        getMaxops();
        testParmManager->setMaxopsLoaded(true);
    }

    setEditable(false);
    runTest();
}

/*********************************************************************
 *
 */
void FracTest::stopTest()
{
    testParmManager->stopTest();
    setEditable(true);
    for(int i = 0; i < ft_tests_end; ++i)
        problemList[i]->clear();
}

/*********************************************************************
 *
 */
void FracTest::runTest()
{
    TestParmManager *ptm = testParmManager;

    // Sanity check. We should never get here if isRunning is not true, but
    // as one engineer once said to me, paranoia is good in this business.
    //
    if(! testParmManager->isRunning()) {
        stopTest();
        return;
    }

    // If we've completed a set of tests, update the total correct
    // answers and generate the score for this test.
    //
    if(ptm->isEnabled() && ptm->atTestEnd() && ptm->getTotalCount() > 0)
        genScore();

    // If we've reached the end of a test sequence, but we have not
    // completed all test sequences checked, then set the parameters
    // for the start of the next test. Else, we have completed all
    // the tests checked, so generate the final score and exit.
    //
    if(ptm->getNextTestIndex() >= ft_tests_end) {
        ptm->setRunning(false);
        genScore();
        msgHandler->sendInfo("Tests complete.");
        stopTest();
        return;
    }

    // If the above call to getNextTestIndex() returned a value less
    // than the number of tests checked, then the pass number has been
    // zeroed, because we are starting a new test sequence. So, now we
    // must update the resultfile with the header for the start of a
    // new test.
    //
    if(ptm->getPass() == 0) {
        resultFileManager->startTest(ptm);
        problemList[ft_answer]->setInputMask(inputMasks[ptm->getIndex()]);
    }

    // At this point, the TestParmManager pointer, "ptm" is pointing to the
    // test parameters for the most current sequence of tests.
    //
    switch(ptm->getIndex()) {
        case ft_lcm: doLcm(); break;
        case ft_reduce: doReduce(); break;
        case ft_combine: doCombine(); break;
        default: break;
    }
}

//////////////////////////////////////////////////////////////////////////
//
// doLcm - Least Common Multiple test
//
// Does not have max/min for Right Operand (Rop), because there is no
// operand, but rather there are terms presented for which the Least
// Common Multiple must be determined by the user.
//
void FracTest::doLcm()
{
    TestParmManager* ptm = testParmManager;
    RandManager randman = ptm->getRandman();

    QString problem;
    LeastComMult lcm;
    RandOp rnd;
    QVector<int> ops;
    int terms;

    // Get a "column" of random numbers for the operands.
    //
    randman.setNoZero(true);
    randman.getValues(ops);

    // Terms is the number of terms in a problem. For LCM, if there
    // are three terms, the problem would ask for the Least Common
    // multiple of the three terms, a,b,c. E.g, the Least Common
    // multiple of 3,4,5 would be 60 (3*4*5).
    // The minimum number of terms for a Least Common Multiple is 2,
    // since you need at least 2 numbers to determine their Least
    // Common Multiple.
    //
    // If the test allows more than the minimum number of multiples,
    // then randomize the number of multiples that can be presented.
    //
    int maxterms = ptm->getMaxTerms();
    int minterms = ptm->getMinTerms();
    terms = (maxterms > minterms) ? rnd.getOne(minterms, maxterms)
                                  : minterms;
    // Create the problem string.
    //
    problem.clear();
    for(int i = 0; i < terms; ++i)
        problem += QString("%1, ").arg(ops[i]);

    int theAnswer = lcm.getLeastCommonMultiple(ops);
    ptm->setCorrectAnswer(QString("%1").arg(theAnswer));
    problem.resize(problem.size()-2);
    showProblem(problem);
}

//////////////////////////////////////////////////////////////////////////
//
// doReduce - Reduce fractions
//
enum {numIdx, denIdx, mulIdx};
void FracTest::doReduce()
{
    TestParmManager* ptm = testParmManager;
    RandManager randman = ptm->getRandman();
    QVector<int> ops;

    randman.getValues(ops);

    // If the numerator is smaller than the denominator, then make sure
    // that they have a common factor so the fraction can be reduced.
    //
    Factors fac(ops[numIdx], ops[denIdx]);
    if((ops[numIdx] < ops[denIdx]) && ! fac.existCommonFactors()) {
        ops[numIdx] *= ops[mulIdx];
        ops[denIdx] *= ops[mulIdx];
    }

    fac.getCommonFactors(ops[numIdx], ops[denIdx]);
    int num = ops[numIdx] / fac.getGreatestComFactor();
    int den = ops[denIdx] / fac.getGreatestComFactor();

    QString answer;
    if((num > den) && (num % den != 0))
        answer = QString("%1  %2 / %3").arg(num / den).arg(num % den).arg(den);
    else
        answer = QString("%1 / %2").arg(num).arg(den);

    ptm->setCorrectAnswer(answer);

    QString problem = QString("%1 / %2").arg(ops[numIdx]).arg(ops[denIdx]);
    showProblem(problem);
}

/*********************************************************************
 *
 */
void FracTest::doCombine()
{
    msgHandler->sendNotify("\"Combine Terms\" test not available yet.");
    stopTest();
}

/*********************************************************************
 *
 */
void FracTest::showProblem(const QString& problem)
{
    TestParmManager* ptm = testParmManager;
    QString problemNumber = QString("%1.").arg(ptm->getPass() + 1);
    ptm->setProblem(problem);
    problemList[ft_pnum]->setText(problemNumber);
    problemList[ft_problem]->setText(ptm->getProblem());
    problemList[ft_answer]->setFocus();
    problemList[ft_answer]->clear();
    problemList[ft_answer]->setCursorPosition(0);
    ptm->startTimer();
}

/*********************************************************************
 * extractNumbers - extracts numbers from a string and returns them
 *                  as a QVector
 *
 * Contains a static QVector, because it must persist across calls.
 *
 */
QVector<int>& FracTest::extractNumbers(const QString& string)
{
    QTextStream stream;
    QString buf;
    QString temp = string;
    static QVector<int> numbers;    // Must persist across calls
    int num;
    bool ok;

    numbers.clear();
    stream.setString(&temp);

    while(! stream.atEnd()) {
        stream >> buf;
        num = buf.toInt(&ok);
        if(ok)
            numbers << num;
    }
    return numbers;
}

/*********************************************************************
 *
 */
void FracTest::genScore()
{
    TestParmManager* ptm = testParmManager;

    if(ptm->isRunning()) {
        testParmManager->writeEndOfTest(msgHandler);
        resultFileManager->writeEndOfTest(ptm);
    }
    else {
        testParmManager->writeFinals(msgHandler);
        resultFileManager->writeFinals(ptm);
        QString txt = QString("Final Score: %1").arg(ptm->getFinalScore());
        QMessageBox mbox;
        mbox.setWindowTitle("Fractions Test");
        mbox.setText(txt);
        mbox.exec();
    }
}

/*********************************************************************
 *
 */
void FracTest::openResultFile()
{
    QDateTime dt = QDateTime::currentDateTime();
    QString dtStr = dt.toString("yyyy.MM.dd-HH.mm.ss");
    QString suffix = dtStr % ".txt";
    QString resultFileName;
    QString temp;

    userName = nameBox->text();
    temp = (userName == "") ? "test" : userName;
    resultFileName = temp % "-fractions-" % suffix;

    QpFile *pFile = new QpFile(resultFileName, msgHandler);
    resultFileManager->init(pFile);

    if (! pFile->open(QIODevice::Append | QIODevice::Text))
        msgHandler->sendInfo("Continuing without saving results to a file.");
    else
        resultFileManager->startFile(temp, dtStr);
}

/*********************************************************************
 *
 */
void FracTest::setEditable(bool state)
{
    for(int i = 0; i < ft_tests_end; ++i)
    {
        testChkBoxList[i]->setEnabled(state);
        quanEditList[i]->setEnabled(state);
        levelList[i]->setEnabled(state);
    }
    quanEditList[ft_answer]->setEnabled(true);
    quanEditList[ft_answer]->setReadOnly(false);
}

/*********************************************************************
 *
 */
int FracTest::openDefaults(QpFile& inFile, QTextStream& stream, bool builtin)
{
    // Default Test Parameters
    // =====================================================
    //
    //  Test Selection, number of problems, and time allowed
    //
    //  The number 2 sent to a checkbox sets it to checked.
    //
    //                         Test    Number of
    //                        Checked  Problems
    //  Least Common Multiple    2         10          10
    //  Reduce to LCM            2         10          15
    //  Combine Terms            2         10          25
    //
    //  Level   0 (first level)
    //
    //  userName "" (NULL)
    //
    QString defStr = QString(
                "2 10 \n"
                "2 10 \n"
                "0 10 \n"
                "0 \n "
                );

    QString defaultFileName;
    int status;

    if(nameBox->text() != "")
        defaultFileName = nameBox->text() % ".txt";
    else
        defaultFileName = "ft-default.txt";

    QFlags<QIODevice::OpenModeFlag>
        flags = QIODevice::ReadWrite | QIODevice::Text;

    inFile.setQuietOnSuccess(true);
    if((status = inFile.get(defaultFileName, flags)) != qpfile::fFailed)
        stream.setDevice(&inFile);
    else {
        stream.setString(&defStr);
        msgHandler->sendInfo("Using built-in default test parameters.");
    }

    if((status == qpfile::fCreated) && builtin)
        stream << defStr;

    stream.seek(0);
    return status;
}

/*********************************************************************
 *
 */
void FracTest::getDefaults()
{
    QTextStream in;
    QpFile file(msgHandler);

    openDefaults(file, in, true);
    setDefaults(in);
    if(file.exists())
        file.close();
}

/*********************************************************************
 *
 */
void FracTest::setDefaults(QTextStream& in)
{
    QString buff;

    // Get the Test Parameters from the text stream
    //
    for(int i = 0; i < ft_tests_end; ++i) {
        in >> buff;
        testChkBoxList[i]->setChecked(QString(buff).toInt());
        in >> buff;
        quanEditList[i]->setText(buff);
    }

    // Get the Grade Level
    //
    in >> buff;
    levelList[QString(buff).toInt()]->setChecked(true);

    // Get the userName, but only if there isn't one in the username edit box.
    //
    in >> buff;
    if(nameBox->text() == "")
        nameBox->setText(buff);
}

/*********************************************************************
 *
 */
void FracTest::getMaxops()
{
    // The following are the default max and min values for the operands
    // for the respective arithmetic operations and grade levels, as well
    // as the maximum and minimum number of terms for each problem.
    //
    // The maximum number of terms is first, so we know how many of the
    // next numbers in the stream belong to each problem.
    //
    //  Max Operand Values for the given grade levels.
    //
    //         Level    1       2       3
    //  LCM
    //      maxTerms    2       3       3
    //      minTerms    2       2       2
    //
    //      op1 max    16      24      32
    //          min     0       0       0
    //
    //      op2 max    16      24      32
    //          min     0       0       0
    //
    //      op3 max   n/a      16      24
    //          min   n/a       0       0
    //
    //  Reduce
    //      maxTerms    2       2       2
    //      minTerms    2       2       2
    //
    //      op1 max     8      16      32
    //          min     0       0       0
    //
    //      op2 max     8      16      32
    //          min     0       0       0
    //
    //  Combine
    //      maxTerms    4       6       6
    //      minTerms    4       4       6
    //
    //      op1 max     8      16      32
    //          min     0       0       0
    //
    //      op2 max     8      16      32
    //          min     0       0       0
    //
    QString qsMaxops = QString(
        // LCM (Lowest Common Multiple
        "  2  2 10  0 10  0 \n"           // Level 1
        "  3  2 10  0 10  0 10  0 \n"     // Level 2
        "  3  2 16  0 10  0 10  0 \n"     // Level 3
        // Reduce terms
        "  3  3  8  0  8  0 10  2 \n"           // Level 1
        "  3  3 16  0 16  0 10  2 \n"           // Level 2
        "  3  3 32  0 32  0 10  2 \n"           // Level 3
        // Combine terms
        "  4  4  8  0  8  0  8  0  8  0 \n"             // Level 1
        "  6  4 16  0 16  0 16  0 16  0 16  0 16  0 \n" // Level 2
        "  6  6 32  0 32  0 32  0 32  0 32  0 32  0 \n" // Level 3
        );

    int status;
    QString buff;
    QTextStream inStream;
    QpFile maxopsFile(msgHandler);
    QString maxopsFileName = "ft-maxops.txt";

    // Get the pointer to the TestParmManager and a reference to the parameter
    // list of TestParm classes that was created when the TestParmManager was
    // instantiated.
    //
    TestParmManager* ptm = testParmManager;
    QList<TestParm*> tpList = ptm->getTestParmList();

    QFlags<QIODevice::OpenModeFlag>
        flags = QIODevice::ReadWrite | QIODevice::Text;

    if((status = maxopsFile.get(maxopsFileName, flags, true)) != qpfile::fFailed)
        inStream.setDevice(&maxopsFile);
    else
        inStream.setString(&qsMaxops);

    // If the file did not previously exist, then it has just been created
    // and has no data. So take the data from the default string.
    //
    if(status == qpfile::fCreated)
        inStream << qsMaxops;

    inStream.seek(0);

    // Get the Operand Limits
    //
    buff.clear();

    // For each test ...
    // The operand limits are stored by levels for each test.
    //
    for(int j = 0; j < ft_tests_end; ++j) {

        // The vectors of int vectors must be resized to their
        // outermost dimenstion, first.
        //
        tpList[j]->maxvals.resize(ft_lvl_end);
        tpList[j]->minvals.resize(ft_lvl_end);

        // Get the maxterms and minterms for this particluar test.
        // The maxterms also tells us how many maxval/minval pairs follow.
        //
        for(int k = 0; k < ft_lvl_end; ++k) {

            inStream >> buff;
            tpList[j]->maxterms << buff.toInt();
            inStream >> buff;
            tpList[j]->minterms << buff.toInt();

            // Get the max/min operand values for this level of this test
            //
            for(int m = 0; m < tpList[j]->maxterms[k]; ++m) {
                inStream >> buff;
                tpList[j]->maxvals[k] << buff.toInt();
                inStream >> buff;
                tpList[j]->minvals[k] << buff.toInt();
            }
        }
    }

    if(maxopsFile.exists())
        maxopsFile.close();
}

/*********************************************************************
 *
 */
void FracTest::buffToInt(QTextStream& str, QVector<int>& list)
{
    QString buff;

    str.seek(0);
    while(! str.atEnd()) {
        str >> buff;
        list << buff.toInt();
    }
    // The "\n" is read as a NULL string, and hence converted to a
    // zero, so we will have one more item in the list than we
    // want. To fix that, we will simply remove this last zero
    // item.
    //
    if(list.last() == 0)
        list.removeLast();
}

/*********************************************************************
 *
 */
int FracTest::getProblemCount()
{
    return testParmManager->getTotalCount();
}


/*********************************************************************
 *
 */
QString FracTest::getFinalScore()
{
    return testParmManager->getFinalScore();
}

/*********************************************************************
 *
 */
void FracTest::setUserName(const QString& name)
{
    nameBox->setText(name);
    onNameBox();
}
