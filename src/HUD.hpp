#pragma once

#include <SFML/Graphics.hpp>
#include "Block.hpp"

namespace Tetris
{
	class HUD
	{
	public:
		HUD(float cellSize, float centerAxisX, const sf::Font& font, uint32_t charSize = 24u) noexcept;

		void SetNextBlock(const Status& status) noexcept;
		void SetScore(uint32_t score) noexcept;
		void SetHighscore(uint32_t highscore) noexcept;

		void Draw(sf::RenderTarget& target) const noexcept;
	private:
		void DrawNextBlock(sf::RenderTarget& target) const noexcept;
	private:
		static constexpr size_t s_GridSize = 5;
	private:
		const float m_CellSize;
		const float m_CenterAxisX;

		const Status* m_NextBlock;

		sf::Text m_StaticNextBlockText;

		sf::Text m_StaticScoreText;
		sf::Text m_ScoreText;

		sf::Text m_StaticHighscoreText;
		sf::Text m_HighscoreText;

	};
}

