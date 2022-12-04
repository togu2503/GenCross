#pragma once

#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include <fstream>
#include <time.h>
#include "SQLBase.h"
#include "Board.h"
#include "Utility.h"

class PuzzleSolver
{
private:
    Board* m_pBoard;
    std::vector<int> m_intersections;
    // As we can't order of questions(sorting of questions also should be expensive)
    // we can hold in which order we should traverse them in this vector
    std::vector<int> m_questionIndicies;
    std::vector<std::vector<std::string>> m_badAnswers;
    std::vector<std::vector<TableRow>> m_dictionary;
    std::string m_pathToDatabase;
    int m_maxLengthOfWord;
    int m_minLengthOfWord;
    int m_seed;
    int m_zeroIntersections;

    std::vector<TableRow> FindWords(std::string mask);

    void CalcIntersections();

    int FindCountOfWords(std::string mask);

    void SortQuestionsIndiciesByIntersections();

    const Question& GetSortedQuestion(int index);

    bool IsWordInDict(std::string);
    bool IsPossibleToSolve(int);

    bool FillDictionary();

    const std::vector<TableRow>& GetWordsWithLength(int length);

public:
    PuzzleSolver(std::string pathToDatabase);

    void SetSeed(int seed) { m_seed = seed; }
    int GetSeed() { return m_seed; }

    void SolvePuzzle(Board& board, int seed);

};
