/*

CMATRIX clone

*/

#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <unistd.h>

using namespace std;

const int max_distance = 7;
const int width = 30;
const int height = 30;
const int max_pillar_height = 10;
const int min_pillar_heigt = 7;
const int bitmap_height = max_pillar_height + height + max_distance + 1;

char matrix[height][width];

int wait[width]; 
int speed[width];
int tempSpeed[width];

bool bitmap[bitmap_height][width];

void initSpeed() {
    for (int i = 0; i < width; i++) {
        speed[i] = 1 + rand() % 5;
        tempSpeed[i] = 1 + rand() % 5;
    }
}
void initWait() {
    for (int i = 0; i < width; i++) {
        wait[i] = 1 + rand() % (max_distance);
    }
}

void initMatrix() {
	int i, j;
	char character;

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			character = 65 + rand() % (90 - 65 + 1);
			if (rand() % 2) {
				character += 32;
			}
			matrix[i][j] = character;
		}
	}
}

void genPillar() {
    int i, j;
    int pillar_height;

    initWait();

    for (j = 0; j < width; j++) {
        pillar_height = min_pillar_heigt + rand() % (max_pillar_height - min_pillar_heigt + 1);
        for (i = 0; i < pillar_height; i++) {
            bitmap[i + wait[j]][j] = true; 
        }
    }
}

void initBitmap() {
    int i, j;

    for (i = 0; i < bitmap_height; i++) {
        for (j = 0; j < width; j++) {
            bitmap[i][j] = false; 
        }
    }

    genPillar();
}

void slide(int column) {
    for (int i = bitmap_height - 2; i >= 0; i--) {
        if (bitmap[i][column]){
                bitmap[i + 1][column] = true; 
                bitmap[i][column] = false;
            }
    }

   /* for (i = bitmap_height - 2; i >= 0; i--) {
        for (j = 0; j < width; j++) {
            if (bitmap[i][j]){
                bitmap[i + 1][j] = true; 
                bitmap[i][j] = false;
            }
        }
    }*/
}

void control() {
    int i, j;

    for (j = 0; j < width; j++) {
        for (i = 0; i < max_pillar_height + 3; i++) {
            if (bitmap[i][j]) {
                return;
            }
        }
    }

    genPillar();
}
void view() {
    int i, j;
	HANDLE hStdout;
	COORD destCoord;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	destCoord.X = 0;
	destCoord.Y = 0;
	for ( ; ; ) {		
		usleep(500);
        initMatrix();
		SetConsoleCursorPosition(hStdout, destCoord);
		for (i = 0; i < height - 1; i++) {
			for (j = 0; j < width; j++) {
                if (bitmap[max_pillar_height + max_distance + i][j]) {
			        cout << matrix[i][j] << ' ';
                } else {
                    cout << "  ";
                }
			}
			cout << '\n';
		}
        for (j = 0; j < width; j++) {
            tempSpeed[j]--;
            if (tempSpeed[j] == 0){
                slide(j);
                tempSpeed[j] = speed[j];
            }
        }
        control();
	}
}

void viewDebug() {
	int i, j;

	cout << "\n\n";
	cout << "\n\n";
	for (i = 0; i < bitmap_height; i++) {
		for (j = 0; j < width; j++) {
			cout << bitmap[i][j] << ' ';
		}
		cout << '\n';
	}
}

int main() {
    system("color 02 && cls");

    initSpeed();
    initBitmap();
    view();
    return 0;
}
