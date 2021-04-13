#include "PuzzleSolver.h"

std::vector<DirectionArrows> PuzzleSolver::arrows{
       { {0 , 1},  {0 , 1} },  //0   (y,x)
       { {1 , 0},  {0 , 1} },  //1
       { {-1 , 0}, {0 , 1} },  //2
       { {1 , 0},  {1 , 0} },  //3
       { {0 , -1}, {1 , 0} },  //4
       { {0 , 1},  {1 , 0} }   //5
};

    //ctor

PuzzleSolver::PuzzleSolver(Board &t_board, std::vector<Question> t_questions, std::string t_pathToDatabase, int t_seed)
{
    Question temp;
    pBoard = &t_board;
    Cell DistanceVec;
    std::vector<TableRow> sqlQuery;
    SQLBase base(t_pathToDatabase);
    maxLengthOfWord = 0;
    seed = t_seed;
    if (seed == 0)
    {
        srand(time(NULL));
    }
    else
        srand(seed);

    pBoard->ClearQuestions();
    for (int i = 0; i < t_questions.size(); ++i)
    {
        temp.questionPos = t_questions[i].questionPos;
        temp.start = t_questions[i].start;
        temp.end = t_questions[i].end;
        temp.direction = t_questions[i].direction;
        DistanceVec = t_questions[i].end - t_questions[i].start;
        temp.length = std::max(DistanceVec.Col, DistanceVec.Row) + 1;
        temp.answer = std::string(temp.length, '_');
        pBoard->PushbackQuestion(temp);
        if (temp.length > maxLengthOfWord)
            maxLengthOfWord = temp.length;
        badAnswers.push_back(std::vector < std::string >());
    }

    wordList.resize(maxLengthOfWord+1);
    for (int i = 2; i <= maxLengthOfWord; ++i)
    {
        try
        {
            wordList[i] = std::vector<TableRow>(base.getWords(std::string(i, '_')));
        }
        catch (std::string error)
        {
            throw error;
        }
    }
}

PuzzleSolver::~PuzzleSolver()
{
}

    //Seed
int PuzzleSolver::getSeed()
{
    return seed;
}

void PuzzleSolver::setSeed(int t_seed)
{
    seed = t_seed;
    srand(seed);
}

    //methods to check 

bool PuzzleSolver::isWordInDict(std::string t_word)
{
    int res = 0;
    int wordLength = t_word.length();
    int sequenceLength = wordList[wordLength].size();
    for (int i = 0; i < sequenceLength; ++i)
    {
        int k;
        for (k = 0; k < wordList[wordLength][i].answer.size(); ++k)
        {
            if (!(t_word[k] == wordList[wordLength][i].answer[k] || t_word[k] == '_'))
            {
                break;
            }
        }
        if (k == wordList[wordLength][i].answer.size())
        {
            return true;
        }
    }

    return false;
}

bool PuzzleSolver::isPossibleToSolve(int t_start)
{
    int countOfWords = 1;
  /*  if (t_start < 5)countOfWords = 2;
    else
        countOfWords = 1;*/
    for (int i = t_start + 1; i < pBoard->GetAllQuestions().size(); ++i)
    {
        if (findCountOfWords(pBoard->GetQuestion(i).answer) < countOfWords)
            return false;
    }
    return true;
}

int PuzzleSolver::findCountOfWords(std::string t_word)
{
    int res=0;
    int wordLength = t_word.length();
    int sequenceLength = wordList[wordLength].size();
    for (int i = 0; i < sequenceLength; ++i)
    {
        int k;
        for (k = 0; k < wordList[wordLength][i].answer.size(); ++k)
        {
            if (!(t_word[k] == wordList[wordLength][i].answer[k] || t_word[k] == '_'))
            {
                break;
            }
        }
        if (k == wordList[wordLength][i].answer.size())
        {
            res++;
        }
    }

    return res;
}

std::vector<TableRow> PuzzleSolver::findWords(std::string t_word)
{
    std::vector<TableRow> res;
    int wordLength = t_word.length();
    int sequenceLength = wordList[wordLength].size();
    for (int i=0;i<sequenceLength;++i)
    {
        int k;
        for (k = 0; k < wordList[wordLength][i].answer.size(); ++k)
        {
            if(!(t_word[k]==wordList[wordLength][i].answer[k] || t_word[k]=='_'))
            {
                break;
            }
        }
        if (k == wordList[wordLength][i].answer.size())
        {
            res.push_back(wordList[wordLength][i]);
        }
    }

    return res;
}


void PuzzleSolver::calcIntersections()
{
    std::vector<std::vector<int>> table(pBoard->GetHeight(), std::vector<int>(pBoard->GetWidth(), 0));
    for (int i = 0; i < pBoard->GetAllQuestions().size(); ++i)
    {
        for (Cell j = pBoard->GetQuestion(i).start; j != pBoard->GetQuestion(i).end+arrows[pBoard->GetQuestion(i).direction].NormVec; j = j + arrows[pBoard->GetQuestion(i).direction].NormVec)
        {
            table[j.Row][j.Col]++;
        }
    }

    for (int i = 0; i < pBoard->GetAllQuestions().size(); ++i)
    {
        int countOfIntersection = 0;
        for (Cell j = pBoard->GetQuestion(i).start; j != pBoard->GetQuestion(i).end; j = j + arrows[pBoard->GetQuestion(i).direction].NormVec)
        {
            if (table[j.Row][j.Col] > 1) countOfIntersection++;
        }
        Question temp = pBoard->GetQuestion(i);
        temp.intersections = countOfIntersection;
        pBoard->ChangeQuestion(i,temp);
    }
}


