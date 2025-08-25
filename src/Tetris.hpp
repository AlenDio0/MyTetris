#pragma once

#include <SFML/Graphics.hpp>
#include "Game.hpp"

namespace Tetris
{
	// Is a class necessary?
	// Not really, but I wanted to simplify the old Tetris::RunGame() function with
	// a constructor and an OnEvent() function that it can't be directly used by the Main.cpp.
	class Engine
	{
	public:
		Engine() noexcept;

		void RunGame() noexcept;
	private:
		void OnEvent(const std::optional<sf::Event>& event) noexcept;
	private:
		sf::RenderWindow m_Window;
		Game m_Game;
	private:
		struct GameData
		{
			static constexpr uint32_t s_CellSize = 40u;
			static constexpr sf::Vector2u s_GridSize = { 10u, 20u };

			static constexpr uint32_t s_HUDSizeX = 300u;

			static constexpr sf::Vector2u s_ScreenSize = { s_GridSize.x * s_CellSize + s_HUDSizeX, s_GridSize.y * s_CellSize };
			static constexpr const char* s_WindowTitle = "MyTetris";
			static constexpr uint32_t s_WindowStyle = sf::Style::Default;

			static constexpr sf::Color s_BackgroundColor = sf::Color(50, 50, 60);

			static constexpr const char* s_HighscoreFileName = "highscore.txt";

			static inline const sf::Font s_HUDFont = sf::Font("assets/PressStart2P.ttf");
		};
	};
}
