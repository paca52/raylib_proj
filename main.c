#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>

const int ScreenHeight = 720;
const int ScreenWidth = 1024;

int main(){
    //screen & fps
    InitWindow(ScreenWidth, ScreenHeight, "projekat_v1");
    SetTargetFPS(60);

    while(!WindowShouldClose()){
        ClearBackground(BLACK);
        BeginDrawing();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}