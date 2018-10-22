#pragma once

#include "piece.h"
#include "magic.h"
#include "helper.h"
#include "board.h"
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <vector>

namespace GameHandler {
	bool isAttacked(sf::Vector2i, Magic::color);
	bool movePossible(Magic::color, bool);
	bool isKingAttacked(Magic::color);
	bool canPieceMove(sf::Vector2i, Piece::Base&);
	bool checkWin();

	bool attemptMove(Piece::Base&, sf::Vector2i, bool = true);
	void pawnUpgrade(Piece::Base&, bool = false);
	bool validatePieceMove(sf::Vector2i, sf::Vector2i, Piece::Base&, bool);
	bool pawnValidateMove(sf::Vector2i, sf::Vector2i, Piece::Base&, bool);
	bool knightValidateMove(sf::Vector2i, sf::Vector2i);
	bool bishopValidateMove(sf::Vector2i, sf::Vector2i);
	bool rookValidateMove(sf::Vector2i, sf::Vector2i);
	bool queenValidateMove(sf::Vector2i, sf::Vector2i);
	bool kingValidateMove(sf::Vector2i, sf::Vector2i, Piece::Base&, bool);
}