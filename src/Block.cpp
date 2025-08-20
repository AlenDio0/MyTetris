#include "Block.hpp"

namespace Tetris
{
	namespace Utils
	{
		std::array<sf::Vector2i, 4> GetRelativePositions(const Status& status)
		{
			const Block& block = g_Blocks[status._Type];
			const uint32_t rotationState = status._Rotation % block._PossibleRotations;

			std::array<sf::Vector2i, 4> positions = { sf::Vector2i(0, 0) };
			for (size_t i = 0; i < block._RelativePositions.size(); i++)
			{
				sf::Vector2i rotateRelativePosition = block._RelativePositions[i];
				for (size_t j = 0; j < rotationState; j++)
				{
					int tempX = rotateRelativePosition.x;

					rotateRelativePosition.x = rotateRelativePosition.y;
					rotateRelativePosition.y = -tempX;
				}

				positions[i + 1] = rotateRelativePosition;
			}

			return positions;
		}
	}
}