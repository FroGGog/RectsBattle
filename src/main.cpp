#include <iostream>

#include "Game.h"

int main() {

	srand(time(NULL));

	Game game{};

	while (game.isRunning()) {

		game.update();

	}

	ImGui::SFML::Shutdown();

	return 0;

}