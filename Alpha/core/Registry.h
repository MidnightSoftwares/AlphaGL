#pragma once

#include "Assert.h"
#include "Entity.h"

#include <entt/entity/prototype.hpp>
#include <entt/entity/registry.hpp>

#include <vector>

using Prototype = entt::Prototype<Entity>;

class Hierarchy {
public:
    Entity parent() const;
    std::size_t depth() const;
    const std::vector<Entity>& children() const;

private:
    Entity parent_{NullEntity};
    std::size_t depth_{};
    std::vector<Entity> children_;

private:
    friend class Registry;
};

struct HierarchyRoot {};

class Registry : public entt::Registry<Entity> {
public:
    Entity create();
    void destroy(Entity entity);

    template <typename Tag>
    void destroy(entt::tag_t /*unused*/);

    template <typename... Component, typename... Type>
    void destroy(Type... /*unused*/);

    void setParent(Entity entity, Entity parent);

private:
    void destroyExplicit(Entity entity);
    void updateDepth(Entity entity, std::size_t depth);
};

#include "Registry.inl"
