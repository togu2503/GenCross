#pragma once
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <time.h>
#include "Board.h"
#include "SQLBase.h"
#include "Cell.h"

struct DirectionArrows
{
    Cell Offset;
    Cell NormVec;
};

class PuzzleSolver
{
	private:
		static std::vector<DirectionArrows> arrows;
        Board* pBoard;
		std::vector<std::vector<std::string>> badAnswers;
		std::vector<std::vector<TableRow>> wordList;
		int maxLengthOfWord, seed, zeroIntersections;

		std::vector<TableRow> findWords(std::string mask);

		void reloadQuestions();
		void clearUselessWords();
        void putOnBoard(Question);
		void calcIntersections();

		int findCountOfWords(std::string mask);

		bool isWordInDict(std::string);
		bool isPossibleToSolve(int);


public:

        PuzzleSolver(Board &t_board, std::vector<Question> Questions, std::string PathToDatabase, int Seed=0);
        ~PuzzleSolver();

		void setSeed(int Seed);
		int getSeed();
		void exportToFile(std::string FilePath);

		void showBoard();
        bool solvePuzzle();
		
};

