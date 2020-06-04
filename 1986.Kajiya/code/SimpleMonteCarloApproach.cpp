#include "SimpleMonteCarloApproach.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <random>

#include "MaterialBase.h"
#include "framework/PinholeCamera.h"

namespace RayTracingHistory {
constexpr float FLOAT_MAX = std::numeric_limits<float>::max();
constexpr int BOUNCES = 100;
constexpr int MAX_DEPTH = 4;

void SimpleMonteCarloApproach::_tileRenderThread(Tile tile, MyScene::Ptr scene,
                                                 MyCamera::Ptr camera) {
  PinholeCamera* pCamera = static_cast<PinholeCamera*>(camera.get());
  MyScene* pScene = scene.get();

  std::random_device randDevice;
  std::mt19937 stdRand(randDevice());
  std::uniform_real_distribution<float> uniformDist(0, 1);

  int W = mFrameWidth;
  int H = mFrameHeight;

  for (int y = tile.top; y < tile.bottom; y++)
    for (int x = tile.left; x < tile.right; x++) {
      if (!mRuning) break;

      Ray primaryRay =
          pCamera->generateViewingRay((x + 0.5f) / W, (y + 0.5f) / H);

      glm::vec3 color = _traceRay(primaryRay, scene.get(), 0);

      _writePixel(x, y, glm::vec4(color, 1), 1.0f);
      mPixelCount++;
    }  // end of for(x)
}

glm::vec3 SimpleMonteCarloApproach::_traceRay(Ray ray, MyScene* pScene,
                                              int depth) {
  const glm::vec3 bgColor(0.f, 0.f, 0.f);
  if (depth > MAX_DEPTH) return bgColor;

  HitRecord hitRec;
  bool bHit = pScene->closestHit(ray, 0.01f, FLOAT_MAX, hitRec);
  if (!bHit) return bgColor;

  return _shade(ray.direction, hitRec, pScene, depth);
}

glm::vec3 SimpleMonteCarloApproach::_shade(const glm::vec3& dir,
                                           const HitRecord& shadingPoint,
                                           MyScene* pScene, int depth) {
  MaterialBase* pMtl = static_cast<MaterialBase*>(shadingPoint.mtl);

  // error check
  if (!pMtl) return glm::vec3(1, 0, 0);

  if (pMtl->getEmission() > 0.001f) {
    // hit a light
    return pMtl->getEmission() *
           pMtl->getBaseColor(shadingPoint.uv, shadingPoint.p);
  }

  // Monte Carlo Estimating
  glm::vec3 sum(0);

  for (int i = 0; i < BOUNCES; i++) {
    glm::vec3 bounceDir = pMtl->scatter(shadingPoint.normal);
    float pdf = pMtl->pdf(bounceDir, shadingPoint.normal);
    float cosine = std::max(0.0f, glm::dot(bounceDir, shadingPoint.normal));
    if (pdf > 0) {
      Ray ray(shadingPoint.p, bounceDir);
      sum += _traceRay(ray, pScene, depth + 1) * cosine / pdf;
    }
  }

  return sum / (float)BOUNCES;
}

}  // namespace RayTracingHistory