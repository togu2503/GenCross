#include "BoardBuilder.h"
//static vector of arrows
 std::vector<DirectionArrow> BoardBuilder::s_arrows{
        { {0 , 1},  {0 , 1} },  //0
        { {1 , 0},  {0 , 1} },  //1
        { {-1 , 0}, {0 , 1} },  //2
        { {1 , 0},  {1 , 0} },  //3
        { {0 , -1}, {1 , 0} },  //4
        { {0 , 1},  {1 , 0} }   //5
};

    //ctor


 BoardBuilder::BoardBuilder(Board &board, int t_minWordLength , int t_maxWordLength)
 {

     pBoard = std::make_unique<Board>(board);
     minWordLength = t_minWordLength;
     maxWordLength = t_maxWordLength;
 }

    //get

 int BoardBuilder::getHeight()
 {
     return pBoard->GetHeight();
 }

 int BoardBuilder::getWidth()
 {
     return pBoard->GetWidth();
 }

 Board BoardBuilder::getBoard()
 {
     return *pBoard;
 }

 std::vector<Question> BoardBuilder::getAllQuestions()
 {
     return pBoard->GetAllQuestions();
 }
 
 int BoardBuilder::getMaxWordLength()
 {
     return maxWordLength;
 }

 int BoardBuilder::getMinWordLength()
 {
     return minWordLength;
 }

    //set
 void BoardBuilder::setMinWordLength(int t_minWordLength)
 {
     if (t_minWordLength < 2) { minWordLength = 2; return; }
     if (t_minWordLength > maxWordLength) { minWordLength = maxWordLength; return; }
     minWordLength = t_minWordLength;
 }

 void BoardBuilder::setMaxWordLength(int t_maxWordLength)
 {
     if (t_maxWordLength > 20) { maxWordLength = 20; return; }
     if (t_maxWordLength < minWordLength) { maxWordLength = minWordLength; return; }
     minWordLength = t_maxWordLength;
 }

    //display
/*
 void BoardBuilder::exportToFile(std::string t_filePath)
 {
     std::ofstream fout;
     fout.open(t_filePath);
     fout << pBoard.getHeight() << " " << board.getWidth() << std::endl;
     for (int i = 0; i < board.getHeight(); i++)
     {
         fout << board[i] << std::endl;
     }
     fout << allQuestions.size() << std::endl;
     for (auto i = 0; i < allQuestions.size(); i++)
     {
         fout << allQuestions[i].q << " " << allQuestions[i].start << " " << allQuestions[i].end << " " << allQuestions[i].direction << std::endl;
     }
     fout.close();
 }
*/
    //auxiliary methods

 int BoardBuilder::distanceBetweenCells(Cell t_from,Cell t_to)
 {
     return std::max( abs(t_from.Row - t_to.Row), abs(t_from.Col - t_to.Col));
 }

bool BoardBuilder::isVoidCell()
{
    for (int i = 0; i < pBoard->GetHeight(); ++i)
    {
        for (int j = 0; j < pBoard->GetWidth(); ++j)
        {
            if (pBoard->GetBoard()[i][j] == '.')return false;
        }
    }
    return true;
}

Cell BoardBuilder::findCell()
{
    for (int i = 0; i < pBoard->GetHeight(); i++)
    {
        for (int j = 0; j < pBoard->GetWidth(); j++)
        {
            if (pBoard->GetBoard()[i][j] == '.') return Cell{ i, j };
        }
    }
}

bool BoardBuilder::isIntersection(Cell t_cell,Question t_question)  // перевірка чи є перетин з іншими словам
{

    Cell currentToStart, endToCurrent;
    for (auto i = 0; i < pBoard->GetAllQuestions().size(); ++i)
    {
        if (!(t_question==pBoard->GetQuestion(i)) && t_cell.IsOnLine(pBoard->GetQuestion(i).start, pBoard->GetQuestion(i).end))
        {
            return true;
        }
    }
    return false;

}

int BoardBuilder::chooseDirection(std::vector<std::vector<int>> const & t_directions, std::vector<int> const & t_suggestedDirection)
{
    if (t_suggestedDirection.size() > 0)
        for (auto i = 0; i < t_suggestedDirection.size(); i++)
        {
            if (t_directions[t_suggestedDirection[i]].size() > 0 && t_directions[t_suggestedDirection[i]].back() >= minWordLength)
                return t_suggestedDirection[i];
        }
    int wordDirection = 0;
    while (t_directions[wordDirection].size() == 0 || t_directions[wordDirection].back() < minWordLength)
    {
        wordDirection = rand() % 6;
    }
    return wordDirection;
}

