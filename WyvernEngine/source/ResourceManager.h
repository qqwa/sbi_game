// Copyright (c) 2016 Benjamin Bäumler

#ifndef WYVERNENGINE_RESOURCEMANAGER_H
#define WYVERNENGINE_RESOURCEMANAGER_H

#include "rendering/Mesh.h"
#include "rendering/Shader.h"
#include "rendering/Texture.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace wyvern_engine {

/**
 * @brief The ResourceManager has the task to load resources from the hard drive
 *
 * ResourceManager furthermore keeps the data in memory unless Clear() is called
 */
class ResourceManager {
  std::unordered_map<std::string, std::shared_ptr<rendering::Mesh>> m_meshes;
  std::unordered_map<std::string, std::shared_ptr<rendering::Texture>>
      m_textures;
  std::unordered_map<std::string, std::shared_ptr<rendering::Shader>> m_shader;

public:
  typedef std::shared_ptr<ResourceManager> Ptr;

  ResourceManager();

  ~ResourceManager();

  /**
   * @brief LoadMesh loads the provided Mesh into memory
   * @param path The relative or total path to the mesh. E.g. "res/model.obj"
   * @return The Mesh object to access and use the mesh
   */
  std::shared_ptr<rendering::Mesh> LoadMesh(std::string path);

  /**
   * @brief LoadShader loads and compile the provided shader
   * @param path The relative or total path to the the two shader files. E.g.
   * "res/shader" to load "res/shader.vert" and "res/shader.frag"
   * @return The Shader object to access and use the shader
   */
  std::shared_ptr<rendering::Shader> LoadShader(std::string path);

  /**
   * @brief LoadTexture loads the provided texture and uploads it to the gpu
   * @param path The relative or total path to the texture. E.g "res/image.png"
   * @return The Texture object to access and use the texture
   */
  std::shared_ptr<rendering::Texture> LoadTexture(std::string path);

  /**
   * @brief UploadMeshData loads the Mesh Data to the GPU so that it can be
   * rendern.
   */
  void UploadMeshData();

  /**
   * @brief ClearMeshData clears the data which has already been uploaded to the
   * GPU
   */
  void ClearMeshData();

  /**
   * @brief Clear deletes all Objects that are just referenced by the
   * ResourceManager
   */
  void Clear();
};

} // namespace wyvern_engine

#endif // WYVERNENGINE_RESOURCEMANAGER_H
