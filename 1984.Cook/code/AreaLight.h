/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <algorithm>
#include <glm/glm.hpp>
#include <tuple>

#include "geometry/Ray.h"

namespace RTKit1 {

struct AreaLight {
  glm::vec3 edge1, edge2;
  glm::vec3 corner;

  float ambient = 0.1f;
  float intensity = 2.0f;
  float range = 10;

  std::tuple<Ray, float> jitteredShadowRay(const glm::vec3& shadingPt,
                                           const glm::vec2& xi) const {
    glm::vec3 pos = corner + xi.x * edge1 + xi.y * edge2;
    glm::vec3 L = pos - shadingPt;
    return std::make_tuple(Ray(shadingPt, L), glm::length(L));
  }

  float lighting(const glm::vec3& shadingPt, const glm::vec3& normal,
                 const glm::vec3& wo, const glm::vec2& xi,
                 float shininess) const {
    glm::vec3 pos = corner + xi.x * edge1 + xi.y * edge2;

    glm::vec3 L = glm::normalize(pos - shadingPt);
    glm::vec3 H = glm::normalize(L + wo);
    float NdotH = glm::dot(normal, H);
    float NdotL = glm::dot(normal, L);

    float falloff = 1.0f;
    float d = glm::distance(pos, shadingPt);
    if (d > range) {
      float r = range / d;
      falloff = r * r;
    }

    float diffuse = std::max(0.0f, NdotL) * falloff;
    float spec = glm::pow(std::max(0.0f, NdotH), shininess) * falloff;
    return intensity * (diffuse + spec);
  }

  AreaLight& setParallelogram(const glm::vec3& _edge1, const glm::vec3& _edge2,
                              const glm::vec3& _corner) {
    edge1 = _edge1;
    edge2 = _edge2;
    corner = _corner;
    return *this;
  }

  AreaLight& setRange(float val) {
    range = val;
    return *this;
  }

  AreaLight& setAmbient(float a) {
    ambient = a;
    return *this;
  }

  AreaLight& setIntensity(float i) {
    intensity = i;
    return *this;
  }
};

}  // namespace RTKit1
