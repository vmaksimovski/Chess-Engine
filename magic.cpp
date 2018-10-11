
#include "magic.h"	

namespace Magic {
	const int size = 1920;
	const int boardSize = 8;
	const int cellSize = size / boardSize;
	const int pieces = 7;
	const double pieceWorthCoefficient = 50.0;
	const double INF = 1e10;
	const double pieceWorth[] = {0, 1, 3.2, 3.33, 5.1, 8.8, 100000.0};
	const int propagationLimit = 6;
	color playerColor = Magic::color::white;
	std::string boardString = "img/board.png";
	std::string possibleMove = "img/highlight.png";

	type initialType[][boardSize] = {
	    { rook , knight, bishop , queen , king , bishop, knight, rook  },
	    { pawn , pawn  , pawn   , pawn  , pawn , pawn  , pawn  , pawn  },
	    { blank, blank , blank  , blank , blank, blank , blank , blank },
	    { blank, blank , blank  , blank , blank, blank , blank , blank },
	    { blank, blank , blank  , blank , blank, blank , blank , blank },
	    { blank, blank , blank  , blank , blank, blank , blank , blank },
	    { pawn , pawn  , pawn   , pawn  , pawn , pawn  , pawn  , pawn  },
	    { rook , knight, bishop , queen , king , bishop, knight, rook  }
	};

	color initialColor[][boardSize] = {
		{black, black, black, black, black, black, black, black},
		{black, black, black, black, black, black, black, black},
		{none , none , none , none , none , none , none , none },
		{none , none , none , none , none , none , none , none },
		{none , none , none , none , none , none , none , none },
		{none , none , none , none , none , none , none , none },
		{white, white, white, white, white, white, white, white},
		{white, white, white, white, white, white, white, white}
	};

	
	int pieceValue[][8][8] = {
	   {{ 0 ,  0,  0,  0,  0,  0,  0,  0},
		{ 0 ,  0,  0,  0,  0,  0,  0,  0},
		{ 0 ,  0,  0,  0,  0,  0,  0,  0},
		{ 0 ,  0,  0,  0,  0,  0,  0,  0},
		{ 0 ,  0,  0,  0,  0,  0,  0,  0},
		{ 0 ,  0,  0,  0,  0,  0,  0,  0},
		{ 0 ,  0,  0,  0,  0,  0,  0,  0},
		{ 0 ,  0,  0,  0,  0,  0,  0,  0}},
	   {{ 0 ,  0,  0,  0,  0,  0,  0,  0},
		{ 50, 50, 50, 50, 50, 50, 50, 50},
		{ 10, 10, 20, 30, 30, 20, 10, 10},
		{ 5 ,  5, 10, 25, 25, 10,  5,  5},
		{ 0 ,  0,  0, 20, 20,  0,  0,  0},
		{ 5 , -5,-10,  0,  0,-10, -5,  5},
		{ 5 , 10, 10,-20,-20, 10, 10,  5},
		{ 0 ,  0,  0,  0,  0,  0,  0,  0}},
	   {{-50,-40,-30,-30,-30,-30,-40,-50},
		{-40,-20,  0,  0,  0,  0,-20,-40},
		{-30,  0, 10, 15, 15, 10,  0,-30},
		{-30,  5, 15, 20, 20, 15,  5,-30},
		{-30,  0, 15, 20, 20, 15,  0,-30},
		{-30,  5, 10, 15, 15, 10,  5,-30},
		{-40,-20,  0,  5,  5,  0,-20,-40},
		{-50,-40,-30,-30,-30,-30,-40,-50}},
	   {{ 20,-10,-10,-10,-10,-10,-10,-20},
		{ 10,  0,  0,  0,  0,  0,  0,-10},
		{ 10,  0,  5, 10, 10,  5,  0,-10},
		{ 10,  5,  5, 10, 10,  5,  5,-10},
		{ 10,  0, 10, 10, 10, 10,  0,-10},
		{ 10, 10, 10, 10, 10, 10, 10,-10},
		{ 10,  5,  0,  0,  0,  0,  5,-10},
		{ 20,-10,-10,-10,-10,-10,-10,-20}},
	   {{  0,  0,  0,  0,  0,  0,  0,  0},
		{  5, 10, 10, 10, 10, 10, 10,  5},
		{ -5,  0,  0,  0,  0,  0,  0, -5},
		{ -5,  0,  0,  0,  0,  0,  0, -5},
		{ -5,  0,  0,  0,  0,  0,  0, -5},
		{ -5,  0,  0,  0,  0,  0,  0, -5},
		{ -5,  0,  0,  0,  0,  0,  0, -5},
		{  0,  0,  0,  5,  5,  0,  0,  0}},
	   {{-20,-10,-10, -5, -5,-10,-10,-20},
		{-10,  0,  0,  0,  0,  0,  0,-10},
		{-10,  0,  5,  5,  5,  5,  0,-10},
		{ -5,  0,  5,  5,  5,  5,  0, -5},
		{  0,  0,  5,  5,  5,  5,  0, -5},
		{-10,  5,  5,  5,  5,  5,  0,-10},
		{-10,  0,  5,  0,  0,  0,  0,-10},
		{-20,-10,-10, -5, -5,-10,-10,-20}},
	   {{-30,-40,-40,-50,-50,-40,-40,-30},
		{-30,-40,-40,-50,-50,-40,-40,-30},
		{-30,-40,-40,-50,-50,-40,-40,-30},
		{-30,-40,-40,-50,-50,-40,-40,-30},
		{-20,-30,-30,-40,-40,-30,-30,-20},
		{-10,-20,-20,-20,-20,-20,-20,-10},
		{ 20, 20,  0,  0,  0,  0, 20, 20},
		{ 20, 30, 10,  0,  0, 10, 30, 20}}
	};

	std::string location[][pieces] = {
		{"img/blank.png", 
			"img/white_pawn.png", 
			"img/white_knight.png", 
			"img/white_bishop.png", 
			"img/white_rook.png", 
			"img/white_queen.png",
			 "img/white_king.png"},
		{"img/blank.png", 
			"img/black_pawn.png", 
			"img/black_knight.png", 
			"img/black_bishop.png", 
			"img/black_rook.png", 
			"img/black_queen.png", 
			"img/black_king.png"}
	};

	sf::Texture pieceTextures[2][7];
}
