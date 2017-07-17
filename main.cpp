#include <stdio.h>
#include <iostream>
#include "util/SDLHandler.h"
#include "util/filereader.h"
#include "tinyxml/tinyxml.h"
#include "tinyxml/tinystr.h"
using namespace std;

const int WIDTH = 960, HEIGHT = 544;
SDL_Window* window;
SDL_GLContext context;

int main(int argc, char** args)
{
  TiXmlDocument doc("../data/config.xml");
  bool loadokay = doc.LoadFile();
  if(!loadokay)
  {
    printf("Unable to load xml.\n %s\n", doc.ErrorDesc());
    cin.get(); cin.ignore();
    return -1;
  }

  TiXmlElement* root = doc.RootElement();
  TiXmlElement* configElement = NULL;
  int count = 0;
  for(TiXmlElement* e = root->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
  {
    if(e->Value() == string("CONFIG"))
    {
      configElement = e;
    }
  }
  TiXmlElement* configData = configElement->FirstChildElement();
  int width = 0, height = 0;
  bool fullscreen = false;
  string fspath("");

  configData->Attribute("width", &width);
  configData->Attribute("height", &height);
  string isFullscreen = configData->Attribute("fullscreen");
  fspath = configData->Attribute("fragmentshaderpath");
  if(width != 0 && height != 0)
  {
    fullscreen = (isFullscreen == "true") ? true : false;
    fspath = (fspath == "") ? "../shaders/fs.fs" : fspath;
    SDLhandler::init("OpenGL Fragment Shader Lab", width, height, window, &context, fullscreen);
  }
  else
  {
    SDLhandler::init("OpenGL Fragment Shader Lab", WIDTH, HEIGHT, window, &context);
  }

  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  const GLchar* vssource = { "#version 150 core\n in vec2 position;\n uniform float time;\n void main() {\n gl_Position = vec4(position, 0.0, 1.0);  }"};
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
  string fsst = FileReader::readFile(fspath.c_str());
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
