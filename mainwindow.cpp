#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Disassembler.h"
#include "Assembler.h"

#include <QDebug>
#include <QPushButton>
#include <QLineEdit>
#include <QDialog>
#include <QWidget>
#include <QGridLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QDateTime>
#include <QTimer>
#include <QFont>
#include <QIcon>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(QSize(600, 400));

    font.setKerning(true);
    font.setPointSize(12);
    font.setFixedPitch(true);

    QTimer *timer = new QTimer(this);
    timer->start(1000);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(ui->textEdit);
    ui->centralWidget->setLayout(layout);

    isUntitled = true;
    curFile = "untitled.txt";
    setWindowTitle(curFile);
    setWindowIcon(QIcon("myIcon.ico"));

    findDlg = new QDialog(this);
    findDlg->setWindowTitle(tr("Find"));
    findLineEdit = new QLineEdit(findDlg);
    QPushButton *btn = new QPushButton(tr("Find next"), findDlg);
    QVBoxLayout *layoutV = new QVBoxLayout(findDlg);
    layoutV->addWidget(findLineEdit);
    layoutV->addWidget(btn);

    helpDlg = new QDialog(this);
    helpDlg->setWindowTitle(tr("Imprint"));
    helpDlg->setMinimumSize(750, 400);
    helpTextEdit = new QTextEdit(helpDlg);
    helpTextEdit->setReadOnly(true);
    helpTextEdit->setFont(font);
    QGridLayout *helpLayout = new QGridLayout(helpDlg);
    helpLayout->addWidget(helpTextEdit);
    helpTextEdit->setPlainText(imprint);

    connect(btn, &QPushButton::clicked, this, &MainWindow::showFindText);
    connect(timer, &QTimer::timeout, this, &MainWindow::timeUpdate);

    permanent = new QLabel;
    permanent->setFrameStyle(QFrame::Box | QFrame::Sunken | QFrame::WinPanel);
    ui->statusBar->addPermanentWidget(permanent);
}

void MainWindow::newFile()
{
    if(mayBeSave())
    {
        isUntitled = true;
        curFile = "untitled.txt";
        setWindowTitle(curFile);
        ui->textEdit->clear();
        ui->textEdit->setVisible(true);
    }
}

bool MainWindow::mayBeSave()
{   //If the file is modified
    if(ui->textEdit->document()->isModified())
    {
        QMessageBox box;
        box.setWindowTitle(tr("warning"));
        box.setIcon(QMessageBox::Warning);
        box.setText(curFile + tr(" has not been saved. Save it?"));
        QPushButton *yesbtn = box.addButton(tr("Yes(&Y)"), QMessageBox::YesRole);
        box.addButton(tr("No(N)"), QMessageBox::NoRole);
        QPushButton *cancelBut = box.addButton(tr("Cancel"), QMessageBox::RejectRole);
        box.exec();

        if(box.clickedButton() == yesbtn)
            return save();
        else if(box.clickedButton() == cancelBut)
            return false;
    }

    return true;//If the file is not modified, return true directly.
}

bool MainWindow::save()
{
    if(isUntitled)
        return saveAs();
    else
        return saveFile(curFile);
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("save as"), curFile);
    if(fileName.isEmpty() == true)
        return false;
    else
        return saveFile(fileName);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);

    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Multiple document editor"), tr("unable to write in %1: \n %2").arg(fileName).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);//mouse pointer changes into waiting status
    out << ui->textEdit->toPlainText();
    QApplication::restoreOverrideCursor();//restore mouse pointer
    isUntitled = false;
    curFile = QFileInfo(fileName).canonicalFilePath();//get the standard path of the file
    setWindowTitle(curFile);

    return true;
}

bool MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Multiple file editor"), tr("Unable to read the file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return false;
    }
    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->textEdit->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();

    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);

    return true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //if mayBeSave is true, then close.
    if(mayBeSave())
        event->accept();
    else
        event->ignore();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_New_triggered()
{
    newFile();
}

void MainWindow::on_action_Save_triggered()
{
    save();
}

void MainWindow::on_action_SaveAs_triggered()
{
    saveAs();
}

