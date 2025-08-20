#include "HUD.hpp"

#include <algorithm>

namespace Tetris
{
	HUD::HUD(float cellSize, sf::FloatRect view)
		: m_NextBlock(nullptr), m_CellSize(cellSize), m_View(view),
		m_BlockBackground({ m_CellSize * 5.f, m_CellSize * 5.f }),
		m_Font("assets/CascadiaMono.ttf"),
		m_StaticScoreText(m_Font, "Score", 50u), m_ScoreText(m_Font, "0", 50u),
		m_StaticHighScoreText(m_Font, "HighScore", 50u), m_HighScoreText(m_Font, "0", 50u)
	{
		m_BlockBackground.setFillColor(sf::Color(30, 30, 40));
		m_BlockBackground.setOutlineColor(sf::Color(100, 100, 120));
		m_BlockBackground.setOutlineThickness(-2.f);

		m_BlockBackground.setPosition({ m_View.position.x + m_View.size.x / 2.f - m_BlockBackground.getSize().x / 2.f, m_View.position.y + 50.f });

		m_StaticScoreText.setOutlineThickness(-1.5f);
		m_ScoreText.setOutlineThickness(-1.5f);
		m_StaticHighScoreText.setOutlineThickness(-1.5f);
		m_HighScoreText.setOutlineThickness(-1.5f);

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
		target.draw(m_BlockBackground);
		DrawNextBlock(target);

		target.draw(m_StaticScoreText);
		target.draw(m_ScoreText);

		target.draw(m_StaticHighScoreText);
		target.draw(m_HighScoreText);
	}

	void HUD::DrawNextBlock(sf::RenderTarget& target) const
	{
		std::array<sf::Vector2i, 4> positions = { sf::Vector2i(0, 0) };

		const Block& block = g_Blocks[m_NextBlock->_Type];
		for (size_t i = 0; i < block._RelativePositions.size(); i++)
		{
			const uint32_t rotationState = m_NextBlock->_Rotation % block._PossibleRotations;

			sf::Vector2i rotateRelativePosition = block._RelativePositions[i];

			for (size_t j = 0; j < rotationState; j++)
			{
				int tempX = rotateRelativePosition.x;

				rotateRelativePosition.x = rotateRelativePosition.y;
				rotateRelativePosition.y = -tempX;
			}

			positions[i + 1] = rotateRelativePosition;
		}

		std::sort(positions.begin(), positions.end(), [](const sf::Vector2i& a, const sf::Vector2i& b) {
			return a.x > b.x;
			});
		int maxX = positions.at(0).x != 0 ? positions.at(0).x : 1;

		std::sort(positions.begin(), positions.end(), [](const sf::Vector2i& a, const sf::Vector2i& b) {
			return a.y > b.y;
			});
		int maxY = positions.at(0).y != 0 ? positions.at(0).y : 1;

		float x = m_BlockBackground.getGlobalBounds().getCenter().x - (maxX * m_CellSize) / 2.f;
		float y = m_BlockBackground.getGlobalBounds().getCenter().y - (maxY * m_CellSize) / 2.f;

		sf::RectangleShape cellShape(sf::Vector2f(m_CellSize, m_CellSize));
		cellShape.setFillColor(g_BlockColors[m_NextBlock->_Type]);
		cellShape.setOutlineColor(sf::Color::Black);
		cellShape.setOutlineThickness(-1.f);

		for (sf::Vector2i relativePosition : positions)
		{
			cellShape.setPosition({ x + (relativePosition.x * m_CellSize), y + (relativePosition.y * m_CellSize) });

			target.draw(cellShape);
		}
	}
}
