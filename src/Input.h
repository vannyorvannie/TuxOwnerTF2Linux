#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <SDL2/SDL.h>

#include "SDK.h" // why not

class CInput
{
public:
bool isSpaceBarPressed(Display *display);
bool isMouseButtonPressed(Display *display, int button);
bool InArea( int x, int y, int w, int h, int mouse_x, int mouse_y );
bool isLeftMouseButtonPressed();
void ProcessEvents();
};

extern CInput gInput;