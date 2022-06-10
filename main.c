#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
//720×1280
//480×800
const int ScreenHeight = 800;
const int ScreenWidth = 480;

int main(){
    //screen & fps
    InitWindow(ScreenWidth, ScreenHeight, "projekat_v1");
    SetTargetFPS(60);

    Texture2D pozadina = LoadTexture("./textures/pozadina.png");
    Texture2D button_dodaj_racun = LoadTexture("./textures/button_dodaj_racun.png");
    button_dodaj_racun.height /= 2;
    button_dodaj_racun.width /= 2;

    while(!WindowShouldClose()){
        //update

        //draw
        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexture(pozadina, 0, 0, WHITE);
            DrawTexture(button_dodaj_racun, 100, 100, CLITERAL(Color){ 255, 255, 255, 128 });
            
        EndDrawing();
    }

    CloseWindow();
    return 0;
}