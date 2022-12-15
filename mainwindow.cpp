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
#include <QActionGroup>
#include <lengthdialog.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    CreateLanguageMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetActiveDocument(const Document& doc)
{
    m_ActiveDocument = doc;
}

void MainWindow::ChangeEvent(QEvent* event) {
 if(0 != event) {
  switch(event->type()) {
   // this event is send if a translator is loaded
   case QEvent::LanguageChange:
    ui->retranslateUi(this);
    break;

   // this event is send, if the system, language changes
   case QEvent::LocaleChange:
   {
    QString locale = QLocale::system().name();
    locale.truncate(locale.lastIndexOf('_'));
    LoadLanguage(locale);
   }
   break;
  }
 }
 QMainWindow::changeEvent(event);
}

void MainWindow::CreateLanguageMenu() {
 QActionGroup* langGroup = new QActionGroup(ui->menuLanguage->parent());
 langGroup->setExclusive(true);

 connect(langGroup, SIGNAL (triggered(QAction *)), this, SLOT (LanguageChanged(QAction *)));

 //Ukrainian by default
 QString defaultLocale = "ua";

 m_LanguagesPath = QApplication::applicationDirPath();
 m_LanguagesPath.append("/languages");
 QDir dir(m_LanguagesPath);
 QStringList fileNames = dir.entryList(QStringList("Translation_*.qm"));

 for (int i = 0; i < fileNames.size(); ++i) {

  // get locale extracted by filename
  QString locale;
  locale = fileNames[i];
  locale.truncate(locale.lastIndexOf('.'));
  locale.remove(0, locale.lastIndexOf('_') + 1);

  QString lang;

  // Issue with ukrainian
  if(locale != "ua")
  {
     lang = QLocale::languageToString(QLocale(locale).language());
  }
  else
  {
      lang = "Українська";
  }
  QAction *action = new QAction(lang, this);
  action->setCheckable(true);
  action->setData(locale);

  ui->menuLanguage->addAction(action);
  langGroup->addAction(action);

  if (defaultLocale == locale) {
   action->setChecked(true);
   LoadLanguage(defaultLocale);
  }
 }
}


void MainWindow::LanguageChanged(QAction* action)
{
 if(0 != action) {
  // load the language dependant on the action content
  LoadLanguage(action->data().toString());
 }
}

void MainWindow::switchTranslator(QTranslator& translator, const QString& filename) {
 // remove the old translator
 qApp->removeTranslator(&translator);

 // load the new translator
QString path = QApplication::applicationDirPath();
    path.append("/languages/");
 if(translator.load(path + filename)) //Here Path and Filename has to be entered because the system didn't find the QM Files else
 {
     qApp->installTranslator(&translator);
     ui->retranslateUi(this);
 }
 }

void MainWindow::LoadLanguage(const QString& rLanguage) {
 if(m_CurrentLanguage != rLanguage) {
  m_CurrentLanguage = rLanguage;
  QLocale locale = QLocale(m_CurrentLanguage);
  QLocale::setDefault(locale);
  //QString languageName = QLocale::languageToString(locale.language());
  switchTranslator(m_Translator, QString("Translation_") + rLanguage + ".qm");
  switchTranslator(m_TranslatorQt, QString("Translation_") + rLanguage + ".qm");
 }
}

void MainWindow::ClearCrossTable()
{
    auto board = m_ActiveDocument.GetBoard();

    if(!board)
        return;

    ui->CrossTable->setColumnCount(m_ActiveDocument.GetBoard()->GetWidth());
    ui->CrossTable->setRowCount(m_ActiveDocument.GetBoard()->GetHeight());

    int nCellWidth = m_ActiveDocument.GetCellWidth();
    int nCellHeight = m_ActiveDocument.GetCellHeight();

    for(int i = 0; i < m_ActiveDocument.GetBoard()->GetHeight(); i++)
    {
        ui->CrossTable->setRowHeight(i, nCellHeight);
    }

    for(int i = 0; i < m_ActiveDocument.GetBoard()->GetWidth(); i++)
    {
        ui->CrossTable->setColumnWidth(i, nCellWidth);
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
    if(!m_ActiveDocument.GetBoard())
        return;

    int nCellWidth = m_ActiveDocument.GetCellWidth();
    int nCellHeight = m_ActiveDocument.GetCellHeight();

    ui->CrossTable->setColumnCount(m_ActiveDocument.GetBoard()->GetWidth());
    ui->CrossTable->setRowCount(m_ActiveDocument.GetBoard()->GetHeight());

    for(int i = 0; i < m_ActiveDocument.GetBoard()->GetHeight(); i++)
    {
        ui->CrossTable->setRowHeight(i, nCellHeight);
    }

    for(int i = 0; i < m_ActiveDocument.GetBoard()->GetWidth(); i++)
    {
        ui->CrossTable->setColumnWidth(i, nCellWidth);
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
    if(!m_ActiveDocument.GetBoard())
        return;

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
    if(!m_ActiveDocument.GetBoard())
        return;

    int nCellWidth = m_ActiveDocument.GetCellWidth();
    int nCellHeight = m_ActiveDocument.GetCellHeight();

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
            arrow->setBackground(QBrush(backgroundIcon.scaled(nCellWidth,nCellHeight,Qt::AspectRatioMode::KeepAspectRatio,Qt::TransformationMode::SmoothTransformation)));
            ui->CrossTable->setItem(currentQuestion.questionPos.m_row, currentQuestion.questionPos.m_col, arrow);
        }

    }
}

