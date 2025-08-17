#include "Grid.hpp"

namespace Tetris
{
	Grid::Grid(sf::Vector2u size, float cellSize)
		: m_Size(size), m_CellSize(cellSize), m_Grid(size.x* size.y, 0), m_Current(GetNextBlock()), m_TimeAccomulator(0.f)
	{
		AddBlock(m_Current);
	}

	Status Grid::GetNextBlock() const
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
		Status shadow = m_Current;
		RemoveBlock(m_Current);
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
		AddBlock(m_Current);

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
		RemoveBlock(m_Current);

		m_Current._Position.y++;
		if (!AddBlock(m_Current))
		{
			m_Current._Position.y--;
			AddBlock(m_Current);

			ClearCompletedRows();

			m_Current = GetNextBlock();
			if (!AddBlock(m_Current))
			{
				GameOver();
			}
		}
	}

	void Grid::HardDropBlock()
	{
		auto fallPosition = GetFallPositions();
		if (!fallPosition.has_value())
			return;

		RemoveBlock(m_Current);
		m_Current._Position.y = fallPosition.value().at(0).y;
		MoveDownBlock();
	}

	void Grid::ClearCompletedRows()
	{
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
				for (uint32_t x = 0; x < m_Size.x; x++)
					SetCell({ x, y }, 0);

				for (uint32_t row = y; row > 1; row--)
					for (uint32_t x = 0; x < m_Size.x; x++)
						SetCell({ x, row }, GetCell({ x, row - 1 }));
			}
		}
	}

	void Grid::GameOver()
	{
		m_Grid.assign((size_t)(m_Size.x * m_Size.y), 0);
		m_Current = GetNextBlock();
		AddBlock(m_Current);
	}

	void Grid::HandleInput(const sf::Keyboard::Key& keyPressed)
	{
		Status newStatus = m_Current;

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

		RemoveBlock(m_Current);
		if (AddBlock(newStatus))
			m_Current = newStatus;
		else
			AddBlock(m_Current);
	}

	void Grid::Update(float deltaTime)
	{
		m_TimeAccomulator += deltaTime;
		if (m_TimeAccomulator <= 1.f)
			return;
		m_TimeAccomulator = 0.f;

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
			sf::Color shadowColor = g_BlockColors[m_Current._Type];
			shadowColor.a /= 3;
			cellShape.setFillColor(shadowColor);

			for (const auto& pos : shadowPositions.value())
			{
				cellShape.setPosition({ pos.x * m_CellSize, pos.y * m_CellSize });
				target.draw(cellShape);
			}
		}
	}

	int Grid::GetRandom(int max)
	{
		return (rand() % (std::max(max, 2) - 1)) + 1;
	}
}