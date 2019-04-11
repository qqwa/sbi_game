// Copyright (c) 2016 Benjamin Bäumler

#include "Helper.h"

#include <fstream>
#include <sstream>

namespace wyvern_engine {
namespace helper {

std::string loadFile(std::string path) {
  std::ifstream file;
  file.open(path);
  std::string line;
  std::stringstream ss;
  while (getline(file, line)) {
    ss << line << "\n";
  }
  file.close();
  return ss.str();
}

} // namespace helper
} // namespace wyvern_engine