#include "Tetris.hpp"

#include "HUD.hpp"
#include <chrono>

namespace Tetris
{
	Engine::Engine()
		: m_Window(sf::VideoMode(GameData::_ScreenSize), GameData::_WindowTitle, GameData::_WindowStyle),
		m_Game(GameData::_GridSize, GameData::_CellSize, GameData::_ScreenSize.x - (GameData::_RightSide / 2.f),
			GameData::_HighscoreFileName, GameData::_HUDFont)
	{
		m_Window.setVerticalSyncEnabled(true);
	}

	void Engine::RunGame()
	{
		while (m_Window.isOpen())
		{
			{
				using namespace std::chrono_literals;
				while (auto event = m_Game.IsRunning() ? m_Window.pollEvent() : m_Window.waitEvent({ 20ms }))
					OnEvent(event);
			}

			m_Game.Update();

			m_Window.clear(GameData::_BackgroundColor);

			m_Game.Draw(m_Window);

			m_Window.display();
		}
	}

	void Engine::OnEvent(const std::optional<sf::Event>& event)
	{
		if (event->is<sf::Event::Closed>())
		{
			m_Window.close();
			return;
		}

		if (const auto* resize = event->getIf<sf::Event::Resized>())
		{
			sf::View view = m_Window.getView();
			view.setSize((sf::Vector2f)resize->size);
			m_Window.setView(view);
		}

		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed->code == sf::Keyboard::Key::Escape)
			{
				m_Window.close();
				return;
			}

			m_Game.HandleInput(keyPressed->code);
		}
	}
}