#include <iostream>
#include <vector>

#include "Cell.h"


Cell::Cell()
{
    m_row = 0;
    m_col = 0;
}

Cell::Cell(int row, int col)
{
    this->m_row = row;
    this->m_col = col;
}

Cell::Cell(const Cell& cell)
{
    m_row = cell.m_row;
    m_col = cell.m_col;
}

Cell::Cell(Cell&& cell) noexcept
{
    swap(*this, cell);
}

Cell Cell::operator = (const Cell& cell)
{
    Cell temp(cell);
    swap(*this, temp);
    return *this;
}

bool Cell::operator == (const Cell& a) const
{
    return(a.m_row == m_row && a.m_col == m_col);
}

Cell Cell::operator + (const Cell& a) const
{
    Cell res;
    res.m_row = m_row + a.m_row;
    res.m_col = m_col + a.m_col;
    return res;
}

Cell Cell::operator - (const Cell& a) const
{
    Cell res;
    res.m_row = m_row - a.m_row;
    res.m_col = m_col - a.m_col;
    return res;
}

Cell Cell::operator += (const Cell& a)
{
    m_row += a.m_row;
    m_col += a.m_col;
    return *this;
}

Cell Cell::operator * (int k)
{
    Cell res = *this;
    res.m_row *= k;
    res.m_col *= k;
    return res;
}

bool Cell::operator == (int k) const
{
    int distance;
    distance = std::max(m_row, m_col);
    if (distance == k)
        return true;
    else
        return false;
}

bool Cell::operator != (int k) const
{
    return !(*this == k);
}

bool Cell::operator != (const Cell& a) const
{
    return !(*this == a);
}

bool Cell::IsOnLine(const Cell& start, const Cell& end) const
{
    if (m_row == start.m_row && m_row == end.m_row)
    {
        if (m_col >= start.m_col && m_col <= end.m_col)
            return true;
        else
            return false;
    }
    if (m_col == start.m_col && m_col == end.m_col)
    {
        if (m_row >= start.m_row && m_row <= end.m_row)
            return true;
        else
            return false;
    }
    return false;
}

void swap(Cell& cell1, Cell& cell2) noexcept
{
    using std::swap;
    swap(cell1.m_row, cell2.m_row);
    swap(cell1.m_col, cell2.m_col);
}

 std::ostream& operator<<(std::ostream& outstream, const Cell& cell)
{
    outstream << cell.m_row << " " << cell.m_col;
    return outstream;
}
