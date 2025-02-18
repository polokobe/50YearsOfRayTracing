#include <iostream>

#include "BoxScene.h"
#include "DemoScene.h"
#include "TestScene.h"
#include "WhittedRayTracer.h"
#include "framework/MyApp.h"
#include "framework/PinholeCamera.h"

using namespace RTKit1;

const char* const APP_NAME = "Recursive Ray Tracing - Turner Whitted 1980";
const uint32_t WINDOW_WIDTH = 640;
const uint32_t WINDOW_HEIGHT = 360;

int main(int argc, char* argv[]) {
  MyApp app;
  try {
    app.init();
    app.createWindow(1280, 720, APP_NAME);

    auto renderer = app.createRenderer<WhittedRayTracer>();
#if 0
    glm::vec3 eyePos(0, 2, -8);
    glm::vec3 lookAt(0, 1, 0);
    auto scene = std::make_shared<DemoScene>();
#else
    glm::vec3 eyePos(0, 3, -6);
    glm::vec3 lookAt(0, 1, 0);
    auto scene = std::make_shared<TestScene>();
#endif
    scene->init();

    auto camera = std::make_shared<PinholeCamera>();
    camera->setAspect((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT)
        .setView(eyePos, lookAt, glm::vec3(0, 2, 0))
        .setFOV(45);
    camera->init();

    renderer->renderScene(scene, camera, glm::vec4(0, 0, 0, 1));

    app.mainLoop();

  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }

  app.shutdown();
  return 0;
}
