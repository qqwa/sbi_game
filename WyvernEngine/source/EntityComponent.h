// Copyright (c) 2016 Benjamin Bäumler

#ifndef WYVERNENGINE_ENTITYCOMPONENT_H
#define WYVERNENGINE_ENTITYCOMPONENT_H

namespace wyvern_engine {

class World;

class Entity;

/**
 * @brief EntityComponent serves as a container, which will be used by Entities.
 *
 * An EntityComponent should be used to store relative data for an entity. Like
 * a Transform or CollisionBox component. If the Component should execute game
 * logic, it should inherit from Script, or must provide an own way to call the
 * defined methods.
 *
 */
class EntityComponent {
protected:
  World *m_world;
  Entity *m_owner;

public:
  EntityComponent() {}

  virtual ~EntityComponent();

  friend class Entity;
};

/**
 * @brief Script is the base class for logic relevant components.
 *
 * Script provides a interface to define functions which will be executed by the
 * engine to the right time and doesn't require extra work other then creating a
 * Script-component and adding it to an entity.
 */
class Script : public EntityComponent {
public:
  Script() {}

  virtual ~Script();

  /**
   * @brief OnInit will be called if a script get registered to a world
   */
  virtual void OnInit() {}

  /**
   * @brief OnUpdate will be called for every registered Script if the world
   * calls RunScripts
   */
  virtual void OnUpdate() {}

  /**
   * @brief OnRemove will be called before the script get removed from the World
   */
  virtual void OnRemove() {}
};

} // namespace wyvern_engine

#endif // WYVERNENGINE_ENTITYCOMPONENT_H
