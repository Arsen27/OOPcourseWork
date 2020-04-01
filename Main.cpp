#include <SDL.h>
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <future>
#include <cstdlib>
#include <ctime>

using namespace std;

const int SCREEN_WIDTH = 450;
const int SCREEN_HEIGHT = 450;

int FIELD_SIZE = 4;

int NUM_SIZE = NULL;
int GAP_SIZE = NULL;

//if (FIELD_SIZE == 4) {
//    NUM_SIZE = 100;
//    GAP_SIZE = 10;
//}

SDL_Window *win = NULL;
SDL_Surface *scr = NULL;
SDL_Surface *john = NULL;

SDL_Surface *two = NULL;
SDL_Surface *two2 = NULL;
SDL_Surface *four = NULL;

enum eventType
{
    RIGHT,
    LEFT,
    UP,
    DOWN
};

enum actionType {
    GENERATE,
    MOVE,
    PLUS
};

class Point {
private:
    int x = NULL, y = NULL;

public:
    int getX() {
        return x;
    }

    int getY() {
        return y;
    }

    void setX(int x) {
        this->x = x;
    }

    void setY(int y) {
        this->y = y;
    }

    Point() {
        x = NULL;
        y = NULL;
    }

    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

class Action {
private:
    actionType type;
    Point from;
    Point to;
    int numberFrom = NULL;
    int numberTo = NULL;
    int rect = NULL;

public:
    Point getFrom() {
        return from;
    }

    Point getTo() {
        return to;
    }

    actionType getType() {
        return type;
    }

    int getNumberFrom() {
        return numberFrom;
    }

    int getNumberTo() {
        return numberTo;
    }

    int getRect() {
        return rect;
    }

    // MOVE
    Action(actionType type, Point from, Point to) {
        this->type = type;
        this->from = from;
        this->to = to;
    } 
};

class Model
{
private:
    vector<vector<int>> map;

    void resetMap() {
        for (int i = 0; i < FIELD_SIZE; i++) {
            vector<int> v;
            for (int j = 0; j < FIELD_SIZE; j++) {
                v.push_back(0);
            }
            map.push_back(v);
        }
    }

    void printMap() {
        cout << endl;
        for (int i = 0; i < FIELD_SIZE; i++) {
            for (int j = 0; j < FIELD_SIZE; j++) {
                cout << map[i][j] << " ";
            }
            cout << endl;
        }
    }

    int getRand(int min, int max) {
        static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
        return static_cast<int>(rand() * fraction * (max - min + 1) + min);
    }
public:

    vector<Action> checkRight() {
        vector<Action> actions;
        cout << "hello from check right" << endl;
        for (int i = 0; i < FIELD_SIZE; i++) {
            vector<int> elements;
            vector<Point> elPositions;
            
            // Element Search
            for (int j = FIELD_SIZE - 1; j >= 0; j--) {
                if (map[i][j] != 0) {
                    elPositions.push_back(Point(j, i));
                    elements.push_back(map[i][j]);
                }
            }

            // Element Processing
            int k = 0;
            for (int j = FIELD_SIZE - 1; j >= 0; j--)
            {   
                if (k < elements.size()) {
                    int lastEl = k != 0 ? elements[k - 1] : NULL;

                    if (lastEl == elements[k]) {
                        map[i][j + 1] = lastEl * 2;
                        if (k + 1 < elements.size()) {
                            k++;
                            map[i][j] = elements[k];
                        } else {
                            map[i][j] = 0;
                        }
                    } else {
                        map[i][j] = elements[k];
                        actions.push_back(
                            Action(MOVE, elPositions[k], Point(j, i))
                        );
                    }
                } else {
                    map[i][j] = 0;
                }
                k++;
            }
        }

        printMap();

        return actions;
    }

    vector<vector<int>> getMap() {
        return map;
    }

