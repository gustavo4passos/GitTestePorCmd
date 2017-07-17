#include "stdio.h"
#include <SDL2/SDL.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

using namespace std;

const int WIDTH = 640, HEIGHT = 480;
SDL_Window* window;
SDL_GLContext context;

  bool init(const char* title, int windowWidth, int windowHeight, SDL_GLContext* glContext)
{
  //Check if SDL was initialized correctly (SDL_Init returns a negative value in case it was not able to initialize SDL)
  if( SDL_Init(SDL_INIT_VIDEO) < 0 )
  {
    printf("Unable to initialize SDL. Error: %s\n", SDL_GetError());
    return false;
  }
  else
  {
    printf("SDL successfully initialized.\n");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_OPENGL);

    if(window == NULL)
    {
      printf("Unable to create window. Error:%s\n", SDL_GetError());
    }
    else
    {
      *glContext = SDL_GL_CreateContext(window);

      if(context == NULL)
      {
        printf("Unable to create OpenGL context from SDL. Error: %s", SDL_GetError());
      }
      else
      {
        glewExperimental = GL_TRUE;
        GLenum glewErr = glewInit();

        if(glewErr != GLEW_NO_ERROR)
        {
          printf("Unable to initialize Glew. Error: %s\n", glewGetErrorString(glewErr));
          return false;
        }
        else
        {
          glClearColor(0.f, 1.f, 0.5f, 1.f);
        }
      }
    }
  }
  return true;
}

int main(int argc, char** args)
{
  init("Git I love You", WIDTH, HEIGHT, &context);

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
