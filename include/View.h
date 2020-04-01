#ifndef VIEW_H
#define VIEW_H

#include <SDL.h>

class View
{
    public:
        View();
        virtual ~View();

        void animate(SDL_Rect &r, SDL_Window *win, SDL_Surface *scr);

    protected:

    private:
};

#endif // VIEW_H
