#pragma once
#include <QtSql>
#include <string>
#include <vector>
#include <iostream>
#include <QString>
#include <memory>
#include "Utility.h"

class SQLBase
{
	private:
        std::string m_dataBaseName;
        QSqlDatabase m_db;
	public:
		SQLBase(std::string DataBaseName);
        virtual ~SQLBase();

		std::vector<TableRow> getWords(std::string Mask);
		int getNumberOfWords(std::string Mask);
};

