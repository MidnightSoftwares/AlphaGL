template <typename Tag>
void Registry::destroy(entt::tag_t /*unused*/) {
    return has<Tag>() ? destroy(attachee<Tag>()) : void();
}

template <typename... Component, typename... Type>
void Registry::destroy(Type... /*unused*/) {
    for (const Entity entity : view<Component...>(Type{}...)) {
        destroy(entity);
    }
}
