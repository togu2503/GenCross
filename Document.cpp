#include "Document.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>

void Document::SetBoard(const Board& b)
{
    m_Board = std::make_shared<Board>(b);
    CopyMainToUserBoard();
}

std::shared_ptr<Board> Document::GetBoard()
{
    return m_Board;
}

void Document::CopyMainToUserBoard()
{
    m_UserBoard = std::make_shared<Board>(Board(m_Board->GetHeight(), m_Board->GetWidth()));

    for(int i = 0; i < m_Board->GetQuestions().size(); i++)
    {
        auto currentQuestion = m_Board->GetQuestions()[i];
        m_UserBoard->PutLetterInCell(Cell(currentQuestion.questionPos.m_row,currentQuestion.questionPos.m_col), '#');
    }
}

void Document::SetUserBoard(const Board& b)
{
    m_UserBoard = std::make_shared<Board>(b);
}

std::shared_ptr<Board> Document::GetUserBoard()
{
    return m_UserBoard;
}

QString Document::GetSaveFilePath()
{
    return m_SaveFilePath;
}

void Document::SetSaveFilePath(QString filePath)
{
    m_SaveFilePath = filePath;
}


Document::Document(QString pathToFile)
{
    QFile file;

    m_SaveFilePath = pathToFile;

    file.setFileName(pathToFile);

    if(!file.open(QIODevice::ReadOnly))
        return;


    QByteArray byteArray;
    byteArray = file.readAll();
    file.close();

    QJsonParseError parseError;

    QJsonDocument jsonDoc;
    jsonDoc = QJsonDocument::fromJson(byteArray, &parseError);

    if(parseError.error != QJsonParseError::NoError)
    {
        qWarning() << "Parse error at " << parseError.offset << ":" << parseError.errorString();
        return;
    }

    QJsonObject document;
    document = jsonDoc.object();

    int height = document["MainBoard"].toArray().size();
    int width = document["MainBoard"].toArray().at(0).toString().size();

    m_Board = std::make_shared<Board>(Board(height, width));
    m_UserBoard = std::make_shared<Board>(Board(height, width));

    for(int i = 0; i < document["Questions"].toArray().size(); i++)
    {
        QJsonObject questionJson;

        questionJson = document["Questions"].toArray().at(i).toObject();

        Question question;

        question.question = questionJson["Question"].toString().toStdString();
        question.answer = questionJson["Answer"].toString().toStdString();
        question.length = questionJson["Length"].toInt();
        question.direction = static_cast<Direction::DirectionType>(questionJson["Direction"].toInt());

        Cell temp;

        temp.m_row = questionJson["QuestionPos"].toObject()["Row"].toInt();
        temp.m_col = questionJson["QuestionPos"].toObject()["Col"].toInt();
        question.questionPos = temp;

        temp.m_row = questionJson["Start"].toObject()["Row"].toInt();
        temp.m_col = questionJson["Start"].toObject()["Col"].toInt();
        question.start = temp;

        temp.m_row = questionJson["End"].toObject()["Row"].toInt();
        temp.m_col = questionJson["End"].toObject()["Col"].toInt();
        question.end = temp;

        m_Board->PutQuestionOnBoard(question);
    }

    for(int i = 0; i < document["UserBoard"].toArray().size(); i++)
    {
        m_UserBoard->SetRow(i, document["UserBoard"].toArray().at(i).toString().toStdString());
    }

}

Document::Document()
{

}

void Document::SaveDocumentAsJSON()
{
    QJsonObject document;

    QJsonArray board;
    for(int i = 0;i < m_Board->GetHeight(); i++)
    {
        board.append(QString::fromStdString(m_Board->GetRow(i)));
    }

    document["MainBoard"] = board;

    QJsonArray questions;

    for(int i = 0; i < m_Board->GetQuestions().size(); i++)
    {
        QJsonObject questionJson;
        auto currentQuestion = m_Board->GetQuestions()[i];

        QJsonObject questionPos;
        questionPos["Row"] = currentQuestion.questionPos.m_row;
        questionPos["Col"] = currentQuestion.questionPos.m_col;

        QJsonObject startCell;
        startCell["Row"] = currentQuestion.start.m_row;
        startCell["Col"] = currentQuestion.start.m_col;

        QJsonObject endCell;
        endCell["Row"] = currentQuestion.end.m_row;
        endCell["Col"] = currentQuestion.end.m_col;

        questionJson["QuestionPos"] = questionPos;
        questionJson["Start"] = startCell;
        questionJson["End"] = endCell;
        questionJson["Direction"] = static_cast<int>(currentQuestion.direction);
        questionJson["Length"] = currentQuestion.length;
        questionJson["Question"] = QString::fromStdString(currentQuestion.question);
        questionJson["Answer"] = QString::fromStdString(currentQuestion.answer);

        questions.append(questionJson);
    }

    document["Questions"] = questions;

    QJsonArray userBoard;
    for(int i = 0;i < m_UserBoard->GetHeight(); i++)
    {
        userBoard.append(QString::fromStdString(m_UserBoard->GetRow(i)));
    }

    document["UserBoard"] = userBoard;

    QByteArray byteArray;
    byteArray = QJsonDocument(document).toJson();

    QFile file;
    file.setFileName(m_SaveFilePath);

    if(!file.open(QIODevice::WriteOnly))
        return;

    file.write(byteArray);
    file.close();
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
