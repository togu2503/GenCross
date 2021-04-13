#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <time.h>
#include <QFileDialog>
#include <QHeaderView>
#include <string>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// returns original board NOT copy
Board& MainWindow::GetActiveDocument()
{
    return m_ActiveDocument;
}

void MainWindow::SetActiveDocument(Document doc)
{
    m_ActiveDocument = doc;
}
void MainWindow::ShowActiveDocument()
{
    int cellHeight = 60;
    int cellWidth = 60;
    ui->CrossTable->setColumnCount(m_ActiveDocument.GetWidth());
    ui->CrossTable->setRowCount(m_ActiveDocument.GetHeight());

    for(int i = 0; i < m_ActiveDocument.GetHeight(); i++)
    {
        ui->CrossTable->setRowHeight(i,cellHeight);
    }

    for(int i = 0; i < m_ActiveDocument.GetWidth(); i++)
    {
        ui->CrossTable->setColumnWidth(i,cellWidth);
    }


    for(int i=0;i<m_ActiveDocument.GetAllQuestions().size();i++)
    {
        Question currentQuestion = m_ActiveDocument.GetQuestion(i);
        int charInLine = 10;
        MultiLineText temp(currentQuestion.question,charInLine);

        QTableWidgetItem* questionCell = new QTableWidgetItem;
        questionCell->setText(QString::fromStdString(temp.GetString()));
        questionCell->setTextAlignment(Qt::AlignCenter);
        questionCell->setFont(QFont("Times New Roman",8));
        ui->CrossTable->setItem(currentQuestion.questionPos.Row,currentQuestion.questionPos.Col,questionCell);

        QString imagePath = "./img/" + QString::fromStdString(GetImagePathByArrow(currentQuestion.direction));
        QImage backgroundIcon(imagePath);
        QTableWidgetItem* arrow = new QTableWidgetItem;
        arrow->setBackground(QBrush(backgroundIcon.scaled(cellWidth,cellHeight,Qt::AspectRatioMode::KeepAspectRatio,Qt::TransformationMode::SmoothTransformation)));
        arrow->setFont(QFont ("Times New Roman",12));
        arrow->setTextAlignment(Qt::AlignCenter);
        ui->CrossTable->setItem(currentQuestion.start.Row,currentQuestion.start.Col,arrow);
    }
    //ui->CrossTable->resizeRowsToContents();
    //ui->CrossTable->resizeColumnsToContents();
    ShowQuestions();
}

void MainWindow::on_actionCreate_triggered()
{
    CreateCrosswordDlg dlg;
    dlg.setModal(true);
    m_ActiveDocument.SetBoard(dlg.CreateBoard());
    ui->CrossTable->clear();
    ShowActiveDocument();
}

void MainWindow::on_actionOpen_triggered()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    auto pathToFile = dialog.getOpenFileName().toStdString();
    std::ifstream f(pathToFile);
    if(!f.good())
        return;
    m_ActiveDocument.SetBoard(pathToFile);
    ui->CrossTable->clear();
    ShowActiveDocument();
}

void MainWindow::on_actionSave_triggered()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);;
    auto pathToFile = dialog.getSaveFileName().toStdString();
    m_ActiveDocument.ExportToFile(pathToFile);
    ShowActiveDocument();
}

void MainWindow::on_actionGenerate_Empy_Board_triggered()
{
    m_ActiveDocument.ClearQuestions();
    m_ActiveDocument.ClearBoard();
    BoardBuilder builder(m_ActiveDocument,3,5);
    bool isBuilded = false;
    while(!isBuilded)
    try
    {
    builder.generateBoard(time(NULL));
    m_ActiveDocument.SetBoard(builder.getBoard());
    ui->CrossTable->clear();
    ShowActiveDocument();
    isBuilded = true;
    }
    catch(...)
    {

    }

}

void MainWindow::on_actionGenerate_Solved_Board_triggered()
{
    PuzzleSolver solver(m_ActiveDocument,m_ActiveDocument.GetAllQuestions(),"boardbuilder.db");
    bool isBuilded = false;
    while(!isBuilded)
    try
    {
        solver.setSeed(25);
        solver.solvePuzzle();
        ui->CrossTable->clear();
        ShowActiveDocument();
        ShowQuestions();
        isBuilded = true;
    }
    catch(...)
    {

    }
}

void MainWindow::ShowQuestions()
{
    for(int i=0;i<m_ActiveDocument.GetAllQuestions().size();i++)
    {
        new QListWidgetItem(tr(m_ActiveDocument.GetQuestion(i).answer.c_str()), ui->WordList);
    }
}

void MainWindow::on_WordList_itemClicked(QListWidgetItem *item)
{
    Question q = m_ActiveDocument.GetQuestion(ui->WordList->currentRow());
    if (ui->WordList->isActiveWindow())
    {
        ui->CrossTable->clearSelection();
        ui->CrossTable->setRangeSelected(QTableWidgetSelectionRange(q.start.Row,q.start.Col,q.end.Row,q.end.Col),true);
    }


}
