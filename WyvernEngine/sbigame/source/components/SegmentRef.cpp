// Copyright (c) 2016 Benjamin Bäumler

#include "SegmentRef.h"
#include <iostream>

SegmentRef::SegmentRef(std::vector<wyvern_engine::Entity *> segments,
                       float startPoint, float endPoint, float segmentLength)
    : m_start(startPoint), m_end(endPoint), m_segLength(segmentLength),
      m_segments(segments) {}

std::vector<wyvern_engine::Entity *> SegmentRef::GetSegments(float z) {
  std::vector<wyvern_engine::Entity *> result;

  auto pos = m_start - z;
  //    auto pos = (z + 5);
  auto seg = static_cast<int>(pos / m_segLength);

  if (seg == 0) {
    result.push_back(m_segments[0]);
    result.push_back(m_segments[1]);
  } else if (seg == m_segments.size() - 1) {
    result.push_back(m_segments[m_segments.size() - 1]);
    result.push_back(m_segments[m_segments.size() - 2]);
  } else {
    result.push_back(m_segments[seg - 1]);
    result.push_back(m_segments[seg]);
    result.push_back(m_segments[seg + 1]);
  }

  return result;
}
