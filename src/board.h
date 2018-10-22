#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include "magic.h"
#include "helper.h"
#include "piece.h"

namespace BoardStructure {
	struct BoardState {
		struct BoardStateRow {
			Piece::Base b[8];
			Piece::Base& operator[](int x){
				return b[x];
			}
		};

		BoardStateRow bR[8];
		BoardStateRow& operator[](int x){
			return (BoardStateRow&) bR[x];
		}
	};

	extern bool gameEnded;
	extern std::vector<BoardState> boardHistory;
	extern BoardState board;
	extern int boardHistorySize;
	extern BoardState initBoard;
	extern Magic::color currMoveColor;

	double getBoardValue();
	Piece::Base* find(sf::Vector2i);
	void initTextures();
	void init();
	void drawPieces(sf::RenderWindow &window);
	bool undoMove();
	void recordMove();
}