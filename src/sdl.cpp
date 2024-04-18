#include "sdl.h"
#include "CMenu.h"
int h_PollEvent(SDL_Event* event) {
    int ret = ORIGINAL(PollEvent, event);

    if (ret) 
    {
        
        if (event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN) {
            int mouseX = event->motion.x;
            int mouseY = event->motion.y;

            gCheatMenu.HandleControls(event);

            gCheatMenu.cur_x = mouseX;
            gCheatMenu.cur_y = mouseY;
            //printf("Mouse X: %d, Mouse Y: %d\n", mouseX, mouseY);
        }
    }

    return ret;

}