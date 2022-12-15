#include <fstream>
#include <vector>
#include <iostream>
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
    Name: Board::RefreshBoard

    Params:

    Action: sync questions and board.

    Return:
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
    Name: Board::PutQuestionOnBoard

    Params:
            question - question that will be inserted

    Action: Puts question.

    Return:
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
    Name: Board::IsAnyVoidCell

    Params:

    Action: Checks if there void cell on the board

    Return: bool
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
    Name: Board::RemoveQuestionFromBoard

    Params:
            index - index of question

    Action: Removes question at index from board.

    Return:
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
    Name: Board::RemoveLastQuestionFromBoard

    Params:

    Action: Removes last question from board

    Return:
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
    if(cell.m_row >= m_data.size() || cell.m_col > m_data[0].size() || cell.m_row < 0 || cell.m_col < 0)
        return;

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
    Name: Board::FindEmptyCell

    Params:

    Action: Finds empty cell

    Return: cell
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
    Name: Board::IsOverlapping()

    Params:
            cell - cell to check
            directionType - direction of the question

    Action: check if we have overlapping with other words.

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
    Name: Board::IsCellOnBoard()

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
    Name: Board::GetRow()

    Params:

    Return:
*******************************************************/
const std::string& Board::GetRow(int index) const
{
    if(index >= 0 && index < GetHeight())
        return m_data[index];

    return "";
}

/******************************************************
    Name: Board::SetRow()

    Params: index - index of row in which we will insert string
            str - string that will be inserted

    Action: inserts string in row

    Return:
*******************************************************/
void Board::SetRow(int index, std::string str)
{
    if(index >= 0 && index < GetHeight() - 1)
        m_data[index] = str;
}

/******************************************************
    Name: Board::CleanBoard

    Params:

    Action: Cleans board.

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
