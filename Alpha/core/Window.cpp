#include "Window.h"

#include "Assert.h"

#include <SDL2/SDL.h>

#include <GL/glew.h>

Window::Window(const WindowDescription& description) {
    // SDL init
    static bool sdlInitialized{false};

    if (!sdlInitialized) {
        const int sdlInit = SDL_Init(SDL_INIT_VIDEO);
        ASSERT(sdlInit >= 0);

        std::atexit(SDL_Quit);

        sdlInitialized = true;
    }

    // GL attribures
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, description.context.versionMajor);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, description.context.versionMinor);
    if (description.context.coreProfile) {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    }
    if (description.context.debug) {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    }
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, description.context.depthBits);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, description.context.stencilBits);
    if (description.context.samples > 0) {
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, description.context.samples);
    }

    // Window creation
    window_ = SDL_CreateWindow(description.title, description.position.x, description.position.y,
                               description.size.x, description.size.y,
                               SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    ASSERT(window_);

    // Context creation
    context_ = SDL_GL_CreateContext(window_);
    ASSERT(context_);

    // GLEW init
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    ASSERT(glewError == GLEW_OK);

    // Internal state init
    const Uint32 flags = SDL_GetWindowFlags(window_);
    shown_ = static_cast<bool>(flags & SDL_WINDOW_SHOWN);
    minimized_ = static_cast<bool>(flags & SDL_WINDOW_MINIMIZED);
    maximized_ = static_cast<bool>(flags & SDL_WINDOW_MAXIMIZED);
    focused_ = static_cast<bool>(flags & SDL_WINDOW_INPUT_FOCUS);
    mouseFocused_ = static_cast<bool>(flags & SDL_WINDOW_MOUSE_FOCUS);
    title_ = SDL_GetWindowTitle(window_);
    SDL_GetWindowPosition(window_, &position_.x, &position_.y);
    SDL_GetWindowSize(window_, &size_.x, &size_.y);
    SDL_GL_GetDrawableSize(window_, &contextSize_.x, &contextSize_.y);
}

Window::~Window() {
    SDL_GL_DeleteContext(context_);
    SDL_DestroyWindow(window_);
}

bool Window::shown() const {
    return shown_;
}

void Window::show() {
    SDL_ShowWindow(window_);
}

void Window::hide() {
    SDL_HideWindow(window_);
}

bool Window::minimized() const {
    return minimized_;
}

void Window::minimize() {
    SDL_MinimizeWindow(window_);
}

bool Window::maximized() const {
    return maximized_;
}

void Window::maximize() {
    SDL_MaximizeWindow(window_);
}

void Window::restore() {
    SDL_RestoreWindow(window_);
}

bool Window::focused() const {
    return focused_;
}

void Window::focus() {
    SDL_SetWindowInputFocus(window_);
}

bool Window::mouseFocused() const {
    return mouseFocused_;
}

const char* Window::title() const {
    return title_;
}

void Window::setTitle(const char* title) {
    SDL_SetWindowTitle(window_, title);

    title_ = SDL_GetWindowTitle(window_);
}

const glm::ivec2& Window::position() const {
    return position_;
}

void Window::setPosition(const glm::ivec2& position) {
    SDL_SetWindowPosition(window_, position.x, position.y);

    // TODO(MidnightSoftwares): Check if necessary
    SDL_GetWindowPosition(window_, &position_.x, &position_.y);
}

const glm::ivec2& Window::size() const {
    return size_;
}

const glm::ivec2& Window::contextSize() const {
    return contextSize_;
}

void Window::setSize(const glm::ivec2& size) {
    SDL_SetWindowSize(window_, size.x, size.y);
}

void Window::swapBuffers() {
    SDL_GL_SwapWindow(window_);
}

void Window::handleEvent(const SDL_WindowEvent& event) {
    if (event.windowID != SDL_GetWindowID(window_)) {
        return;
    }

    switch (event.type) {
        case SDL_WINDOWEVENT_SHOWN:
            shown_ = true;
            break;

        case SDL_WINDOWEVENT_HIDDEN:
            shown_ = false;
            break;

        case SDL_WINDOWEVENT_EXPOSED:
            break;

        case SDL_WINDOWEVENT_MOVED:
            position_.x = event.data1;
            position_.y = event.data2;
            break;

        case SDL_WINDOWEVENT_RESIZED:
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            size_.x = event.data1;
            size_.y = event.data2;
            SDL_GL_GetDrawableSize(window_, &contextSize_.x, &contextSize_.y);
            break;

        case SDL_WINDOWEVENT_MINIMIZED:
            minimized_ = true;
            break;

        case SDL_WINDOWEVENT_MAXIMIZED:
            maximized_ = true;
            break;

        case SDL_WINDOWEVENT_RESTORED: {
            const Uint32 flags = SDL_GetWindowFlags(window_);
            minimized_ = static_cast<bool>(flags & SDL_WINDOW_MINIMIZED);
            maximized_ = static_cast<bool>(flags & SDL_WINDOW_MAXIMIZED);
            break;
        }

        case SDL_WINDOWEVENT_ENTER:
            mouseFocused_ = true;
            break;

        case SDL_WINDOWEVENT_LEAVE:
            mouseFocused_ = false;
            break;

        case SDL_WINDOWEVENT_FOCUS_GAINED:
            focused_ = true;
            break;

        case SDL_WINDOWEVENT_FOCUS_LOST:
            focused_ = false;
            break;

        case SDL_WINDOWEVENT_CLOSE:
            break;

        case SDL_WINDOWEVENT_TAKE_FOCUS:
            break;

        case SDL_WINDOWEVENT_HIT_TEST:
            break;

        default:
            break;
    }
}
