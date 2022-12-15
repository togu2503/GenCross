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

    void ChangeEvent(QEvent*);

    void ClearCrossTable();

    void ShowActiveDocument();
    void ShowQuestions();
    void ShowAnswers();
    void ShowUserAnswers();
    void FormatCrossTable();

    void NextActiveCell();
    void SelectActiveCell();

    void CheckAnswers();

    void SetCellSize(int height, int width);

    void SetActiveDocument(const Document&);
    Document* GetActiveDocument();

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    void LoadLanguage(const QString& rLanguage);

protected slots:
    void LanguageChanged(QAction* action);

private slots:
    void on_actionOpen_triggered();
    void on_actionCreate_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionExit_triggered();

    void on_actionGenerate_Empy_Board_triggered();
    void on_actionGenerate_Solved_Board_triggered();

    void on_WordList_itemClicked(QListWidgetItem *item);
    void on_CrossTable_cellClicked(int row, int column);
    void on_CrossTable_cellChanged(int row, int column);
    void on_actionShowAnswers_triggered();
    void on_actionHideAnswers_triggered();

private:

    void CreateLanguageMenu();

    void ResizeToBoard();

    void switchTranslator(QTranslator& translator, const QString& filename);

    void CopyMainToUserBoard();

    QTranslator m_Translator;
    QTranslator m_TranslatorQt;
    QString m_CurrentLanguage;
    QString m_LanguagesPath;

    Ui::MainWindow *ui;
    Document m_ActiveDocument;

    Cell m_NormVectorAnswer;
    Cell m_CurrentCell;

    bool m_bShowUserBoard = true;
    enum ViewType
    {
        WithAnswers = 1,
        NoAnswers = 2
    } m_ViewType;

    QThread * m_myThread;

};
#endif // MAINWINDOW_H
