#include "View.h"
#include "Config.h"

#include <iostream>
#include <SDL.h>

View::View()
{
    //ctor
}

View::~View()
{
    //dtor
}

void View::animate(SDL_Rect &r, SDL_Window *win, SDL_Surface *scr) {
    for (int i = 0; i < 10; i++) {
        r.x += 1;
        r.y += 1;
    }
}

//int View::init(SDL_Window &win, SDL_Surface &scr) {
//    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
//        std::cout << "Can't init: " << SDL_GetError() << std::endl;
//        system("pause");
//    }
//
//    win = SDL_CreateWindow("События", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 450, 450, SDL_WINDOW_SHOWN);
//    if (win == NULL) {
//        std::cout << "Can't create window: " << SDL_GetError() << std::endl;
//        system("pause");
//    }
//
//    scr = SDL_GetWindowSurface(win);
//
//    return 1;
//}
