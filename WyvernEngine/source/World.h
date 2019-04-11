// Copyright (c) 2016 Benjamin Bäumler

#ifndef WYVERNENGINE_ENTITYMANAGER_H
#define WYVERNENGINE_ENTITYMANAGER_H

#include "Entity.h"
#include "EntityComponent.h"
#include "Input.h"
#include "ResourceManager.h"
#include "rendering/RenderingSystem.h"
#include <algorithm>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace wyvern_engine {

/**
 * @brief World is the root of our scene graph.
 *
 * It manages all entities in his world, including there construction and
 * destruction.
 */
class World {
  ResourceManager::Ptr m_rm;
  Input *m_input;
  std::vector<std::unique_ptr<Entity>> m_entities;
  std::vector<Script *> m_scripts;
  std::unordered_map<const std::type_info *, std::size_t> m_component_count;
  float m_detlaTime;
  std::shared_ptr<nanogui::Screen> m_screen;
  GLuint m_framebuffer;
  GLuint m_texture;
  bool m_isCamera;
  rendering::RenderingSystem *m_renderingSystem;

public:
  World(ResourceManager::Ptr, Input *, int width, int height);

  ~World();

  ResourceManager *GetResourceManager();

  /**
   * @brief CreateEntity creates a new Entity, ready to use.
   * @return A new Entity managed by this World
   */
  Entity *CreateEntity();

  /**
   * @brief RemoveEntity removes entites from the running world
   * @param entity The specific entity and its children which should be removed
   * from the world
   */
  void RemoveEntity(const Entity *entity);

  /**
   * @brief GetEntites
   * @tparam components A list of components
   * @return A vector of Entites which contains at least all spezified
   * components
   */
  template <typename... components> std::vector<Entity *> GetEntities();

  /**
   * @brief GetEntites
   * @param group The name of the group
   * @return A vector of Entites from the the specified group
   */
  std::vector<Entity *> GetEntities(std::string group);

  /**
   * @brief RegisterScript registers a script, so that it can be runned by
   * RunScripts
   * @param script The script to register
   */
  void RegisterScript(Script *script);

  /**
   * @brief RemoveScript removes a script from the update list
   * @param script The script to remove
   */
  void RemoveScript(Script *script);

  /**
   * @brief Executes the Update function of all registered scripts
   */
  void RunScripts();

  /**
   * @brief Render renders the content of the current world to a texture
   * @return The Texture of the rendered world
   */
  GLuint Render();

  /**
   * @brief SetDeltaTime sets the DeltaTime, which will be used by all entites
   * in this world.
   * @param deltaTime The delta time
   */
  void SetDeltaTime(float deltaTime);

  /**
   * @brief DeltaTime
   * @return The current delta time of this world
   */
  float DeltaTime();

  /**
   * @brief GetScreen
   * @return The Screen of this world, which contains the GUI-Part
   */
  nanogui::Screen *GetScreen();

  /**
   * @brief GetInput gives the entites of the world access to Keyboard and Mouse
   * @return The Input from this world.
   */
  Input *GetInput();
};

template <typename... components> std::vector<Entity *> World::GetEntities() {
  std::vector<std::type_index> comp_list;
  comp_list.insert(comp_list.end(), {typeid(components)...});

  std::vector<Entity *> res;
  for (const auto &entity : m_entities) {
    bool hasComps = true;
    for (const auto &comp : comp_list) {
      if (!entity->HasComponent(comp)) {
        hasComps = false;
      }
    }

    if (hasComps) {
      res.push_back(entity.get());
    }
  }

  return res;
}

} // namespace wyvern_engine

#endif // WYVERNENGINE_ENTITYMANAGER_H
