//#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <bass.h>
#include <ncurses.h>
#include <unistd.h>
//#include <unistd.h> // for sleep()

#define FFTSIZE 128
#define STRINGSIZE 65
#define STRINGAMOUNT 30

using namespace std;

int main(int argc, char* argv[])
{
    HSTREAM stream;
    char* filename;
    float fft[FFTSIZE];
    int prevfft[FFTSIZE];
    int currentFft;
    int i,j;
    chtype result[STRINGSIZE*STRINGAMOUNT];  //30*65
    
    //Colors support
    bool colors;
    
    //Running filename support
    chtype runString[STRINGSIZE];
    int runStringStartIndex = 0;
    
    int length;
    int pos;
    chtype progressString[53];
    int minutes,seconds, minutes2, seconds2, allseconds;
    
    chtype* ptr;
    
    
    for(i=0;i<30;i++)
        for(j=0;j<65;j++){
            result[65*i+j] = ' ';
            if(j == 64)
                result[65*i+j] = '\n';
        }
    result[1949] = '\0';
    
    for(j=0;j<64;j++)
        prevfft[i] = 0;
    
    if(argc != 2) {
        addstr( "Программа принимает 1 аргумент - имя проигрываемого wav файла.\n" );
        refresh();
        return 0;
    }
    
    if(!initscr()){
        addstr( "Ошибка инициализации консоли.\n");
        refresh();
        return 0;
    }
    
    colors = has_colors();
    
    if(colors){
        start_color();

        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        init_pair(2, COLOR_YELLOW, COLOR_BLACK);
        init_pair(3, COLOR_RED, COLOR_BLACK);
    }
    
    filename = argv[1];
    
    for(i=0;i<64;i++){
        if(filename[i] == '\0'){
            for(j=i;j<64;j++)
                runString[j] = ' ';
            break;
        }
        else
            runString[i] = filename[i];
    }
        
    runString[64] = '\0';
    
    if (!BASS_Init(-1, 44100, 0, 0, NULL)) {
        addstr( "Ошибка инициализации BASS.\n");
        refresh();
        return 0;
    }
    
    stream = BASS_StreamCreateFile(FALSE, filename, 0, 0, 0);
    
    if (!stream) {
        addstr( "Ошибка при открытии файла.\n");
        refresh();
        return 0;
    }
    
    if(!BASS_ChannelPlay(stream, FALSE)) {
        addstr("Ошибка воспроизведения файла.\n");
        refresh();
        return 0;
    }
    int pr;
    length = BASS_ChannelGetLength(stream, BASS_POS_BYTE);
    seconds2 = BASS_ChannelBytes2Seconds(stream, length);
    while(seconds2 >= 60){
        seconds2-=60;
        minutes2++;
    }
    while (BASS_ChannelIsActive(stream) != BASS_ACTIVE_STOPPED) {
        BASS_ChannelGetData(stream, fft, BASS_DATA_FFT256);
        pos = BASS_ChannelGetPosition(stream, BASS_POS_BYTE);
        
        for(i=0;i<64;i++){
            currentFft = ceil(fabs(fft[i]) * 100);
            if(currentFft > 29)
                currentFft = 29;
            
            if(currentFft <= prevfft[i]){
                j = prevfft[i];
                result[(29-j)*65+i] = ' ' | COLOR_PAIR(1);
                prevfft[i] = j-1;
                continue;
            }
            
            if(!colors){
                for(j=currentFft-1;j>=prevfft[i];j--)
                    result[(29-j)*65+i] = '*';
            } else {
                for(j=currentFft-1;j>=prevfft[i];j--) {
                    if(j > 26)
                        result[(29-j)*65+i] = '*' | COLOR_PAIR(3);
                    else if(j>24)
                        result[(29-j)*65+i] = '*' | COLOR_PAIR(2);
                    else
                        result[(29-j)*65+i] = '*' | COLOR_PAIR(1);
                }
            }
            
            prevfft[i] = currentFft;
        }
        
        move(30,0);
        runStringStartIndex++;
        if(runStringStartIndex > 64)
            runStringStartIndex = 0;
        mvaddchnstr(30, 0, &runString[runStringStartIndex], 65-runStringStartIndex-1);
        mvaddchnstr(30, 65-runStringStartIndex-1, &runString[0], runStringStartIndex);
        
        //Прогресс бар
        move(31,0);
        pr = floor((float(pos) / float(length)) * 100);
        if(pr%2 == 0){
            progressString[0] = '|';
            pr = int(pr/2)+1;
            for(i=1;i<pr;i++)
                progressString[pr] = '=';
            progressString[i] = '>' | COLOR_PAIR(2);
            for(i=i+1;i<51;i++)
                progressString[i] = '=';
            progressString[51] = '|';
            progressString[52] = '\0';
            allseconds = floor(BASS_ChannelBytes2Seconds(stream, pos));
            seconds = allseconds%60;
            minutes = ceil(allseconds/60);
            addchstr(progressString);
            move(31,52);
            printw("%2d:%2d/%2d:%2d|",minutes, seconds,minutes2, seconds2);
        }

        for(i=29;i>=0;i--){
            ptr = &result[i*65];
            move(i,0);
            addchnstr(ptr, 64);
        }
        refresh();
        usleep(100000);
    }
    
    BASS_ChannelStop(stream);
    BASS_StreamFree(stream); 
    endwin();
}