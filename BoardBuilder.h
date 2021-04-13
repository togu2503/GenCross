#pragma once
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include "DirectionArrow.h"
#include "Utility.h"
#include "Board.h"
#include <memory>



class BoardBuilder
{
private:
    static std::vector<DirectionArrow> s_arrows;
    std::unique_ptr<Board> pBoard;
    // TODO initialize object thorugh the Board
    // TODO maybe just Question with strings will be good choice?

    //parameters of building
    int maxWordLength = 7;
    int minWordLength = 3;
    int errorsToRestart = 50;

    void putQuestionOnBoard(Question);
    void clearWord(Question);

    bool isVoidCell();
    bool isCorrupting(Cell, int Direction);
    bool isIntersection(Cell, Question);
    
    int chooseDirection(std::vector<std::vector<int>> const & Directions, std::vector<int> const & SuggestedDirections);
    int distanceBetweenCells(Cell, Cell);
    
    Cell findCell();

    Question findNextWord(Cell QuestionPosition, std::vector<int> DirectionPriority);
public:
    BoardBuilder(Board &board, int MinWordLength=3, int MaxWordLength=7);

    int getWidth();
    int getHeight();
    int getMaxWordLength();
    int getMinWordLength();
    Board getBoard();
    std::vector<Question> getAllQuestions();

    void setMaxWordLength(int MaxWordLength);
    void setMinWordLength(int MinWordLength);


    void restart();
    bool generateBoard(int seed=0);

};

