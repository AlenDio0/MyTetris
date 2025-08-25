#include "HUD.hpp"

#include <algorithm>
#include <math.h>

namespace Tetris
{
	HUD::HUD(float cellSize, float centerAxisX, const sf::Font& font, uint32_t charSize) noexcept
		: m_NextBlock(nullptr), m_CellSize(cellSize), m_CenterAxisX(centerAxisX),
		m_StaticNextBlockText(font, "NEXT BLOCK", charSize),
		m_StaticScoreText(font, "SCORE", charSize), m_ScoreText(font, "", charSize),
		m_StaticHighscoreText(font, "HIGH SCORE", charSize), m_HighscoreText(font, "", charSize)
	{
		m_StaticNextBlockText.setOutlineThickness(-1.f);
		m_StaticScoreText.setOutlineThickness(-1.f);
		m_ScoreText.setOutlineThickness(-1.f);
		m_StaticHighscoreText.setOutlineThickness(-1.f);
		m_HighscoreText.setOutlineThickness(-1.f);

		{
			float textX = floor(m_CenterAxisX - m_StaticNextBlockText.getGlobalBounds().size.x / 2.f);
			float textY = 75.f;
			m_StaticNextBlockText.setPosition({ textX, textY });
		}
		{
			float textX = floor(m_CenterAxisX - m_StaticScoreText.getGlobalBounds().size.x / 2.f);
			float textY = 400.f;
			m_StaticScoreText.setPosition({ textX, textY });
			m_ScoreText.setPosition({ 0.f, textY + m_StaticScoreText.getGlobalBounds().size.y + 15.f });

		}
		{
			float textX = floor(m_CenterAxisX - m_StaticHighscoreText.getGlobalBounds().size.x / 2.f);
			float textY = m_StaticScoreText.getPosition().y + m_StaticScoreText.getGlobalBounds().size.y + 100.f;
			m_StaticHighscoreText.setPosition({ textX, textY });
			m_HighscoreText.setPosition({ 0.f, textY + m_StaticHighscoreText.getGlobalBounds().size.y + 15.f });
		}

		SetScore(0);
		SetHighscore(0);
	}

	void HUD::SetNextBlock(const Status& status) noexcept
	{
		m_NextBlock = &status;
	}

	void HUD::SetScore(uint32_t score) noexcept
	{
		m_ScoreText.setString(std::to_string(score));

		float centerTextX = floor(m_CenterAxisX - m_ScoreText.getGlobalBounds().size.x / 2.f);
		m_ScoreText.setPosition({ centerTextX, m_ScoreText.getPosition().y });
	}

	void HUD::SetHighscore(uint32_t highscore) noexcept
	{
		m_HighscoreText.setString(std::to_string(highscore));

		float centerTextX = floor(m_CenterAxisX - m_HighscoreText.getGlobalBounds().size.x / 2.f);
		m_HighscoreText.setPosition({ centerTextX, m_HighscoreText.getPosition().y });
	}

	void HUD::Draw(sf::RenderTarget& target) const noexcept
	{
		target.draw(m_StaticNextBlockText);
		DrawNextBlock(target);

		target.draw(m_StaticScoreText);
		target.draw(m_ScoreText);

		target.draw(m_StaticHighscoreText);
		target.draw(m_HighscoreText);
	}

	void HUD::DrawNextBlock(sf::RenderTarget& target) const
	{
		std::array<uint32_t, s_GridSize* s_GridSize> nextBlockGrid{};
		nextBlockGrid.fill(0);

		sf::RectangleShape cellShape(sf::Vector2f(m_CellSize, m_CellSize));
		cellShape.setOutlineColor(sf::Color::Black);
		cellShape.setOutlineThickness(-1.f);

		size_t gridCenter = ceil(s_GridSize / 2);
		auto positions = Utils::GetRelativePositions(*m_NextBlock);
		for (sf::Vector2i position : positions)
			nextBlockGrid.at(((gridCenter + position.y) * s_GridSize) + (gridCenter + position.x)) = m_NextBlock->_Type;

		const float offsetX = m_CenterAxisX - ((s_GridSize / 2.f) * m_CellSize);
		const float offsetY = m_StaticNextBlockText.getPosition().y + m_StaticNextBlockText.getGlobalBounds().size.y + 15.f;
		for (size_t i = 0; i < nextBlockGrid.size(); i++)
		{
			const float x = offsetX + (float)((i % s_GridSize) * m_CellSize);
			const float y = offsetY + (float)(floor(i / s_GridSize) * m_CellSize);

			cellShape.setPosition({ x, y });
			cellShape.setFillColor(g_BlockColors.at(nextBlockGrid.at(i)));

			target.draw(cellShape);
		}

		sf::RectangleShape borderShape(sf::Vector2f(m_CellSize * s_GridSize, m_CellSize * s_GridSize));
		borderShape.setPosition({ offsetX, offsetY });
		borderShape.setFillColor(sf::Color::Transparent);
		borderShape.setOutlineColor(sf::Color(70, 70, 100));
		borderShape.setOutlineThickness(1.f);

		target.draw(borderShape);
	}
}
