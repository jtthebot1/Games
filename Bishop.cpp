#include "Bishop.h"

namespace Chess
{
  bool Bishop::legal_move_shape(const Position &start, const Position &end) const
  {

    // Check to make sure the end square is a diagonal away from the start square
    // This occurs if the absolute value of the difference between the start first and end first coordinates
    // are the same as the abs value of the difference between the start second and second coordinates
    // We can square the differences and compare them because the squared differences are only equal if and only if
    // the absolute value of the difference are the same
    if (((int)start.first - (int)end.first) * ((int)start.first - (int)end.first) != ((int)start.second - (int)end.second) * ((int)start.second - (int)end.second))
    {
      return false;
    }

    return true;
  }
}
