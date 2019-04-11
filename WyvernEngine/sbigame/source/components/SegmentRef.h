// Copyright (c) 2016 Benjamin Bäumler

#ifndef WYVERNENGINE_SEGMENTREF_H
#define WYVERNENGINE_SEGMENTREF_H

#include <Entity.h>
#include <EntityComponent.h>
#include <vector>

class SegmentRef : public wyvern_engine::EntityComponent {
  float m_start;
  float m_end;
  float m_segLength;
  std::vector<wyvern_engine::Entity *> m_segments;

public:
  SegmentRef(std::vector<wyvern_engine::Entity *> segments, float startPoint,
             float endPoint, float segmentLength);

  std::vector<wyvern_engine::Entity *> GetSegments(float z);

  std::vector<wyvern_engine::Entity *> Segments() { return m_segments; };
};

#endif // WYVERNENGINE_SEGMENTREF_H
