#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "Board.h"


class Document : public Board
{
public:
    void SetBoard(Board&);
    Board& GetBoard();
    void SetCellSize(int,int);
    void SetCellWidth(int);
    void SetCellHeight(int);
    int GetCellWidth() const;
    int GetCellHeight() const;

private:
    Board * m_Board;
    int m_CellWidth;
    int m_CellHeight;
};

#endif // DOCUMENT_H
