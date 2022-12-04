#pragma once
#include <iostream>

class Cell
{
public:
    int m_row;
    int m_col;


    Cell();
    Cell(int row, int col);
    Cell(const Cell& cell);
    Cell(Cell&& cell) noexcept;

    bool operator == (const Cell &cell) const;
    Cell operator = (const Cell &cell);
    Cell operator + (const Cell &cell) const;
    Cell operator - (const Cell &cell) const;
    Cell operator += (const Cell &cell);
    Cell operator * (int k);
    bool operator == (int k) const;
    bool operator != (int k) const;
    bool operator != (const Cell &cell) const;
    friend void swap(Cell&, Cell&) noexcept;
    friend std::ostream& operator<<(std::ostream& outstream, const Cell& cell);

    bool IsOnLine(const Cell& start, const Cell& end) const;
};
