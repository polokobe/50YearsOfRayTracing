/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include "../asset/ImageRGBA.h"
#include "MyTexture.h"

namespace RTKit2 {

class Texture2D : public MyTexture {
 private:
  ImageRGBA::Ptr mImage;

 public:
  void setImage(const std::string& szAssetKey);

  virtual glm::vec3 sample(const glm::vec2& uv, const glm::vec3& p) override;
};

}  // namespace RTKit1
