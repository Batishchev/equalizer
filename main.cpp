//#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <bass.h>
#include <ncurses.h>
#include <unistd.h>
//#include <unistd.h> // for sleep()

using namespace std;

int main(int argc, char* argv[])
{
    HSTREAM stream;
    char* filename;
    float fft[128];
    int current, i, j;
    chtype result[1950];  //30*65
    chtype* ptr;
    int prevfft[64];
    bool colors;
    chtype runString[65];
    int runStringPtr = 0;
    
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
        
    while (BASS_ChannelIsActive(stream) != BASS_ACTIVE_STOPPED) {
        BASS_ChannelGetData(stream, fft, BASS_DATA_FFT256);
        
        for(i=0;i<64;i++){
            current = ceil(fabs(fft[i]) * 100);
            if(current > 29)
                current = 29;
            
            if(current <= prevfft[i]){
                j = prevfft[i];
                result[(29-j)*65+i] = ' ' | COLOR_PAIR(1);
                prevfft[i] = j-1;
                continue;
            }
            
            if(!colors){
                for(j=current-1;j>=prevfft[i];j--)
                    result[(29-j)*65+i] = '*';
            } else {
                for(j=current-1;j>=prevfft[i];j--) {
                    if(j > 26)
                        result[(29-j)*65+i] = '*' | COLOR_PAIR(3);
                    else if(j>24)
                        result[(29-j)*65+i] = '*' | COLOR_PAIR(2);
                    else
                        result[(29-j)*65+i] = '*' | COLOR_PAIR(1);
                }
            }
            
            prevfft[i] = current;
        }
        
        move(30,0);
        runStringPtr++;
        if(runStringPtr > 64)
            runStringPtr = 0;
        mvaddchnstr(30, 0, &runString[runStringPtr], 65-runStringPtr);
        mvaddchnstr(30, 65-runStringPtr, &runString[0], runStringPtr);
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