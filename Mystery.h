#ifndef MYSTERY_H
#define MYSTERY_H

#include "Piece.h"

namespace Chess
{
	class Mystery : public Piece {

	public:
		bool legal_move_shape(const Position& start, const Position& end) const override{// Check if the abs value of difference between first coordinates is 2 and the second coordinates is 1
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

		char to_ascii() const override { return is_white() ? 'M' : 'm';	}
    
    std::string to_unicode() const override { return is_white() ? "\u2687" : "\u2689"; }

		int point_value() const override {return 20;}

	private:
		Mystery(bool is_white) : Piece(is_white) {}

		friend Piece* create_piece(const char& piece_designator);
	};
}
#endif // MYSTERY_H
