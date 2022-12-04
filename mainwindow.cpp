#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Utility.h"
#include <time.h>
#include <QFileDialog>
#include <QHeaderView>
#include <string>
#include <QThread>
#include <QString>
#include <waitdialog.h>
#include <QAbstractItemModel>
#include <lengthdialog.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->actionShowAnswers->setEnabled(m_bShowUserBoard);
    ui->actionHideAnswers->setEnabled(!m_bShowUserBoard);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetActiveDocument(const Document& doc)
{
    m_ActiveDocument = doc;
}

void MainWindow::ClearCrossTable()
{
    auto board = m_ActiveDocument.GetBoard();

    ui->CrossTable->setColumnCount(m_ActiveDocument.GetBoard()->GetWidth());
    ui->CrossTable->setRowCount(m_ActiveDocument.GetBoard()->GetHeight());

    for(int i = 0; i < m_ActiveDocument.GetBoard()->GetHeight(); i++)
    {
        ui->CrossTable->setRowHeight(i, m_nCellHeight);
    }

    for(int i = 0; i < m_ActiveDocument.GetBoard()->GetWidth(); i++)
    {
        ui->CrossTable->setColumnWidth(i, m_nCellWidth);
    }

    for(int i = 0;i < board->GetHeight(); i++)
    {
        for(int j = 0; j< board->GetWidth(); j++)
        {
            auto* tableItem = new QTableWidgetItem;
            tableItem->setTextAlignment(Qt::AlignCenter);
            tableItem->setFont(QFont ("Times New Roman",12));
            ui->CrossTable->setItem(i, j, tableItem);
        }
    }
;}

void MainWindow::FormatCrossTable()
{
    for(int i = 0; i < m_ActiveDocument.GetBoard()->GetHeight(); i++)
    {
        ui->CrossTable->setRowHeight(i, m_nCellHeight);
    }

    for(int i = 0; i < m_ActiveDocument.GetBoard()->GetWidth(); i++)
    {
        ui->CrossTable->setColumnWidth(i, m_nCellWidth);
    }

    auto board = m_ActiveDocument.GetBoard();

    for(int i = 0;i < board->GetHeight(); i++)
    {
        for(int j = 0; j< board->GetWidth(); j++)
        {
            QTableWidgetItem* tableItem = ui->CrossTable->item(i,j);
            tableItem->setFont(QFont ("Times New Roman",12));
            tableItem->setTextAlignment(Qt::AlignCenter);
        }
    }

}

void MainWindow::ShowAnswers()
{
    for(int i = 0;i < m_ActiveDocument.GetBoard()->GetHeight(); i++)
        for(int j = 0; j < m_ActiveDocument.GetBoard()->GetWidth(); j++)
        {
            char letter = m_ActiveDocument.GetBoard()->GetLetterFromCell(Cell(i,j));
            if(letter == '#' || letter =='.' || letter == '_')
                continue;

            auto cell = ui->CrossTable->item(i,j);
            cell->setFont(QFont ("Times New Roman",12));
            cell->setTextAlignment(Qt::AlignCenter);
            cell->setText(QString(letter));
            ui->CrossTable->setItem(i,j,cell);
        }
}

void MainWindow::ShowQuestions()
{
    ui->WordList->clear();

    for(int i = 0;i < m_ActiveDocument.GetBoard()->GetQuestions().size(); i++)
    {
        new QListWidgetItem(tr(m_ActiveDocument.GetBoard()->GetQuestions()[i].question.c_str()), ui->WordList);
    }

    for(int i = 0;i < m_ActiveDocument.GetBoard()->GetQuestions().size(); i++)
    {
        Question currentQuestion = m_ActiveDocument.GetBoard()->GetQuestions()[i];

        QTableWidgetItem* questionCell = new QTableWidgetItem;
        questionCell->setFlags(Qt::NoItemFlags);
        questionCell->setTextAlignment(Qt::AlignCenter);

        if(currentQuestion.question.empty())
        {
            questionCell->setFont(QFont("Times New Roman", 12));
            questionCell->setText("Q");
        }
        else
        {
            int charInLine = 10;
            MultiLineText temp(currentQuestion.question,charInLine);
            questionCell->setFont(QFont("Times New Roman", 8));
            questionCell->setText(QString::fromStdString(temp.GetString()));
        }

        ui->CrossTable->setItem(currentQuestion.questionPos.m_row, currentQuestion.questionPos.m_col, questionCell);

        QString imagePath = "./img/" + QString::fromStdString(Direction::GetImagePathByArrow(currentQuestion.direction));
        if(imagePath != "./img/")
        {
            QImage backgroundIcon(imagePath);
            QTableWidgetItem* arrow = ui->CrossTable->item(currentQuestion.start.m_row,currentQuestion.start.m_col);
            arrow->setBackground(QBrush(backgroundIcon.scaled(m_nCellWidth,m_nCellHeight,Qt::AspectRatioMode::KeepAspectRatio,Qt::TransformationMode::SmoothTransformation)));
            ui->CrossTable->setItem(currentQuestion.questionPos.m_row, currentQuestion.questionPos.m_col, arrow);
        }

    }
}