bool BoardBuilder::isCorrupting(Cell t_cell, int t_direction)        //corrupting of other words by this word
{
    if (pBoard->GetCell(t_cell) == '.')
        return false;
    Cell currentToStart, endToCurrent;
    for (int i = 0; i < pBoard->GetAllQuestions().size(); ++i)
    {
        if (t_cell.IsOnLine(pBoard->GetQuestion(i).start, pBoard->GetQuestion(i).end) && s_arrows[pBoard->GetQuestion(i).direction].NormVec == s_arrows[t_direction].NormVec)
        {
            return true;
        }
    }
    return false;
}

    //methods that works with board

void BoardBuilder::clearWord(Question t_question)
{
    Cell cell = t_question.questionPos;
    pBoard->SetCell(cell,'.');
    cell = cell + s_arrows[t_question.direction].Offset;
    while (!(cell == t_question.end))
    {
        if(!isIntersection(cell,t_question))
            pBoard->SetCell(cell, '.');
        cell += s_arrows[t_question.direction].NormVec;
    }
    if (!isIntersection(cell, t_question))
         pBoard->SetCell(cell, '.');
}


void BoardBuilder::putQuestionOnBoard(Question t_question)
{
    Cell cell = t_question.questionPos;
    pBoard->SetCell(cell, '#');
    cell = cell + s_arrows[t_question.direction].Offset;
    while (!(cell == t_question.end))
    {
        pBoard->SetCell(cell, '_');
        cell += s_arrows[t_question.direction].NormVec;
    }
    pBoard->SetCell(cell, '_');
}

    // main methods
Question BoardBuilder::findNextWord(Cell t_position, std::vector<int> t_suggestedDirections)
{
    if(!pBoard->cellIsOnBoard(t_position))
        throw "WordIsAbsent";
    
    Question res({0,0}, {0,0}, {0,0},0, 0);
    std::vector<std::vector<int>> directions(6,std::vector<int>());
    char symb;
    Cell currentCell{ 0,0 }, tempStart{0,0};
    int wordLength=0,maxLength=0, countOfCells, distanceToEmpty;
    bool intersect;

    for (int i = 0; i < s_arrows.size(); i++)
    {

        intersect = false;
        currentCell = t_position;
        currentCell += s_arrows[i].Offset;
        tempStart = currentCell;
        countOfCells = 1;

        if (!pBoard->cellIsOnBoard(tempStart)) continue;

        symb = pBoard->GetCell(currentCell);
        while (pBoard->cellIsOnBoard(currentCell) && (symb = pBoard->GetCell(currentCell)) != '#')
        {

            if (!pBoard->cellIsOnBoard(currentCell + s_arrows[i].NormVec) || pBoard->GetCell(currentCell + s_arrows[i].NormVec) == '#'|| pBoard->GetCell(currentCell+s_arrows[i].NormVec)=='.')
            {
               if(countOfCells>=minWordLength)
                   directions[i].push_back(countOfCells);
               if (countOfCells > maxLength) maxLength = countOfCells;
            }
            if (isCorrupting(currentCell, i))
            {
                break;
            }
            currentCell += s_arrows[i].NormVec;
            countOfCells++;
        }
    }


    if (maxLength < minWordLength)
        throw "WordIsAbsent";
   
    res.questionPos = t_position;
    int wordDirection = chooseDirection(directions,t_suggestedDirections);

    res.start = res.questionPos + s_arrows[wordDirection].Offset;
    int currentLength=0;
    bool wordIsFound=false;
    do
    {
        if (directions[wordDirection].back() <= maxWordLength) currentLength = directions[wordDirection].back();
        else
        {
            int index=0;
            for (int i = 0; i < directions[wordDirection].size(); ++i)
            {
                if (directions[wordDirection][i] < maxWordLength)index = i;
            }
            currentLength = directions[wordDirection][rand() % (index+1)];
        }


        currentCell = res.start + s_arrows[wordDirection].NormVec * currentLength;
        if (pBoard->cellIsOnBoard(currentCell))
        {
            if (pBoard->GetCell(currentCell) == '.' || pBoard->GetCell(currentCell) == '#')
            {
                if (pBoard->GetCell(currentCell) == '#') currentLength--;
                wordIsFound = true;
            }
        }
        else if (pBoard->cellIsOnBoard(currentCell - s_arrows[wordDirection].NormVec))
        {
            wordIsFound = true;
        }
    } while (!wordIsFound);
    res.end = currentCell - s_arrows[wordDirection].NormVec;
    res.direction = wordDirection;
    return res;
}

