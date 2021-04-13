#include "Document.h"

void Document::SetBoard(Board& b)
{
    m_Board = &b;
}

Board& Document::GetBoard()
{
    return *m_Board;
}

void Document::SetCellSize(int width,int height)
{
    m_CellHeight = height;
    m_CellWidth = width;
}

void Document::SetCellWidth(int width)
{
    m_CellWidth = width;
}

void Document::SetCellHeight(int height)
{
    m_CellHeight = height;
}

int Document::GetCellWidth() const
{
    return m_CellWidth;
}

int Document::GetCellHeight() const
{
    return m_CellHeight;
}
