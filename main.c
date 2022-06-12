#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAX_INPUT_CHARS 100

//480×800
const int ScreenHeight = 800;
const int ScreenWidth = 480;

typedef struct red{
    char tip[100];
    int iznos;
    struct red *sledeci;
}red;

typedef struct korisnik{
    char ime[30], prezime[30], mesto[30], broj_tel[20];
    int stanje;
}korisnik;

int len(char s[]){
    int i=0;
    while(s[i] != '\0') i++;
    return i;
}

void ured(red **poc, red **kraj, int iznos, char tip[])
{
    red *novi = (red*)malloc(sizeof(red));
    novi->sledeci = NULL;
    strcpy(novi->tip, tip);
    novi->iznos = iznos;

    if(*poc == NULL)
        *poc = novi;
    else
        (*kraj)->sledeci = novi;
    *kraj = novi;
}

FILE* izreda(red **poc, FILE *naplaceni)
{
    if(*poc == NULL)
        return naplaceni;
    
    if((*poc)->sledeci == NULL){
        fprintf(naplaceni, "%s[%d]\n", (*poc)->tip, (*poc)->iznos);
        *poc = NULL;
        free(*poc);
    }
    else{
        red *tmp = *poc;
        fprintf(naplaceni, "%s[%d]\n", (*poc)->tip, (*poc)->iznos);
        *poc = (*poc)->sledeci;
        free(tmp);
    }
    return naplaceni;
}

void ispis_rac(red *glava){
    if(glava == NULL){
        printf("Trenutno nema racuna.\n");
        return;
    }

    printf("Racuni:\n");
    while(glava != NULL){
        printf("%s(%d)\n", glava->tip, glava->iznos);
        glava = glava->sledeci;
    }
}