bool BoardBuilder::generateBoard(int t_seed)
{
    Cell questionPosition, nextQuestionPos;
    std::vector<bool> errors;
    srand(t_seed);
    int questionNum = 0, countOfErrors = 0;;
    int timesInRow = 0;
    Question currentQuestion;
    bool newPosition = false;
    errors.resize(pBoard->GetWidth() * pBoard->GetHeight() / 2,false);
    for (int i = 0; i < pBoard->GetWidth(); i++)
    {
        try {
            questionPosition = Cell{ 0 , i };
            if (i != 0)
                currentQuestion = findNextWord(questionPosition, { rand() % 2 == 0 ? 3 : 5 });
            else
                currentQuestion = findNextWord(questionPosition, { 5 });
            if (currentQuestion.direction == 3)timesInRow++; else timesInRow = 0;
            if (timesInRow >= 2)
            {
                currentQuestion = findNextWord(questionPosition, { 5 });
                timesInRow = 0;
            }
            if (currentQuestion.direction == 5)i++;
            pBoard->PushbackQuestion(currentQuestion);
            questionNum++;
            putQuestionOnBoard(currentQuestion);
        }
        catch (...)
        {
            throw("Bad board");
        }
        questionPosition = currentQuestion.end + s_arrows[currentQuestion.direction].NormVec;
        while (pBoard->cellIsOnBoard(questionPosition) && pBoard->GetCell(questionPosition) == '.')
        {
            try
            {
                if (i != pBoard->GetWidth() - 2)
                    currentQuestion = findNextWord(questionPosition, { 3,5 });
                else
                    currentQuestion = findNextWord(questionPosition, { 3 });
                pBoard->PushbackQuestion(currentQuestion);
                putQuestionOnBoard(currentQuestion);
                questionPosition = currentQuestion.end + s_arrows[currentQuestion.direction].NormVec;
                questionNum++;
            }
            catch (...)
            {
                clearWord(pBoard->GetBackQuestion());
                pBoard->PopbackQuestion();
                questionNum--;
                if (errors[questionNum]) 
                {
                    clearWord(pBoard->GetBackQuestion());
                    pBoard->PopbackQuestion();
                    errors[questionNum] = false;
                    questionNum--;
                    errors[questionNum] = true;
                }
                else
                {
                    errors[questionNum] = true;
                }
                questionPosition = pBoard->GetBackQuestion().questionPos;
            }
        }

    }
    timesInRow = 0;
    while (!isVoidCell())
    {
        questionPosition = findCell();
        while (pBoard->cellIsOnBoard(questionPosition) && pBoard->GetCell(questionPosition) == '.')
        {
            try
            {
                currentQuestion = findNextWord(questionPosition, {});
                pBoard->PushbackQuestion(currentQuestion);
                questionNum++;
                errors[questionNum] = false;
                putQuestionOnBoard(currentQuestion);
                questionPosition = currentQuestion.end + s_arrows[currentQuestion.direction].NormVec;
            }
            catch (...)
            {
                countOfErrors++;
                if (countOfErrors > errorsToRestart)
                    throw("Bad board");
                clearWord(pBoard->GetBackQuestion());
                pBoard->PopbackQuestion();
                questionNum--;
                if (errors[questionNum])
                {
                    while (errors[questionNum])
                    {
                        clearWord(pBoard->GetBackQuestion());
                        pBoard->PopbackQuestion();
                        errors[questionNum] = false;
                        questionNum--;
                    }
                    errors[questionNum] = true;
                }
                else
                {
                    errors[questionNum] = true;
                }
                questionPosition = pBoard->GetBackQuestion().questionPos;
            }
        }

    }
    return true;
}

    //red button IF SMTH GOES WRONG or just to build new board :) 
void BoardBuilder::restart()
{
    pBoard->ClearQuestions();
    for (int i = 0; i < pBoard->GetHeight(); i++)
    {
        for (int j = 0; j < pBoard->GetWidth(); j++)
        {
            pBoard->SetCell({i,j},'.');
        }
    }
}
