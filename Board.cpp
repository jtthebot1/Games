#include <iostream>
#include <utility>
#include <map>
#ifndef _WIN32
#include "Terminal.h"
#endif // !_WIN32
#include "Board.h"
#include "CreatePiece.h"
#include "Exceptions.h"

namespace Chess
{
	Board::iterator &Board::iterator::operator++()
	{
		// If we are at the end of the row, go to next row
		if (pos.first == 'H')
		{
			pos = Position('A', pos.second - 1);
			return *this;
		}
		else
		{
			pos = Position(pos.first + 1, pos.second);
			return *this;
		}
	}
	/////////////////////////////////////
	// DO NOT MODIFY THIS FUNCTION!!!! //
	/////////////////////////////////////
	Board::Board() {}

	Board::Board(const Board &board)
	{
		for (std::map<Position, Piece *>::const_iterator it = board.occ.begin(); it != board.occ.end(); ++it)
		{
			if (it->second != NULL)
			{
				add_piece(it->first, it->second->to_ascii());
			}
		}
	}

	Board::~Board()
	{
		clear();
	}

	void Board::clear()
	{
		// Loop through each occupied position and delete the Piece occupying the square
		for (std::map<Position, Piece *>::iterator it = occ.begin(); it != occ.end(); ++it)
		{
			delete it->second;
		}

		// Clear the map
		occ.clear();
	}

	void Board::move_piece(const Position &start, const Position &end)
	{
		if (occ[end] != NULL)
		{
			delete occ[end];
		}
		occ[end] = occ[start];
		occ[start] = nullptr;
	}

	const Piece *Board::operator()(const Position &position) const
	{
		if (occ.find(position) != occ.end())
		{
			return occ.at(position);
		}

		return NULL;
	}

	void Board::add_piece(const Position &position, const char &piece_designator)
	{
		// Check if the piece designator is invalid

		if (!(piece_designator == 'K' || piece_designator == 'k' ||
			  piece_designator == 'Q' || piece_designator == 'q' ||
			  piece_designator == 'R' || piece_designator == 'r' ||
			  piece_designator == 'B' || piece_designator == 'b' ||
			  piece_designator == 'N' || piece_designator == 'n' ||
			  piece_designator == 'P' || piece_designator == 'p' ||
			  piece_designator == 'M' || piece_designator == 'm'))
		{
			throw Exception("invalid designator");
		}
		// Check if the position is on the board
		if (position.first > 'H' || position.first < 'A' || position.second > '8' || position.second < '1')
		{
			throw Exception("invalid position");
		}
		// Check if the position is already occupied
		if (occ.find(position) != occ.end() && occ.at(position) != NULL)
		{
			throw Exception("position is occupied");
		}

		occ[position] = create_piece(piece_designator);
	}

	int Board::point_value(const bool &white) const
	{
		int total_material = 0;
		for (std::map<Position, Piece *>::const_iterator it = occ.cbegin(); it != occ.cend(); ++it)
		{
			if (it->second != NULL)
			{
				if (it->second->is_white() == white)
				{
					total_material += it->second->point_value();
				}
			}
		}
		return total_material;
	}

