#pragma once

#include <SFML/Graphics.hpp>
#include <array>

namespace Tetris
{
	struct Block
	{
		uint32_t _PossibleRotations;
		std::array<sf::Vector2i, 3> _RelativePositions;
	};

	struct Status
	{
		sf::Vector2u _Position;
		uint32_t _Type;
		uint32_t _Rotation;
	};

	inline const std::array<Block, 8> g_Blocks = {
		Block{ 1, { { { 0, 0 }, { 0, 0 }, { 0, 0 } } } },		// None
		Block{ 2, { { { 0, -1 }, { 0, 1 }, { 0, 2 } } } },		// I
		Block{ 4, { { { 0, -1 }, { 0, 1 }, { 1, 1 } } } },		// L
		Block{ 4, { { { 0, -1 }, { 0, 1 }, { -1, 1 } } } },		// J
		Block{ 2, { { { 0, -1 }, { 1, 0 }, { 1, 1 } } } },		// S
		Block{ 2, { { { 0, -1 }, { -1, 0 }, { -1, 1 } } } },	// Z
		Block{ 4, { { { 1, 0 }, { 0, -1 }, { 0, 1 } } } },		// T
		Block{ 1, { { { 1, 0 }, { 1, -1 }, { 0, -1 } } } }		// O
	};

	inline const std::array<sf::Color, 8> g_BlockColors = {
		sf::Color(30, 30, 40),	// None
		sf::Color(0, 255, 255),		// I
		sf::Color(255, 165, 0),		// L
		sf::Color(0, 0, 255),		// J
		sf::Color(0, 255, 0),		// S
		sf::Color(255, 0, 0),		// Z
		sf::Color(128, 0, 128),		// T
		sf::Color(255, 255, 0)		// O
	};
}
