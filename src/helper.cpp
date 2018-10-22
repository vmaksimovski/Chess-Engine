#include "helper.h"

namespace Helper {
	sf::Vector2i getIndices(sf::Vector2i v){
		if(0 <= v.x and v.x < Magic::size and 0 <= v.y and v.y < Magic::size){
			return sf::Vector2i(v.x / Magic::cellSize, v.y / Magic::cellSize);
		} else {
			return sf::Vector2i(-1, -1);
		}
	}
	
	sf::Vector2f getLocation(sf::Vector2i v){
		v = getIndices(v);
		return sf::Vector2f(v.x * Magic::cellSize, v.y * Magic::cellSize);
	}

	bool withinBounds(sf::Vector2i pos){
		if(0 <= pos.x and pos.x < Magic::boardSize and 0 <= pos.y and pos.y < Magic::boardSize){
			return true;
		}

		return false;
	}

	Magic::color getOtherColor(Magic::color currColor){
		if(currColor == Magic::color::white){
			return Magic::color::black;
		} else if(currColor == Magic::color::black){
			return Magic::color::white;
		}

		return Magic::color::none;
	}
}