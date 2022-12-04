#pragma once

#include <vector>
#include <random>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <optional>
#include <memory>
#include "Board.h"


class BoardBuilder
{
private:

    Board* m_pBoard;

    //parameters of building
    int m_maxWordLength;
    int m_minWordLength;
    int m_errorsToRestart;
    std::default_random_engine m_randEngine;
    unsigned long int m_seed;

    std::vector<Cell> ScanDirectionFromCell(const Cell& cell, const Direction::DirectionType direction);
    bool IsPossibleToInsertQuestion(const Cell& cell);

    std::optional<Question> FindNextWord(const Cell& position, std::vector<Direction::DirectionType> arrowPriority);

public:
    BoardBuilder() = delete;
    BoardBuilder(const BoardBuilder&) = delete;
    BoardBuilder(int minWordLength, int maxWordLength, int errorsToRestart);

    char GetNextLetter(const Cell& cell, const Direction::DirectionArrow& arrow);
    int GetMaxWordLength() { return m_maxWordLength; }
    int GetMinWordLength() { return m_minWordLength; }
    int GetErrorsToRestart() { return m_errorsToRestart; };

    void GenerateBoard(Board& board, unsigned long int seed);

    void SetMaxWordLength(int maxWordLength) { m_maxWordLength = maxWordLength; }
    void SetMinWordLength(int minWordLength) { m_minWordLength = minWordLength; }
    void SetErrorsToRestart(int errorsToRestart) { m_errorsToRestart = errorsToRestart; }


    void Restart();

};
