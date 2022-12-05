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

    void ClearCrossTable();

    void ShowQuestions();
    void ShowAnswers();
    void ShowUserAnswers();

    void NextActiveCell();

    void CheckAnswers();

    void FormatCrossTable();

    void ShowActiveDocument();

    void SelectActiveCell();
    void SetCellSize(int height, int width);

    void SetActiveDocument(const Document&);
    Document* GetActiveDocument();

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionCreate_triggered();

    void on_actionSave_triggered();

    void on_actionGenerate_Empy_Board_triggered();

    void on_actionGenerate_Solved_Board_triggered();

    void on_WordList_itemClicked(QListWidgetItem *item);

    void on_CrossTable_cellClicked(int row, int column);

    void on_CrossTable_cellChanged(int row, int column);

    void on_actionShowAnswers_triggered();

    void on_actionHideAnswers_triggered();

private:

    void CopyMainToUserBoard();

    Ui::MainWindow *ui;
    Document m_ActiveDocument;

    Cell m_NormVectorAnswer;
    Cell m_CurrentCell;

    int m_nCellWidth = 60;
    int m_nCellHeight = 60;

    bool m_bShowUserBoard = true;
    enum ViewType
    {
        WithAnswers = 1,
        NoAnswers = 2
    } m_ViewType;

    QThread * m_myThread;

};
#endif // MAINWINDOW_H
