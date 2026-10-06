#pragma once

struct GameProgress {
	int rupees = 0;
	int gamesPlayed = 0;
	bool brownChickenUnlocked = false;
	bool blueChickenUnlocked = false;
	bool darkChickenUnlocked = false;
	bool goldenChickenUnlocked = false;

	bool isChickenUnlocked (const int type) const {
		switch (type) {
			case 1: return true;
			case 2: return brownChickenUnlocked;
			case 3: return blueChickenUnlocked;
			case 4: return darkChickenUnlocked;
			case 5: return goldenChickenUnlocked;
			default: return false;
		}
	}

	bool operator == (const GameProgress& other) const {
		return rupees == other.rupees &&
			gamesPlayed == other.gamesPlayed &&
			brownChickenUnlocked == other.brownChickenUnlocked &&
			blueChickenUnlocked == other.blueChickenUnlocked &&
			darkChickenUnlocked == other.darkChickenUnlocked &&
			goldenChickenUnlocked == other.goldenChickenUnlocked;
	}

	bool operator != (const GameProgress& other) const {
		return !(*this == other);
	}
};