int main(){
    //projekat
    char error_output[200];
    int error = 0;


    char s[101];
    korisnik kor;
    int br = 0, pom;
    FILE *unos = fopen("Korisnici.txt", "r"), *naplaceni = fopen("Naplaceniracuni.txt", "w");
    if(unos == NULL){
        strcpy(error_output, "Nedostaje datoteka \n'Korisnici.txt' u folderu \nprograma ili program \nne moze da je ucita.\n");
        error = 1;
        error_output[92] = '\0';
    }
    else if(naplaceni == NULL){
        strcpy(error_output, "Program nije uspeo \nda napravi datoteku\n'Naplaceniracuni.txt'.");
        error = 1;
        error_output[65] = '\0';
    }
    if(!error){
        //unos iz datoteke
        
        while(fscanf(unos, "%s", s)!= EOF){
            if(br < 4){
                pom = len(s);
                s[pom-1] = '\0';
            }
            //printf("%s\n", s);
            switch(br){
                case 0:
                    strcpy(kor.ime, s);
                    break;
                case 1:
                    strcpy(kor.prezime, s);
                    break;
                case 2:
                    strcpy(kor.mesto, s);
                    break;
                case 3:
                    strcpy(kor.broj_tel, s);
                    break;
                case 4:
                    pom = atoi(s);
                    kor.stanje = pom;
                    break;
            }

            br++;
        }

        if(br != 5){
            strcpy(error_output, "Pogresno su uneti \npodatci o korisniku.\nForma za unos podataka: \nime, prezime, mesto, \nbroj(sve spojeno), stanje na \nracunu.\n");
            error = 1;
            error_output[127] = '\0';
        }
    }

    char broj[100];
    broj[0] = 0;
    char tip[100];
    tip[0] = 0;
    red *racuni_poc = NULL, *racuni_kraj = NULL;

    //screen & fps
    InitWindow(ScreenWidth, ScreenHeight, "projekat_v1");
    SetTargetFPS(60);
    //LoadFont...

    Texture2D pozadina = LoadTexture("./textures/pozadina.png");
    
    //buttons;rez = 538x150
    Texture2D button_dodaj_racun = LoadTexture("./textures/button_dodaj_racun.png");
    Texture2D button_dodaj_na_stanje = LoadTexture("./textures/button_dodaj_na_stanje.png");
    Texture2D button_exit = LoadTexture("./textures/button_exit.png");
    Texture2D button_plati = LoadTexture("./textures/button_plati.png");
    Texture2D button_prikazi = LoadTexture("./textures/button_prikazi.png");
    
    button_dodaj_racun.height /= 2;
    button_dodaj_racun.width /= 2;

    button_dodaj_na_stanje.height /= 2;
    button_dodaj_na_stanje.width /= 2;

    //button_exit.height /= 2;
    //button_exit.width /= 2;

    button_exit.height /= 2;
    button_exit.width /= 2;

    button_plati.height /= 2;
    button_plati.width /= 2;

    button_prikazi.height /= 2;
    button_prikazi.width /= 2;

    int left_click, mouse_x, mouse_y, menu = 1, dodaj_na_racun = 0, dodaj_stanje = 0, plati_racune = 0, suma = 0, prikazi = 0;
    while(!WindowShouldClose()){
        //update
        //textbox update
        
        if(dodaj_na_racun == 1){
            if(IsKeyPressed(KEY_ENTER)){
                menu = 0;
                dodaj_na_racun = 2;
            }
            else{
                int i=len(tip);
                int key = GetCharPressed();
                while (key > 0)
                {
                    if ((key >= 32) && (key <= 125) && (i < MAX_INPUT_CHARS)){
                        tip[i] = (char)key;
                        tip[i+1] = '\0'; 
                        i++;
                    }
                    key = GetCharPressed();
                }
                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    i--;
                    if (i < 0) i = 0;
                    tip[i] = '\0';
                }
            }
        }
        
        else if(dodaj_na_racun == 2){
            if(IsKeyPressed(KEY_ENTER)){
                menu = 1;
                dodaj_na_racun = 0;

                if(len(broj) != 0 && len(tip) != 0)
                    ured(&racuni_poc, &racuni_kraj, atoi(broj), tip);
                broj[0] = tip[0] = 0;
            }
            else{
                int i=len(broj);
                int key = GetCharPressed();
                while (key > 0)
                {
                    if ((key >= 48) && (key <= 57) && (i < MAX_INPUT_CHARS)){
                        broj[i] = (char)key;
                        broj[i+1] = '\0'; 
                        i++;
                    }
                    key = GetCharPressed();
                }
                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    i--;
                    if (i < 0) i = 0;
                    broj[i] = '\0';
                }
            }
        }
        else if(dodaj_stanje == 1){
            if(IsKeyPressed(KEY_ENTER)){
                menu = 1;
                dodaj_stanje = 0;

                if(len(broj) != 0)
                    kor.stanje += atoi(broj);
                
                broj[0] = 0;
            }
            else{
                int i=len(broj);
                int key = GetCharPressed();
                while (key > 0)
                {
                    if ((key >= 48) && (key <= 57) && (i < MAX_INPUT_CHARS)){
                        broj[i] = (char)key;
                        broj[i+1] = '\0'; 
                        i++;
                    }
                    key = GetCharPressed();
                }
                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    i--;
                    if (i < 0) i = 0;
                    broj[i] = '\0';
                }
            }
        }
        else if(plati_racune == 1){
            if(IsKeyPressed(KEY_ENTER)){
                menu = 1;
                plati_racune = 0;
                suma = 0;
            }
        }
        else if(prikazi == 1){
            if(IsKeyPressed(KEY_ENTER)){
                menu = 1;
                prikazi = 0;
            }
        }
        //menu update
        else if(menu == 1){
            left_click = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
            mouse_x = GetMouseX();
            mouse_y = GetMouseY();
            if(error != 1 && left_click == 1){
                if(mouse_x >= 100 && mouse_x <= 100+538/2 && mouse_y >= 600 && mouse_y <= 600+150/2)
                    break;
                else if(mouse_x >= 100 && mouse_x <= 100+538/2 && mouse_y >= 200 && mouse_y <= 200 + 150/2){
                    menu = 0;
                    dodaj_na_racun = 1;
                }
                else if(mouse_x >= 100 && mouse_x <= 100+538/2 && mouse_y >= 300 && mouse_y <= 300 + 150/2){
                    menu = 0;
                    dodaj_stanje = 1;
                }
                else if(mouse_x >= 100 && mouse_x <= 100+538/2 && mouse_y >= 400 && mouse_y <= 400 + 150/2){
                    menu = 0;
                    plati_racune = 1;
                }
                else if(mouse_x >= 100 && mouse_x <= 100+538/2 && mouse_y >= 500 && mouse_y <= 500 + 150/2){
                    menu = 0;
                    prikazi = 1;
                }
            }
        }

        //draw
        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexture(pozadina, 0, 0, WHITE);
            //error
            if(error == 1)
                DrawText( TextFormat("Error: %s", error_output), 0, 0, 30, WHITE);
            //nema error-a
            else{
                if(menu == 1){
                    DrawText(TextFormat("Trenutno stanje: %d", kor.stanje), 75, 100, 30, WHITE);
                    DrawTexture(button_dodaj_racun, 100, 200, CLITERAL(Color){ 255, 255, 255, 128 });
                    DrawTexture(button_dodaj_na_stanje, 100, 300, CLITERAL(Color){ 255, 255, 255, 128 });
                    
                    DrawTexture(button_plati, 100, 400, CLITERAL(Color){ 255, 255, 255, 128 });
                    DrawTexture(button_prikazi, 100, 500, CLITERAL(Color){ 255, 255, 255, 128 });

                    DrawTexture(button_exit, 100, 600, CLITERAL(Color){ 255, 255, 255, 128 });
                }
                else if(dodaj_na_racun == 1){
                    DrawText("Unesite tip racuna: ", 30, 30, 30, WHITE);
                    DrawText(TextFormat("%s\n", tip), 60, 60, 30, WHITE);
                }
                else if(dodaj_na_racun == 2){
                    DrawText("Unesite iznos racuna: ", 30, 30, 30, WHITE);
                    DrawText(TextFormat("%s\n", broj), 60, 60, 30, WHITE);
                }
                else if(dodaj_stanje == 1){
                    DrawText("Unesite zeljeni iznos: ", 30, 30, 30, WHITE);
                    DrawText(TextFormat("%s\n", broj), 60, 60, 30, WHITE);
                }
                else if(plati_racune == 1){
                    if(racuni_kraj != NULL && racuni_poc != NULL){

                        while(racuni_poc != NULL && racuni_kraj != NULL && racuni_poc->iznos <= kor.stanje){
                            kor.stanje -= racuni_poc->iznos;
                            suma += racuni_kraj->iznos;
                            naplaceni = izreda(&racuni_poc, naplaceni);
                        }
                    }
                    DrawText("Svi moguci racuni su placeni.\n", 30, 30, 30, WHITE);
                    DrawText(TextFormat("Njihov iznos je bio %d", suma), 60, 60, 30, WHITE);
                }
                else if(prikazi == 1){
                    int i = 2;
                    DrawText("Racuni(tip[iznos]):", 30, 30, 30, WHITE);
                    for(red *tmp = racuni_poc; tmp != NULL; tmp = tmp->sledeci){
                        DrawText(TextFormat("%s[%d]", tmp->tip, tmp->iznos), 50, 60*i, 30, WHITE);
                        i++;
                    }
                }
            }
            
            
        EndDrawing();
    }

    CloseWindow();
    return 0;
}