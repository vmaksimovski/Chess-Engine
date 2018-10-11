#include "gameHandler.h"

namespace GameHandler {
	BoardStructure::BoardState board;
	
	bool isAttacked(sf::Vector2i boardPos, Magic::color attackingColor){
		assert(BoardStructure::board[boardPos.y][boardPos.x].pieceColor != attackingColor);

		for(int i = 0; i < Magic::boardSize; i++){
			for(int j = 0; j < Magic::boardSize; j++){
				sf::Vector2i nextPos = sf::Vector2i(j, i);
				if(BoardStructure::board[i][j].pieceColor == attackingColor
						 and !BoardStructure::board[i][j].checkDestroyed() 
						 and validatePieceMove(nextPos, boardPos, BoardStructure::board[i][j], false)){
					return true;
				}
			}
		}

		return false;
	}

	bool canPieceMove(sf::Vector2i boardPos, Piece::Base& currPiece){
		for(int k = 0; k < Magic::boardSize; k++){
			for(int l = 0; l < Magic::boardSize; l++){
				sf::Vector2i newPos = sf::Vector2i(l, k);
				if(Helper::withinBounds(newPos) and boardPos != newPos and (BoardStructure::board[newPos.y][newPos.x].checkDestroyed() or
																						BoardStructure::board[newPos.y][newPos.x].pieceColor != currPiece.pieceColor)){
					if(attemptMove(BoardStructure::board[boardPos.y][boardPos.x], newPos, false)){
						BoardStructure::undoMove();
						return true;
					}
				}
			}
		}

		return false;
	}

	bool movePossible(Magic::color queryColor, bool check){
		for(int i = 0; i < Magic::boardSize; i++){
			for(int j = 0; j < Magic::boardSize; j++){
				if(BoardStructure::board[i][j].pieceColor == queryColor and !BoardStructure::board[i][j].checkDestroyed()){
					sf::Vector2i startPos = sf::Vector2i(j, i);
					if(canPieceMove(startPos, BoardStructure::board[i][j])){
						return true;
					}
				}
			}
		}

		return false;
	}

	bool isKingAttacked(Magic::color kingColor){
		sf::Vector2i kingPos = sf::Vector2i(-1, -1);
		for(int i = 0; i < Magic::boardSize; i++){
			for(int j = 0; j < Magic::boardSize; j++){
				if(BoardStructure::board[i][j].pieceType == Magic::type::king and BoardStructure::board[i][j].pieceColor == kingColor){
					kingPos = sf::Vector2i(j, i);
				}
			}
		}

		return isAttacked(kingPos, Helper::getOtherColor(kingColor));
	}

	bool checkWin(){
		bool check = isKingAttacked(BoardStructure::currMoveColor);
		bool movesLeft = movePossible(BoardStructure::currMoveColor, check);

		if(check and !movesLeft){
			std::cout << "Loss for ";
			if(BoardStructure::currMoveColor == Magic::color::black){
				std::cout << "black!\n";
			} else {
				std::cout << "white!\n";
			}
			BoardStructure::gameEnded = true;
		} else if(!movesLeft){
			std::cout << "Draw from ";
			if(BoardStructure::currMoveColor == Magic::color::black){
				std::cout << "black!\n";
			} else {
				std::cout << "white!\n";
			}
			BoardStructure::gameEnded = true;
		}

		return false;
	}

	bool attemptMove(Piece::Base& currPiece, sf::Vector2i newPos, bool queryForPawnUpgrade){
		if(BoardStructure::gameEnded){
			return false;
		}
		sf::Vector2i startPos = currPiece.getBoardPos();

		if(!Helper::withinBounds(newPos) or startPos == newPos or (!BoardStructure::board[newPos.y][newPos.x].checkDestroyed() and
																	BoardStructure::board[newPos.y][newPos.x].pieceColor == currPiece.pieceColor)){
			return false;
		}


		if(!validatePieceMove(startPos, newPos, currPiece, true)){
			return false;
		}

		currPiece.moveId = BoardStructure::boardHistorySize;

		currPiece.boardPos = newPos;

		BoardStructure::board[newPos.y][newPos.x].setDestroyed();

		if(BoardStructure::board[startPos.y][startPos.x].pieceType == Magic::type::pawn and (newPos.y == 0 or newPos.y == Magic::boardSize - 1)){
			pawnUpgrade(currPiece, !queryForPawnUpgrade);
		}

		std::swap(BoardStructure::board[startPos.y][startPos.x], BoardStructure::board[newPos.y][newPos.x]);

		if(isKingAttacked(BoardStructure::board[newPos.y][newPos.x].pieceColor)){
			BoardStructure::recordMove();
			BoardStructure::undoMove();
			return false;
		}

		BoardStructure::recordMove();
		return true;
	}

