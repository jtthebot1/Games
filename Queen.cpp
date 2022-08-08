#include "Queen.h"

namespace Chess
{
  bool Queen::legal_move_shape(const Position& start, const Position& end) const {

    // Check if the queen is moving like a bishop. Rationale for this code in Bishop.cpp
    if(((int)start.first - (int)end.first) * ((int)start.first - (int)end.first) == 
        ((int)start.second - (int)end.second) * ((int)start.second - (int)end.second))
    {
      return true;
    }

    // Check if the queen is moving like a rook.
    // We already checked if start and end are the same position
    // Therefore, if either of these conditions are satisfied, the other must be not satisfied, meaning the queen moves either horizontally or vertically
    if(start.first == end.first || start.second == end.second)
    {
      return true;
    }


    return false;
  }
}
