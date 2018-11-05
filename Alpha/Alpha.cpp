#include "core/Registry.h"
#include "core/Window.h"

#include "components/Transform.h"

#include "systems/TransformSystem.h"

#include <GL/glew.h>

int main(int /*argc*/, char* /*argv*/[]) {
    // Window init
    GLContextDescription glContextDescription{4, 6, true, false, 24, 8, 4};
    DEBUG_ONLY(glContextDescription.debug = true);
    Window window{WindowDescription{glContextDescription, "Alpha",
                                    glm::ivec2{SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
                                    glm::ivec2{1280, 720}}};

    // GL init
    glClearColor(1.f, 0.f, 1.f, 1.f);

    // Application init
    Registry registry;

    const Entity e1 = registry.create();
    registry.assign<Transform>(e1);

    const Entity e2 = registry.create();
    registry.setParent(e2, e1);
    registry.assign<Transform>(e2);

    const Entity e3 = registry.create();
    registry.setParent(e3, e2);
    registry.assign<Transform>(e3);

    // Application loop
    bool quit{};
    while (!quit) {
        // SDL events
        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent) != 0) {
            switch (sdlEvent.type) {
                case SDL_QUIT:
                    quit = true;
                    break;

                case SDL_WINDOWEVENT:
                    window.handleEvent(sdlEvent.window);
                    break;

                case SDL_KEYDOWN:
                    if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
                        quit = true;
                    }
                    break;

                default:
                    break;
            }
        }

        // Fixed update
        TransformSystem::update(registry);

        // Frame update
        glClear(GL_COLOR_BUFFER_BIT);

        //

        window.swapBuffers();
    }

    return EXIT_SUCCESS;
}
