#pragma once
#include <iostream>
#include <vector>
#include <math.h>

struct Cell
{
    int Row;
    int Col;

    bool operator == (Cell a)
    {
        return(a.Row == Row && a.Col == Col);
    }

    friend std::ostream& operator<<(std::ostream& outstream, const Cell& cell)
    {
        outstream << cell.Row << " " << cell.Col;
        return outstream;
    }

    Cell operator = (Cell const a)
    {
        Row = a.Row;
        Col = a.Col;
        return (*this);
    }


    Cell operator + (Cell const a) const
    {
        Cell res;
        res.Row = Row + a.Row;
        res.Col = Col + a.Col;
        return res;
    }

    Cell operator - (Cell const a) const
    {
        Cell res;
        res.Row = Row - a.Row;
        res.Col = Col - a.Col;
        return res;
    }

    Cell operator +=(Cell const a)
    {
        Row += a.Row;
        Col += a.Col;
        return *this;
    }

    Cell operator *(int k) const
    {
        Cell res = *this;
        res.Row *= k;
        res.Col *= k;
        return res;
    }

    bool operator ==(int k)
    {
        int distance;
        distance = sqrt(Row * Row + Col * Col);
        if (distance == k) 
            return true;
        else
            return false;
    }

    bool operator !=(int k)
    {
        return !(*this == k);
    }

    bool operator !=(Cell a)
    {
        return !(*this == a);
    }

    bool IsOnLine(Cell start, Cell end)
    {
        if (Row == start.Row && Row == end.Row)
        {
            if (Col >= start.Col && Col <= end.Col)
                return true;
            else
                return false;
        }
        if (Col == start.Col && Col == end.Col)
        {
            if (Row >= start.Row && Row <= end.Row)
                return true;
            else
                return false;
        }
        return false;
    }
};




