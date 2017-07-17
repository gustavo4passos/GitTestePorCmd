#ifndef __SDLHANDLER_H__
#define __SDLHANDLER_H__

#include <SDL2/SDL.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

namespace SDLhandler
{
  bool init(const char* title, int windowWidth, int windowHeight, SDL_Window*& window, SDL_GLContext* glContext, bool fullscreen = false)
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

      //Fullscreen flag
      Uint32 flags = (fullscreen) ? SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL : SDL_WINDOW_OPENGL;
      window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, flags);

      if(window == NULL)
      {
        printf("Unable to create window. Error:%s\n", SDL_GetError());
      }
      else
      {
        *glContext = SDL_GL_CreateContext(window);

        if(glContext == NULL)
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
}

#endif
