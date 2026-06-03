inline float clip(float value, float min, float max) {
	if (value < min) return min;
	else if (value > max) return max;
	else return value;
}

inline int GetRandomIntrBetween(int min, int max) {
	mt19937 gen(random_device{}());
	std::uniform_int_distribution<int> dis(min,max);
	return dis(gen);
}

inline bool HasHitChance(float chance) {
	mt19937 gen(random_device{}());
	std::uniform_real_distribution<float> dis(0.0,1.0);
	return dis(gen) <= chance;
}