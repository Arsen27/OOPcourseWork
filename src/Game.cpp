#include "Game.h"
#include "View.h"

#include <iostream>

using namespace std;

Game::Game()
{
    //ctor
}

Game::~Game()
{
    //dtor
}

void Game::start() {
    View view;

    SDL_Window *win;
    SDL_Surface *scr;

    win = SDL_CreateWindow("События", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 450, 450, SDL_WINDOW_SHOWN);
    scr = SDL_GetWindowSurface(win);

    SDL_Surface *field = SDL_LoadBMP("field.bmp");
    SDL_Rect r;
    r.x = 0;
    r.y = 0;

    bool run = true;
    while(run) {
        view.animate(r, win, scr);

        SDL_FillRect(scr, NULL, SDL_MapRGB(scr -> format, 255, 255, 255));
        SDL_BlitSurface(field, NULL, scr, &r);
        SDL_UpdateWindowSurface(win);
    }

    cout << "Game started" << endl;
}


