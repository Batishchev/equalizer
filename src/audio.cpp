#include "audio.h"

audio::audio() {
	if (!BASS_Init(-1, 44100, 0, (void*) 0, (void*) 0)) {
		throw "Ошибка инициализации BASS";
		return;
	}
	fftSize = 256;
	fft = new double[fftSize];
	totalMinutes = 0;
	totalSeconds = 0;
	currentMinutes = 0;
	currentSeconds = 0;

	eq = new equalizer();
}

audio::~audio() {
	// TODO Auto-generated destructor stub
}

void audio::open(char* filename) {
	stream = BASS_StreamCreateFile(FALSE, filename, 0, 0, 0);

	if (!stream) {
		throw "Ошибка при открытии файла.\n";
		return;
	}
}

void audio::play() {
	if (!BASS_ChannelPlay(stream, FALSE)) {
		throw "Ошибка воспроизведения файла.\n";
		return;
	}
	processTotalTimeData();
	while (BASS_ChannelIsActive(stream) != BASS_ACTIVE_STOPPED) {
		processFftData();
		processCurrentTimeData();
		SLEEP
	}
}

void audio::processFftData() {
	BASS_ChannelGetData(stream, fft, BASS_DATA_FFT512);
	eq->redraw(fft);
}

void audio::processTotalTimeData() {
	BYTE length = BASS_ChannelGetLength(stream, BASS_POS_BYTE);
	totalSeconds = BASS_ChannelBytes2Seconds(stream, length);
	while (totalSeconds >= 60) {
		totalSeconds -= 60;
		totalMinutes++;
	}
}

void audio::processCurrentTimeData() {
	BYTE length = BASS_ChannelGetLength(stream, BASS_POS_BYTE);
	double seconds = BASS_ChannelBytes2Seconds(stream, length);
	currentSeconds = ceil(seconds) - currentMinutes * 60;
	while (currentSeconds >= 60) {
		currentSeconds -= 60;
		currentMinutes++;
	}
}

