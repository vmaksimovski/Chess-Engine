#include "board.h"

namespace BoardStructure {
	std::vector<BoardState> boardHistory;
	BoardState board;
	bool gameEnded = false;
	int boardHistorySize;
	Magic::color currMoveColor;
	BoardState initBoard;

	void init(){
		currMoveColor = Magic::color::white;
		boardHistory.clear();

		for(int i = 0; i < 2; i++){
			for(int j = 0; j < Magic::pieces; j++){
				Magic::pieceTextures[i][j].loadFromFile(Magic::location[i][j]);
				Magic::pieceTextures[i][j].setSmooth(true);
			}
		}

		for(int i = 0; i < Magic::boardSize; i++){
			for(int j = 0; j < Magic::boardSize; j++){
				Magic::color assignedColor = Magic::color::none;
				Magic::type assignedType = Magic::type::blank;
				
				assignedColor = Magic::initialColor[i][j];
				assignedType = Magic::initialType[i][j];

				initBoard[i][j].setPosition(sf::Vector2i(j, i));

				initBoard[i][j] = Piece::Base(assignedColor, assignedType, sf::Vector2i(j, i));
			}
		}

		board = initBoard;
		boardHistory.push_back(initBoard);
		boardHistorySize = 1;
	}

	double getBoardValue(){
		double sum = 0;

		for(int i = 0; i < Magic::boardSize; i++){
			for(int j = 0; j < Magic::boardSize; j++){
				if(!board[i][j].checkDestroyed() and board[i][j].getColor() != Magic::color::none){
					if(board[i][j].getColor() == currMoveColor){
						sum += Magic::pieceWorthCoefficient * Magic::pieceWorth[board[i][j].getType()];
						if(board[i][j].getColor() == Magic::color::white){
							sum += Magic::pieceValue[board[i][j].getType()][i][j];
						} else {
							sum += Magic::pieceValue[board[i][j].getType()][7 - i][j];
						}
					} else {
						sum -= Magic::pieceWorthCoefficient * Magic::pieceWorth[board[i][j].getType()];
						if(board[i][j].getColor() == Magic::color::white){
							sum -= Magic::pieceValue[board[i][j].getType()][i][j];
						} else {
							sum -= Magic::pieceValue[board[i][j].getType()][7 - i][j];
						}
					}
				}
			}
		}

		return sum;
	}

	Piece::Base* find(sf::Vector2i v){
		v = Helper::getIndices(v);
		if(Helper::withinBounds(v) and !board[v.y][v.x].checkDestroyed() and board[v.y][v.x].getColor() == currMoveColor){
			return &board[v.y][v.x];
		}

		return NULL;
	}

	bool undoMove(){
		if(boardHistory.size() == 1){
			return false;
		}

		boardHistory.pop_back();
		boardHistorySize--;
		board = boardHistory[boardHistory.size() - 1];
		currMoveColor = Helper::getOtherColor(currMoveColor);
		gameEnded = false;

		return true;
	}

	void recordMove(){
		boardHistory.push_back(board);
		boardHistorySize++;
		currMoveColor = Helper::getOtherColor(currMoveColor);
	}

	void drawPieces(sf::RenderWindow &window){
		sf::Sprite x;

		for(int i = 0; i < Magic::boardSize; i++){
			for(int j = 0; j < Magic::boardSize; j++){
				if(!board[i][j].checkDestroyed() and board[i][j].getColor() != Magic::color::none and board[i][j].getType() != Magic::type::blank){
					if(currMoveColor == board[i][j].getColor()){
						sf::RectangleShape rectangle(sf::Vector2f(Magic::cellSize, Magic::cellSize));
						rectangle.setPosition(sf::Vector2f(j * Magic::cellSize, i * Magic::cellSize));
						rectangle.setFillColor(sf::Color(0, 0, 0, 0));
						rectangle.setOutlineColor(sf::Color(127, 255, 0, 255));
						rectangle.setOutlineThickness(12.5);
						window.draw(rectangle);
					}

					x.setPosition(sf::Vector2f(board[i][j].getPosition().x * Magic::cellSize, board[i][j].getPosition().y * Magic::cellSize));
					x.setTexture(Magic::pieceTextures[board[i][j].getColor()][board[i][j].getType()]);
					x.setScale(sf::Vector2f((double) Magic::cellSize / Magic::pieceTextures[board[i][j].getColor()][board[i][j].getType()].getSize().x, 
											   (double) Magic::cellSize / Magic::pieceTextures[board[i][j].getColor()][board[i][j].getType()].getSize().y));
					window.draw(x);
				}
			}
		}
	}
}
