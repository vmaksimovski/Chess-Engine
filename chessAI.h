#pragma once

#include "gameHandler.h"
#include "board.h"
#include "piece.h"
#include "helper.h"
#include "magic.h"

namespace AI {
	std::pair<sf::Vector2i, sf::Vector2i> getBestMove();
}