    vector<Action> generateFirst() {
        vector<Action> actions;

        int run = 2;
        // int i = 0;
        while(run > 0)
        {
            int x, y;
            x = getRand(0, 3);
            y = getRand(0, 3);

            if (map[y][x] == 0)
            {
                int num;
                if (getRand(1, 4) == 4) {
                    num = 4;
                } else {
                    num = 2;
                }
                map[y][x] = num;

                Point to(x, y);
//                Action action(GENERATE, num, to);
//                actions.push_back(action);

                run -= 1;
                // i++;
            }
        }
        printMap();
        // cout << "Rand: " << getRand(1, 6) << endl;

        return actions;
    }

    Model() {
        resetMap();
        // showMap();
    }
};

class Controller
{
private:
    Model model;

    // Rect Map
    vector<vector<int>> rectMap;

    void rectMapInit()
    {
        int n = 0;
        for (int i = 0; i < FIELD_SIZE; i++)
        {
            vector<int> v;
            for (int j = 0; j < FIELD_SIZE; j++)
            {
                v.push_back(n);
                n++;
            }
            rectMap.push_back(v);
        }
    }

    void rectMapMove(Point a, Point b) {
        int temp = rectMap[a.getY()][a.getX()];
        rectMap[a.getY()][a.getX()] = rectMap[b.getY()][b.getX()];
        rectMap[b.getY()][b.getX()] = temp;
    }

public:
    vector<Action> eventHandler(eventType type) {
        vector<Action> actions;

        switch(type) {
            case RIGHT:
                actions = model.checkRight();
        }

        for (int i = 0; i < actions.size(); i++) {
            if (actions[i].getType() == 1) {
                rectMapMove(actions[i].getFrom(), actions[i].getTo());
            }

            cout << endl << "Action: " << endl << "Type: " << actions[i].getType() << " From: " << " point(" << actions[i].getFrom().getX() 
            << ", " << actions[i].getFrom().getY() << " To: " << " point(" << actions[i].getTo().getX() << ", " << actions[i].getTo().getY() << ")" << endl;
        }

        printRectMap();

        return actions;
    }

    // FOR FUTURE
    vector<Action> startGame() {
        vector<Action> inputActions = model.generateFirst();
        vector<Action> outputActions;
        // vector<Action> outputActions;

        // // Translation
        // for (int i = 0; i < inputActions.size(); i++) {
        //     Action curAction = inputActions[i];

        //     int x, y;
        //     x = curAction.getTo().getX() != 0 ? (curAction.getTo().getX() * 110) + 10 : 10;
        //     y = curAction.getTo().getY() != 0 ? (curAction.getTo().getY() * 110) + 10 : 10;

        //     Point p(x, y);
        //     cout << "x: " << Point(x, y).getX();
        //     Action newAction(GENERATE, inputActions[i].getNumber(), Point(x, y));
        // }

//        for (int i = 0; i < inputActions.size(); i++) {
//            Action curAction = inputActions[i];
//
//            outputActions.push_back(Action(GENERATE, curAction.getNumber(), rectMap[curAction.getTo().getY()][curAction.getTo().getX()]));
//        }

        return outputActions;
    }

    void printRectMap()
    {
        cout << endl << "Rect Map:" << endl;
        for (int i = 0; i < rectMap.size(); i++)
        {
            for (int j = 0; j < rectMap.size(); j++)
            {
                cout << rectMap[i][j] << " ";
            }
            cout << endl;
        }
    }

    vector<vector<int>> getRectMap() {
        return rectMap;
    }

        vector<vector<int>> getModelMap()
    {
        return model.getMap();
    }

    Controller() {
        rectMapInit();
        printRectMap();
    }
};

class View {
private:
    SDL_Window *win = NULL;
    SDL_Surface *scr = NULL;

    // Assets
    SDL_Surface *field = SDL_LoadBMP("field.bmp");
    SDL_Surface *two = SDL_LoadBMP("two.bmp");
    SDL_Surface *four = SDL_LoadBMP("four.bmp");

    SDL_Rect fieldRect;

        // Numbers
        vector <SDL_Rect> rects;

