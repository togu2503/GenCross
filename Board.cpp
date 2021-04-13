#include "Board.h"

//ctor

Board::Board()
{
    m_width = 0;
    m_height = 0;
    m_board.resize(0);
    m_allQuestions.resize(0);
}

Board::Board(std::string path)
{
    std::ifstream fin;
    fin.open(path);
    fin >> m_height >> m_width;
    m_board.resize(m_height);
    for(int i=0;i<m_height;i++)
    {
        m_board[i].resize(m_width);
        for(int j=0;j<m_width;j++)
            fin >> m_board[i][j];
    }
    int tempSize;
    fin>> tempSize;
    m_allQuestions.resize(tempSize);
    for(int i=0;i<m_allQuestions.size();i++)
    {
        fin >> m_allQuestions[i].questionPos.Row >> m_allQuestions[i].questionPos.Col;
        fin >> m_allQuestions[i].start.Row >> m_allQuestions[i].start.Col;
        fin >> m_allQuestions[i].end.Row >> m_allQuestions[i].end.Col;
        fin >> m_allQuestions[i].direction;
        fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(fin,m_allQuestions[i].question);
        std::getline(fin,m_allQuestions[i].answer);
        m_allQuestions[i].length = std::max((m_allQuestions[i].start - m_allQuestions[i].end).Row,(m_allQuestions[i].start - m_allQuestions[i].end).Col);
    }

}

void Board::ClearBoard()
{
    for(int i=0;i<m_height; i++)
    {
        for(int j=0;j<m_width; j++)
        {
            m_board[i][j]='.';
        }
    }
}

void Board::SortByIntersections()
{
  std::sort(m_allQuestions.begin(), m_allQuestions.end(), Question::intersectionsCompare);
}

bool Board::ExportToFile(std::string t_path)
{
    std::ofstream fout;
    fout.open(t_path);
    fout << m_height << " " << m_width << std::endl;

    for (int i = 0; i < m_height; ++i)
    {
        for(int j=0;j<m_width;j++)
            fout << GetCell({i,j}) <<" ";
        fout << std::endl;
    }
    fout << std::endl;
    fout << m_allQuestions.size() << std::endl;
    for (int i = 0; i < m_allQuestions.size(); ++i)
    {
        fout <<m_allQuestions[i].questionPos<<" "<<m_allQuestions[i].start<<" "<<m_allQuestions[i].end<<" "<<m_allQuestions[i].direction<<std::endl;
        fout << m_allQuestions[i].question << std::endl << m_allQuestions[i].answer << std::endl;
    }
    fout.close();
}

Board::Board(const Board& t_board)
{
    m_width = t_board.m_width;
    m_height = t_board.m_height;
    m_board.clear();
    m_board.resize(m_height);
    for (int i = 0; i < m_height; ++i)
    {
        m_board[i].resize(m_width);
        for (int j = 0; j < m_board[i].size(); ++j)
        {
            m_board[i][j] = t_board.m_board[i][j];
        }
    }
    m_allQuestions.clear();
    m_allQuestions.resize(m_allQuestions.size());
    for(auto const & it : t_board.m_allQuestions)
    {
        m_allQuestions.push_back(it);
    }
}

Board::Board(int t_height, int t_width)
{
    m_width = t_width;
    m_height = t_height;
    m_board.resize(m_height);
    m_allQuestions.resize(0);
    for (int i = 0; i < m_board.size(); ++i)
    {
        m_board[i].resize(m_width);
        for (int j = 0; j < m_board[i].size(); ++j)
        {
            m_board[i][j] = '.';
        }
    }
}

void Board::PushbackQuestion(Question question)
{
    m_allQuestions.push_back(question);
}

Question& Board::GetOriginalQuestion(int index)
{
    return m_allQuestions[index];
}

Question Board::GetQuestion(int index) const
{
    return m_allQuestions[index];
}

const std::vector<Question>& Board::GetAllQuestions()
{
    return m_allQuestions;
}

void Board::ChangeQuestion(int index, Question question)
{
    m_allQuestions[index] = question;
}

    //get

int Board::GetWidth() const
{
    return m_width;
}

int Board::GetHeight() const
{
    return m_height;
}

std::vector<std::string> Board::GetBoard() const
{
    return m_board;
}

    //operators


void Board::SetCell(Cell t_cell, char symbol)
{
    m_board[t_cell.Row][t_cell.Col] = symbol;
}

char Board::GetCell(Cell cell) const
{
    return m_board[cell.Row][cell.Col];
}

Question Board::GetBackQuestion() const
{
    return m_allQuestions.back();
}

void Board::PopbackQuestion()
{
    m_allQuestions.pop_back();
}

void Board::ClearQuestions()
{
    m_allQuestions.clear();
    m_allQuestions.resize(0);
}

std::string& Board::operator[](int t_x)
{
    return m_board[t_x];
}

//methods

void Board::ShowBoard()
{
    for (int i = 0; i < m_height; ++i)
    {
        for (int j = 0; j < m_width; ++j)
        {
            std::cout << m_board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

bool Board::cellIsOnBoard(Cell t_cell)
{
    if (t_cell.Row < m_height && t_cell.Row >= 0 && t_cell.Col < m_width && t_cell.Col >= 0)
        return true;
    else
        return false;
}

