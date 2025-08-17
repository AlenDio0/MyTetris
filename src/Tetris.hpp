#pragma once

#include <SFML/Graphics.hpp>

namespace Tetris
{
	constexpr struct
	{
		static inline uint32_t _CellSize = 40u;
		static inline sf::Vector2u _GridSize = { 10u, 20u };

		static inline sf::Vector2u _ScreenSize = { _GridSize.x * _CellSize, _GridSize.y * _CellSize };
		static inline std::string _WindowTitle = "MyTetris";
		static inline uint32_t _WindowStyle = sf::Style::Default;
	} g_Data;

	void RunGame();
}
