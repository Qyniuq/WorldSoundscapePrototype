#pragma once
#include <vector>
#include "Instrument.h"
#include "Weather.h"
#include <string>

enum intervals { sT = 1, T = 2, TsT = 3, TT = 4, TTsT = 5, TTT = 6, TTTsT = 7, TTTT = 8};
enum scale_degree { minor_second = 1, second, minor_third, third, fourth, diminished_fifth, fifth, minor_sixth, sixth, minor_seventh, seventh, octave};

struct Scale {
	std::vector <intervals> matrix;
	std::string name;
};

enum notes {
	C2 = -24, Db2, D2, Eb2, E2, F2, Gb2, G2, Ab2, A2, Bb2, B2,
	C3 = -12, Db3 = -11, D3 = -10, Eb3 = -9, E3 = -8, F3 = -7, Gb3= -6, G3= -5, Ab3 = -4, A3 = -3, Bb3 = -2, B3 = -1,
	C4 = 0, Db4 = 1, D4 = 2, Eb4 = 3, E4 = 4, F4 = 5, Gb4 = 6, G4 = 7, Ab4 = 8, A4 = 9, Bb4 = 10, B4 = 11,
	C5 = 12, Db5, D5, Eb5, E5, F5, Gb5, G5, Ab5, A5, Bb5, B5,
	C6 = 24, Db6, D6, Eb6, E6, F6, Gb6, G6, Ab6, A6, Bb6, B6
};

Scale getScaleMatrix(Weather& weather);

std::vector <notes> setScaleForInstrument(const Instrument& instrument, Weather& weather);
