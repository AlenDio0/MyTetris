#include "HUD.hpp"

#include <algorithm>
#include <math.h>

namespace Tetris
{
	HUD::HUD(float cellSize, sf::FloatRect view)
		: m_NextBlock(nullptr), m_CellSize(cellSize), m_View(view),
		m_Font("assets/CascadiaMono.ttf"),
		m_StaticNextBlockText(m_Font, "Next Block", 45u),
		m_StaticScoreText(m_Font, "Score", 50u), m_ScoreText(m_Font, "0", 50u),
		m_StaticHighScoreText(m_Font, "HighScore", 50u), m_HighScoreText(m_Font, "0", 50u)
	{
		m_StaticNextBlockText.setOutlineThickness(-1.5f);
		m_StaticScoreText.setOutlineThickness(-1.5f);
		m_ScoreText.setOutlineThickness(-1.5f);
		m_StaticHighScoreText.setOutlineThickness(-1.5f);
		m_HighScoreText.setOutlineThickness(-1.5f);

		{
			float textX = m_View.getCenter().x - m_StaticNextBlockText.getGlobalBounds().size.x / 2.f;
			float textY = m_View.position.y + 75.f;
			m_StaticNextBlockText.setPosition({ textX, textY });
		}
		{
			float textX = m_View.getCenter().x - m_StaticScoreText.getGlobalBounds().size.x / 2.f;
			float textY = m_View.position.y + 400.f;
			m_StaticScoreText.setPosition({ textX, textY });
		}
		{
			float textX = m_View.getCenter().x - m_StaticHighScoreText.getGlobalBounds().size.x / 2.f;
			float textY = m_StaticScoreText.getGlobalBounds().position.y + m_StaticScoreText.getGlobalBounds().size.y + 100.f;
			m_StaticHighScoreText.setPosition({ textX, textY });
		}

		SetScore(0);
		SetHighScore(0);
	}

	void HUD::SetNextBlock(const Status& status)
	{
		m_NextBlock = &status;
	}

	void HUD::SetScore(uint32_t score)
	{
		m_ScoreText.setString(std::to_string(score));

		float textX = m_View.getCenter().x - m_ScoreText.getGlobalBounds().size.x / 2.f;
		float textY = m_StaticScoreText.getGlobalBounds().position.y + m_StaticScoreText.getGlobalBounds().size.y + 10.f;
		m_ScoreText.setPosition({ textX, textY });
	}

	void HUD::SetHighScore(uint32_t highscore)
	{
		m_HighScoreText.setString(std::to_string(highscore));

		float textX = m_View.getCenter().x - m_HighScoreText.getGlobalBounds().size.x / 2.f;
		float textY = m_StaticHighScoreText.getGlobalBounds().position.y + m_HighScoreText.getGlobalBounds().size.y + 10.f;
		m_HighScoreText.setPosition({ textX, textY });
	}

	void HUD::Draw(sf::RenderTarget& target) const
	{
		target.draw(m_StaticNextBlockText);
		DrawNextBlock(target);

		target.draw(m_StaticScoreText);
		target.draw(m_ScoreText);

		target.draw(m_StaticHighScoreText);
		target.draw(m_HighScoreText);
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
			nextBlockGrid[((gridCenter + position.y) * s_GridSize) + (gridCenter + position.x)] = m_NextBlock->_Type;

		const float offsetX = m_View.getCenter().x - ((s_GridSize / 2.f) * m_CellSize);
		const float offsetY = m_StaticNextBlockText.getGlobalBounds().position.y + m_StaticNextBlockText.getGlobalBounds().size.y + 10.f;
		for (size_t i = 0; i < nextBlockGrid.size(); i++)
		{
			float x = offsetX + (float)((i % s_GridSize) * m_CellSize);
			float y = offsetY + (float)(floor(i / s_GridSize) * m_CellSize);

			cellShape.setPosition({ x, y });
			cellShape.setFillColor(g_BlockColors[nextBlockGrid[i]]);

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
