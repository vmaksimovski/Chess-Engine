#pragma once

#include <SFML/Graphics.hpp>
#include "magic.h"

namespace Helper {
	sf::Vector2i getIndices(sf::Vector2i);
	sf::Vector2f getLocation(sf::Vector2i);
	bool withinBounds(sf::Vector2i);
	Magic::color getOtherColor(Magic::color currColor);
}