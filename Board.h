#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include "Utility.h"

class Board
{
private:
    std::vector<Question> m_allQuestions;
    std::vector<std::string> m_board;
    int m_width, m_height;

public:
    Board();
    Board(std::string);
    Board(const Board& Board);
    Board(int Height, int Width);
    void SortByIntersections();

    void PushbackQuestion(Question question);
    Question GetQuestion(int) const;
    const std::vector<Question>& GetAllQuestions();
    void ClearQuestions();
    void PopbackQuestion();
    Question GetBackQuestion() const;
    void ChangeQuestion(int, Question);
    Question& GetOriginalQuestion(int);
    void ClearBoard();

    int GetWidth() const;
    int GetHeight() const;
    void SetWidth(int);
    void SetHeight(int);
    void SetCell(Cell, char);
    char GetCell(Cell) const;
    std::vector<std::string> GetBoard() const;

    char& operator[](Cell Cell);
    std::string& operator[](int Row);

    void ShowBoard();
    bool cellIsOnBoard(Cell Cell);
    bool ExportToFile(std::string);
};

