#include "Registry.h"

#include <algorithm>

int Registry::create() {
    int entity;

    if (!availableEntities_.empty()) {
        entity = availableEntities_.top();
        availableEntities_.pop();
    } else {
        entity = nextEntity_++;
        entities_.emplace_back();
    }

    EntityState& state = entities_[entity];

    state.isAlive = true;

    if (state.depth >= components_.size()) {
        components_.resize(state.depth + 1);
    }

    return entity;
}

bool Registry::contains(int entity) const {
    return entity >= 0 && entity < entities_.size() && entities_[entity].isAlive;
}

int Registry::parent(int entity) const {
    DEBUG_ASSERT(contains(entity));

    return entities_[entity].parent;
}

// NOTE: Possible issues:
// - depth=1, setParent(parentDepth=2), depth=2 => will be processed twice
// - depth=1, childrenDepth=2, removeParent, depth=0, childrenDepth=1
//     => children will not be processed
//        (for example a child with an "id" lower than current entity)
void Registry::setParent(int entity, int parent) {
    DEBUG_ASSERT(contains(entity));

    EntityState& state = entities_[entity];

    if (state.parent != -1) {
        std::vector<int>& parentChildren = entities_[state.parent].children;
        parentChildren.erase(std::find(parentChildren.begin(), parentChildren.end(), entity));
    }

    const int oldDepth = state.depth;

    state.parent = parent;
    state.depth = 0;

    if (state.parent != -1) {
        DEBUG_ASSERT(contains(state.parent));

        EntityState& parentState = entities_[state.parent];
        parentState.children.push_back(entity);

        state.depth = parentState.depth + 1;
    }

    if (state.depth != oldDepth) {
        if (state.depth >= components_.size()) {
            components_.resize(state.depth + 1);
        }

        auto& oldDepthComponents = components_[oldDepth];
        auto& depthComponents = components_[state.depth];

        if (oldDepthComponents.size() > depthComponents.size()) {
            depthComponents.resize(oldDepthComponents.size());
        }

        for (size_t cId{}; cId < oldDepthComponents.size(); ++cId) {
            auto& oldCIdComponents = oldDepthComponents[cId];
            auto& cIdComponents = depthComponents[cId];

            if (entity >= cIdComponents.size()) {
                cIdComponents.resize(entity + 1);
            }

            cIdComponents[entity] = oldCIdComponents[entity];
            oldCIdComponents[entity].reset();
        }
    }

    // TODO: Implement a more optimized way of changing children depth
    for (const int child : state.children) {
        setParent(child, entity);
    }
}

int Registry::depth(int entity) const {
    DEBUG_ASSERT(contains(entity));

    return entities_[entity].depth;
}

const std::vector<int>& Registry::children(int entity) const {
    DEBUG_ASSERT(contains(entity));

    return entities_[entity].children;
}

void Registry::remove(int entity) {
    DEBUG_ASSERT(contains(entity));

    EntityState& state = entities_[entity];

    for (const int child : state.children) {
        remove(child);
    }

    if (state.parent != -1) {
        std::vector<int>& parentChildren = entities_[state.parent].children;
        parentChildren.erase(std::find(parentChildren.begin(), parentChildren.end(), entity));
    }

    auto& depthComponents = components_[state.depth];
    for (size_t cId{}; cId < depthComponents.size(); ++cId) {
        auto& cIdComponents = depthComponents[cId];
        if (entity < cIdComponents.size()) {
            cIdComponents[entity].reset();
        }
    }

    state.isAlive = false;
    state.parent = -1;
    state.depth = 0;
    state.children.clear();

    availableEntities_.push(entity);
}

void Registry::reset(int entity) {
    DEBUG_ASSERT(contains(entity));

    auto& depthComponents = components_[entities_[entity].depth];
    for (size_t cId{}; cId < depthComponents.size(); ++cId) {
        auto& cIdComponents = depthComponents[cId];
        if (entity < cIdComponents.size()) {
            cIdComponents[entity].reset();
        }
    }
}

void Registry::clear() {
    nextEntity_ = 0;
    availableEntities_ = {};
    entities_.clear();
    components_.clear();
}
