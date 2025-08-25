#pragma once

#include <random>
#include <ctime>

namespace Tetris::Utils
{
	struct RandomData
	{
		inline RandomData() noexcept
		{
			_Engine.seed((uint32_t)time(nullptr));
		}

		std::default_random_engine _Engine;
		std::uniform_int_distribution<uint32_t> _Distribution;
	} g_Random;

	inline int GetRandom(int max) noexcept
	{
		return (g_Random._Distribution(g_Random._Engine) % (std::max(max, 2) - 1)) + 1;
	}
}
