#include "Input.h"

CInput gInput;

/*
* Credits: Dr.Coomer
*/
bool CInput::isSpaceBarPressed(Display *display) 
{
    char keys[32];
    XQueryKeymap(display, keys);

    KeyCode spaceKeyCode = XKeysymToKeycode(display, XK_space);
    return (keys[spaceKeyCode / 8] & (1 << (spaceKeyCode % 8))) != 0;
}
bool CInput::isLeftMouseButtonPressed()
{
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            return true;
        }
    }

    return false;
}


bool CInput::InArea( int x, int y, int w, int h, int mouse_x, int mouse_y )
{
	if ( mouse_x >= x && mouse_x <= x + w && mouse_y >= y && mouse_y <= y + h )
	{
		return true;
	}

	return false;
}
void CInput::ProcessEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0) {
        // Handle other events if needed
    }
}
