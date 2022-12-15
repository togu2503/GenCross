#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "Board.h"
#include <memory>


class Document
{
public:
    Document();
    Document(QString pathToFile);

    void CopyMainToUserBoard();

    void SetBoard(const Board& b);
    std::shared_ptr<Board> GetBoard();

    void SetUserBoard(const Board& b);
    std::shared_ptr<Board> GetUserBoard();

    void SetCellSize(int,int);
    void SetCellWidth(int);
    void SetCellHeight(int);
    int GetCellWidth() const;
    int GetCellHeight() const;

    void SetSaveFilePath(QString pathToFile);
    QString GetSaveFilePath();
    void SaveDocumentAsJSON();

    void ExportDocumentToPDF(QString pathToFile);

private:
    std::shared_ptr<Board> m_Board;
    std::shared_ptr<Board> m_UserBoard;

    int m_CellWidth = 60;
    int m_CellHeight = 60;

    QString m_SaveFilePath;
};

#endif // DOCUMENT_H
