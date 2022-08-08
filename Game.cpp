#include <cassert>
#include "Game.h"
#include "Exceptions.h"
#include <exception>

namespace Chess
{
	Game::Game() : is_white_turn(true)
	{
		// Add the pawns
		for (int i = 0; i < 8; i++)
		{
			board.add_piece(Position('A' + i, '1' + 1), 'P');
			board.add_piece(Position('A' + i, '1' + 6), 'p');
		}

		// Add the rooks
		board.add_piece(Position('A' + 0, '1' + 0), 'R');
		board.add_piece(Position('A' + 7, '1' + 0), 'R');
		board.add_piece(Position('A' + 0, '1' + 7), 'r');
		board.add_piece(Position('A' + 7, '1' + 7), 'r');

		// Add the knights
		board.add_piece(Position('A' + 1, '1' + 0), 'N');
		board.add_piece(Position('A' + 6, '1' + 0), 'N');
		board.add_piece(Position('A' + 1, '1' + 7), 'n');
		board.add_piece(Position('A' + 6, '1' + 7), 'n');

		// Add the bishops
		board.add_piece(Position('A' + 2, '1' + 0), 'B');
		board.add_piece(Position('A' + 5, '1' + 0), 'B');
		board.add_piece(Position('A' + 2, '1' + 7), 'b');
		board.add_piece(Position('A' + 5, '1' + 7), 'b');

		// Add the kings and queens
		board.add_piece(Position('A' + 3, '1' + 0), 'Q');
		board.add_piece(Position('A' + 4, '1' + 0), 'K');
		board.add_piece(Position('A' + 3, '1' + 7), 'q');
		board.add_piece(Position('A' + 4, '1' + 7), 'k');
	}

	Game::Game(const Game &game) : board(game.board), is_white_turn(game.is_white_turn) {}

	void Game::make_move(const Position &start, const Position &end)
	{
		Piece *start_piece = board.get_occ()[start];
		Piece *end_piece = board.get_occ()[end];
		if ((start.first < 'A') || (start.first > 'H') || (start.second < '1') || (start.second > '8'))
		{
			throw Exception("start position is not on board");
		}
		if ((end.first < 'A') || (end.first > 'H') || (end.second < '1') || (end.second > '8'))
		{
			throw Exception("end position is not on board");
		}
		if (!(start_piece))
		{
			throw Exception("no piece at start position");
		}
		if (start_piece->is_white() != turn_white())
		{
			throw Exception("piece color and turn do not match");
		}
		if (!(end_piece))
		{ //if no piece in end position
			if (!(start_piece->legal_move_shape(start, end)))
			{
				throw Exception("illegal move shape");
			}
		}
		else
		{
			if (start_piece->is_white() == end_piece->is_white())
			{
				throw Exception("cannot capture own piece");
			}
			if (!(start_piece->legal_capture_shape(start, end)))
			{
				throw Exception("illegal capture shape");
			}
		}
		if (!(board.is_path_clear(start, end)))
		{
			throw Exception("path is not clear");
		}
		Board copy_board(board);
		copy_board.move_piece(start, end);
		if (copy_board.in_check(turn_white()))
		{
			throw Exception("move exposes check");
		}
		board.move_piece(start, end);
		end_piece = board.get_occ()[end];
		if (is_white_turn)
		{
			if ((end.second == '8') && (end_piece->to_ascii()) == 'P')
			{
				delete board.get_occ()[end];
				board.get_occ()[end] = nullptr;
				board.add_piece(end, 'Q');
			}
			is_white_turn = false;
		}
		else
		{
			if ((end.second == '1') && (end_piece->to_ascii()) == 'p')
			{
				delete board.get_occ()[end];
				board.get_occ()[end] = nullptr;
				board.add_piece(end, 'q');
			}
			is_white_turn = true;
		}
	}

	bool Game::in_check(const bool &white) const
	{
		return board.in_check(white);
	}

	bool Game::in_mate(const bool &white) const
	{	
		//if in check but doesn't have legal move return true in mate
		if (in_check(white) && !has_legal_move(white))
		{
			return true;
		}
		return false;
	}

	bool Game::in_stalemate(const bool &white) const
	{	
		if (!has_legal_move(white) && !in_check(white))
		{
			return true;
		}
		return false;
	}

	bool Game::has_legal_move(const bool &white) const
	{
		for (std::map<Position, Piece *>::const_iterator it = board.get_occ_const().cbegin(); it != board.get_occ_const().cend(); ++it)
		{
			if (it->second != NULL)
			{
				if ((white && it->second->is_white()) || (!white && !it->second->is_white()))
				{
					for (Board::iterator bit = board.begin(); bit != board.end(); ++bit)
					{
						Game tempGame(*this);
						try
						{
							tempGame.make_move(it->first, *bit);
							return true;
						}
						catch (const std::exception &e)
						{
							// Invalid move
						}
					}
				}
			}
		}
		return false;
	}

	// Return the total material point value of the designated player
	int Game::point_value(const bool &white) const
	{
		return board.point_value(white);
	}

	std::istream &operator>>(std::istream &is, Game &game)
	{
		if (is.fail())
		{
			throw Exception("Cannot load the game!");
		}
		game.board.clear();
		char curChar;
		for (Board::iterator it = game.board.begin(); it != game.board.end(); ++it)
		{
			is >> curChar;

			// Only continue reading of piece designator is not the dash
			if (curChar != '-')
			{
				try
				{
					game.board.add_piece(*it, curChar);
				}
				catch (const std::exception &e) // If there is an exception, the file format is incorrect
				{
					throw Exception("Cannot load the game!");
				}
			}
		}

		is >> curChar;
		// Throw error if the turn designator is not w or b
		if (curChar != 'w' && curChar != 'b')
		{
			throw Exception("Cannot load the game!");
		}

		game.is_white_turn = curChar == 'w';
		return is;
	}

	std::ostream &operator<<(std::ostream &os, const Game &game)
	{
		// Write the board out and then either the character 'w' or the character 'b',
		// depending on whose turn it is
		return os << game.board << (game.turn_white() ? 'w' : 'b');
	}
}
