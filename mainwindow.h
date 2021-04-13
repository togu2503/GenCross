#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "createcrossworddlg.h"
#include "Document.h"
#include "BoardBuilder.h"
#include "PuzzleSolver.h"
#include <memory>
#include <QString>
#include <QMessageBox>
#include <fstream>
#include <QFileDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    void ShowQuestions();
    void ShowActiveDocument();

    void SetActiveDocument(Document);
    Board& GetActiveDocument();

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionCreate_triggered();

    void on_actionSave_triggered();

    void on_actionGenerate_Empy_Board_triggered();

    void on_actionGenerate_Solved_Board_triggered();

    void on_WordList_itemClicked(QListWidgetItem *item);

    void on_WordList_currentRowChanged(int currentRow);

private:
    Ui::MainWindow *ui;
    Document m_ActiveDocument;

};
#endif // MAINWINDOW_H
