#pragma once

#include <SFML/Graphics.hpp>
#include "Block.hpp"
#include "HUD.hpp"
#include <vector>
#include <array>
#include <optional>

namespace Tetris
{
	class Game
	{
	public:
		Game(sf::Vector2u size, float cellSize, float hudCenterAxisX, const char* highscoreFileName, const sf::Font& hudFont) noexcept;

		bool IsRunning() const noexcept;

		void HandleInput(const sf::Keyboard::Key& keyPressed);
		void Update() noexcept;
		void Draw(sf::RenderTarget& target) const noexcept;
	private:
		void DrawGrid(sf::RenderTarget& target) const;
		void DrawShadow(sf::RenderTarget& target) const;

		Status CreateRandomBlock() const;

		size_t GetIndex(sf::Vector2u position) const noexcept;
		uint32_t GetCell(sf::Vector2u position) const;
		void SetCell(sf::Vector2u position, uint32_t type);

		std::optional<std::array<sf::Vector2u, 4>> GetPositions(const Status& status) const;
		std::optional<std::array<sf::Vector2u, 4>> GetFallPositions() const noexcept;

		bool AddBlock(const Status& status) noexcept;
		void RemoveBlock(const Status& status) noexcept;

		void MoveDownBlock() noexcept;
		void HardDropBlock();
		bool SearchCompletedRows() noexcept;
		void ClearCompletedRows() noexcept;

		void PutNextBlock() noexcept;

		void GameOver() noexcept;
		void Reset() noexcept;

		void AddScore(uint32_t score) noexcept;

		void SaveHighscore(uint32_t highscore) const noexcept;
		uint32_t LoadHighscore() const noexcept;
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
		const char* m_HighscoreFileName;

		sf::Clock m_ClearClock;
		mutable sf::Clock m_ClearBlinkClock;
		mutable bool m_ClearBlink;
		std::vector<uint32_t> m_ClearRows;

		bool m_IsGameOver;
	};
}
