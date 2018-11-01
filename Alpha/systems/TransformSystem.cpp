#include "TransformSystem.h"

#include "../core/Registry.h"

#include "../components/Transform.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

namespace TransformSystem {

void updateModelMatrix(Registry& registry, const Hierarchy& hierarchy, Transform& transform) {
    bool transformWasDirty = transform.dirty();
    if (transform.dirty()) {
        transform.setModelMatrix(glm::translate(transform.position())
                                 * glm::toMat4(glm::quat{transform.rotation()})
                                 * glm::scale(transform.scale()));
        transform.setDirty(false);
    }

    for (const Entity child : hierarchy.children()) {
        if (registry.has<Transform>(child)) {
            auto& childTransform = registry.get<Transform>(child);
            if (transformWasDirty) {
                childTransform.setDirty(true);
            }
            updateModelMatrix(registry, registry.get<Hierarchy>(child), childTransform);
        }
    }
}

void update(Registry& registry) {
    registry.view<Hierarchy, HierarchyRoot, Transform>().each(
        [&registry](Entity /*unused*/, const Hierarchy& hierarchy, const HierarchyRoot& /*unused*/,
                    Transform& transform) { updateModelMatrix(registry, hierarchy, transform); });
}

} // namespace TransformSystem
