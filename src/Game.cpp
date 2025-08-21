#include "Game.hpp"

#include <fstream>
#include <algorithm>

#include <iostream>

namespace Tetris
{
	Game::Game(sf::Vector2u size, float cellSize, sf::FloatRect hudView)
		: m_Size(size), m_CellSize(cellSize),
		m_Grid(size.x* size.y, 0), m_CurrentBlock(CreateRandomBlock()), m_NextBlock(CreateRandomBlock()),
		m_HUD(cellSize, hudView), m_GameSpeed(1.f), m_Score(0), m_IsGameOver(false)
	{
		m_HUD.SetNextBlock(m_NextBlock);
		AddBlock(m_CurrentBlock);

		m_HUD.SetHighScore(LoadHighScore());
	}

	bool Game::IsRunning() const
	{
		return !m_IsGameOver;
	}

	void Game::HandleInput(const sf::Keyboard::Key& keyPressed)
	{
		if (keyPressed == sf::Keyboard::Key::Enter)
		{
			Reset();
			return;
		}

		if (m_IsGameOver)
			return;

		Status newStatus = m_CurrentBlock;

		switch (keyPressed)
		{
		case sf::Keyboard::Key::Left:
		case sf::Keyboard::Key::A:
			newStatus._Position.x--;
			break;
		case sf::Keyboard::Key::Right:
		case sf::Keyboard::Key::D:
			newStatus._Position.x++;
			break;
		case sf::Keyboard::Key::Up:
		case sf::Keyboard::Key::W:
			newStatus._Rotation = (newStatus._Rotation + 1) % g_Blocks[newStatus._Type]._PossibleRotations;
			break;
		case sf::Keyboard::Key::Down:
		case sf::Keyboard::Key::S:
			MoveDownBlock();
			return;
		case sf::Keyboard::Key::Space:
			HardDropBlock();
			return;
		default:
			return;
		}

		RemoveBlock(m_CurrentBlock);
		if (AddBlock(newStatus))
			m_CurrentBlock = newStatus;
		else
			AddBlock(m_CurrentBlock);
	}

	void Game::Update()
	{
		if (m_IsGameOver)
			return;

		if (m_GameClock.getElapsedTime().asSeconds() >= m_GameSpeed)
			MoveDownBlock();
	}

	void Game::Draw(sf::RenderTarget& target) const
	{
		DrawGrid(target);

		if (!m_IsGameOver)
			DrawShadow(target);

		m_HUD.Draw(target);
	}

	void Game::DrawGrid(sf::RenderTarget& target) const
	{
		sf::RectangleShape cellShape(sf::Vector2f(m_CellSize, m_CellSize));
		cellShape.setOutlineColor(sf::Color::Black);
		cellShape.setOutlineThickness(-1.f);

		for (uint32_t y = 0; y < m_Size.y; y++)
		{
			for (uint32_t x = 0; x < m_Size.x; x++)
			{
				cellShape.setFillColor(g_BlockColors[GetCell({ x, y })]);
				cellShape.setPosition({ x * m_CellSize, y * m_CellSize });

				target.draw(cellShape);
			}
		}
	}

	void Game::DrawShadow(sf::RenderTarget& target) const
	{
		sf::RectangleShape cellShape(sf::Vector2f(m_CellSize, m_CellSize));
		cellShape.setOutlineColor(sf::Color::Black);
		cellShape.setOutlineThickness(-1.f);

		auto shadowPositions = GetFallPositions();
		if (shadowPositions.has_value())
		{
			sf::Color shadowColor = g_BlockColors[m_CurrentBlock._Type];
			shadowColor.a /= 3;
			cellShape.setFillColor(shadowColor);

			for (const auto& pos : shadowPositions.value())
			{
				cellShape.setPosition({ pos.x * m_CellSize, pos.y * m_CellSize });
				target.draw(cellShape);
			}
		}
	}

	Status Game::CreateRandomBlock() const
	{
		Status status;

		status._Position = { (m_Size.x / 2) - 1, 1 };
		status._Type = GetRandom(g_Blocks.size());
		status._Rotation = GetRandom(g_Blocks[status._Type]._PossibleRotations + 1) - 1;

		return status;
	}


	size_t Game::GetIndex(sf::Vector2u position) const
	{
		return (size_t)position.y * m_Size.x + position.x;
	}

	uint32_t Game::GetCell(sf::Vector2u position) const
	{
		return m_Grid.at(GetIndex(position));
	}

	void Game::SetCell(sf::Vector2u position, uint32_t type)
	{
		m_Grid.at(GetIndex(position)) = type;
	}

	std::optional<std::array<sf::Vector2u, 4>> Game::GetPositions(const Status& status) const
	{
		const sf::Vector2u& blockPosition = status._Position;

		std::array<sf::Vector2u, 4> positions;
		auto relativePositions = Utils::GetRelativePositions(status);
		for (size_t i = 0; i < relativePositions.size(); i++)
		{
			sf::Vector2u newPosition(blockPosition.x + relativePositions[i].x, blockPosition.y + relativePositions[i].y);
			if (newPosition.x >= m_Size.x || newPosition.y >= m_Size.y)
				return std::nullopt;

			positions[i] = newPosition;
		}

		return positions;
	}

