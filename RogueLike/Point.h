#pragma once
#include "Coord.h"
char emptyChar = '.';
using namespace std;
class Point
{
public:
    char icon = emptyChar;
    Coord coord;
    SuperObject* into;
    Point() : icon{ emptyChar }, coord(0, 0), into{ nullptr } {};
    Point(Coord coordP) : coord{ coordP } {};
    Point(Coord coordP, SuperObject* intoP, char iconP = emptyChar) :
        icon{ iconP }, coord(coordP), into{ intoP } {};

    Point* operator()(char iconP)
    {
        icon = iconP;
        return this;
    }

    void clear(int xP = 0, int yP = 0)
    {
        coord(0, 0);
        icon = emptyChar;
        into = nullptr;

    }
};