void PuzzleSolver::reloadQuestions()
{
    for (int i = 0; i < pBoard->GetAllQuestions().size(); ++i)
    {
        int k = 0;
        for (Cell j = pBoard->GetQuestion(i).start; j != pBoard->GetQuestion(i).end + arrows[pBoard->GetQuestion(i).direction].NormVec; j = j + arrows[pBoard->GetQuestion(i).direction].NormVec,k++)
        {
            pBoard->GetOriginalQuestion(i).answer[k] = pBoard->GetCell(j);
        }
    }
}

void PuzzleSolver::putOnBoard(Question t_question)
{
    Cell currentCell = t_question.start;
    for (int k = 0; k<t_question.answer.size(); k++)
    {
        pBoard->SetCell(currentCell,t_question.answer[k]);
        currentCell = currentCell + arrows[t_question.direction].NormVec;
    }
}



void PuzzleSolver::clearUselessWords()
{
    for (int i = pBoard->GetAllQuestions().size()-1; i >= 0; i--)
    {
        Cell j = pBoard->GetQuestion(i).start;
        for (int k=0;k<pBoard->GetQuestion(i).answer.size(); k++)
        {
            pBoard->SetCell(j,pBoard->GetQuestion(i).answer[k]);
            j=j+arrows[pBoard->GetQuestion(i).direction].NormVec;
        }
    }
}

bool PuzzleSolver::solvePuzzle()
{
    calcIntersections();
    std::vector<TableRow> sqlQuery;
    for (int i = 0; i < pBoard->GetAllQuestions().size(); ++i)
    {
        if (pBoard->GetQuestion(i).intersections == 0)
        {
            zeroIntersections = i;
            break;
        }
    }
    std::vector<int> chosenWords;
    std::vector<std::string> usedWords;
    std::string strProblemWord,prevWord;
    bool isAnswers;
    int currentQuestion = 0;
    while (currentQuestion < pBoard->GetAllQuestions().size() && currentQuestion != -1)
    {
        sqlQuery = findWords(pBoard->GetQuestion(currentQuestion).answer);
        std::vector<int> suitableWords;
        suitableWords.clear();
        suitableWords.resize(pBoard->GetAllQuestions().size()-currentQuestion);
        int overallMin = 0;
        int bestWord = 0;
        for(int i = 0; i < sqlQuery.size(); i++)
        {
            int currentMin = 100000000;
            isAnswers = true;
            if(std::find(usedWords.begin(),usedWords.end(),sqlQuery[i].answer) == usedWords.end())
            {
                pBoard->GetOriginalQuestion(currentQuestion).answer = sqlQuery[i].answer;
                pBoard->GetOriginalQuestion(currentQuestion).question = sqlQuery[i].question;
                putOnBoard(pBoard->GetQuestion(currentQuestion));
                reloadQuestions();
                for(int j = currentQuestion; j < pBoard->GetAllQuestions().size();j++)
                {
                    int temp = findCountOfWords(pBoard->GetQuestion(j).answer);
                    if(temp < currentMin)
                        currentMin = temp;
                }
                if (currentMin > 10 && currentQuestion < 5)
                {
                    bestWord = i;
                    overallMin = currentMin;
                    break;
                }
                if (overallMin < currentMin)
                {
                    overallMin = currentMin;
                    bestWord = i;
                }
            }
        }
/* deprecated maybe
        chosenWords.assign(sqlQuery.size(), 0);
        do
        {
            isBadAnswer = false;
            isAnswers = false;
            //checking if there is possible answer
            for (int i = 0; i < chosenWords.size(); ++i)
            {
                if (chosenWords[i] == 0)
                {
                    isAnswers = true;
                    break;
                }
            }
            if (!isAnswers) break;

            //Choosing uncheked word
            do
                wordNumber = rand() % sqlQuery.size();
            while (chosenWords[wordNumber] == 1);
            chosenWords[wordNumber] = 1;
            if (std::find(badAnswers[currentQuestion].begin(), badAnswers[currentQuestion].end(), sqlQuery[wordNumber].answer) != badAnswers[currentQuestion].end())
                isBadAnswer = true;
        } while (isBadAnswer && isAnswers);
*/
        if(overallMin == 0 )
        {
            pBoard->GetOriginalQuestion(currentQuestion).answer = std::string(pBoard->GetQuestion(currentQuestion).length, '_');
            pBoard->GetOriginalQuestion(currentQuestion).question = "";
            isAnswers = false;
        }
        
        if (!isAnswers)
        {
            currentQuestion--;
            badAnswers[currentQuestion].push_back(pBoard->GetQuestion(currentQuestion).answer);
            pBoard->GetOriginalQuestion(currentQuestion).answer = std::string(pBoard->GetQuestion(currentQuestion).length, '_');
            pBoard->GetOriginalQuestion(currentQuestion).question = "";
            clearUselessWords();
            reloadQuestions();
            continue;
        }

        pBoard->GetOriginalQuestion(currentQuestion).answer = sqlQuery[bestWord].answer;
        pBoard->GetOriginalQuestion(currentQuestion).question = sqlQuery[bestWord].question;
        putOnBoard(pBoard->GetQuestion(currentQuestion));
        reloadQuestions();
        usedWords.push_back(pBoard->GetQuestion(currentQuestion).answer);
       /* if (!isPossibleToSolve(currentQuestion))
        {
            badAnswers[currentQuestion].push_back(pBoard->GetQuestion(currentQuestion).answer);
            pBoard->GetOriginalQuestion(currentQuestion).answer = std::string(pBoard->GetQuestion(currentQuestion).length, '_');
            clearUselessWords();
            reloadQuestions();
            continue;
        }*/

        currentQuestion++;
        if(currentQuestion<badAnswers.size())
            badAnswers[currentQuestion].clear();
    }

    if(currentQuestion == -1)
        return false;

    return true;
}
