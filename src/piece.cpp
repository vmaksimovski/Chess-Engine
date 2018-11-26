#include "piece.h"

namespace Piece {
	Base::Base(){
		cleared = false;
		lastMoveID = 0;
		movesDone = 0;
		pieceColor = Magic::color::none;
		pieceType = Magic::type::blank;
		boardPos = {-1, -1};
	}

	Base::Base(Magic::color newPieceColor, Magic::type newPieceType, sf::Vector2i newPos){
		cleared = false;
		lastMoveID = 0;
		movesDone = 0;
		pieceColor = newPieceColor;
		pieceType = newPieceType;
		boardPos = newPos;
	}

	Magic::color Base::getColor(){
		return pieceColor;
	}

	Magic::type Base::getType(){
		return pieceType;
	}

	sf::Vector2i Base::getPosition(){
		return boardPos;
	}

	void Base::setPosition(sf::Vector2i newPos){
		boardPos = newPos;
	}

	void Base::move(sf::Vector2i newPos, int moveID){
		lastMoveID = moveID;
		movesDone += 1;
		boardPos = newPos;
	}

	int Base::getLastMoveID(){
		return lastMoveID;
	}
	int Base::getMoveCount(){
		return movesDone;
	}

	void Base::setDestroyed(){
		cleared = true;
	}

	void Base::revertDestroyed(){
		cleared = false;
	}

	bool Base::checkDestroyed(){
		return cleared or pieceColor == Magic::color::none;
	}
}
