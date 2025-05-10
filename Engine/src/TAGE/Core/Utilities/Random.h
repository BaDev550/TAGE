#pragma once
#include <random>

#define RANDOM_FLOAT() Random::Float()
#define RANDOM_FLOAT_RANGE(min, max) Random::Float(min, max)
#define RANDOM_INT() Random::Int(0, 100)
#define RANDOM_INT_RANGE(min, max) Random::Int(min, max)

namespace TAGE {
	class Random {
	public:
		static void Init() {
			s_RandomEngine.seed(static_cast<unsigned int>(time(nullptr)));
		}
		static float Float() {
			return s_Distribution(s_RandomEngine);
		}
		static float Float(float min, float max) {
			return min + (max - min) * Float();
		}
		static int Int(int min, int max) {
			std::uniform_int_distribution<int> dist(min, max);
			return dist(s_RandomEngine);
		}

	private:
		static std::mt19937 s_RandomEngine;
		static std::uniform_real_distribution<float> s_Distribution;
	};
}
