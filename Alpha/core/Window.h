#pragma once

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>

#include <glm/vec2.hpp>

struct GLContextDescription {
    int versionMajor;
    int versionMinor;
    bool coreProfile;
    bool debug;
    int depthBits;
    int stencilBits;
    int samples;
};

struct WindowDescription {
    GLContextDescription context;
    const char* title;
    glm::ivec2 position;
    glm::ivec2 size;
};

class Window {
public:
    Window(const WindowDescription& description);

    Window(const Window& other) = delete;
    Window(Window&& other) = default;

    Window& operator=(const Window& other) = delete;
    Window& operator=(Window&& other) = default;

    virtual ~Window();

public:
    bool shown() const;
    void show();
    void hide();

    bool minimized() const;
    void minimize();

    bool maximized() const;
    void maximize();

    void restore();

    bool focused() const;
    void focus();

    bool mouseFocused() const;

    const char* title() const;
    void setTitle(const char* title);

    const glm::ivec2& position() const;
    void setPosition(const glm::ivec2& position);

    const glm::ivec2& size() const;
    const glm::ivec2& contextSize() const;
    void setSize(const glm::ivec2& size);

    void swapBuffers();

    void handleEvent(const SDL_WindowEvent& event);

private:
    SDL_Window* window_;
    SDL_GLContext context_;
    // fullscreen
    bool shown_;
    // borderless
    // resizable
    bool minimized_;
    bool maximized_;
    bool focused_;
    bool mouseFocused_;
    // alwaysOnTop
    // skipTaskbar
    const char* title_;
    glm::ivec2 position_{};
    glm::ivec2 size_{};
    glm::ivec2 contextSize_{};
};
