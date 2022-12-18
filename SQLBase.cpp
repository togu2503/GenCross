#include "SQLBase.h"

SQLBase::SQLBase(std::string DataBaseName)
{
    m_dataBaseName = DataBaseName;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(QString::fromStdString(m_dataBaseName));
    if (!m_db.open()) {
        qDebug() << "Something goes wrongs";
    }
}

SQLBase::~SQLBase()
{
    m_db.close();
}

int SQLBase::getNumberOfWords(std::string word)
{
    for (int i = 0; i < word.size(); ++i)
    {
        word[i] = std::tolower(word[i]);
    }

    QSqlQuery query;
    QString sqlRequest = "SELECT COUNT(word) AS number FROM crossword_words WHERE word LIKE '"+QString::fromStdString(word)+"'";
    sqlRequest = sqlRequest.arg(QString::fromStdString(word));


    if(!query.exec())
    {
        return -1;
    }

    QSqlRecord rec = query.record();

    return query.value(rec.indexOf("number")).toInt();
}

std::vector<TableRow> SQLBase::getWords(std::string word)
{
    for (int i = 0; i < word.size(); ++i)
    {
        word[i] = std::tolower(word[i]);
    }
    std::vector<TableRow> res;
    QSqlQuery query;
    QString sqlRequest = "SELECT * FROM crossword_words WHERE word LIKE '"+ QString::fromStdString(word)+"'";
    query.prepare(sqlRequest);


    if(!query.exec())
    {
        return res;
    }

    QSqlRecord rec = query.record();

    while (query.next())
    {
        std::string answer =  query.value(rec.indexOf("word")).toString().toStdString();
        std::string question = query.value(rec.indexOf("question")).toString().toStdString();
        res.push_back(TableRow(question,answer));
    }

    return res;
}

