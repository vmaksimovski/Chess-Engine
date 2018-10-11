#include "piece.h"

namespace Piece {
	Base::Base(){}

	Base::Base(Magic::color newPieceColor, Magic::type newPieceType, sf::Vector2i newPos){
		pieceColor = newPieceColor;
		pieceType = newPieceType;
		cleared = false;
		moveId = 0;
		boardPos = newPos;
		pawnTwoRowJump = false;
	}

	Magic::color Base::getColor(){
		return pieceColor;
	}

	sf::Vector2i Base::getBoardPos(){
		return boardPos;
	}

	void Base::setPosition(sf::Vector2i newPos){
		boardPos = newPos;
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
