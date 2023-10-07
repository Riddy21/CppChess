#include <stdio.h>
#include <board.h>

using namespace std;

int main(){
    printf("hello world!");

    Board board = Board("presets/check.txt");
    board.set({0, 0}, board.get({1, 1}));
    return 0;
}
