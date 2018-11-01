#include "core/Registry.h"

#include "components/Transform.h"

#include "systems/TransformSystem.h"

#include <SDL2/SDL.h>

#include <GL/glew.h>

int main(int /*argc*/, char* /*argv*/[]) {
    // SDL init
    const int sdlInit = SDL_Init(SDL_INIT_VIDEO);
    ASSERT(sdlInit >= 0);

    // OpenGL attribures
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    DEBUG_ONLY(SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG));
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    // Window creation
    SDL_Window* const window
        = SDL_CreateWindow("Alpha", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720,
                           SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    ASSERT(window);

    // Context creation
    SDL_GLContext context = SDL_GL_CreateContext(window);
    ASSERT(context);
    SDL_GL_SetSwapInterval(1);

    // GLEW init
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    ASSERT(glewError == GLEW_OK);

    // OpenGL init
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

        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
