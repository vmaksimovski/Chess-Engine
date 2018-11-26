#include "chessAI.h"
#include <ctime>

namespace AI {
	const double INF = 1e9;

	double checkWin(){
		bool check = GameHandler::isKingAttacked(BoardStructure::currMoveColor);
		bool movesLeft = GameHandler::movePossible(BoardStructure::currMoveColor, check);

		if(check and !movesLeft){
			return -INF;
		} else if(!movesLeft){
			return INF/2.0;
		}

		return 0;
	}

	std::pair<sf::Vector2i, sf::Vector2i> bestFirstAction;
	double dfs(int movesLeft, double alpha, double beta){
		double checkValue = checkWin();

		if(checkValue != 0.0){
			return checkValue;
		}

		double value = BoardStructure::getBoardValue();

		if(movesLeft == Magic::propagationLimit){
			return value;
		}

		double bestAction = -2.0*INF;

		for(int i = 0; i < Magic::boardSize; i++){
			for(int j = 0; j < Magic::boardSize; j++){
				if(!BoardStructure::board[i][j].checkDestroyed() and 
					BoardStructure::board[i][j].getColor() == BoardStructure::currMoveColor){
					sf::Vector2i startPos = sf::Vector2i(j, i);
					for(int k = 0; k < Magic::boardSize; k++){
						for(int l = 0; l < Magic::boardSize; l++){
							sf::Vector2i newPos = sf::Vector2i(l, k);
							if(Helper::withinBounds(newPos) and startPos != newPos and (BoardStructure::board[newPos.y][newPos.x].checkDestroyed() or
																									BoardStructure::board[newPos.y][newPos.x].getColor() != BoardStructure::currMoveColor)){
								if(GameHandler::validatePieceMove(startPos, newPos, BoardStructure::board[startPos.y][startPos.x], false) and
									 GameHandler::attemptMove(BoardStructure::board[startPos.y][startPos.x], newPos, false)){
									auto response = dfs(movesLeft + 1, alpha, beta);
									response *= -1;
									if(response > bestAction){
										bestAction = response;
										if(movesLeft == 0){
											bestFirstAction = {startPos, newPos};
										}
									}

									BoardStructure::undoMove();

									if(BoardStructure::currMoveColor == Magic::color::white){
										alpha = std::max(alpha, response);
									} else {
										beta = std::min(beta, -response);
									}

									if(alpha >= beta){
										i = j = k = l = Magic::boardSize;
									}
								}
							}
						}
					}
				}
			}
		}

		return bestAction;
	} 

	std::pair<sf::Vector2i, sf::Vector2i> getBestMove(){
		bestFirstAction = {{-1, -1}, {-1, -1}};
		if(BoardStructure::currMoveColor == Magic::playerColor){
			return bestFirstAction;
		}

		dfs(0, -INF, INF);

		return bestFirstAction;
	}
}
