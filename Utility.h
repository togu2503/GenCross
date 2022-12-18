#pragma once
#include <iostream>
#include <istream>
#include <vector>

#include "Cell.h"

std::vector<std::string> SplitOnWords(std::string str);


struct MultiLineText
{
    std::vector<std::string> lines;
    int lineLength;

    MultiLineText(std::string str,int lengthOfLine)
    {
       lineLength = lengthOfLine;
       std::string line;
       int currentLength = 0;
       std::vector<std::string> words = SplitOnWords(str);
       for(int i = 0; i < words.size(); i++)
       {
           if(currentLength + words[i].size() <= lengthOfLine)
           {
               line.append(words[i]);
               currentLength += words[i].size();
               if(currentLength + 1 <= lengthOfLine)
               {
                   line.append(" ");
                   currentLength++;
               }
               else
               {
                   line.append("-\n");
                   lines.push_back(line);
                   line.clear();
                   currentLength = 0;
               }
           }
           else
           {
               if(lengthOfLine - currentLength > 2)
               {
                   line.append(words[i].c_str(), lengthOfLine - currentLength - 1);
                   line.append("-\n");
                   lines.push_back(line);
                   line.clear();
                   line.append(words[i], lengthOfLine - currentLength - 1, std::string::npos);
               }
               else
               {
                   line.insert(0,std::string(" ",(lengthOfLine-currentLength)/2));
                   line.append("\n");
                   lines.push_back(line);
                   line.clear();
                   line.append(words[i]);
               }

               line.append(" ");
               currentLength = line.size();
           }
       }
       lines.push_back(line);
    }

    std::string GetString()
    {
        std::string res;
        for(int i = 0; i < lines.size(); i++)
        {
            res.append(lines[i]);
        }
        return res;
    }

};

namespace Direction
{

    struct DirectionArrow
    {
        Cell offset;
        Cell normVec;

        bool operator == (DirectionArrow another)
        {
            return offset == another.offset && normVec == another.normVec;
        }

        bool operator != (DirectionArrow another)
        {
            return !(*this == another);
        }
    };


    const DirectionArrow kRight     { Cell(0,  1), Cell(0,  1) };
    const DirectionArrow kDown      { Cell(1,  0), Cell(1,  0) };
    const DirectionArrow kDownRight { Cell(1,  0), Cell(0,  1) };
    const DirectionArrow kUpRight   { Cell(-1, 0), Cell(0, 1) };
    const DirectionArrow kRightDown { Cell(0,  1), Cell(1,  0) };
    const DirectionArrow kLeftDown  { Cell(0, -1), Cell(1, 0) };

    constexpr int DirectionsCount = 6;

    enum class DirectionType
    {
        None = -1,
        Right,
        Down,
        RightDown,
        LeftDown,
        DownRight,
        UpRight
    };

    inline std::string GetImagePathByArrow(Direction::DirectionType i)
    {
        switch (i) {
        case Direction::DirectionType::Right: return std::string("right.png");
        case Direction::DirectionType::DownRight: return std::string("down_right.png");
        case Direction::DirectionType::UpRight: return std::string("up_right.png");
        case Direction::DirectionType::Down: return std::string("down.png");
        case Direction::DirectionType::LeftDown: return std::string("left_down.png");
        case Direction::DirectionType::RightDown: return std::string("right_down.png");
        default: return std::string("");
        }
    }


    DirectionType GetDirectionType(DirectionArrow arrow);

    bool IsValidDirectionArrow(DirectionArrow arrow);

    DirectionArrow GetDirectionArrow(DirectionType direction);
}

struct Question
{
    Cell questionPos, start, end;
    Direction::DirectionType direction;
    int length;
    std::string question;
    std::string answer;

    Question();
    Question(const Cell& tQuestionPosition, const Cell& tStart, const Cell& tEnd, const Direction::DirectionType& tDirection,
        const int& tLength, const std::string& tQuestion, const std::string& tAnswer);

    bool operator == (const Question& a)
    {
        return questionPos == a.questionPos && start == a.start && end == a.end && direction == a.direction && length == a.length &&
            question == a.question && answer == a.answer;
    }
};

struct TableRow
{
    std::string question;
    std::string answer;

    TableRow(std::string t_question, std::string t_answer);
};


int Clamp(int min, int max, int value);
