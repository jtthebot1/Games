#include "Knight.h"

namespace Chess
{
  bool Knight::legal_move_shape(const Position& start, const Position& end) const {

    // Check if the abs value of difference between first coordinates is 2 and the second coordinates is 1
    // We compare the square of the difference due to reasons stated in King.cpp and Bishop.cpp
    if(((int)start.first - (int)end.first) * ((int)start.first - (int)end.first) == 4 &&
        ((int)start.second - (int)end.second) * ((int)start.second - (int)end.second) == 1)
    {
      return true;
    }
    // Check if the abs value of difference between first coordinates is 1 and the second coordinates is 2
    // We compare the square of the difference due to reasons stated in King.cpp and Bishop.cpp
    if(((int)start.first - (int)end.first) * ((int)start.first - (int)end.first) == 1 &&
        ((int)start.second - (int)end.second) * ((int)start.second - (int)end.second) == 4)
    {
      return true;
    }
    return false;
  }
}