	void pawnUpgrade(Piece::Base& currPiece, bool assumeQueen){
		Magic::type newPieceType = Magic::type::queen;

		if(!assumeQueen){
			std::cout << "What piece would you like to upgrade to? (1 for a knight, 2 for a bishop, 3 for a rook, and 4 for a queen)" << std::endl;
			int x;
			std::cin >> x;
	
			if(x <= 0 or x >= 5){
				std::cerr << "INVALID, ASSUMING QUEEN" << std::endl;
				x = 4;
			}
			
			if(x == 1){
				newPieceType = Magic::type::knight;
			} else if(x == 2){
				newPieceType = Magic::type::bishop;
			} else if(x == 3){
				newPieceType = Magic::type::rook;
			} else if(x == 4){
				newPieceType = Magic::type::queen;
			}
		}

		currPiece = Piece::Base(currPiece.pieceColor, newPieceType, currPiece.boardPos);
	}

	bool validatePieceMove(sf::Vector2i startPos, sf::Vector2i newPos, Piece::Base& currPiece, bool performMovement){
		switch (currPiece.pieceType) {
		case Magic::type::pawn:
			if (!pawnValidateMove(startPos, newPos, currPiece, performMovement)){
				return false;
			}	
			break;
		case Magic::type::knight:
			if (!knightValidateMove(startPos, newPos)){
				return false;
			}	
			break;
		case Magic::type::bishop:
			if (!bishopValidateMove(startPos, newPos)){
				return false;
			}	
			break;
		case Magic::type::rook:
			if (!rookValidateMove(startPos, newPos)){
				return false;
			}	
			break;
		case Magic::type::queen:
			if (!queenValidateMove(startPos, newPos)){
				return false;
			}	
			break;
		case Magic::type::king:
			if (!kingValidateMove(startPos, newPos, currPiece, performMovement)){
				return false;
			}	
			break;
		default:
			std::cerr << "ERR: BLANK CALLED FOR VALIDATEPIECEMOVE\n";
			exit(-1);
		}
		return true;
	}

	bool pawnValidateMove(sf::Vector2i boardPos, sf::Vector2i newPos, Piece::Base& currPiece, bool performMovement){
		int step = 0;
		if(currPiece.pieceColor == Magic::color::black){
			step = 1;
		} else {
			step = -1;
		}

		if(boardPos.x == newPos.x){
			if(boardPos.y + step == newPos.y and BoardStructure::board[newPos.y][newPos.x].checkDestroyed()){
				return true;
			} else if(currPiece.moveId == 0 and boardPos.y + 2 * step == newPos.y and BoardStructure::board[boardPos.y + step][boardPos.x].checkDestroyed() and
																			BoardStructure::board[boardPos.y + 2 * step][boardPos.x].checkDestroyed()){
				if(performMovement){
					BoardStructure::board[boardPos.y][boardPos.x].pawnTwoRowJump = true;
				}
				return true;
			}
		} else if((boardPos.x - 1 == newPos.x or boardPos.x + 1 == newPos.x) and boardPos.y + step == newPos.y and !BoardStructure::board[newPos.y][newPos.x].checkDestroyed()){
			return true;
		} else if((boardPos.x - 1 == newPos.x or boardPos.x + 1 == newPos.x) and boardPos.y + step == newPos.y and ((step == 1 and boardPos.y == 4) or (step == -1 and boardPos.y == 3))
																				and !BoardStructure::board[boardPos.y][newPos.x].checkDestroyed()
																				and BoardStructure::board[boardPos.y][newPos.x].pieceColor != currPiece.pieceColor
																			  	and BoardStructure::board[boardPos.y][newPos.x].pieceType == Magic::type::pawn
																			  	and BoardStructure::board[boardPos.y][newPos.x].moveId + 1 == BoardStructure::boardHistorySize
																			  	and BoardStructure::board[boardPos.y][newPos.x].pawnTwoRowJump){
			if(performMovement){
				BoardStructure::board[boardPos.y][newPos.x].setDestroyed();
			}
			return true;
		}

		return false;
	}