        void rectsInit() {
            int row = 0;
            for (int i = 0; i < FIELD_SIZE; i++)
            {
                for (int j = 0; j < FIELD_SIZE; j++)
                {
                    SDL_Rect r;
                    r.y = row != 0 ? (row * 110) + 10 : 10;
                    r.x = j != 0 ? (j * 110) + 10 : 10;
                    rects.push_back(r);
                }
                row++;
            }
        }

    bool run = true;
    SDL_Event e;

    Controller ctrl;

    int init()
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            std::cout << "Can't init: " << SDL_GetError() << std::endl;
            system("pause");
            return 1;
        }

        win = SDL_CreateWindow("2048", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (win == NULL)
        {
            std::cout << "Can't create window: " << SDL_GetError() << std::endl;
            system("pause");
            return 1;
        }

        scr = SDL_GetWindowSurface(win);

        return 0;
    }

    void listen()
    {
        while (SDL_PollEvent(&e) != NULL)
        {
            if (e.type == SDL_QUIT) {
                run = false;
            }

            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_UP)
                {
                    cout << "up" << endl;
                }

                else if (e.key.keysym.sym == SDLK_DOWN)
                {
                    cout << "down" << endl;
                }

                else if (e.key.keysym.sym == SDLK_RIGHT)
                {
                    ctrl.eventHandler(RIGHT);
                }

                else if (e.key.keysym.sym == SDLK_LEFT)
                {

                }
            }
        }
    }

    void play()
    {
        listen();

        // Background
        SDL_FillRect(scr, NULL, SDL_MapRGB(scr->format, 255, 255, 255));
        SDL_BlitSurface(field, NULL, scr, &fieldRect);
        for (int i = 0; i < FIELD_SIZE; i++) {
            for (int j = 0; j < FIELD_SIZE; j++) {
                // Output
                vector<vector<int>> map = ctrl.getModelMap();
                SDL_Surface *el;

                switch(map[i][j]) {
                    case 2:
                        el = two;
                        break;
                    case 4:
                        el = four;
                        break;
                    default:
                        el = NULL;
                }

                SDL_BlitSurface(el, NULL, scr, &rects[ctrl.getRectMap()[i][j]]);
            }
        }



        // SDL_BlitSurface(two, NULL, scr, &rects[0]);
        // SDL_BlitSurface(two, NULL, scr, &rects[1]);

        SDL_UpdateWindowSurface(win);
    }

    void rectUpdate(vector<Action> actions) {
        // for (int i = 0; i < actions.size(); i++) {

        // }
        ctrl.printRectMap();
    }




public:
    void start()
    {
        init();

        fieldRect.x = 0;
        fieldRect.y = 0;

        ctrl.startGame();

        while(run) {
           play();
        }
    }

    View() {
        rectsInit();
    }
};

int quit() {
    SDL_FreeSurface(john);

    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}

void eventListener(SDL_Event e, int &x) {

}