	bool Board::in_check(const bool &white) const
	{
		//pos for king and enemy
		Position king_pos;
		Position enemy_pos;

		//loop to find king
		for (char j = 'A'; j <= 'H'; j++)
		{
			for (char i = '1'; i <= '8'; i++)
			{
				king_pos = Position(j, i);
				//make sure not null
				if (occ.find(king_pos) != occ.end() && occ.at(king_pos) != nullptr)
				{
					if (occ.at(king_pos)->to_ascii() == 'K' && occ.at(king_pos)->is_white() == white)
					{
						//check to see if any players can capture king
						for (char b = 'A'; b <= 'H'; b++)
						{
							for (char c = '1'; c <= '8'; c++)
							{
								enemy_pos = Position(b, c);
								if (occ.find(enemy_pos) != occ.end() && occ.at(enemy_pos) != nullptr && occ.at(enemy_pos)->is_white() != white)
								{	
									//check if the path from given piece to king is clear
									if (is_path_clear(enemy_pos, king_pos))
									{	
										//if can be captured legally, return true in check
										if (occ.at(enemy_pos)->legal_capture_shape(enemy_pos, king_pos))
										{
											return true;
										}
									}
								}
							}
						}
					}
					else if (occ.at(king_pos)->to_ascii() == 'k' && occ.at(king_pos)->is_white() == white)
					{
						//check to see if any players can capture king
						for (char b = 'A'; b <= 'H'; b++)
						{
							for (char c = '1'; c <= '8'; c++)
							{
								enemy_pos = Position(b, c);
								if (occ.find(enemy_pos) != occ.end() && occ.at(enemy_pos) != nullptr && occ.at(enemy_pos)->is_white() != white)
								{	
									//check if the path from given piece to king is clear
									if (is_path_clear(enemy_pos, king_pos))
									{	
										//if can be captured legally, return true in check
										if (occ.at(enemy_pos)->legal_capture_shape(enemy_pos, king_pos))
										{
											return true;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		return false;
	}

	void Board::display() const
	{
		for (iterator it = begin(); it != end(); ++it)
		{
			// Add numbers to the side
			if ((*it).first == 'A')
			{
				std::cout << (*it).second;
			}

			// Change the background of the board depending on whether the square should be white or black (displayed as green)
			if (((int)(*it).first + (int)(*it).second) % 2 == 0)
			{
				Terminal::color_bg(Terminal::GREEN);
			}
			else
			{
				Terminal::color_bg(Terminal::WHITE);
			}

			// Check to see if square is occupied by a piece and display the piece
			if ((*this)(*it) == NULL)
			{
				std::cout << " ";
			}
			else
			{
				// Display pieces differently based on color
				if ((*this)(*it)->is_white())
				{
					Terminal::color_fg(true, Terminal::WHITE);
				}
				else
				{
					Terminal::color_fg(true, Terminal::BLACK);
				}
				std::cout << (*this)(*it)->to_unicode();
			}

			Terminal::set_default();

			// Insert newline when reaching end of row
			if ((*it).first == 'H')
			{
				std::cout << std::endl;
			}
		}
		// Add letters to the bottom
		std::cout << " ABCDEFGH" << std::endl;
	}

	bool Board::is_path_clear(const Position &start, const Position &end) const
	{
		// Start and end can't be the same
		if (start == end)
		{
			return false;
		}

		// Check vertical blockage
		if (start.first == end.first)
		{
			if (start.second > end.second)
			{
				for (int i = 1; i < (int)start.second - (int)end.second; ++i)
				{
					if (occ.find(Position(start.first, end.second + i)) != occ.end() && occ.at(Position(start.first, end.second + i)) != NULL)
					{
						return false;
					}
				}
			}
			else
			{
				for (int i = 1; i < (int)end.second - (int)start.second; ++i)
				{
					if (occ.find(Position(start.first, start.second + i)) != occ.end() && occ.at(Position(start.first, start.second + i)) != NULL)
					{
						return false;
					}
				}
			}
		}

		// Check horizontal blockage
		if (start.second == end.second)
		{
			if (start.first > end.first)
			{
				for (int i = 1; i < (int)start.first - (int)end.first; ++i)
				{
					if (occ.find(Position(end.first + i, start.second)) != occ.end() && occ.at(Position(end.first + i, start.second)) != NULL)
					{
						return false;
					}
				}
			}
			else
			{
				for (int i = 1; i < (int)end.first - (int)start.first; ++i)
				{
					if (occ.find(Position(start.first + i, start.second)) != occ.end() && occ.at(Position(start.first + i, start.second)) != NULL)
					{
						return false;
					}
				}
			}
		}

		// Check lowerleft to upperright diagonal blockage
		if ((int)start.second - (int)end.second == (int)start.first - (int)end.first)
		{
			if (start.first > end.first)
			{
				for (int i = 1; i < (int)start.first - (int)end.first; ++i)
				{
					if (occ.find(Position(end.first + i, end.second + i)) != occ.end() && occ.at(Position(end.first + i, end.second + i)) != NULL)
					{
						return false;
					}
				}
			}
			else
			{
				for (int i = 1; i < (int)end.first - (int)start.first; ++i)
				{
					if (occ.find(Position(start.first + i, start.second + i)) != occ.end() && occ.at(Position(start.first + i, start.second + i)) != NULL)
					{
						return false;
					}
				}
			}
		}

		// Check upperleft to lowerright diagonal blockage
		if ((int)start.second - (int)end.second == (int)end.first - (int)start.first)
		{
			if (start.first > end.first)
			{
				for (int i = 1; i < (int)start.first - (int)end.first; ++i)
				{
					if (occ.find(Position(end.first + i, end.second - i)) != occ.end() && occ.at(Position(end.first + i, end.second - i)) != NULL)
					{
						return false;
					}
				}
			}
			else
			{
				for (int i = 1; i < (int)end.first - (int)start.first; ++i)
				{
					if (occ.find(Position(start.first + i, start.second - i)) != occ.end() && occ.at(Position(start.first + i, start.second - i)) != NULL)
					{
						return false;
					}
				}
			}
		}
		return true;
	}

	bool Board::has_valid_kings() const
	{
		int white_king_count = 0;
		int black_king_count = 0;
		for (std::map<std::pair<char, char>, Piece *>::const_iterator it = occ.begin();
			 it != occ.end();
			 it++)
		{
			if (it->second)
			{
				switch (it->second->to_ascii())
				{
				case 'K':
					white_king_count++;
					break;
				case 'k':
					black_king_count++;
					break;
				}
			}
		}
		return (white_king_count == 1) && (black_king_count == 1);
	}

	/////////////////////////////////////
	// DO NOT MODIFY THIS FUNCTION!!!! //
	/////////////////////////////////////
	std::ostream &operator<<(std::ostream &os, const Board &board)
	{
		for (char r = '8'; r >= '1'; r--)
		{
			for (char c = 'A'; c <= 'H'; c++)
			{
				const Piece *piece = board(Position(c, r));
				if (piece)
				{
					os << piece->to_ascii();
				}
				else
				{
					os << '-';
				}
			}
			os << std::endl;
		}
		return os;
	}
}
