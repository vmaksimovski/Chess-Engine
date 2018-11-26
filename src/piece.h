#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "magic.h"
#include "helper.h"

namespace Piece {
	class Base {
		Magic::color pieceColor;
		Magic::type pieceType;
		bool cleared;
		int lastMoveID;
		int movesDone;
		sf::Vector2i boardPos;
	public:
		Base();
		Base(Magic::color, Magic::type, sf::Vector2i);
		Magic::color getColor();
		Magic::type getType();

		sf::Vector2i getPosition();
		void setPosition(sf::Vector2i);
		void move(sf::Vector2i, int);
		
		int getLastMoveID();
		int getMoveCount();

		bool checkDestroyed();
		void setDestroyed();
		void revertDestroyed();
	};
}