#ifndef EQUALIZER_H
#define	EQUALIZER_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <curses.h>
#else
#include <ncurses.h>
#endif

#include <math.h>

class equalizer {
private:
	int width,
		height;
	int* prevFft;
	int* normalizedFft;
	bool colors;
	chtype** result;
	int runNameShift;

	void normalizeFft(double* fft, int* normalizedFft);
public:
    equalizer();
    ~equalizer();
    void redraw(double* fft);
    void drawRunName(char* songName);
};

#endif	/* EQUALIZER_H */

