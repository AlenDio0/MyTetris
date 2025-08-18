#include "Tetris.hpp"

#include "Grid.hpp"
#include "HUD.hpp"
#include <ctime>

namespace Tetris
{
	void RunGame()
	{
		srand(int(time(0)));

		sf::RenderWindow window(sf::VideoMode(g_Data._ScreenSize), g_Data._WindowTitle, g_Data._WindowStyle);
		window.setVerticalSyncEnabled(true);

		Grid grid(g_Data._GridSize, g_Data._CellSize);
		HUD hud(grid.GetNextBlock(), g_Data._CellSize * 1.1f, sf::FloatRect({ (float)g_Data._ScreenSize.x - g_Data._RightSide, 0.f }, { (float)g_Data._RightSide, (float)g_Data._ScreenSize.y }));

		while (window.isOpen())
		{
			while (auto event = window.pollEvent())
			{
				if (event->is<sf::Event::Closed>())
					window.close();

				if (const auto* resize = event->getIf<sf::Event::Resized>())
				{
					sf::View view = window.getView();
					view.setSize((sf::Vector2f)resize->size);
					window.setView(view);
				}

				if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
				{
					if (keyPressed->code == sf::Keyboard::Key::Escape)
						window.close();

					grid.HandleInput(keyPressed->code);
				}
			}

			grid.Update();

			hud.SetNextBlock(grid.GetNextBlock());
			hud.SetScore(grid.GetScore());

			window.clear(g_Data._BackgroundColor);

			grid.Draw(window);
			hud.Draw(window);

			window.display();
		}
	}
}