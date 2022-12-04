#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "Board.h"
#include <memory>


class Document
{
public:
    void SetBoard(std::shared_ptr<Board> b);
    std::shared_ptr<Board> GetBoard();

    void SetUserBoard(Board b);
    std::shared_ptr<Board> GetUserBoard();

    void SetCellSize(int,int);
    void SetCellWidth(int);
    void SetCellHeight(int);
    int GetCellWidth() const;
    int GetCellHeight() const;

    void SaveDocumentAsJson();

private:
    std::shared_ptr<Board> m_pBoard = nullptr;
    int m_CellWidth;
    int m_CellHeight;

    QString savePath;

    Board m_UserBoard;
};

#endif // DOCUMENT_H
