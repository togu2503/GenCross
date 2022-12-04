#include "Document.h"

void Document::SetBoard(std::shared_ptr<Board> b)
{
    m_pBoard = std::move(b);
}

std::shared_ptr<Board> Document::GetBoard()
{
    return m_pBoard;
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
