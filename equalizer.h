#include <ncurses.h>
#include <bass.h>

#ifndef EQUALIZER_H
#define	EQUALIZER_H

#define FFTSIZE 128
#define STRINGSIZE 64
#define STRINGAMOUNT 30

class equalizer {
private:
    HSTREAM stream;
    char* filename;
    float fft[FFTSIZE];
    int previousFft[FFTSIZE];
    int currentFft;
    chtype result[STRINGSIZE*STRINGAMOUNT];  //30*65
    
    //Colors support
    bool colorsEnabled;
    
    //Running string support
    chtype runString[STRINGSIZE];
    int runStringStartIndex;
    
    void initResult();
    void initFft();
    void initConsole();
    void initColors();
    void initFileName(char* filename);
    void initRunString();
    void initBASS();
public:
    equalizer(char* filename);
    ~equalizer();
    void play();
private:

};

#endif	/* EQUALIZER_H */

