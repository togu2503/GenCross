#pragma once
#include <vector>
#include <QString>
#include "Utility.h"

class Board
{
private:
    std::vector<std::string> m_data;
    std::vector<Question> m_questions;
    int m_width, m_height;

    void RefreshBoard();
    void RefreshQuestions();

public:
    Board();
    Board(const Board& board);
    Board(Board&& board) noexcept;
    Board(const int height, const int width);

    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

    const std::vector<std::string>& GetBoardAsVectorOfString() { return m_data; }

    const std::vector<Question>& GetQuestions() { return m_questions; }
    const Question& GetLastQuestion() { return m_questions.back(); }

    char GetLetterFromCell(const Cell& cell) { return m_data[cell.m_row][cell.m_col]; };
    void PutLetterInCell(const Cell& cell, char letter);

    void CleanBoard();

    const std::string& GetRow(int index) const;
    void SetRow(int index, std::string);

    void SetQuestionAnswer(int index, std::string question, std::string answer);

    bool IsAnyVoidCell();
    bool IsOverlapping(const Cell& cell, const Direction::DirectionType& direction);

    Cell FindEmptyCell();

    void RemoveQuestionFromBoard(const int index);
    void RemoveLastQuestionFromBoard();
    void PutQuestionOnBoard(const Question& question);

    friend void swap(Board&, Board&) noexcept;

    void ShowBoard();
    bool IsCellOnBoard(const Cell &cell);

    void operator =(const Board& b);
};
