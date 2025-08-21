#pragma once

#include <random>
#include <ctime>

namespace Tetris::Utils
{
	static inline struct RandomData
	{
		inline RandomData()
		{
			s_Engine.seed((uint32_t)time(0));
		}

		static inline std::default_random_engine s_Engine;
		static inline std::uniform_int_distribution<uint32_t> s_Distribution;
	} s_Random;

	static inline int GetRandom(int max)
	{
		s_Random.s_Engine.seed(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
		return (s_Random.s_Distribution(s_Random.s_Engine) % (std::max(max, 2) - 1)) + 1;
	}
}
