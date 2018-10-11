#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "magic.h"
#include "helper.h"

namespace Piece {
	class Base {
	public:
		Magic::color pieceColor = Magic::color::none;
		Magic::type pieceType;
		bool cleared;
		int moveId;
		bool pawnTwoRowJump;
		sf::Vector2i boardPos;
		Base();
		Base(Magic::color, Magic::type, sf::Vector2i);
		Magic::color getColor();
		sf::Vector2i getBoardPos();
		void setPosition(sf::Vector2i);
		void setDestroyed();
		void revertDestroyed();
		bool checkDestroyed();
	};
}