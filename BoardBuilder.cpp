#include "BoardBuilder.h"
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>

BoardBuilder::BoardBuilder(int minWordLength, int maxWordLength, int errorsToRestart = 50)
{
    m_minWordLength = Clamp(3, 10, minWordLength);
    m_maxWordLength = Clamp(m_minWordLength, 14, maxWordLength);
    m_errorsToRestart = Clamp(0,1000,errorsToRestart);
}

// Maybe change it to lambda?
char BoardBuilder::GetNextLetter(const Cell& cell, const Direction::DirectionArrow& arrow)
{
    if(m_pBoard->IsCellOnBoard(cell))
        return m_pBoard->GetLetterFromCell(cell + arrow.normVec);

    return '.';
}

std::vector<Cell> BoardBuilder::ScanDirectionFromCell(const Cell& cell, const Direction::DirectionType direction)
{
    std::vector<Cell> result;

    if (!m_pBoard->IsCellOnBoard(cell))
        return result;


    Direction::DirectionArrow arrow = Direction::GetDirectionArrow(direction);

    Cell cellAboveTheStart = cell + arrow.offset - arrow.normVec;

    if(!(!m_pBoard->IsCellOnBoard(cellAboveTheStart) || m_pBoard->GetLetterFromCell(cellAboveTheStart) == '#' || cell == cellAboveTheStart))
        return result;

    for(int i = 0;i < m_pBoard->GetQuestions().size(); i++)
    {
       if(m_pBoard->GetQuestions()[i].start == cell+arrow.offset)
           return result;
    }

    Cell currentCell = cell + arrow.offset;

    for (int length = 1; m_pBoard->IsCellOnBoard(currentCell) && m_pBoard->GetLetterFromCell(currentCell) != '#'; length++, currentCell += arrow.normVec)
    {
        if (!m_pBoard->IsCellOnBoard(currentCell + arrow.normVec) || GetNextLetter(currentCell, arrow) == '#' ||  GetNextLetter(currentCell , arrow) == '.')
        {
            if(length >= m_minWordLength && length <=m_maxWordLength)
                result.push_back(currentCell);
        }
    }

    return result;
}

bool BoardBuilder::IsPossibleToInsertQuestion(const Cell& cell)
{
    for (int i = 0; i < Direction::DirectionsCount; i++)
    {
        if(!ScanDirectionFromCell(cell, static_cast<Direction::DirectionType>(i)).empty())
            return true;
    }

    return false;
}

std::optional<Question> BoardBuilder::FindNextWord(const Cell& position, std::vector<Direction::DirectionType> arrowPriority)
{
    // We cannot find Word if it is already out of the board.
    if (!m_pBoard->IsCellOnBoard(position))
        return {};

    // fixedArrows it is count of prioritized directions that we will NOT shuffle with other.
    int fixedArrows = arrowPriority.size();

    std::shuffle(std::begin(arrowPriority), std::end(arrowPriority), m_randEngine);

    std::vector<int> tempArrows;

    // Adding missed direction that apsent in prioritized vector.
    for (int i = 0; i < Direction::DirectionsCount; i++)
    {
        if (std::find(arrowPriority.begin(), arrowPriority.end(), static_cast<Direction::DirectionType>(i)) == arrowPriority.end())
            arrowPriority.push_back(static_cast<Direction::DirectionType>(i));
    }

    // Shuffle only non prioritized directions.
    std::shuffle(std::begin(arrowPriority) + fixedArrows, std::end(arrowPriority), m_randEngine);

    // Choose direction to scan due to priority
    for (auto& direction : arrowPriority)
    {
        //Contains Cells with the tails of possible words
        std::vector<Cell> possibleWords = ScanDirectionFromCell(position, direction);

        if (!possibleWords.empty())
        {
            std::shuffle(std::begin(possibleWords), std::end(possibleWords), m_randEngine);
            for (const auto& wordEnd : possibleWords)
            {
                Cell nextToEndCell = wordEnd + Direction::GetDirectionArrow(direction).normVec;

                // If we not able to create next word we don't need current word. So next word should have some space
                // or end of this word is closed by end of the board.
                if ((!m_pBoard->IsCellOnBoard(nextToEndCell) || IsPossibleToInsertQuestion(nextToEndCell)) && !m_pBoard->IsOverlapping(position, direction))
                {
                    Cell questionPos = position;
                    Cell start = position + Direction::GetDirectionArrow(direction).offset;
                    Cell end = wordEnd;
                    Direction::DirectionType directionRes = direction;
                    int length = std::max((end - start).m_col, (end - start).m_row) + 1;
                    std::string question = "";
                    std::string answer = std::string(length, '_');

                    Question res(questionPos, start, end, directionRes, length, question, answer);

                    return res;
                }
            }
        }
    }

    return {};
}



