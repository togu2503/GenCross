#include <fstream>
#include <vector>
#include <iostream>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextTable>
#include <QTextTableCell>
#include <QPrinter>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>

#include "Board.h"
#include "Cell.h"


/******************************************************
    Name: Board::Board()

    Params:

    Action: Default constructor for Board.

    Return:
*******************************************************/
Board::Board()
{
    m_width = 0;
    m_height = 0;
    m_data.resize(0);
}

/******************************************************
    Name: Board::Board()

    Params:

    Action: Copy constructor for Board.

    Return:
*******************************************************/
Board::Board(const Board& board)
{
    m_width = board.m_width;
    m_height = board.m_height;

    m_data.resize(m_height);

    for (int i = 0; i < m_height; ++i)
    {
        m_data[i].resize(m_width);
        for (int j = 0; j < m_data[i].size(); ++j)
        {
            m_data[i][j] = board.m_data[i][j];
        }
    }

    for (const auto& question : board.m_questions )
    {
        m_questions.push_back(question);
    }
}

/******************************************************
    Name: Board::Board()

    Params:

    Action: Copy constructor for Board.

    Return:
*******************************************************/
void Board::operator =(const Board& board)
{
    m_width = board.m_width;
    m_height = board.m_height;

    m_data.resize(m_height);

    for (int i = 0; i < m_height; ++i)
    {
        m_data[i].resize(m_width);
        for (int j = 0; j < m_data[i].size(); ++j)
        {
            m_data[i][j] = board.m_data[i][j];
        }
    }

    for (const auto& question : board.m_questions )
    {
        m_questions.push_back(question);
    }
}


/******************************************************
    Name: Board::Board()

    Params:

    Action: Copy constructor for Board.

    Return:
*******************************************************/
Board::Board(Board&& board) noexcept
{
    m_width = board.m_width;
    m_height = board.m_height;

    m_data.resize(m_height);

    for (int i = 0; i < m_height; ++i)
    {
        m_data[i].resize(m_width);
        for (int j = 0; j < m_data[i].size(); ++j)
        {
            m_data[i][j] = board.m_data[i][j];
        }
    }

    for (const auto& question : board.m_questions )
    {
        m_questions.push_back(question);
    }
}

/******************************************************
    Name: Board::Board()

    Params:

    Action: Constructor with params for Board.

    Return:
*******************************************************/
Board::Board(int height,int width)
{
    m_width = width;
    m_height = height;

    m_data.resize(m_height);
    for (int i = 0; i < m_data.size(); ++i)
    {
        m_data[i] = std::string(m_width, '.');
    }
}

void Board::SetQuestionAnswer(int index, std::string question, std::string answer)
{
    m_questions[index].question = question;
    m_questions[index].answer = answer;
    RefreshBoard();
    RefreshQuestions();
}

/******************************************************
    Name: Board::TBD

    Params:

    Action: Get actual bort as vector of strings.

    Return: Vector of strings.
*******************************************************/
void Board::RefreshBoard()
{
    for (int i = 0; i < m_height; i++)
        m_data[i] = std::string(m_width, '.');

    for (const auto& question : m_questions)
    {
        PutLetterInCell(question.questionPos, '#');

        auto normVec = GetDirectionArrow(question.direction).normVec;
        auto currentCell = question.start;

        for (int i = 0; i < question.length; i++, currentCell += normVec)
        {
            m_data[currentCell.m_row][currentCell.m_col] = question.answer.at(i);
        }
    }
}

/******************************************************
    Name: Board::TBD

    Params:

    Action: Get actual bort as vector of strings.

    Return: Vector of strings.
*******************************************************/
void Board::PutQuestionOnBoard(const Question& question)
{
    m_questions.push_back(question);
    RefreshBoard();
    RefreshQuestions();
}

void Board::RefreshQuestions()
{
    for (auto& question : m_questions)
    {
        Cell currentCell = question.start;
        for (int currentLetter = 0; currentLetter < question.length; currentLetter++)
        {
            question.answer[currentLetter] = GetLetterFromCell(currentCell);
            currentCell += Direction::GetDirectionArrow(question.direction).normVec;
        }
    }
}

/******************************************************
    Name: Board::TBD

    Params:

    Action: Get actual bort as vector of strings.

    Return: Vector of strings.
*******************************************************/
bool Board::IsAnyVoidCell()
{
    for (const auto& str : m_data)
    {
        if (str.find_first_of(".") != std::string::npos)
            return true;
    }

    return false;
}


/******************************************************
    Name: Board::TBD

    Params:

    Action: Get actual bort as vector of strings.

    Return: Vector of strings.
*******************************************************/
void Board::RemoveQuestionFromBoard(const int index)
{
    if(index < 0 || index >= m_questions.size())
        return;
    m_questions.erase(std::next(m_questions.begin(), index));

    RefreshBoard();
    RefreshQuestions();
}

