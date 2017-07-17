#include <stdio.h>
#include "util/SDLHandler.h"
#include "util/filereader.h"
using namespace std;

const int WIDTH = 960, HEIGHT = 544;
SDL_Window* window;
SDL_GLContext context;

int main(int argc, char** args)
{
  SDLhandler::init("Git I love You", WIDTH, HEIGHT, window, &context);

  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  string vsst = FileReader::readFile("../shaders/vs.vs");
  const GLchar* vssource = vsst.c_str();
  glShaderSource(vs, 1, &vssource, NULL);
  GLint vsstatus;
  glCompileShader(vs);
  glGetShaderiv(vs, GL_COMPILE_STATUS, &vsstatus);
  if(vsstatus != GL_TRUE)
  {
    GLchar buffer[512];
    glGetShaderInfoLog(vs, 512, NULL, buffer);
    printf("Unable to compile vertex shader.\n%s\n", buffer);
  }
  else
  {
    printf("Vertex shader successfully compiled.\n");
  }

  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  string fsst = FileReader::readFile("../shaders/fs.fs");
  const GLchar* fssource = fsst.c_str();
  glShaderSource(fs, 1, &fssource, NULL);
  GLint fsstatus;
  glCompileShader(fs);
  glGetShaderiv(vs, GL_COMPILE_STATUS, &fsstatus);
  if(fsstatus != GL_TRUE)
  {
    GLchar buffer[512];
    glGetShaderInfoLog(fs, 512, NULL, buffer);
    printf("Unable to compile vertex shader.\n%s\n", buffer);
  }
  else
  {
    printf("Fragment shader successfully compiled.\n");
  }

  GLuint program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glUseProgram(program);

  GLuint vao, vbo;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  GLfloat vertices[] = {
    -1.f,  1.f,
     1.f,  1.f,
     1.f, -1.f,
    -1.f, -1.f
  };
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  GLuint vertexattrib = glGetAttribLocation(program, "position");
  glVertexAttribPointer(vertexattrib, 2, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(vertexattrib);

  GLuint timeuniform = glGetUniformLocation(program, "time");
  GLuint resolutionuniform = glGetUniformLocation(program, "resolution");
  GLuint mouseuniform = glGetUniformLocation(program, "mouse");

  GLenum err = glGetError();
  if(err != GL_NO_ERROR)
  {
    printf("GL error: %s\n", gluErrorString(err));
  }

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

      int mousex, mousey;
      SDL_GetMouseState(&mousex, &mousey);

      GLfloat time = SDL_GetTicks() / 1000.f;
      glUniform1f(timeuniform, time);
      glUniform2f(resolutionuniform, float(WIDTH), float(HEIGHT));
      glUniform2f(mouseuniform, float(mousex), float(mousey));
      glClear(GL_COLOR_BUFFER_BIT);
      glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
      SDL_GL_SwapWindow(window);
  }

  return 0;
}
