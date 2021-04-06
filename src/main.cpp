
#include "camera.hpp"
#include "light/directionalLight.hpp"
#include "light/pointLight.hpp"
#include "material/material.hpp"
// #include "material/deferredMaterial.hpp"
#include "mesh.hpp"
#include "model.hpp"
#include "renderer.hpp"

#include <chrono>
#include <GL/glew.h>
#include <memory>
#include <stdlib.h>

enum class PBRPreset {
    metallic, // metal = 1, roughness = 0
    glossy, // metal = 0, roughness = 0
    rough, // metal = 0, roughness = 1
    rough_metal // metal = 1, roughness = 1
};

constexpr float ONE_SECOND = 1000.0f;
constexpr float FPS = 60.0f;

constexpr uint32_t DEFAULT_WIDTH = 1600;
constexpr uint32_t DEFAULT_HEIGHT = 900;

int main(int argc, char* argv[]) {
    auto width = DEFAULT_WIDTH;
    auto height = DEFAULT_HEIGHT;

    if (argc == 3) {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
    }

    auto frameLength = 1.0f / FPS;

    auto aspect = static_cast<float>(width) / height;

    auto sun = std::make_shared<DirectionalLight>(
        glm::vec3(-3.0f, 1.0f, -3.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        0.5f,
        0.2f
    );

    auto sun2 = std::make_shared<DirectionalLight>(
        glm::vec3(3.0f, 1.0f, 3.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        0.5f,
        0.2f
    );

    auto camera = std::make_unique<Camera>(aspect, 45.0f, -8.000f, glm::vec3(0.0f, 0.0f, 0.0f));
    auto renderer = Renderer(width, height, std::move(camera));

    renderer.addLight(sun);
    renderer.addLight(sun2);

    std::shared_ptr<Mesh> sphereMesh = std::make_shared<Mesh>();
    sphereMesh->fromOBJ("assets/sphere.obj");

    std::unique_ptr<Material> sphereMaterial = std::make_unique<Material>(
        glm::vec3(0.13f, 0.53f, 0.83f),
        0.1f,
        32.0f
    );

    std::shared_ptr<Model> sphere = std::make_shared<Model>(sphereMesh, std::move(sphereMaterial));
    sphere->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    renderer.addModel(sphere);

    auto last = std::chrono::steady_clock::now();

    bool quit = false;

    bool mouseDown = false;
    while (!quit) {
        auto now = std::chrono::steady_clock::now();
        auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count();

        if (dt >= frameLength) {
            // handleEvents(quit);
            if (quit) {
                break;
            }

            // handle events

            SDL_Event e;

            while(SDL_PollEvent(&e) != 0) {
                if (
                    e.type == SDL_QUIT ||
                    (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
                ) {
                    quit = true;
                    break;
                } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                    mouseDown = true;
                } else if (e.type == SDL_MOUSEMOTION) {
                    int x = 0, y = 0;
                    if (mouseDown) {
                        SDL_GetRelativeMouseState(&x, &y);
                        renderer.updateCameraRotation(glm::vec3(-static_cast<float>(y) / 100.0f, -static_cast<float>(x) / 100.0f, 0.0f));
                    } else {
                        SDL_GetRelativeMouseState(&x, &y);
                    }
                } else if (e.type == SDL_MOUSEBUTTONUP) {
                    mouseDown = false;
                } else if (e.type == SDL_KEYUP) {
                }
            }

            if (quit) {
                break;
            }

            renderer.render();
            last = now;
        }

    }
}
