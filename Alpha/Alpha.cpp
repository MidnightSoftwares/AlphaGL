#define _DEBUG

#include "Registry.h"

#include <iostream>

struct Test {
    int x;
};

int main(int /*argc*/, char* /*argv*/[]) {
    Registry registry;

    int e1 = registry.create();
    registry.attach<Test>(e1, 42);

    int e2 = registry.create();
    registry.attach<Test>(e2, 123);

    registry.setParent(e1, e2);

    registry.view<Test>([](int entity, int depth) {
        std::cout << "Entity: " << entity << " | Depth: " << depth << '\n';
    });

    return EXIT_SUCCESS;
}