	std::optional<std::array<sf::Vector2u, 4>> Game::GetFallPositions() const
	{
		auto currentPositions = GetPositions(m_CurrentBlock);
		if (!currentPositions.has_value())
			return std::nullopt;

		Status fallBlock = m_CurrentBlock;

		while (true)
		{
			Status nextFall = fallBlock;
			nextFall._Position.y++;
			auto positions = GetPositions(nextFall);
			if (!positions.has_value())
				break;

			bool collision = std::any_of(positions->begin(), positions->end(), [&](const sf::Vector2u& position) {
				return position.y >= m_Size.y || GetCell(position) &&
					!std::any_of(currentPositions->begin(), currentPositions->end(), [&](const sf::Vector2u& currentPosition) {
					return currentPosition == position; }); });

			if (collision)
				break;

			fallBlock = nextFall;
		}

		return GetPositions(fallBlock);
	}

	bool Game::AddBlock(const Status& status)
	{
		auto positions = GetPositions(status);
		if (!positions.has_value())
			return false;

		if (std::any_of(positions.value().begin(), positions.value().end(),
			[&](const sf::Vector2u& position) { return position.y >= m_Size.y || GetCell(position); }))
			return false;

		for (const sf::Vector2u& position : positions.value())
			SetCell(position, status._Type);

		return true;
	}

	void Game::RemoveBlock(const Status& status)
	{
		auto positions = GetPositions(status);
		if (!positions.has_value())
			return;

		for (sf::Vector2u position : positions.value())
			SetCell(position, 0);
	}

	void Game::MoveDownBlock()
	{
		m_GameClock.restart();
		RemoveBlock(m_CurrentBlock);

		m_CurrentBlock._Position.y++;
		if (!AddBlock(m_CurrentBlock))
		{
			m_CurrentBlock._Position.y--;
			AddBlock(m_CurrentBlock);

			ClearCompletedRows();

			m_CurrentBlock = std::move(m_NextBlock);
			m_NextBlock = CreateRandomBlock();

			if (!AddBlock(m_CurrentBlock))
			{
				GameOver();
				return;
			}

			m_GameSpeed *= 0.99f;
			AddScore(10 / m_GameSpeed);
		}
	}

	void Game::HardDropBlock()
	{
		auto fallPosition = GetFallPositions();
		if (!fallPosition.has_value())
			return;

		RemoveBlock(m_CurrentBlock);
		m_CurrentBlock._Position.y = fallPosition.value().at(0).y;
		MoveDownBlock();
	}

	void Game::ClearCompletedRows()
	{
		uint32_t rowsCleared = 0;
		for (uint32_t y = 0; y < m_Size.y; y++)
		{
			bool isFull = true;
			for (uint32_t x = 0; x < m_Size.x; x++)
			{
				if (!GetCell({ x, y }))
				{
					isFull = false;
					break;
				}
			}

			if (isFull)
			{
				rowsCleared++;

				for (uint32_t x = 0; x < m_Size.x; x++)
					SetCell({ x, y }, 0);

				for (uint32_t row = y; row > 1; row--)
					for (uint32_t x = 0; x < m_Size.x; x++)
						SetCell({ x, row }, GetCell({ x, row - 1 }));
			}
		}

		AddScore([](uint32_t rows) {
			switch (rows)
			{
			case 1:
				return 400;
			case 2:
				return 1000;
			case 3:
				return 3000;
			case 4:
				return 12000;
			default:
				return 0;
			}
			}(rowsCleared) / m_GameSpeed);
	}

	void Game::GameOver()
	{
		m_IsGameOver = true;

		if (LoadHighScore() < m_Score)
			SaveHighScore(m_Score);
	}

	void Game::Reset()
	{
		m_IsGameOver = false;

		m_GameSpeed = 1.f;
		m_GameClock.restart();

		if (LoadHighScore() < m_Score)
			SaveHighScore(m_Score);
		AddScore(-m_Score);
		m_HUD.SetHighScore(LoadHighScore());

		m_Grid.assign((size_t)(m_Size.x * m_Size.y), 0);

		m_CurrentBlock = std::move(m_NextBlock);
		m_NextBlock = CreateRandomBlock();
		AddBlock(m_CurrentBlock);
	}

	void Game::AddScore(uint32_t score)
	{
		m_Score += score;
		m_HUD.SetScore(m_Score);
	}

	void Game::SaveHighScore(uint32_t highscore) const
	{
		std::ofstream file("highscore.txt");

		if (!file.is_open())
			return;

		file << highscore;
		file.close();
	}

	uint32_t Game::LoadHighScore() const
	{
		std::ifstream file("highscore.txt");

		if (!file.is_open())
			return 0;

		uint32_t highscore = 0;
		file >> highscore;
		file.close();

		return highscore;
	}

	int Game::GetRandom(int max)
	{
		return (rand() % (std::max(max, 2) - 1)) + 1;
	}
}