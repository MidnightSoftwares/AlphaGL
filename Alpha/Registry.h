#pragma once

#include "Assert.h"

#include <any>
#include <functional>
#include <optional>
#include <queue>
#include <vector>

class Registry {
private:
    struct EntityState {
    public:
        bool isAlive{};
        int parent{-1};
        int depth{};
        std::vector<int> children;
    };

public:
    int create();
    bool contains(int entity) const;
    int parent(int entity) const;
    void setParent(int entity, int parent);
    int depth(int entity) const;
    const std::vector<int>& children(int entity) const;
    void remove(int entity);

    template <typename TComponent, typename... TArgs>
    TComponent& attach(int entity, TArgs&&... args);

    template <typename TComponent>
    bool has(int entity) const;

    template <typename TComponent>
    TComponent& get(int entity);

    template <typename TComponent>
    const TComponent& get(int entity) const;

    template <typename TComponent>
    void detach(int entity);

    void reset(int entity);

    template <typename... TComponents>
    void view(const std::function<void(int entity, int depth)>& func) const;

    template <typename... TComponents>
    void view(const std::function<void(int entity)>& func) const;

    void clear();

private:
    template <typename TComponent>
    size_t componentId() const;

private:
    int nextEntity_{};
    mutable size_t nextComponentId_{};
    std::priority_queue<int, std::vector<int>, std::greater<>> availableEntities_;
    // [entity] => state
    std::vector<EntityState> entities_;
    // [depth][componentId][entity] => component
    std::vector<std::vector<std::vector<std::optional<std::any>>>> components_;
};

#include "Registry.inl"
