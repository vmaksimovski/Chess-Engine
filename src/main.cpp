#include <iostream>
#include <vector>
#include <cstdlib> 
#include <cstring>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "magic.h"
#include "helper.h"
#include "piece.h"
#include "board.h"
#include "gameHandler.h"
#include "chessAI.h"

bool validateInteger(char* array){
	while(*array != '\0'){
		std::cerr << *array << '\n';
		if('0' > *array or *array > '9'){
			return false;
		}
		array++;
	}

	return true;
}

void printHelp(){
	std::cout << "ChessGame - a (nearly) fully functioned chess clone and chess engine. \n";
	std::cout << "By default, the AI plays by itself.\n";
	std::cout << " -p, --player COLOR      Specifies the player color. The AI will play the other color.\n";
	std::cout << " 						   recognized tokens for COLOR are b, w\n";
	std::cout << " -m, --moves-ahead MOVE  M specifies how many moves ahead the AI simulates\n";
	std::cout << " -h, --help              Print this help message\n";
}

int main(int argc, char* argv[]){
	sf::RenderWindow window(sf::VideoMode(Magic::size, Magic::size), "It Works!");
	window.setFramerateLimit(60);

	Magic::playerColor = Magic::color::none;

	for(int i = 1; i < argc; i++){
		if(strcmp(argv[i], "-h") == 0 or strcmp(argv[i], "--help") == 0){
			printHelp();
			exit(0);
		} else if(strcmp(argv[i], "-p") == 0 or strcmp(argv[i], "--player") == 0){
			if(i + 1 == argc){
				std::cerr << "ERR: -p argument given with no COLOR specified" << std::endl;
				exit(-1);
			}
			if(Magic::playerColor != Magic::color::none){
				std::cerr << "ERR: -p argument already specified" << std::endl;
				exit(-1);
			}
			if(strcmp(argv[i + 1], "w") == 0){
				Magic::playerColor = Magic::color::white;
			} else if(strcmp(argv[i + 1], "b") == 0){
				Magic::playerColor = Magic::color::black;
			} else {
				std::cerr << "ERR: unrecognized token for COLOR\n";
				exit(-1);
			}
			i++;
		} else if(strcmp(argv[i], "-m") == 0 or strcmp(argv[i], "--moves-ahead") == 0){
			if(i + 1 == argc){
				std::cerr << "ERR: -m/--moves-ahead argument given with no MOVE specified" << std::endl;
				exit(-1);
			}
			if(validateInteger(argv[i + 1])){
				Magic::propagationLimit = strtol(argv[i + 1], NULL, 10);
			} else {
				std::cerr << "ERR: -m/--moves-ahead could not parse integer" << std::endl;
				exit(-1);				
			}
			i++;
		} else {
			std::cerr << "ERR: unrecognized command\n";
			exit(-1);
		}
	}

	BoardStructure::init();
	sf::SoundBuffer buffer;
	buffer.loadFromFile("media/Move.ogg");
	sf::Sound sound;
	sound.setBuffer(buffer);

	sf::Texture boardImage;
	boardImage.loadFromFile(Magic::boardString);

	sf::Sprite board;
	board.setTexture(boardImage, true);
	board.setPosition(0.f, 0.f);
	board.setScale((double) Magic::size / boardImage.getSize().x, 
					(double) Magic::size / boardImage.getSize().y);

	Piece::Base *held = NULL;

	std::pair<sf::Vector2i, sf::Vector2i> nullMove = {{-1, -1}, {-1, -1}};

	std::pair<sf::Vector2i, sf::Vector2i> lastMove = nullMove;
	std::pair<sf::Vector2i, sf::Vector2i> AIMove = nullMove;

	bool undoDone = false;

	while (window.isOpen()){
		sf::Event event;
		auto mouseLocation = sf::Mouse::getPosition(window);

		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed){
				window.close();
			} else if(event.type == sf::Event::MouseButtonPressed){
				if(event.mouseButton.button == sf::Mouse::Left){
					held = BoardStructure::find(mouseLocation);
				} else if(event.mouseButton.button == sf::Mouse::Right){
					BoardStructure::undoMove();
					AIMove = lastMove = nullMove;
					undoDone = true;
				}
			} else if(event.type == sf::Event::MouseButtonReleased){
				if(held == NULL){
					continue;
				}

				sf::Vector2i prevBoardPos = held->getPosition();
				if(GameHandler::attemptMove(*held, Helper::getIndices(mouseLocation), true)){
					lastMove = {prevBoardPos, Helper::getIndices(mouseLocation)};
					undoDone = false;
					GameHandler::checkWin();
					sound.play();
				}			

				held = NULL;
			}
		}

		window.draw(board);

		if(AIMove != nullMove){
			if(GameHandler::attemptMove(BoardStructure::board[AIMove.first.y][AIMove.first.x], AIMove.second, false)){
				lastMove = AIMove;
				sound.play();
				GameHandler::checkWin();
			}

			AIMove = nullMove;
		}

		if(lastMove != nullMove){
			sf::Vector2f startPos = sf::Vector2f(lastMove.first.x * Magic::cellSize, lastMove.first.y * Magic::cellSize);
			sf::Vector2f nextPos = sf::Vector2f(lastMove.second.x * Magic::cellSize, lastMove.second.y * Magic::cellSize);
			sf::CircleShape startPosCircle(Magic::cellSize / 2.0);
			sf::CircleShape nextPosCircle(Magic::cellSize / 2.0);
			startPosCircle.setFillColor(sf::Color::Green);
			nextPosCircle.setFillColor(sf::Color::Red);
			startPosCircle.setPosition(startPos);
			nextPosCircle.setPosition(nextPos);
			window.draw(startPosCircle);
			window.draw(nextPosCircle);
		}
		
		// draw dragged item		
		if(held != NULL){
			if(held == NULL){
				std::cerr << "No unit selected" << '\n';
				return -1;
			}

			held->setDestroyed();

			sf::Sprite draggedItem;
			draggedItem.setPosition(sf::Vector2f(mouseLocation.x - Magic::cellSize / 2.0, 
												 mouseLocation.y - Magic::cellSize / 2.0));
			draggedItem.setScale(sf::Vector2f((double) Magic::cellSize / Magic::pieceTextures[held->getColor()][held->getType()].getSize().x, 
											   (double) Magic::cellSize / Magic::pieceTextures[held->getColor()][held->getType()].getSize().y));
   			draggedItem.setTexture(Magic::pieceTextures[held->getColor()][held->getType()]);
			window.draw(draggedItem);
		}

		BoardStructure::drawPieces(window);

		window.display();

		if(held != NULL){
			held->revertDestroyed();
		}

		if(AIMove == nullMove and !undoDone and BoardStructure::currMoveColor != Magic::playerColor){
			AIMove = AI::getBestMove();
		} //postpone calculating AI move so that the board is rendered properly
	}

	return 0;
}
