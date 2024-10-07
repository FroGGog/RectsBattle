#include <iostream>

#include "Game.h"

int main() {

	Game game{};

	while (game.isRunning()) {

		game.update();

	}

	return 0;

}