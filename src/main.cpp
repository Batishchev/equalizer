#include "audio.h"
#include <cstdlib>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	try {
		if(argc != 2) {
			cout << "Программа принимает 1 аргумент - имя проигрываемого файла.\n";
			return 0;
		}

		audio* a = new audio();
		a->open(argv[1]);
		a->play();
	} catch (char* er) {
		cout << er << endl;
	}
}
