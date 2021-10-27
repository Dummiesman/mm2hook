#pragma once

namespace MM2
{
    const auto SegmentToSphereIntersections = (int(*)(Vector3 const& start, Vector3 const& dir, float sqRadius, float* normalizedDist, float* a5))(0x4BBD50);
}