// Copyright (c) 2016 Benjamin Bäumler

#include "ResourceManager.h"

namespace wyvern_engine {

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager() {}

void ResourceManager::UploadMeshData() {
  for (auto mesh : m_meshes) {
    if (!mesh.second->Usable()) {
      mesh.second->UploadMesh();
    }
  }
}

void ResourceManager::ClearMeshData() {
  for (auto mesh : m_meshes) {
    if (mesh.second->Usable()) {
      mesh.second->ClearMeshData();
    }
  }
}

void ResourceManager::Clear() {
  for (auto mesh : m_meshes) {
    if (mesh.second.unique()) {
      m_meshes.erase(mesh.first);
    }
  }
  for (auto texture : m_textures) {
    if (texture.second.unique()) {
      m_textures.erase(texture.first);
    }
  }
}

std::shared_ptr<rendering::Mesh> ResourceManager::LoadMesh(std::string path) {
  if (m_meshes.count(path) == 0) {
    m_meshes[path] = rendering::Mesh::MakeMeshData(path);
  }

  return m_meshes[path];
}

std::shared_ptr<rendering::Shader>
ResourceManager::LoadShader(std::string path) {
  if (m_shader.count(path) == 0) {
    m_shader[path] =
        std::make_shared<rendering::Shader>(path + ".vert", path + ".frag");
  }

  return m_shader[path];
}

std::shared_ptr<rendering::Texture>
ResourceManager::LoadTexture(std::string path) {
  if (m_textures.count(path) == 0) {
    m_textures[path] = rendering::Texture::MakeShared(path);
  }

  return m_textures[path];
}

} // namespace wyvern_engine