#include "Registry.h"

#include <algorithm>

Entity Hierarchy::parent() const {
    return parent_;
}

std::size_t Hierarchy::depth() const {
    return depth_;
}

const std::vector<Entity>& Hierarchy::children() const {
    return children_;
}

Entity Registry::create() {
    const Entity entity = entt::Registry<Entity>::create();
    assign<Hierarchy>(entity);
    assign<HierarchyRoot>(entity);
    return entity;
}

void Registry::destroy(Entity entity) {
    DEBUG_ASSERT(valid(entity));

    const auto& hierarchy = get<Hierarchy>(entity);
    if (hierarchy.parent_ != NullEntity) {
        std::vector<Entity>& children = get<Hierarchy>(hierarchy.parent_).children_;
        children.erase(std::find(children.begin(), children.end(), entity));
    }

    destroyExplicit(entity);
}

void Registry::setParent(Entity entity, Entity parent) {
    auto& hierarchy = get<Hierarchy>(entity);
    const Entity oldParent = hierarchy.parent_;

    if (hierarchy.parent_ != NullEntity) {
        std::vector<Entity>& children = get<Hierarchy>(hierarchy.parent_).children_;
        children.erase(std::find(children.begin(), children.end(), entity));
    } else if (parent != NullEntity) {
        remove<HierarchyRoot>(entity);
    }

    hierarchy.parent_ = parent;
    hierarchy.depth_ = 0;

    if (hierarchy.parent_ != NullEntity) {
        auto& parentHierarchy = get<Hierarchy>(hierarchy.parent_);
        updateDepth(entity, parentHierarchy.depth_ + 1);

        parentHierarchy.children_.push_back(entity);
    } else if (oldParent != NullEntity) {
        assign<HierarchyRoot>(entity);
    }
}

void Registry::destroyExplicit(Entity entity) {
    for (const Entity child : get<Hierarchy>(entity).children_) {
        destroyExplicit(child);
    }

    entt::Registry<Entity>::destroy(entity);
}

void Registry::updateDepth(Entity entity, std::size_t depth) {
    auto& hierarchy = get<Hierarchy>(entity);
    hierarchy.depth_ = depth;

    for (const Entity child : hierarchy.children_) {
        updateDepth(child, hierarchy.depth_ + 1);
    }
}
