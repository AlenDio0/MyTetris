#include "Tetris.hpp"

#include "Grid.hpp"
#include <ctime>

namespace Tetris
{
	void RunGame()
	{
		srand((int)(time(nullptr)));

		sf::RenderWindow window(sf::VideoMode(g_Data._ScreenSize), g_Data._WindowTitle, g_Data._WindowStyle);
		window.setVerticalSyncEnabled(true);

		Grid grid(g_Data._GridSize, g_Data._CellSize);

		sf::Clock clock;
		while (window.isOpen())
		{
			float deltaTime = clock.restart().asSeconds();

			while (auto event = window.pollEvent())
			{
				if (event->is<sf::Event::Closed>())
					window.close();

				if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
					grid.HandleInput(keyPressed->code);
			}

			grid.Update(deltaTime);

			window.clear(sf::Color::Black);
			grid.Draw(window);
			window.display();
		}
	}
}