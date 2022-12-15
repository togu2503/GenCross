#include "PuzzleSolver.h"
#include "Utility.h"
#include "regex"
#include <numeric>

PuzzleSolver::PuzzleSolver(std::string pathToDatabase)
{
    m_pathToDatabase = pathToDatabase;
}

bool PuzzleSolver::IsWordInDict(std::string t_word)
{
    int res = 0;
    int wordLength = t_word.length();
    int sequenceLength = m_dictionary[wordLength].size();
    for (int i = 0; i < sequenceLength; ++i)
    {
        int k;
        for (k = 0; k < m_dictionary[wordLength][i].answer.size(); ++k)
        {
            if (!(t_word[k] == m_dictionary[wordLength][i].answer[k] || t_word[k] == '_'))
            {
                break;
            }
        }
        if (k == m_dictionary[wordLength][i].answer.size())
        {
            return true;
        }
    }

    return false;
}

bool PuzzleSolver::IsPossibleToSolve(int start)
{
    int countOfWords = 1;

    for (int i = start + 1; i < m_zeroIntersections; ++i)
    {
        if (FindCountOfWords(GetSortedQuestion(i).answer) < countOfWords)
            return false;
    }
    return true;
}

int PuzzleSolver::FindCountOfWords(std::string word)
{
    int res = 0;
    int wordLength = word.length();
    int sequenceLength = GetWordsWithLength(wordLength).size();

    for (int i = 0; i < sequenceLength; ++i)
    {
        int k;
        for (k = 0; k < GetWordsWithLength(wordLength)[i].answer.size(); ++k)
        {
            if (!(word[k] == GetWordsWithLength(wordLength)[i].answer[k] || word[k] == '_'))
            {
                break;
            }
        }
        if (k == GetWordsWithLength(wordLength)[i].answer.size())
        {
            res++;
        }
    }

    return res;
}

std::vector<TableRow> PuzzleSolver::FindWords(std::string word)
{
    std::vector<TableRow> res;
    int wordLength = word.length();
    int sequenceLength = GetWordsWithLength(word.size()).size();
    for (int i = 0; i < sequenceLength; ++i)
    {
        int k;
        for (k = 0; k < GetWordsWithLength(wordLength)[i].answer.size(); ++k)
        {
            if (!(word[k] == GetWordsWithLength(wordLength)[i].answer[k] || word[k] == '_'))
            {
                break;
            }
        }
        if (k == GetWordsWithLength(wordLength)[i].answer.size())
        {
            res.push_back(GetWordsWithLength(wordLength)[i]);
        }
    }

    return res;
}

const Question& PuzzleSolver::GetSortedQuestion(int index)
{
    return m_pBoard->GetQuestions()[m_questionIndicies[index]];
}


void PuzzleSolver::CalcIntersections()
{
    std::iota(m_questionIndicies.begin(), m_questionIndicies.end(), 0);

    std::vector<std::vector<int>> table(m_pBoard->GetHeight(), std::vector<int>(m_pBoard->GetWidth(), 0));
    for (int i = 0; i < m_pBoard->GetQuestions().size(); ++i)
    {
        for (Cell j = m_pBoard->GetQuestions()[i].start; j != m_pBoard->GetQuestions()[i].end + Direction::GetDirectionArrow(m_pBoard->GetQuestions()[i].direction).normVec;
            j += Direction::GetDirectionArrow(m_pBoard->GetQuestions()[i].direction).normVec)
        {
            table[j.m_row][j.m_col]++;
        }
    }

    for (int i = 0; i < m_pBoard->GetQuestions().size(); ++i)
    {
        int countOfIntersection = 0;
        for (Cell j = m_pBoard->GetQuestions()[i].start; j != m_pBoard->GetQuestions()[i].end; j += Direction::GetDirectionArrow(m_pBoard->GetQuestions()[i].direction).normVec)
        {
            if (table[j.m_row][j.m_col] > 1) countOfIntersection++;
        }
        m_intersections[i] = countOfIntersection;
    }
}

void PuzzleSolver::SortQuestionsIndiciesByIntersections()
{
    std::vector<std::pair<int, int>> sortingVector;
    sortingVector.reserve(m_intersections.size());
    std::transform(m_intersections.begin(), m_intersections.end(), m_questionIndicies.begin(), std::back_inserter(sortingVector),
        [](int& a, int& b) { return std::make_pair(a, b); });

    std::sort(std::begin(sortingVector), std::end(sortingVector), [](std::pair<int, int> a, std::pair<int, int> b) {return a.first > b.first; });

    for (int i = 0; i < m_intersections.size(); i++)
    {
        m_intersections[i] = sortingVector[i].first;
        m_questionIndicies[i] = sortingVector[i].second;
    }
}

