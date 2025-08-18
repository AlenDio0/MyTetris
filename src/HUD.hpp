#pragma once

#include <SFML/Graphics.hpp>
#include "Block.hpp"

namespace Tetris
{
	class HUD
	{
	public:
		HUD(const Status& nextBlock, float cellSize, sf::FloatRect view);

		void SetNextBlock(const Status& status);
		void SetScore(uint32_t score);

		void Draw(sf::RenderTarget& target) const;
	private:
		void DrawNextBlock(sf::RenderTarget& target) const;
	private:
		float m_CellSize;
		sf::FloatRect m_View;

		const Status* m_NextBlock;
		sf::RectangleShape m_BlockBackground;

		sf::Font m_Font;

		sf::Text m_StaticScoreText;
		sf::Text m_ScoreText;

	};
}