void MainWindow::ShowUserAnswers()
{
    for(int i = 0;i < m_UserBoard.GetHeight(); i++)
        for(int j = 0; j < m_UserBoard.GetWidth(); j++)
        {
            char letter = m_UserBoard.GetLetterFromCell(Cell(i,j));
            if(letter == '#' || letter =='.' || letter == '_')
                continue; 

            auto cell = ui->CrossTable->item(i,j);
            cell->setFont(QFont ("Times New Roman",12));
            cell->setTextAlignment(Qt::AlignCenter);
            cell->setText(QString(letter));
            ui->CrossTable->setItem(i,j,cell);
        }
}

void MainWindow::ShowActiveDocument()
{
    ClearCrossTable();

    if(!m_bShowUserBoard)
        ShowAnswers();
    else
        ShowUserAnswers();

    ShowQuestions();
}

void MainWindow::CopyMainToUserBoard()
{
    m_UserBoard = Board(m_ActiveDocument.GetBoard()->GetHeight(), m_ActiveDocument.GetBoard()->GetWidth());

    auto currentBoard = m_ActiveDocument.GetBoard();

    for(int i =0; i < currentBoard->GetQuestions().size(); i++)
    {
        auto currentQuestion = currentBoard->GetQuestions()[i];
        m_UserBoard.PutLetterInCell(Cell(currentQuestion.questionPos.m_col,currentQuestion.questionPos.m_row), '#');
    }
}

void MainWindow::CheckAnswers()
{

    auto solvedBoard = m_ActiveDocument.GetBoard();

    for(int i = 0; i < solvedBoard->GetQuestions().size(); i++)
    {
        bool isSolved = false;
        bool isComplete = false;

        std::string userWord = "";

        Question currentQuestion = solvedBoard->GetQuestions()[i];

        for(auto currentCell = currentQuestion.start; currentCell != currentQuestion.end + Direction::GetDirectionArrow(currentQuestion.direction).normVec; currentCell += Direction::GetDirectionArrow(currentQuestion.direction).normVec)
        {
            userWord += m_UserBoard.GetLetterFromCell(Cell(currentCell.m_row, currentCell.m_col));
        }

        if (userWord.find('.') == std::string::npos && userWord.find('_') == std::string::npos)
            isComplete = true;

        if(isComplete && userWord == currentQuestion.answer)
            isSolved = true;

        for(auto currentCell = currentQuestion.start; currentCell != currentQuestion.end  + Direction::GetDirectionArrow(currentQuestion.direction).normVec; currentCell += Direction::GetDirectionArrow(currentQuestion.direction).normVec)
        {
            auto cell = ui->CrossTable->item(currentCell.m_row, currentCell.m_col);

            QBrush brush;

            brush.setStyle(Qt::BrushStyle::SolidPattern);

            if(isSolved)
            {
                brush.setColor(Qt::GlobalColor::green);

            }
            else
            {
                brush.setColor(Qt::GlobalColor::red);
            }

            if(isComplete)
                cell->setForeground(brush);

            ui->CrossTable->setItem(currentCell.m_row, currentCell.m_col, cell);
        }
    }
}

void MainWindow::on_actionCreate_triggered()
{
    CreateCrosswordDlg dlg;
    dlg.setModal(true);
    Board temp;
    dlg.CreateBoard(&temp);
    std::shared_ptr<Board> pBoard = std::make_shared<Board>(temp);

    m_ActiveDocument.SetBoard(pBoard);

    CopyMainToUserBoard();

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
    //m_ActiveDocument->SetBoard(pathToFile);
    ShowActiveDocument();

    CheckAnswers();
}

void MainWindow::on_actionSave_triggered()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);;
    auto pathToFile = dialog.getSaveFileName();
    if(m_ActiveDocument.GetBoard())
        m_ActiveDocument.GetBoard()->SaveAsJSONFile(pathToFile);
    ShowActiveDocument();
}

