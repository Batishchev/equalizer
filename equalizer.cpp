#include "equalizer.h"

void equalizer::initResult() {
    int i, j;

    for (i = 0; i < STRINGAMOUNT; i++) {
        for (j = 0; j < STRINGSIZE + 1; j++) {
            result[(STRINGSIZE + 1) * i + j] = ' ';
            if (j == STRINGSIZE)
                result[(STRINGSIZE + 1) * i + j] = '\n';
        }
    }

    result[(STRINGSIZE + 1) * STRINGAMOUNT - 1] = '\0';
}

void equalizer::initFft() {
    int i;
    for (i = 0; i < STRINGAMOUNT - 1; i++)
        previousFft[i] = 0;
}

void equalizer::initConsole() {
    if (!initscr()) {
        throw "Ошибка инициализации консоли.\n";
        return 0;
    }
    initColors();
}

void equalizer::initColors() {
    colorsEnabled = has_colors();

    if (colorsEnabled) {
        start_color();

        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        init_pair(2, COLOR_YELLOW, COLOR_BLACK);
        init_pair(3, COLOR_RED, COLOR_BLACK);
    }
}

void equalizer::initFileName(char* filename) {
    this->filename = filename;

    initRunString();
}

void equalizer::initRunString() {
    int i, j;

    for (i = 0; i < STRINGSIZE; i++) {
        if (filename[i] == '\0') {
            for (j = i; j < STRINGSIZE; j++)
                runString[j] = ' ';
            break;
        } else
            runString[i] = filename[i];
    }
    
    runString[STRINGSIZE] = '\0';
}

void equalizer::initBASS() {
    if (!BASS_Init(-1, 44100, 0, 0, NULL)) {
        throw "Ошибка инициализации BASS.\n";
        return 0;
    }
    
    stream = BASS_StreamCreateFile(FALSE, filename, 0, 0, 0);
    
    if (!stream) {
        throw "Ошибка при открытии файла.\n";
        return 0;
    }
}

void equalizer::play() {
    if(!BASS_ChannelPlay(stream, FALSE)) {
        throw "Ошибка воспроизведения файла.\n";
        return 0;
    }
}

equalizer::equalizer(char* filename) {
    initResult();
    initFft();
    initConsole();
    initFileName(filename);
    initBASS();
}

equalizer::~equalizer() {

}

