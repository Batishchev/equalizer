#ifndef AUDIO_H_
#define AUDIO_H_

#include <bass.h>
#include <math.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#include <cstddef>
#define SLEEP Sleep(100);
#else
#include <unistd.h>
#define SLEEP usleep(100000);
#endif

#include "equalizer.h"

#include "stdio.h"

class audio {
private:
	//Размер буфера fft
	int fftSize;

	//Буфер fft из double значений
	double* fft;

	//Имя проигрываемого файла, и проигрываемый поток
	char* filename;
	HSTREAM stream;

	int totalMinutes,
		totalSeconds,
		currentMinutes,
		currentSeconds;

	void processTotalTimeData();
	void processCurrentTimeData();
	//Вычисление значений fft при проигрывании потока
	void processFftData();
	//Логарифмирование и округление значений fft.
	void normalizeFftData();
public:
	audio();
	~audio();

	void open(char* filename);
	void play();
	void stop();
private:
	equalizer* eq;

};

#endif /* AUDIO_H_ */