void MainWindow::on_actionGenerate_Empy_Board_triggered()
{
    m_ActiveDocument.GetBoard()->CleanBoard();

    LengthDialog dlg(this);

    int min,max,seed;

    if(!dlg.ShowDialog(&min, &max, &seed))
        return;

    WaitDialog spiner(this);

    m_myThread = QThread::create([&](){
        bool isBuilded = false;
        // 10 x 10
        // 3 - 7 seed 10
        // 3 - 5 seed 13, 15, 17
        BoardBuilder builder(min, max, 100);
        while(!isBuilded)
        {
            try
            {
            builder.GenerateBoard(*m_ActiveDocument.GetBoard(), seed);
            ShowActiveDocument();
            isBuilded = true;
            }
            catch(...)
            {

            }}
        }
        );

    spiner.ExecFunction(m_myThread);

    this->setFocus();
}

void MainWindow::on_actionGenerate_Solved_Board_triggered()
{
    m_myThread = QThread::create([&](){

    PuzzleSolver solver("crossword_PDO.db");
    bool isBuilded = false;
    while(!isBuilded)
    try
    {

        solver.SolvePuzzle(*m_ActiveDocument.GetBoard(),11);
        ShowActiveDocument();
        ShowQuestions();
        isBuilded = true;
    }
    catch(...)
    {

    }});
    WaitDialog waitDlg(this);

    waitDlg.ExecFunction(m_myThread);
    this->setFocus();
}

void MainWindow::on_WordList_itemClicked(QListWidgetItem *item)
{
    if(ui->WordList->currentRow() >= m_ActiveDocument.GetBoard()->GetQuestions().size())
        return;
   Question q = m_ActiveDocument.GetBoard()->GetQuestions()[ui->WordList->currentRow()];
    if (ui->WordList->isActiveWindow())
    {
        ui->CrossTable->setRangeSelected(QTableWidgetSelectionRange(q.start.m_row,q.start.m_col,q.end.m_row,q.end.m_col),true);
    }
}

void MainWindow::SelectActiveCell()
{
    ui->CrossTable->setCurrentCell(m_CurrentCell.m_row,m_CurrentCell.m_col);
}

void MainWindow::on_CrossTable_cellClicked(int row, int column)
{
    auto questions = m_ActiveDocument.GetBoard()->GetQuestions();
    for(int i = 0; i < questions.size(); i++)
    {
        if(questions[i].questionPos.m_row == row && questions[i].questionPos.m_col == column)
        {
            m_NormVectorAnswer = Direction::GetDirectionArrow(questions[i].direction).normVec;
            m_CurrentCell = Cell(row,column) + Direction::GetDirectionArrow(questions[i].direction).offset;
            SelectActiveCell();
            return;
        }
    }
    m_CurrentCell = Cell(row, column);
}

void MainWindow::NextActiveCell()
{
    Cell nextCell = m_CurrentCell + m_NormVectorAnswer;

    auto nextItem = ui->CrossTable->item(nextCell.m_row, nextCell.m_col);

    if(m_ActiveDocument.GetBoard()->IsCellOnBoard(nextCell) && nextItem->flags() != Qt::NoItemFlags)
        m_CurrentCell = nextCell;
}

void MainWindow::on_CrossTable_cellChanged(int row, int column)
{

   if(ui->CrossTable->item(row,column)->flags() == Qt::NoItemFlags)
        return;

    QString text =  ui->CrossTable->item(row,column)->text();

    if(text.length() < 1)
        return;

    if(!m_bShowUserBoard)
    {
        ui->CrossTable->item(row,column)->setText(QChar(m_ActiveDocument.GetBoard()->GetLetterFromCell(Cell(row,column))));
        return;
    }


    ui->CrossTable->item(row,column)->setText(text.at(0));

    m_UserBoard.PutLetterInCell(Cell(row,column), text.at(0).toLatin1());


    NextActiveCell();
    SelectActiveCell();

    CheckAnswers();
}


void MainWindow::on_actionShowAnswers_triggered()
{
    m_bShowUserBoard = false;
    ui->actionShowAnswers->setEnabled(m_bShowUserBoard);
    ui->actionHideAnswers->setEnabled(!m_bShowUserBoard);

    ShowActiveDocument();
}


void MainWindow::on_actionHideAnswers_triggered()
{
    m_bShowUserBoard = true;
    ui->actionShowAnswers->setEnabled(m_bShowUserBoard);
    ui->actionHideAnswers->setEnabled(!m_bShowUserBoard);

    ShowActiveDocument();

}