const std::vector<TableRow>& PuzzleSolver::GetWordsWithLength(int length)
{
    return m_dictionary[length - m_minLengthOfWord];
}

bool PuzzleSolver::FillDictionary()
{
    SQLBase base(m_pathToDatabase);

    m_dictionary.resize((m_maxLengthOfWord - m_minLengthOfWord) + 1);

    for (int i = 0; i < m_dictionary.size(); ++i)
    {
        try
        {
            m_dictionary[i] = std::vector<TableRow>(base.getWords(std::string(m_minLengthOfWord + i, '_')));
        }
        catch (std::string error)
        {
            std::cout << error;
            return false;
        }
        if(m_dictionary[i].empty())
            return false;
    }
    return true;
}

void PuzzleSolver::SolvePuzzle(Board& board, int seed)
{

    m_pBoard = &board;

    if(m_pBoard->GetQuestions().empty())
        return;

    m_seed = seed;

    m_intersections.resize(m_pBoard->GetQuestions().size());
    m_questionIndicies.resize(m_pBoard->GetQuestions().size());
    m_badAnswers.resize(m_pBoard->GetQuestions().size());

    auto comparator = [](const Question& a, const Question& b) { return a.length < b.length; };
    auto [min, max] = std::minmax_element(m_pBoard->GetQuestions().begin(), m_pBoard->GetQuestions().end(), comparator);
    m_minLengthOfWord = min->length;
    m_maxLengthOfWord = max->length;


    CalcIntersections();
    std::vector<TableRow> sqlQuery;

    if (!FillDictionary())
        return;

    SortQuestionsIndiciesByIntersections();

    for (int i = 0; i < m_pBoard->GetQuestions().size(); ++i)
    {
        if (m_intersections[m_questionIndicies[i]] == 0)
        {
            m_zeroIntersections = i;
            break;
        }
    }
    int wordNumber;
    std::vector<int> chosenWords;
    std::vector<std::string> usedWords;
    Question* problemWord;
    std::string strProblemWord, prevWord;
    bool isBadAnswer, isAnswers;
    int currentQuestion = 0;
    while (currentQuestion < m_pBoard->GetQuestions().size())
    {
        sqlQuery = FindWords(GetSortedQuestion(currentQuestion).answer);

        if(sqlQuery.empty())
        {
            currentQuestion--;
            m_badAnswers[currentQuestion].push_back(m_pBoard->GetQuestions()[currentQuestion].answer);
            m_pBoard->SetQuestionAnswer(m_questionIndicies[currentQuestion], "", std::string(GetSortedQuestion(currentQuestion).length, '_'));

            continue;
        }

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
            if (!m_badAnswers.empty() && std::find(m_badAnswers[currentQuestion].begin(), m_badAnswers[currentQuestion].end(), sqlQuery[wordNumber].answer) != m_badAnswers[currentQuestion].end())
                isBadAnswer = true;
        } while (isBadAnswer && isAnswers);

        if (!isAnswers)
        {
            currentQuestion--;
            m_badAnswers[currentQuestion].push_back(sqlQuery[wordNumber].answer);
            m_pBoard->SetQuestionAnswer(m_questionIndicies[currentQuestion], "", std::string(GetSortedQuestion(currentQuestion).length, '_'));

            continue;
        }


        m_pBoard->SetQuestionAnswer(m_questionIndicies[currentQuestion], sqlQuery[wordNumber].question, sqlQuery[wordNumber].answer);

        usedWords.push_back(GetSortedQuestion(currentQuestion).answer);
        if (!IsPossibleToSolve(currentQuestion))
        {
            m_badAnswers[currentQuestion].push_back(sqlQuery[wordNumber].answer);
            m_pBoard->SetQuestionAnswer(m_questionIndicies[currentQuestion], "", std::string(GetSortedQuestion(currentQuestion).length, '_'));

            continue;
        }

        currentQuestion++;
        if (currentQuestion < m_badAnswers.size())
            m_badAnswers[currentQuestion-1].clear();
    }

}
