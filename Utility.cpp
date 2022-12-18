#pragma once

#include "Utility.h"

namespace Direction
{
    DirectionType GetDirectionType(DirectionArrow arrow)
    {
        if (arrow == kRight)
            return DirectionType::Right;

        if (arrow == kDown)
            return DirectionType::Down;

        if (arrow == kRightDown)
            return DirectionType::RightDown;

        if (arrow == kLeftDown)
            return DirectionType::LeftDown;

        if (arrow == kDownRight)
            return DirectionType::DownRight;

        if (arrow == kUpRight)
            return DirectionType::UpRight;

        return DirectionType::None;
    }

    bool IsValidDirectionArrow(DirectionArrow arrow)
    {
        return GetDirectionType(arrow) != DirectionType::None;
    }

    DirectionArrow GetDirectionArrow(DirectionType direction)
    {
        if (direction == DirectionType::Right)
            return kRight;

        if (direction == DirectionType::Down)
            return kDown;

        if (direction == DirectionType::RightDown)
            return kRightDown;

        if (direction == DirectionType::LeftDown)
            return kLeftDown;

        if (direction == DirectionType::DownRight)
            return kDownRight;

        if (direction == DirectionType::UpRight)
            return kUpRight;

        return DirectionArrow();
    }
}

std::vector<std::string> SplitOnWords(std::string str)
{
    std::vector<std::string> res;

    std::string delimiter = " ";

    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        res.push_back(token);
        str.erase(0, pos + delimiter.length());
    }

    return res;
}

Question::Question() {};

Question::Question(const Cell& tQuestionPosition, const Cell& tStart, const Cell& tEnd, const Direction::DirectionType& tDirection,
    const int& tLength, const std::string& tQuestion, const std::string& tAnswer)
{
    questionPos = tQuestionPosition;
    start = tStart;
    end = tEnd;
    direction = tDirection;
    length = tLength;
    question = tQuestion;
    answer = tAnswer;
}

TableRow::TableRow(std::string t_question, std::string t_answer)
{
    question = t_question;
    answer = t_answer;
}

int Clamp(int min, int max, int value)
{
    return std::max(min, std::min(max, value));
}
