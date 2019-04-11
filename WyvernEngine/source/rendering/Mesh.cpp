// Copyright (c) 2016 Benjamin Bäumler

#include "Mesh.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

namespace wyvern_engine {
namespace rendering {

Mesh::Mesh(std::string path) : m_data(true), m_usable(false) {

  std::ifstream obj(path);
  if (!obj.is_open()) {
    throw std::runtime_error("Couldn't open " + path);
  }

  std::vector<glm::vec3> v;
  std::vector<glm::vec2> vt;
  std::vector<glm::vec3> vn;
  std::vector<int> f; // position, texture, normal
  std::string line;
  while (!getline(obj, line).eof()) {
    if (line.find("v ") == 0) {
      line = line.substr(2);
      std::size_t delimiter1 = line.find(" ");
      std::size_t delimiter2 = line.find_last_of(" ");
      std::string x = line.substr(0, delimiter1);
      std::string y = line.substr(delimiter1 + 1, delimiter2 - delimiter1);
      std::string z = line.substr(delimiter2);
      v.push_back({std::stof(x), std::stof(y), std::stof(z)});
    } else if (line.find("vt ") == 0) {
      line = line.substr(3);
      std::size_t delimiter1 = line.find(" ");
      std::string x = line.substr(0, delimiter1);
      std::string y = line.substr(delimiter1 + 1);
      vt.push_back({std::stof(x), std::stof(y)});
    } else if (line.find("vn ") == 0) {
      line = line.substr(3);
      std::size_t delimiter1 = line.find(" ");
      std::size_t delimiter2 = line.find_last_of(" ");
      std::string x = line.substr(0, delimiter1);
      std::string y = line.substr(delimiter1 + 1, delimiter2 - delimiter1);
      std::string z = line.substr(delimiter2);
      vn.push_back({std::stof(x), std::stof(y), std::stof(z)});
    } else if (line.find("f ") == 0) {
      line = line.substr(2);
      auto count = std::count(line.begin(), line.end(), '/');
      if (line.find("//") != std::string::npos) { // vertex and normal
        // f v/t/n v/t/n ...

        int i = 0;
        std::size_t pos = 0;
        while (pos != std::string::npos) {
          pos = line.find(' ');
          std::string tmp;
          if (pos == std::string::npos) {
            tmp = line;
          } else {
            tmp = line.substr(0, pos);
            line = line.substr(pos + 1);
          }

          std::size_t delimiter1 = tmp.find("//");
          std::string v = tmp.substr(0, delimiter1);
          std::string n = tmp.substr(delimiter1 + 2);
          f.push_back(std::stoi(v));
          f.push_back(-1); // no texture
          f.push_back(std::stoi(n));

          i++;
        }
        if (i != 3) {
          throw std::runtime_error("Mesh is not triangulated " + path);
        }

      } else if (count == 6) { // vertex, texture, normal
        int i = 0;
        std::size_t pos = 0;
        while (pos != std::string::npos) {
          pos = line.find_first_of(' ');
          std::string tmp;
          if (pos == std::string::npos) {
            tmp = line;
          } else {
            tmp = line.substr(0, pos);
            line = line.substr(pos + 1);
          }

          std::size_t delimiter1 = tmp.find("/");
          std::size_t delimiter2 = tmp.find_last_of("/");
          std::string v = tmp.substr(0, delimiter1);
          std::string t =
              tmp.substr(delimiter1 + 1, delimiter2 - delimiter1 - 1);
          std::string n = tmp.substr(delimiter2 + 1);
          f.push_back(std::stoi(v));
          f.push_back(std::stoi(t));
          f.push_back(std::stoi(n));

          i++;
        }

      } else {
        obj.close();
        throw std::runtime_error(
            "Mesh " + path + "doesn't contain normals or is not triangulated");
      }
    }
  }

  obj.close();

  // create mesh
  if (f.size() % 9 != 0) { // 9 ints per face/triangle
    throw std::runtime_error("Mesh " + path +
                             " has insufficient amount of faces");
  }

  for (unsigned int i = 0; i < f.size(); i += 3) {
    glm::vec3 position = v[f[i] - 1];
    glm::vec2 texture;
    if (f[i + 1] == -1) {
      texture = {0.0f, 0.0f};
    } else {
      texture = vt[f[i + 1] - 1];
    }
    glm::vec3 normal = vn[f[i + 2] - 1];

    Vertex vertex = {position, normal, glm::vec2(texture.x, texture.y * -1)};
    int pos = -1;
    int x = 0;
    // really inefficient, specially for large meshes
    for (auto m_vertex : m_vertices) {
      if (vertex.position == m_vertex.position &&
          vertex.normal == m_vertex.normal &&
          vertex.texCoords == m_vertex.texCoords) {
        pos = x;
        break;
      }
      x++;
    }

    if (pos == -1) {
      m_vertices.push_back(vertex);
      m_indices.push_back(static_cast<unsigned int>(m_vertices.size() - 1));
    } else {
      m_indices.push_back(pos);
    }
  }
}

Mesh::~Mesh() {
  if (m_usable) {
    glDeleteVertexArrays(1, &m_mData.vao);
    glDeleteBuffers(1, &m_mData.vbo);
    glDeleteBuffers(1, &m_mData.ibo);
  }
}

void Mesh::UploadMesh() {
  if (!m_data) {
    throw std::runtime_error("Tried to upload mesh to gpu without mesh data");
  }

  glGenVertexArrays(1, &m_mData.vao);
  glGenBuffers(1, &m_mData.vbo);
  glGenBuffers(1, &m_mData.ibo);

  glBindVertexArray(m_mData.vao);
  glBindBuffer(GL_ARRAY_BUFFER, m_mData.vbo);
  glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex),
               m_vertices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mData.ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint),
               m_indices.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0); // position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);
  glEnableVertexAttribArray(1); // normal
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (GLvoid *)(sizeof(GLfloat) * 3));
  glEnableVertexAttribArray(2); // texCoord
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (GLvoid *)(sizeof(GLfloat) * 6));

  glBindVertexArray(0);

  m_mData.indicesCount = m_indices.size();

  m_usable = true;
}

void Mesh::ClearMeshData() {
  m_vertices.clear();
  m_indices.clear();
  m_vertices.shrink_to_fit();
  m_indices.shrink_to_fit();
  m_data = false;
}

bool Mesh::Usable() const { return m_usable; }

MeshMetaData Mesh::GetMetaData() const { return m_mData; }

std::shared_ptr<rendering::Mesh> Mesh::MakeMeshData(const std::string &object) {
  return std::make_shared<rendering::Mesh>(rendering::Mesh(object));
}

} // namespace rendering
} // namespace wyvern_engine