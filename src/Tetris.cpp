#include "Tetris.hpp"

#include "HUD.hpp"
#include <ctime>
#include <chrono>

namespace Tetris
{
	void RunGame()
	{
		srand(int(time(0)));

		sf::RenderWindow window(sf::VideoMode(g_Data._ScreenSize), g_Data._WindowTitle, g_Data._WindowStyle);
		window.setVerticalSyncEnabled(true);

		sf::FloatRect hudView(sf::Vector2f(g_Data._ScreenSize.x - g_Data._RightSide, 0.f), sf::Vector2f(g_Data._RightSide, g_Data._ScreenSize.y));
		Game game(g_Data._GridSize, g_Data._CellSize, hudView);

		while (window.isOpen())
		{
			{
				using namespace std::chrono_literals;
				while (auto event = game.IsRunning() ? window.pollEvent() : window.waitEvent({ 20ms }))
					OnEvent(event, window, game);
			}

			game.Update();

			window.clear(g_Data._BackgroundColor);

			game.Draw(window);

			window.display();
		}
	}

	void OnEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window, Game& game)
	{
		if (event->is<sf::Event::Closed>())
		{
			window.close();
			return;
		}

		if (const auto* resize = event->getIf<sf::Event::Resized>())
		{
			sf::View view = window.getView();
			view.setSize((sf::Vector2f)resize->size);
			window.setView(view);
		}

		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed->code == sf::Keyboard::Key::Escape)
			{
				window.close();
				return;
			}

			game.HandleInput(keyPressed->code);
		}
	}
}