void MainWindow::on_action_Open_triggered()
{
    if(mayBeSave())
    {
        QString fileName = QFileDialog::getOpenFileName(this);
        //if the file name is not empty, load the file
        if(!fileName.isEmpty())
        {
            loadFile(fileName);
            ui->textEdit->setVisible(true);
        }
    }
}

void MainWindow::on_action_Close_triggered()
{
    if(mayBeSave())
    {
        ui->textEdit->setVisible(false);
    }
}

void MainWindow::on_action_Exit_triggered()
{
    //Close at first, then exit.
    //qApp is the global pointer to the application
    on_action_Close_triggered();
    qApp->quit();
}

void MainWindow::on_action_Undo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_action_Cut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_action_Copy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_action_Paste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::showFindText()
{
    QString str = findLineEdit->text();
    if(!ui->textEdit->find(str, QTextDocument::FindBackward))
    {
        QMessageBox::warning(this, tr("Find"), tr("Unable to find %1").arg(str));
    }
}

void MainWindow::on_action_Find_triggered()
{
    findDlg->show();
}

void MainWindow::timeUpdate()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yy-MM-dd hh-mm-ss dddd");
    permanent->setText(str);
}

void MainWindow::on_action_Help_triggered()
{
    helpDlg->show();
}

void MainWindow::on_actionBold_B_triggered()
{
    textFont.setBold(true);
    textFont.setKerning(false);
    textFont.setItalic(false);
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_actionKerning_K_triggered()
{
    textFont.setKerning(true);
    textFont.setBold(false);
    textFont.setItalic(false);
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_actionItalic_I_triggered()
{
    textFont.setItalic(true);
    textFont.setKerning(false);
    textFont.setBold(false);
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_action8_triggered()
{
    textFont.setPointSize(8);
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_action9_triggered()
{
    textFont.setPointSize(9);
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_action10_triggered()
{
    textFont.setPointSize(10);
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_action11_triggered()
{
    textFont.setPointSize(11);
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_action12_triggered()
{
    textFont.setPointSize(12);
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_action13_triggered()
{
    textFont.setPointSize(13);
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_action14_triggered()
{
    textFont.setPointSize(14);
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_action15_triggered()
{
    textFont.setPointSize(15);
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_action16_triggered()
{
    textFont.setPointSize(16);
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_action17_triggered()
{
    textFont.setPointSize(17);
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_action18_triggered()
{
    textFont.setPointSize(18);
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_action19_triggered()
{
    textFont.setPointSize(19);
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_action20_triggered()
{
    textFont.setPointSize(20);
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_actionRed_R_triggered()
{
    ui->textEdit->setTextColor(Qt::red);
}

void MainWindow::on_actionGreen_G_triggered()
{
    ui->textEdit->setTextColor(Qt::green);
}

void MainWindow::on_actionYellow_Y_triggered()
{
    ui->textEdit->setTextColor(Qt::yellow);
}

void MainWindow::on_actionBlack_B_triggered()
{
    ui->textEdit->setTextColor(Qt::black);
}

void MainWindow::on_actionBlue_triggered()
{
    ui->textEdit->setTextColor(Qt::blue);
}

void MainWindow::on_actionGray_triggered()
{
    ui->textEdit->setTextColor(Qt::gray);
}

void MainWindow::on_actionSong_triggered()
{
    textFont.setFamily("宋体");
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_actionConsolas_triggered()
{
    textFont.setFamily("Consolas");
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_actionCalibri_triggered()
{
    textFont.setFamily("Calibri");
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_actionKai_triggered()
{
    textFont.setFamily("楷体");
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_actionComic_Sans_MS_triggered()
{
    textFont.setFamily("Comic Sans MS");
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_actionArial_triggered()
{
    textFont.setFamily("Arial");
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_actionConstantia_triggered()
{
    textFont.setFamily("Constantia");
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_actionBerlin_Sans_FB_triggered()
{
    textFont.setFamily("Berlin Sans FB");
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_actionMicrosoft_Accor_Black_triggered()
{
    textFont.setFamily("微软雅黑");
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_actionTekton_Pro_Cond_triggered()
{
    textFont.setFamily("Tekton Pro Cond");
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_actionPoor_Richard_triggered()
{
    textFont.setFamily("Poor Richard");
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_actionForte_triggered()
{
    textFont.setFamily("Forte");
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_actionBook_Antiqua_triggered()
{
    textFont.setFamily("Book Antiqua");
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_actionTimes_New_Roman_triggered()
{
    textFont.setFamily("Times New Roman");
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_actionTahoma_triggered()
{
    textFont.setFamily("Tahoma");
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_actionWide_Latin_triggered()
{
    textFont.setFamily("Wide Latin");
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_actionDFKai_triggered()
{
    textFont.setFamily("DFKai-SB");
    ui->textEdit->setFont(textFont);
}

bool MainWindow::assemble()
{
    QString fileName;
    QString outFile;
    string fileName1;
    string outFile1;

    save();
    fileName = windowTitle();
    outFile = QFileDialog::getSaveFileName(this, tr("Save the binary file"), tr("untitle.coe"), tr("binary file(*bin *coe)"));

    if(outFile.isEmpty())
        return false;
    else
    {
        outFile1 = replace_all_distinct(outFile.toStdString(), "/", "//");
        fileName1 = replace_all_distinct(fileName.toStdString(), "/", "//");

        Assembler assembler(outFile1);
        ifstream inFile(fileName1);
        if(!inFile.is_open())
        {
            QMessageBox::critical(this, tr("error"), tr("Errors when opening file"), QMessageBox::Ok);
            return false;
        }
        if(!assembler.preTreatment(inFile))
        {
            QMessageBox::critical(this, tr("error"), tr("Errors when pretreating file"), QMessageBox::Ok);
            return false;
        }
        inFile.clear();
        inFile.seekg(0, ios::beg);
        if (!assembler.readLine(inFile))
        {
            QMessageBox::critical(this, tr("error"), tr("Errors when translating file"), QMessageBox::Ok);
            return false;
        }
        inFile.close();
        return true;
    }
}

string& MainWindow::replace_all_distinct(string& str, const string& old_value, const string& new_value)
{
    for(string::size_type pos(0); pos != string::npos; pos += new_value.length())
    {
        if((pos = str.find(old_value, pos)) != string::npos)
            str.replace(pos, old_value.length(), new_value);
        else break;
    }
    return str;
}

bool MainWindow::disassemble()
{
    QString fileName;
    QString outFile;
    string outFile1;
    string fileName1;
    save();
    fileName = windowTitle();
    outFile = QFileDialog::getSaveFileName(this, tr("Save the assembly file"), tr("untitle.asm"), tr("assembly file(*asm)"));

    if(outFile.isEmpty())
        return false;
    else
    {
        outFile1 = replace_all_distinct(outFile.toStdString(), "/", "//");
        fileName1 = replace_all_distinct(fileName.toStdString(), "/", "//");

        Disassembler disassembler(outFile1);
        ifstream inFile(fileName1);
        if(!inFile.is_open())
        {
            QMessageBox::critical(this, tr("error"), tr("Errors when opening file"), QMessageBox::Ok);
            return false;
        }
        if(!disassembler.preTreatment(inFile))
        {
            QMessageBox::critical(this, tr("error"), tr("Errors when pretreating file"), QMessageBox::Ok);
            return false;
        }
        inFile.clear();
        inFile.seekg(0, ios::beg);
        if (!disassembler.readLine(inFile))
        {
            QMessageBox::critical(this, tr("error"), tr("Errors when translating file"), QMessageBox::Ok);
            return false;
        }
        inFile.close();
        return true;
    }
}

void MainWindow::on_actionAssemble_triggered()
{
    if(assemble())
        QMessageBox::information(this, tr("Assemble"), tr("Assemble successful!"), QMessageBox::Ok);
}

void MainWindow::on_actionDisassemble_D_triggered()
{
    if(disassemble())
        QMessageBox::information(this, tr("Disassemble"), tr("Disassemble successful!"), QMessageBox::Ok);
}
