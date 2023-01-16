#include<iostream>
#include "Game.h"

int main()
{
    //Init srand
    srand(static_cast<unsigned>(time(0)));

    //Init Game Engine
    Game game;

    //Game loop
    while (game.running() && !game.getEndGame())
    {
        //Update
        game.update();

        //Render
        game.render();

    }

    //End of app
    return 0;
}
