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
    char result[1950];  //30*65
    int prevfft[64];
    
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
    
    filename = argv[1];
    
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
                result[(29-j)*65+i] = ' ';
                prevfft[i] = j-1;
                continue;
            }
            
            for(j=current-1;j>=prevfft[i];j--)
                result[(29-j)*65+i] = '*';
            
            prevfft[i] = current;
        }
        
        move(0,0);
        printw(result);
        refresh();
        usleep(100000);
    }
    
    BASS_ChannelStop(stream);
    BASS_StreamFree(stream); 
    endwin();
}