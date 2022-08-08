#include "King.h"

namespace Chess
{
  bool King::legal_move_shape(const Position &start, const Position &end) const
  {
    // Check to make sure that the end position is within 1 square from the start
    if (((int)start.first - (int)end.first) * ((int)start.first - (int)end.first) > 1)
    {
      return false;
    }

    // Check second coordinate using above technique
    if (((int)start.second - (int)end.second) * ((int)start.second - (int)end.second) > 1)
    {
      return false;
    }

    return true;
  }
}
