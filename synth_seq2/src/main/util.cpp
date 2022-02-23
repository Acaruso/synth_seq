#include "util.hpp"

#include <iostream>
#include <sstream>

bool isCoordInsideRect(Coord coord, Rect rect)
{
    return (
        coord.x >= rect.x &&
        coord.y >= rect.y &&
        coord.x <= rect.x + rect.w &&
        coord.y <= rect.y + rect.h
    );
}

// clamp x between min inclusive and max exclusive
int clamp(int x, int min, int max)
{
    if (x < min) {
        return min;
    } else if ( x >= max) {
        return max - 1;
    } else {
        return x;
    }
}

std::string pad(int digits, std::string in)
{
    std::stringstream ss;
    for (int i = 0; i < digits - in.size(); i++) {
        ss << " ";
    }
    ss << in;
    return ss.str();
}