void BoardBuilder::GenerateBoard(Board& board, unsigned long int seed)
{
    m_seed = seed;
    m_randEngine = std::default_random_engine{ m_seed };
    m_pBoard = &board;

    Cell questionPosition;
    std::optional<Question> currentQuestion;
    std::vector<Question> badQuestions;
    int errorsCount = 0;

    while (m_pBoard->IsAnyVoidCell())
    {
        if(!m_pBoard->IsCellOnBoard(questionPosition))
            questionPosition = m_pBoard->FindEmptyCell();

        //TIP: We could have different types of building. We can create several strategies for it and use them.

        std::vector<Direction::DirectionType> possibleDirrections;

        if (questionPosition.m_col == 0 && questionPosition.m_row == 0)
            possibleDirrections = { Direction::DirectionType::DownRight, Direction::DirectionType::RightDown };

        if (questionPosition.m_col != 0 && questionPosition.m_row == 0)
            possibleDirrections = { Direction::DirectionType::Down, Direction::DirectionType::RightDown };

        if (questionPosition.m_col == 0 && questionPosition.m_row != 0)
            possibleDirrections = { Direction::DirectionType::Right, Direction::DirectionType::DownRight };

        if (questionPosition.m_col != 0 && questionPosition.m_row != 0)
            possibleDirrections = {};

        currentQuestion = FindNextWord(questionPosition, possibleDirrections);

        //If we haven't new question we have to delete previous and try again with new one.
        if (!currentQuestion.has_value())
        {
            errorsCount++;

            Question questionToDelete = m_pBoard->GetLastQuestion();

            while (std::find(badQuestions.begin(), badQuestions.end(), questionToDelete) != badQuestions.end())
            {
                m_pBoard->RemoveLastQuestionFromBoard();
                questionToDelete = m_pBoard->GetLastQuestion();
            }

            badQuestions.push_back(questionToDelete);

            questionPosition = m_pBoard->GetQuestions().back().questionPos;

            m_pBoard->RemoveLastQuestionFromBoard();
            //system("cls");
            //m_pBoard->ShowBoard();
            continue;
        }

        m_pBoard->PutQuestionOnBoard(currentQuestion.value());

        Cell nextToWordsEnd = currentQuestion->end + Direction::GetDirectionArrow(currentQuestion->direction).normVec;

        if (m_pBoard->IsCellOnBoard(nextToWordsEnd) && m_pBoard->GetLetterFromCell(nextToWordsEnd) == '.')
            questionPosition = nextToWordsEnd;
        else
            questionPosition = Cell(-1, -1);

        //system("cls");
        //m_pBoard->ShowBoard();

        //std::this_thread::sleep_for(std::chrono::milliseconds(100));

    }
}

/*//red button IF SMTH GOES WRONG or just to build new m_pBoard :)
void BoardBuilder::Restart()
{
    m_pBoard->.clear();
    for (int i = 0; i < m_pBoard->GetHeight(); i++)
    {
        for (int j = 0; j < m_pBoard->GetWidth(); j++)
        {
            (*m_pBoard)[i][j] = '.';
        }
    }
}
*/
