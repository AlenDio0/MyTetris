#pragma once

#include <SFML/Graphics.hpp>
#include "Block.hpp"

namespace Tetris
{
	class HUD
	{
	public:
		HUD(float cellSize, sf::FloatRect view);

		void SetNextBlock(const Status& status);
		void SetScore(uint32_t score);
		void SetHighScore(uint32_t highscore);

		void Draw(sf::RenderTarget& target) const;
	private:
		void DrawNextBlock(sf::RenderTarget& target) const;
	private:
		static constexpr size_t s_GridSize = 5;
	private:
		float m_CellSize;
		sf::FloatRect m_View;

		const Status* m_NextBlock;

		sf::Font m_Font;

		sf::Text m_StaticNextBlockText;

		sf::Text m_StaticScoreText;
		sf::Text m_ScoreText;

		sf::Text m_StaticHighScoreText;
		sf::Text m_HighScoreText;

	};
}

