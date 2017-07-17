#include <stdio.h>
#include "util/SDLHandler.h"
using namespace std;

const int WIDTH = 640, HEIGHT = 480;
SDL_Window* window;
SDL_GLContext context;

int main(int argc, char** args)
{
  SDLhandler::init("Git I love You", WIDTH, HEIGHT, window, &context);

  bool running = true;
  while(running)
  {
      SDL_Event e;
      while(SDL_PollEvent(&e) != 0)
      {
        if(e.type == SDL_QUIT)
        {
          running = false;
        }
        else if(e.type == SDL_KEYUP)
        {
          switch (e.key.keysym.sym)
          {
            case SDLK_ESCAPE:
            running = false;
            break;

            default:
            break;
          }
        }
      }

      glClear(GL_COLOR_BUFFER_BIT);
      SDL_GL_SwapWindow(window);
  }

  return 0;
}