void MainWindow::ShowUserAnswers()
{
    if(!m_ActiveDocument.GetUserBoard())
        return;

    for(int i = 0;i < m_ActiveDocument.GetUserBoard()->GetHeight(); i++)
        for(int j = 0; j < m_ActiveDocument.GetUserBoard()->GetWidth(); j++)
        {
            char letter = m_ActiveDocument.GetUserBoard()->GetLetterFromCell(Cell(i,j));
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
    if(!m_ActiveDocument.GetBoard())
        return;

    ClearCrossTable();

    if(!m_bShowUserBoard)
        ShowAnswers();
    else
        ShowUserAnswers();

    ShowQuestions();
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
            userWord += m_ActiveDocument.GetUserBoard()->GetLetterFromCell(Cell(currentCell.m_row, currentCell.m_col));
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

void MainWindow::ResizeToBoard()
{
    int newWindowWidth = std::max(640,std::min(1366,m_ActiveDocument.GetCellWidth() * m_ActiveDocument.GetBoard()->GetWidth()+ 278));
    int newWindowHeight = std::max(420, std::min(768,m_ActiveDocument.GetCellHeight() * m_ActiveDocument.GetBoard()->GetHeight()+ 88));

    resize(newWindowWidth, newWindowHeight);
}

void MainWindow::SelectActiveCell()
{
    ui->CrossTable->setCurrentCell(m_CurrentCell.m_row,m_CurrentCell.m_col);
}

void MainWindow::NextActiveCell()
{
    Cell nextCell = m_CurrentCell + m_NormVectorAnswer;

    auto nextItem = ui->CrossTable->item(nextCell.m_row, nextCell.m_col);

    if(m_ActiveDocument.GetBoard()->IsCellOnBoard(nextCell) && nextItem->flags() != Qt::NoItemFlags)
        m_CurrentCell = nextCell;
}

void MainWindow::on_actionCreate_triggered()
{
    CreateCrosswordDlg dlg;
    dlg.setModal(true);

    std::shared_ptr<Board> temp = dlg.CreateBoard();

    if(!temp)
        return;

    m_ActiveDocument.SetBoard(*temp);
    ui->actionSave->setEnabled(true);

    ResizeToBoard();

    ShowActiveDocument();
}

void MainWindow::on_actionOpen_triggered()
{
    QString filter = "GenCross Board (*.gcb);;";

    auto pathToFile = QFileDialog::getOpenFileName(this,"Open a file", "", filter);

    if(pathToFile.isEmpty())
        return;

    m_ActiveDocument = Document(pathToFile);

    ui->actionSave_As->setEnabled(true);
    ui->actionSave->setEnabled(true);

    ResizeToBoard();

    ClearCrossTable();
    ShowActiveDocument();

    CheckAnswers();
}

void MainWindow::on_actionSave_triggered()
{
    if(m_ActiveDocument.GetSaveFilePath().isEmpty())
    {
        QString filter = "GenCross Board (*.gcb);;";

        auto pathToFile = QFileDialog::getSaveFileName(this,"Save the file", "", filter);
        if(pathToFile.isEmpty())
            return;

        ui->actionSave_As->setEnabled(true);
        m_ActiveDocument.SetSaveFilePath(pathToFile);
    }


    m_ActiveDocument.SaveDocumentAsJSON();

    ShowActiveDocument();
}

void MainWindow::on_actionGenerate_Empy_Board_triggered()
{
    if(!m_ActiveDocument.GetBoard())
        return;

    m_ActiveDocument.GetBoard()->CleanBoard();

    LengthDialog dlg(this);

    int min,max,seed;

    if(!dlg.ShowDialog(&min, &max, &seed))
        return;

    WaitDialog spiner(this);

    m_myThread = QThread::create([&](){

        // 10 x 10
        // 3 - 7 seed 10
        // 3 - 5 seed 13, 15, 17, 23, 26, 33
        // 12 x 14
        // 3 - 5 seed 4, 5
        BoardBuilder builder(min, max, 100);

        auto tempBoard = *m_ActiveDocument.GetBoard();
        builder.GenerateBoard(tempBoard, seed);
         m_ActiveDocument.SetBoard(tempBoard);
         ShowActiveDocument();

        });

    spiner.ExecFunction(m_myThread);

    this->setFocus();
}

void MainWindow::on_actionGenerate_Solved_Board_triggered()
{
    m_myThread = QThread::create([&](){

    PuzzleSolver solver("crossword_PDO.db");

    auto tempBoard = *m_ActiveDocument.GetBoard();
    solver.SolvePuzzle(tempBoard,11);
    m_ActiveDocument.SetBoard(tempBoard);
    ShowActiveDocument();

    });
    WaitDialog waitDlg(this);

    waitDlg.ExecFunction(m_myThread);
    this->setFocus();
}

void MainWindow::on_WordList_itemClicked(QListWidgetItem *item)
{
    if(ui->WordList->currentRow() >= m_ActiveDocument.GetBoard()->GetQuestions().size())
        return;

    ui->CrossTable->clearSelection();

   Question q = m_ActiveDocument.GetBoard()->GetQuestions()[ui->WordList->currentRow()];
    if (ui->WordList->isActiveWindow())
    {
        ui->CrossTable->setRangeSelected(QTableWidgetSelectionRange(q.start.m_row,q.start.m_col,q.end.m_row,q.end.m_col),true);
    }
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

    m_ActiveDocument.GetUserBoard()->PutLetterInCell(Cell(row,column), text.at(0).toLatin1());


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


void MainWindow::on_actionExit_triggered()
{
  close();
}


void MainWindow::on_actionSave_As_triggered()
{
    QString filter = "GenCross Board (*.gcb);;";

    auto pathToFile = QFileDialog::getSaveFileName(this,"Save the file", "", filter);
    m_ActiveDocument.SetSaveFilePath(pathToFile);

    m_ActiveDocument.SaveDocumentAsJSON();

    ShowActiveDocument();
}

