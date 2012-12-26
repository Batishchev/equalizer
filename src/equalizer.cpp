#include "equalizer.h"

equalizer::equalizer() {
	if (!initscr()) {
		throw "Ошибка инициализации консоли.\n";
		return;
	}

	colors = has_colors();

	if (!colors) {
		throw "Ошибка инициализации цветов консоли.\n";
		return;
	}

	start_color();

	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	init_pair(4, COLOR_WHITE, COLOR_BLACK);

	width = 256;
	height = 40;
	prevFft = new int[width];
	for (int i = 0; i < width; i++)
		prevFft[i] = 0;
	result = new chtype*[height];
	for (int i = 0; i < height; i++)
		result[i] = new chtype[width];
	normalizedFft = new int[width];

	runNameShift = 0;
}

equalizer::~equalizer() {
	endwin();
}

void equalizer::normalizeFft(double* fft, int* normalizedFft) {
	double value;
	int result;

	for (int i = 0; i < width; i++) {
		if (fft[i] <= 0) {
			normalizedFft[i] = 0;
			continue;
		}
		value = 1000 / fabs(log(fft[i])) - 10;
		result = ceil(value);
		if (result >= height)
			result = height - 1;
		if (result < 0)
			result = 0;
		normalizedFft[i] = result;
	}
}

void equalizer::redraw(double* fft) {
	int currentFft, i, j;

	normalizeFft(fft, normalizedFft);

	for (i = 0; i < width; i++) {
		currentFft = normalizedFft[i];

		if (currentFft <= prevFft[i]) {
			result[prevFft[i]][i] = ' ';
			prevFft[i] = prevFft[i] - 1;
			if(prevFft[i] == -1)
				prevFft[i] = 0;
			continue;
		}

		for (j = prevFft[i]; j < currentFft; j++) {
			if (j > height - 4)
				result[j][i] = '*' | COLOR_PAIR(3);
			else if (j > height - 6)
				result[j][i] = '*' | COLOR_PAIR(2);
			else
				result[j][i] = '*' | COLOR_PAIR(1);
		}

		prevFft[i] = currentFft;
	}

	for (i = 0; i <= height - 1; i++) {
		move(height - i - 1, 0);
		addchnstr(result[i], width);
	}
	move(height, 0);

	refresh();
}

void equalizer::drawRunName(char* songName) {
	char* name;
	name = songName;
	if(name[runNameShift] != '\0')
		name = &name[runNameShift];
	move(0, 0);
	addstr(name);
	refresh();
	runNameShift++;
	if(runNameShift > width)
		runNameShift = 0;
}

