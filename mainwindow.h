#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QLabel>
#include <QTextEdit>
#include <string>

namespace Ui {
class MainWindow;
}

class QLineEdit;
class QDialog;
class QLabel;
const QString imprint = "Copyright (C) 2017 Ivan Shu\n"
                        "All Right Reserved\n";

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void newFile(); //new
    bool mayBeSave(); //determine whether to save
    bool save(); //save
    bool saveAs(); //save as
    bool saveFile(const QString &fileName); //save the file
    bool loadFile(const QString &fileName);//open the file
    bool assemble();
    bool disassemble();
    std::string& replace_all_distinct(std::string& str, const std::string& old_value, const std::string& new_value);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_action_New_triggered();

    void on_action_Save_triggered();

    void on_action_SaveAs_triggered();

    void on_action_Open_triggered();

    void on_action_Close_triggered();

    void on_action_Exit_triggered();

    void on_action_Undo_triggered();

    void on_action_Cut_triggered();

    void on_action_Copy_triggered();

    void on_action_Paste_triggered();

    void showFindText();

    void on_action_Find_triggered();

    void timeUpdate();

    void on_action_Help_triggered();

    void on_actionBold_B_triggered();

    void on_actionKerning_K_triggered();

    void on_actionItalic_I_triggered();

    void on_action8_triggered();

    void on_action9_triggered();

    void on_action10_triggered();

    void on_action11_triggered();

    void on_action12_triggered();

    void on_action13_triggered();

    void on_action14_triggered();

    void on_action15_triggered();

    void on_action16_triggered();

    void on_action17_triggered();

    void on_action18_triggered();

    void on_action19_triggered();

    void on_action20_triggered();

    void on_actionRed_R_triggered();

    void on_actionGreen_G_triggered();

    void on_actionYellow_Y_triggered();

    void on_actionBlack_B_triggered();

    void on_actionBlue_triggered();

    void on_actionGray_triggered();

    void on_actionSong_triggered();

    void on_actionConsolas_triggered();

    void on_actionCalibri_triggered();

    void on_actionKai_triggered();

    void on_actionComic_Sans_MS_triggered();

    void on_actionArial_triggered();

    void on_actionConstantia_triggered();

    void on_actionBerlin_Sans_FB_triggered();

    void on_actionMicrosoft_Accor_Black_triggered();

    void on_actionTekton_Pro_Cond_triggered();

    void on_actionPoor_Richard_triggered();

    void on_actionForte_triggered();

    void on_actionBook_Antiqua_triggered();

    void on_actionTimes_New_Roman_triggered();

    void on_actionTahoma_triggered();

    void on_actionWide_Latin_triggered();

    void on_actionDFKai_triggered();

    void on_actionAssemble_triggered();

    void on_actionDisassemble_D_triggered();

private:
    Ui::MainWindow *ui;
    bool isUntitled; //If true, the file is not saved
    QString curFile; //the path of the saved file
    QLineEdit *findLineEdit;
    QDialog *findDlg;
    QDialog *helpDlg;
    QTextEdit *helpTextEdit;
    QLabel *permanent;
    QFont font;
    QFont textFont;
    QPalette pa;
};

#endif // MAINWINDOW_H