int main (int argc, char ** args) {
    srand(static_cast<unsigned int>(time(0)));

    View game;
    game.start();



//    if (init() == 1) {
//        return 1;
//    }
//
//    if (load() == 1) {
//        return 1;
//    }
//
//    printNum(2);
//    printFour();
//    printTwo2();
//
//    bool run = true;
//    SDL_Event e;
//    SDL_Rect r;
//    SDL_Rect twoRect;
//    SDL_Rect twoRect2;
//    SDL_Rect fourRect;
//
//    int x = 10;
//    int y = 10;
//
//    twoRect.x = 10;
//    twoRect.y = 10;
//
//    twoRect2.x = 340;
//    twoRect2.y = 10;
//
//    twoRect.h = 100;
//    twoRect.w = 100;
//
//    twoRect2.h = 100;
//    twoRect2.w = 100;
//
//    fourRect.x = 340;
//    fourRect.y = 10;
//
//    fourRect.h = 100;
//    fourRect.w = 100;
//
//    r.x = 0;
//    r.y = 0;
//
//    while (run) {
//         while(SDL_PollEvent(&e) != NULL) {
//            if (e.type == SDL_QUIT) {
//                run = false;
//            }
//
//
//            if (e.type == SDL_KEYDOWN) {
////                if (e.key.keysym.sym == SDLK_UP) {
////                    y -= 2;
////                }
////                if (e.key.keysym.sym == SDLK_DOWN) {
////                    y += 2;
////                }
//                if (e.key.keysym.sym == SDLK_RIGHT) {
//
////                    x += 330;
//                    for (int i = 0; i < 330; i++) {
//                        x += 1;
//                        twoRect.x = x;
//
//
////                      SDL_FillRect(scr, NULL, SDL_MapRGB(scr -> format, 255, 255, 255));
//
//                        SDL_BlitSurface(john, NULL, scr, &r);
//                        SDL_BlitScaled(two2, NULL, scr, &twoRect2);
//                        SDL_BlitScaled(two, NULL, scr, &twoRect);
//
//                        SDL_UpdateWindowSurface(win);
//                    }
//
//                    for (int i = 1; i < 11; i++) {
//                        twoRect.h += 1;
//                        twoRect.w += 1;
//
//                        if (i%2 == 0) {
//                            twoRect.x -= 1;
//                            twoRect.y -= 1;
//                        }
//
//
//                        SDL_BlitSurface(john, NULL, scr, &r);
//                        SDL_BlitScaled(two2, NULL, scr, &twoRect2);
//                        SDL_BlitScaled(two, NULL, scr, &twoRect);
//
//                        SDL_UpdateWindowSurface(win);
//
//                    }
//
//                    cout << endl << "x:" << twoRect.x << endl;
//                        Sleep(50);
//
//                        for (int i = 1; i < 11; i++) {
//                        twoRect.h -= 1;
//                        twoRect.w -= 1;
//
//                        if (i%2 == 0) {
//                            twoRect.x += 1;
//                            twoRect.y += 1;
//                        }
//
//
//
//
//                        SDL_BlitSurface(john, NULL, scr, &r);
//                        SDL_BlitScaled(two2, NULL, scr, &twoRect2);
//                        SDL_BlitScaled(two, NULL, scr, &twoRect);
//
//                        SDL_UpdateWindowSurface(win);
//
//                    }
//
//
////                    for (int i = 0; i < 5; i++) {
////                        twoRect.h -= 1;
////                        twoRect.w -= 1;
////
////                        twoRect.x += 0.5;
////                        twoRect.y += 0.5;
////
////                        SDL_BlitSurface(john, NULL, scr, &r);
////                        SDL_BlitScaled(two, NULL, scr, &twoRect);
////
////                        SDL_UpdateWindowSurface(win);
////
////                    }
//
//
//
//                        SDL_BlitSurface(john, NULL, scr, &r);
//                        SDL_BlitScaled(two2, NULL, scr, &twoRect2);
//                        SDL_BlitScaled(two, NULL, scr, &twoRect);
//
//                        SDL_UpdateWindowSurface(win);
//
//
//                     SDL_BlitSurface(john, NULL, scr, &r);
//                        SDL_BlitScaled(four, NULL, scr, &fourRect);
//
//                        SDL_UpdateWindowSurface(win);
//
//
//                }
//                if (e.key.keysym.sym == SDLK_LEFT) {
//                    cout << x;
//                    for (int i = 0; i < 330; i++) {
//                        x -= 1;
//                        twoRect.x = x;
//
//                        SDL_BlitSurface(john, NULL, scr, &r);
//                        SDL_BlitScaled(two2, NULL, scr, &twoRect2);
//                        SDL_BlitSurface(two, NULL, scr, &twoRect);
//
//                        SDL_UpdateWindowSurface(win);
//                    }
//
//
//                }
//            }
//        }
//
////
//        SDL_FillRect(scr, NULL, SDL_MapRGB(scr -> format, 255, 255, 255));
////
//        SDL_BlitSurface(john, NULL, scr, &r);
//        SDL_BlitScaled(two, NULL, scr, &twoRect);
//        SDL_BlitScaled(two2, NULL, scr, &twoRect2);
//SDL_BlitScaled(four, NULL, scr, &fourRect);
////
//        SDL_UpdateWindowSurface(win);
//
//
//    }

    SDL_Delay(10000);

    return quit();
}
