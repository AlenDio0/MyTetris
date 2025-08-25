#include "Tetris.hpp"

#include "HUD.hpp"
#include <chrono>

namespace Tetris
{
	Engine::Engine() noexcept
		: m_Window(sf::VideoMode(GameData::s_ScreenSize), GameData::s_WindowTitle, GameData::s_WindowStyle),
		m_Game(GameData::s_GridSize, GameData::s_CellSize, GameData::s_ScreenSize.x - (GameData::s_HUDSizeX / 2.f),
			GameData::s_HighscoreFileName, GameData::s_HUDFont)
	{
		m_Window.setVerticalSyncEnabled(true);
	}

	void Engine::RunGame() noexcept
	{
		while (m_Window.isOpen())
		{
			{
				using namespace std::chrono_literals;
				// Why 20ms? Trial and error.
				while (auto event = m_Game.IsRunning() ? m_Window.pollEvent() : m_Window.waitEvent({ 20ms }))
					OnEvent(event);
			}

			m_Game.Update();

			m_Window.clear(GameData::s_BackgroundColor);

			m_Game.Draw(m_Window);

			m_Window.display();
		}
	}

	void Engine::OnEvent(const std::optional<sf::Event>& event) noexcept
	{
		if (event->is<sf::Event::Closed>())
		{
			m_Window.close();
			return;
		}

		if (const auto* resizedEvent = event->getIf<sf::Event::Resized>())
		{
			sf::View view = m_Window.getView();
			view.setSize((sf::Vector2f)resizedEvent->size);
			m_Window.setView(view);
		}

		if (const auto* keyPressedEvent = event->getIf<sf::Event::KeyPressed>())
		{
			if (keyPressedEvent->code == sf::Keyboard::Key::Escape)
			{
				m_Window.close();
				return;
			}

			m_Game.HandleInput(keyPressedEvent->code);
		}
	}
}