	bool knightValidateMove(sf::Vector2i boardPos, sf::Vector2i newPos){
		sf::Vector2i newPosCopy = newPos - boardPos;
		int a = std::abs(newPosCopy.x), b = std::abs(newPosCopy.y);

		if((a == 2 and b == 1) or (a == 1 and b == 2)){
			return true;
		}

		return false;
	}

	bool bishopValidateMove(sf::Vector2i boardPos, sf::Vector2i newPos){
		if(boardPos.x + boardPos.y != newPos.x + newPos.y and std::abs(newPos.x - boardPos.x) != std::abs(newPos.y - boardPos.y)){
			return false;
		}

		sf::Vector2i tempPos = boardPos;

		while(tempPos != newPos){
			if(tempPos.x < newPos.x){
				tempPos.x++;
			} else if(tempPos.x > newPos.x){
				tempPos.x--;
			} else {
				return false;
			}

			if(tempPos.y < newPos.y){
				tempPos.y++;
			} else if(tempPos.y > newPos.y){
				tempPos.y--;
			} else {
				return false;
			}

			if(tempPos != newPos and !BoardStructure::board[tempPos.y][tempPos.x].checkDestroyed()){
				return false;
			}
		}

		return true;
	}
	
	bool rookValidateMove(sf::Vector2i boardPos, sf::Vector2i newPos){
		if(boardPos.x != newPos.x and boardPos.y != newPos.y){
			return false;
		}

		sf::Vector2i tempPos = boardPos;

		while(tempPos != newPos){
			if(tempPos.x < newPos.x){
				tempPos.x++;
			} else if(tempPos.x > newPos.x){
				tempPos.x--;
			}
			if(tempPos.y < newPos.y){
				tempPos.y++;
			} else if(tempPos.y > newPos.y){
				tempPos.y--;
			}

			if(tempPos != newPos and !BoardStructure::board[tempPos.y][tempPos.x].checkDestroyed()){
				return false;
			}
		}

		return true;
	}

	bool queenValidateMove(sf::Vector2i boardPos, sf::Vector2i newPos){
		if(bishopValidateMove(boardPos, newPos) or rookValidateMove(boardPos, newPos)){
			return true;
		}

		return false;
	}
	
	bool kingValidateMove(sf::Vector2i boardPos, sf::Vector2i newPos, Piece::Base& currPiece, bool performMovement){
		sf::Vector2i newPosCopy = newPos - boardPos;
		int a = std::abs(newPosCopy.x), b = std::abs(newPosCopy.y);

		if(std::max(a, b) == 1){
			return true;
		}

		if(currPiece.moveId == 0 and boardPos.y == newPos.y){
			if(newPos.x == 2){
				Piece::Base& partnerRook = BoardStructure::board[newPos.y][newPos.x - 2];

				if(!isAttacked(boardPos, Helper::getOtherColor(currPiece.pieceColor)) and !partnerRook.checkDestroyed() and
						partnerRook.getColor() == currPiece.getColor() and
						partnerRook.moveId == 0 and 
						BoardStructure::board[newPos.y][newPos.x + 1].checkDestroyed() and
						rookValidateMove(partnerRook.boardPos, sf::Vector2i(boardPos.x, boardPos.y))){
					if(performMovement){
						partnerRook.setPosition(sf::Vector2i(newPos.x + 1, newPos.y));
						std::swap(BoardStructure::board[newPos.y][newPos.x - 2], BoardStructure::board[newPos.y][newPos.x + 1]);
					}
					return true;
				}
			} else if(newPos.x == Magic::boardSize - 2){
				Piece::Base& partnerRook = BoardStructure::board[newPos.y][newPos.x + 1];

				if(!isAttacked(boardPos, Helper::getOtherColor(currPiece.pieceColor)) and !partnerRook.checkDestroyed() and
						partnerRook.getColor() == currPiece.getColor() and
						partnerRook.moveId == 0 and 
						BoardStructure::board[newPos.y][newPos.x - 1].checkDestroyed() and
						rookValidateMove(partnerRook.boardPos, sf::Vector2i(boardPos.x, boardPos.y))){
					if(performMovement){
						partnerRook.setPosition(sf::Vector2i(newPos.x - 1, newPos.y));
						std::swap(BoardStructure::board[newPos.y][newPos.x - 1], BoardStructure::board[newPos.y][newPos.x + 1]);
					}
					return true;
				}
			}
		}

		return false;
	}
}
