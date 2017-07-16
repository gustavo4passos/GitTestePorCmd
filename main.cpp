#include "stdio.h"
#include <SDL2/SDL.h>

using namespace std;

SDL_Window* window;
SDL_GLContext context;

bool init()
{
  if( SDL_Init(SDL_INIT_VIDEO) < 0 )
  {

  }
  else
  {
    printf("Unable to initialize SDL. Error: %s\n", SDL_GetError());
    return false;
  }
}

int main(int argc, char** args)
{

  return 0;
}
