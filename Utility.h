#pragma once
#include <iostream>
#include <istream>
#include "Cell.h"

inline std::string GetImagePathByArrow(int i)
{
    switch (i) {
    case 0: return std::string("right.png");
    case 1: return std::string("down_right.png");
    case 2: return std::string("up_right.png");
    case 3: return std::string("down.png");
    case 4: return std::string("left_down.png");
    case 5: return std::string("right_down.png");
    default: return std::string("");
    }
}

inline std::vector<std::string> SplitOnWords(std::string str)
{
    std::vector<std::string> res;
    std::string temp;
    temp.clear();
    for(int i=0;i<str.size();i++)
    {
        if(str[i]!=' ')
            temp.append(1,str[i]);
        else
        {
            res.push_back(temp);
            temp.clear();
        }
    }
    res.push_back(temp);
    return res;
}


struct MultiLineText
{
    std::vector<std::string> lines;
    int lineLength;

    MultiLineText(std::string str,int lengthOfLine)
    {
       lineLength = lengthOfLine;
       std::string line;
       bool isWordInLine = false;
       int currentLength = 0;
       std::vector<std::string> words = SplitOnWords(str);
       for(int i = 0; i < words.size(); i++)
       {
           if(currentLength + words[i].size() < lengthOfLine)
           {
               line.append(words[i]);
               isWordInLine = true;
               currentLength += words[i].size();
               if(currentLength + 1 < lengthOfLine)
               {
                   line.append(" ");
               }
           }
           else
           {
               if(!isWordInLine)
               {
                   line.append(words[i].c_str(),lengthOfLine-1);
                   line.append("-\n");
                   lines.push_back(line);
                   line.clear();
                   line.append(words[i].substr(lengthOfLine-1));
                   line.append(" ");
                   currentLength = words[i].substr(lengthOfLine).size();
                   isWordInLine = true;
                   continue;
               }
               line.insert(0,std::string(" ",(lengthOfLine-currentLength)/2));
               line.append("\n");
               lines.push_back(line);
               line.clear();
               currentLength = 0;
               isWordInLine = false;
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

struct Question
{
public:
    Question()
    {
        questionPos = {0,0};
        start = {0,0};
        end = {0,0};
        direction = 0;
        intersections = 0;
        length = 0;
        question = "";
        answer = "";
    }
    Question(Cell qPos, Cell qStart, Cell qEnd, int d, int len)
    {
        questionPos = qPos;
        start = qStart;
        end = qEnd;
        direction = d;
        length = len;
    }

    Question(const Question& q)
    {
        questionPos = q.questionPos;
        question = q.question;
        answer = q.answer;
        start = q.start;
        end = q.end;
        direction = q.direction;
        length = q.length;
        intersections = q.intersections;
    }
    int intersections;
    std::string question;
    std::string answer;
    Cell questionPos;
    Cell start;
    Cell end;
    int direction, length;

    bool operator == (Question a)
    {
        return(a.questionPos == questionPos && a.direction == direction && start == a.start && end == a.end);
    }
    static bool intersectionsCompare(Question t_first, Question t_second)
    {
        //std::cout << float(t_first.intersections) / float(t_first.answer.size()) << " " << float(t_second.intersections) / float(t_second.answer.size());
        if (float(t_first.intersections)/float(t_first.answer.size()) > float(t_second.intersections)/float(t_second.answer.size()))
        {
            return true;
        }
        return false;
    }

};


struct TableRow
{
    std::string question;
    std::string answer;

    TableRow(std::string t_question, std::string t_answer)
    {
        question = t_question;
        answer = t_answer;
    }
};
