#ifndef __FILEREADER_H__
#define __FILEREADER_H__

#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h>

namespace FileReader
  {
  std::string readFile(const char* path)
  {
    std::fstream file(path);
    if(!file.is_open())
    {
      printf("Unable to open file: %s\n", path);
      std::string empty("");
      return empty;
    }

    std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return fileContent;
  }
}

#endif