/******************************************************
    Name: Board::TBD

    Params:

    Action: Get actual bort as vector of strings.

    Return: Vector of strings.
*******************************************************/
void Board::RemoveLastQuestionFromBoard()
{
    RemoveQuestionFromBoard(m_questions.size() - 1);
}

/******************************************************
    Name: Board::PutLetterInCell

    Params: cell - Place where paste the letter

            letter - actual letter to paste

    Action: Operator to access cell of the board.

    Return: Character of corresponding cell.
*******************************************************/
void Board::PutLetterInCell(const Cell& cell, char letter)
{
    m_data[cell.m_row][cell.m_col] = letter;
}

/******************************************************
    Name: Board::ShowBoard()

    Params:

    Action: Output to cout whole board.

    Return:
*******************************************************/
void Board::ShowBoard()
{
    for (int i = 0; i < m_height; ++i)
    {
        for (int j = 0; j < m_width; ++j)
        {
            std::cout << m_data[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

/******************************************************
    Name: Board::TBD

    Params:

    Action: Output to cout whole board.

    Return:
*******************************************************/
Cell Board::FindEmptyCell()
{
    for (int i = 0; i < m_height; i++)
    {
        size_t pos = m_data[i].find_first_of('.', 0);
        if ( pos != std::string::npos)
            return Cell(i, pos);
    }

    return Cell(-1, -1);
}

/******************************************************
    Name: Board::ShowBoard()

    Params:

    Action: Output to cout whole board.

    Return:
*******************************************************/
bool Board::IsOverlapping(const Cell& cell, const Direction::DirectionType& directionType)
{
    if(GetLetterFromCell(cell) == '.')
        return false;

    Direction::DirectionArrow direction = Direction::GetDirectionArrow(directionType);

    Cell temp = cell + direction.offset;

    for (int i = 0; i < m_questions.size(); ++i)
    {
        if (temp.IsOnLine(m_questions[i].start, m_questions[i].end) && GetDirectionArrow(m_questions[i].direction).normVec == direction.normVec)
        {
            return true;
        }
    }
    return false;
}

/******************************************************
    Name: Board::ShowBoard()

    Params: cell - cell that you want to check on board.

    Action: Check whether cell in on board or not.

    Return:
*******************************************************/
bool Board::IsCellOnBoard(const Cell &cell)
{
    if (cell.m_row < m_height && cell.m_row >= 0 && cell.m_col < m_width && cell.m_col >= 0)
        return true;
    else
        return false;
}

/******************************************************
    Name: Board::TBD

    Params: cell - cell that you want to check on board.

    Action: Check whether cell in on board or not.

    Return:
*******************************************************/
void Board::ExportToPDFFile(const std::string& filePath)
{
    QTextDocument *doc = new QTextDocument;
    doc->setDocumentMargin(0);
    QTextCursor cursor(doc);

    cursor.movePosition(QTextCursor::Start);

    QTextTable *table = cursor.insertTable(GetHeight(), GetWidth());

    QTextTableFormat tableFormat;

    tableFormat.setBorder(1);

    tableFormat.setBorderCollapse(true);

    tableFormat.setAlignment(Qt::AlignCenter);

    tableFormat.setWidth(60 * GetWidth());

    table->setFormat(tableFormat);

    for(int i = 0; i < GetHeight(); i++)
    {
        for(int j = 0; j < GetWidth(); j++)
        {
            QTextCharFormat format;
            format.setVerticalAlignment(QTextCharFormat::VerticalAlignment::AlignMiddle);

            QTextTableCell cell = table->cellAt(i,j);

            QTextCursor cellCursor = cell.firstCursorPosition();

            cellCursor.insertText(QString(GetLetterFromCell({i,j})),format);

        }
    }

    for(int i =0;i< m_questions.size(); i++)
    {
        QTextCharFormat format;

        QTextTableCell cell = table->cellAt(m_questions[i].questionPos.m_row, m_questions[i].questionPos.m_col);

        MultiLineText temp (m_questions[i].question, 10);

        format.setVerticalAlignment(QTextCharFormat::VerticalAlignment::AlignMiddle);

        QTextCursor cellCursor = cell.firstCursorPosition();

        cellCursor.insertText(QString::fromStdString(temp.GetString()), format);
    }


    //Print to PDF
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(QString::fromStdString(filePath));
    doc->print(&printer);
}

/******************************************************
    Name: Board::TBD

    Params: cell - cell that you want to check on board.

    Action: Check whether cell in on board or not.

    Return:
*******************************************************/
void Board::CleanBoard()
{
    for(int i = 0; i < m_height; i++)
        m_data[i] = std::string(m_width, '.');

    m_questions.clear();
}

/******************************************************
    Name: swap(Board& board1, Board& board2)

    Params:

    Action: Swaps to boards.

    Return:
*******************************************************/
void swap(Board& board1, Board& board2) noexcept
{
    using std::swap;
    swap(board1.m_data, board2.m_data);
    swap(board1.m_questions, board2.m_questions);
    swap(board1.m_height, board2.m_height);
    swap(board1.m_width, board2.m_width);
}
