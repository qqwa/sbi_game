// Copyright (c) 2016 Benjamin Bäumler

#ifndef WYVERNENGINE_MESHDATA_H
#define WYVERNENGINE_MESHDATA_H

#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <string>
#include <vector>

namespace wyvern_engine {
namespace rendering {

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
};

/**
 * @brief MeshMetaData contains all necessary informations to draw a mesh
 */
struct MeshMetaData {
  GLuint vao;
  GLuint vbo;
  GLuint ibo;
  unsigned long indicesCount;
};

/**
 * @brief Mesh contains all necessary data for an mesh to transfer it to the GPU
 *
 * It is a modified version of the code which can be found on this website
 * http://learnopengl.com/#!Model-Loading/Mesh
 *
 */
class Mesh {
  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  bool m_data;
  bool m_usable;

  MeshMetaData m_mData;

public:
  typedef std::shared_ptr<Mesh> Ptr;

  Mesh(std::string path);

  ~Mesh();

  void UploadMesh();

  void ClearMeshData();

  bool Usable() const;

  MeshMetaData GetMetaData() const;

  static std::shared_ptr<rendering::Mesh>
  MakeMeshData(const std::string &object);
};

} // namespace rendering
} // namespace wyvern_engine

#endif // WYVERNENGINE_MESHDATA_H
