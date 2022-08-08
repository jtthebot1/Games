#include "Pawn.h"

namespace Chess
{
  bool Pawn::legal_move_shape(const Position &start, const Position &end) const
  {
    if (start.first != end.first)
    {
      return false;
    }
    if (is_white())
    {
      if (start.second == '2')
      {
        if ((end.second == '3') || (end.second == '4'))
        {
          return true;
        }
      }
      else
      {
        if ((int)end.second == (int)start.second + 1)
        {
          return true;
        }
      }
      return false;
    }
    else
    {
      if (start.second == '7')
      {
        if ((end.second == '6') || (end.second == '5'))
        {
          return true;
        }
      }
      else
      {
        if ((int)end.second == (int)start.second - 1)
        {
          return true;
        }
      }
      return false;
    }
  }
  bool Pawn::legal_capture_shape(const Position &start, const Position &end) const
  {
    // If the pawn is white, check the diagonals one row up
    if (is_white())
    {
      // Diagonal to the left
      if ((int)end.first == (int)start.first - 1 && (int)end.second == (int)start.second + 1)
      {
        return true;
      }

      // Diagonal to the right
      if ((int)end.first == (int)start.first + 1 && (int)end.second == (int)start.second + 1)
      {
        return true;
      }

      return false;
    }
    else // If the pawn is black, check the diagonals one row down
    {
      // Diagonal to the left
      if ((int)end.first == (int)start.first - 1 && (int)end.second == (int)start.second - 1)
      {
        return true;
      }
      // Diagonal to the right
      if ((int)end.first == (int)start.first + 1 && (int)end.second == (int)start.second - 1)
      {
        return true;
      }

      return false;
    }
  }
}
