#pragma once

class Registry;
class Hierarchy;
class Transform;

class TransformSystem {
public:
    TransformSystem() = delete;

public:
    static void update(Registry& registry);

private:
    static void updateModelMatrix(Registry& registry, const Hierarchy& hierarchy,
                                  Transform& transform);
};
