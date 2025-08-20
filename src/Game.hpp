#pragma once

#include <SFML/Graphics.hpp>
#include "Block.hpp"
#include "HUD.hpp"

namespace Tetris
{
	class Game
	{
	public:
		Game(sf::Vector2u size, float cellSize, sf::FloatRect hudView);

		bool IsRunning() const;

		void HandleInput(const sf::Keyboard::Key& keyPressed);
		void Update();
		void Draw(sf::RenderTarget& target);
	private:
		void DrawGrid(sf::RenderTarget& target) const;
		void DrawShadow(sf::RenderTarget& target);

		Status CreateRandomBlock() const;

		size_t GetIndex(sf::Vector2u position) const;
		uint32_t GetCell(sf::Vector2u position) const;
		void SetCell(sf::Vector2u position, uint32_t type);

		std::optional<std::array<sf::Vector2u, 4>> GetPositions(const Status& status) const;
		std::optional<std::array<sf::Vector2u, 4>> GetFallPositions();

		bool AddBlock(const Status& status);
		void RemoveBlock(const Status& status);

		void MoveDownBlock();
		void HardDropBlock();
		void ClearCompletedRows();

		void GameOver();
		void Reset();

		void AddScore(uint32_t score);

		void SaveHighScore(uint32_t highscore) const;
		uint32_t LoadHighScore() const;
	private:
		static int GetRandom(int max);
	private:
		std::vector<uint32_t> m_Grid;
		const sf::Vector2u m_Size;
		const float m_CellSize;

		HUD m_HUD;

		Status m_CurrentBlock;
		Status m_NextBlock;

		sf::Clock m_GameClock;
		float m_GameSpeed;

		uint32_t m_Score;

		bool m_IsGameOver;
	};
}
