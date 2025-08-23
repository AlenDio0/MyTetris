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
		Engine();

		void RunGame();
	private:
		void OnEvent(const std::optional<sf::Event>& event);
	private:
		sf::RenderWindow m_Window;
		Game m_Game;
	private:
		struct GameData
		{
			static constexpr uint32_t _CellSize = 40u;
			static constexpr sf::Vector2u _GridSize = { 10u, 20u };

			static constexpr uint32_t _RightSide = 300u;

			static constexpr sf::Vector2u _ScreenSize = { _GridSize.x * _CellSize + _RightSide, _GridSize.y * _CellSize };
			static constexpr const char* _WindowTitle = "MyTetris";
			static constexpr uint32_t _WindowStyle = sf::Style::Default;

			static constexpr sf::Color _BackgroundColor = sf::Color(50, 50, 60);

			static constexpr const char* _HighscoreFileName = "highscore.txt";

			static inline const sf::Font _HUDFont = sf::Font("assets/PressStart2P.ttf");
		};
	};
}
