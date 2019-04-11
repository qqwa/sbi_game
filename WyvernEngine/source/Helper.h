// Copyright (c) 2016 Benjamin Bäumler

#ifndef WYVERNENGINE_HELPER_H
#define WYVERNENGINE_HELPER_H

#include "rendering/Mesh.h"
#include <GL/glew.h>
#include <string>

namespace wyvern_engine {
namespace helper {
/**
 * @brief loadFile provides the content of a File as a string
 * @param path The relative or total path to the file
 * @return The content of the file
 */
std::string loadFile(std::string path);
} // namespace helper
} // namespace wyvern_engine

#endif // WYVERNENGINE_HELPER_H
