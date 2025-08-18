#include "Grid.hpp"

namespace Tetris
{
	Grid::Grid(sf::Vector2u size, float cellSize)
		: m_Size(size), m_CellSize(cellSize),
		m_Grid(size.x* size.y, 0), m_CurrentBlock(CreateRandomBlock()), m_NextBlock(CreateRandomBlock()), m_Score(0), m_GameSpeed(1.f), m_IsGameOver(false)
	{
		AddBlock(m_CurrentBlock);
	}

	const Status& Grid::GetNextBlock() const
	{
		return m_NextBlock;
	}

	uint32_t Grid::GetScore() const
	{
		return m_Score;
	}

	bool Grid::isGameOver() const
	{
		return m_IsGameOver;
	}

	void Grid::HandleInput(const sf::Keyboard::Key& keyPressed)
	{
		if (keyPressed == sf::Keyboard::Key::Enter)
		{
			Reset();
			return;
		}

		if (isGameOver())
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

	void Grid::Update()
	{
		if (isGameOver())
			return;

		if (m_ScoreClock.getElapsedTime().asSeconds() >= 2.f)
		{
			m_ScoreClock.restart();
			m_Score += 10 / m_GameSpeed;
		}

		if (m_GameClock.getElapsedTime().asSeconds() >= m_GameSpeed)
			MoveDownBlock();
	}

	void Grid::Draw(sf::RenderTarget& target)
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

	Status Grid::CreateRandomBlock() const
	{
		Status status;

		status._Position = { (m_Size.x / 2) - 1, 1 };
		status._Type = GetRandom(g_Blocks.size());
		status._Rotation = GetRandom(g_Blocks[status._Type]._PossibleRotations + 1) - 1;

		return status;
	}


	size_t Grid::GetIndex(sf::Vector2u position) const
	{
		return (size_t)position.y * m_Size.x + position.x;
	}

	uint32_t Grid::GetCell(sf::Vector2u position) const
	{
		return m_Grid.at(GetIndex(position));
	}

	void Grid::SetCell(sf::Vector2u position, uint32_t type)
	{
		m_Grid.at(GetIndex(position)) = type;
	}

	std::optional<std::array<sf::Vector2u, 4>> Grid::GetPositions(const Status& status) const
	{
		const sf::Vector2u& position = status._Position;
		if (position.x >= m_Size.x || position.y >= m_Size.y)
			return {};

		std::array<sf::Vector2u, 4> positions = { position };

		const Block& block = g_Blocks[status._Type];
		for (size_t i = 0; i < block._RelativePositions.size(); i++)
		{
			const uint32_t rotationState = status._Rotation % block._PossibleRotations;

			sf::Vector2i rotateRelativePosition = block._RelativePositions[i];

			for (size_t j = 0; j < rotationState; j++)
			{
				int tempX = rotateRelativePosition.x;

				rotateRelativePosition.x = rotateRelativePosition.y;
				rotateRelativePosition.y = -tempX;
			}

			sf::Vector2u newPosition(position.x + rotateRelativePosition.x, position.y + rotateRelativePosition.y);
			if (newPosition.x >= m_Size.x || newPosition.y >= m_Size.y)
				return {};

			positions[i + 1] = newPosition;
		}

		return positions;
	}

	std::optional<std::array<sf::Vector2u, 4>> Grid::GetFallPositions()
	{
		Status shadow = m_CurrentBlock;
		RemoveBlock(m_CurrentBlock);
		while (true)
		{
			Status nextShadow = shadow;
			nextShadow._Position.y++;
			auto positions = GetPositions(nextShadow);
			if (!positions.has_value())
				break;

			bool collision = false;
			for (const sf::Vector2u& position : positions.value())
			{
				if (GetCell(position))
				{
					collision = true;
					break;
				}
			}

			if (collision)
				break;

			shadow = nextShadow;
		}
		AddBlock(m_CurrentBlock);

		return GetPositions(shadow);
	}

	bool Grid::AddBlock(const Status& status)
	{
		auto positions = GetPositions(status);
		if (!positions.has_value())
			return false;

		for (const sf::Vector2u& position : positions.value())
			if (GetCell(position))
				return false;

		for (const sf::Vector2u& position : positions.value())
			SetCell(position, status._Type);

		return true;
	}

	void Grid::RemoveBlock(const Status& status)
	{
		auto positions = GetPositions(status);
		if (!positions.has_value())
			return;

		for (sf::Vector2u position : positions.value())
			SetCell(position, 0);
	}

	void Grid::MoveDownBlock()
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
				GameOver();
		}
	}

	void Grid::HardDropBlock()
	{
		auto fallPosition = GetFallPositions();
		if (!fallPosition.has_value())
			return;

		RemoveBlock(m_CurrentBlock);
		m_CurrentBlock._Position.y = fallPosition.value().at(0).y;
		MoveDownBlock();
	}

	void Grid::ClearCompletedRows()
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

		m_Score += [](uint32_t rows) {
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
			}
			}(rowsCleared) / m_GameSpeed;
	}

	void Grid::GameOver()
	{
		m_IsGameOver = true;
	}

	void Grid::Reset()
	{
		m_IsGameOver = false;
		m_Score = 0;
		m_GameSpeed = 1.f;

		m_GameClock.restart();
		m_ScoreClock.restart();

		m_Grid.assign((size_t)(m_Size.x * m_Size.y), 0);

		m_CurrentBlock = std::move(m_NextBlock);
		m_NextBlock = CreateRandomBlock();
		AddBlock(m_CurrentBlock);
	}

	int Grid::GetRandom(int max)
	{
		return (rand() % (std::max(max, 2) - 1)) + 1;
	}
}