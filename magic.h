#pragma once

#include <string>
#include <SFML/Graphics.hpp>

namespace Magic {
	enum color {
		none = -1,
		white,
		black
	};
	enum type {
		blank = 0,
		pawn,
		knight,
		bishop,
		rook,
		queen,
		king
	};

	extern const int size;
	extern const int boardSize;
	extern const int cellSize;
	extern const int pieces;
	extern const int propagationLimit;
	extern const double pieceWorthCoefficient;
	extern const double pieceWorth[7];
	extern color playerColor;
	extern std::string boardString;

	extern type initialType[8][8];
	extern color initialColor[8][8];
	
	extern int pieceValue[][8][8];
	extern std::string location[2][7];
	extern sf::Texture pieceTextures[2][7];
}