template <typename TComponent, typename... TArgs>
TComponent& Registry::attach(int entity, TArgs&&... args) {
    DEBUG_ASSERT(!has<TComponent>(entity));

    auto& depthComponents = components_[entities_[entity].depth];

    const size_t cId = componentId<TComponent>();
    if (cId >= depthComponents.size()) {
        depthComponents.resize(cId + 1);
    }
    auto& cIdComponents = depthComponents[cId];

    if (entity >= cIdComponents.size()) {
        cIdComponents.resize(entity + 1);
    }
    std::optional<std::any>& component = cIdComponents[entity];
    component.emplace(TComponent{std::forward<TArgs>(args)...});
    return std::any_cast<TComponent&>(component.value());
}

template <typename TComponent>
bool Registry::has(int entity) const {
    DEBUG_ASSERT(contains(entity));

    auto& depthComponents = components_[entities_[entity].depth];

    const size_t cId = componentId<TComponent>();
    if (cId < depthComponents.size()) {
        auto& cIdComponents = depthComponents[cId];
        return entity < cIdComponents.size() && cIdComponents[entity].has_value();
    }
    return false;
}

template <typename TComponent>
TComponent& Registry::get(int entity) {
    DEBUG_ASSERT(has<TComponent>(entity));

    return std::any_cast<TComponent&>(
        components_[entities_[entity].depth][componentId<TComponent>()][entity].value());
}

template <typename TComponent>
const TComponent& Registry::get(int entity) const {
    DEBUG_ASSERT(has<TComponent>(entity));

    return std::any_cast<const TComponent&>(
        components_[entities_[entity].depth][componentId<TComponent>()][entity].value());
}

template <typename TComponent>
void Registry::detach(int entity) {
    DEBUG_ASSERT(has<TComponent>(entity));

    components_[entities_[entity].depth][componentId<TComponent>()][entity].reset();
}

template <typename... TComponents>
void Registry::view(const std::function<void(int entity, int depth)>& func) const {
    const auto cIds = {componentId<TComponents>()...};

    for (int depth{}; depth < components_.size(); ++depth) {
        const auto& depthComponents = components_[depth];

        for (const size_t cId : cIds) {
            if (cId >= depthComponents.size()) {
                continue;
            }
            const auto& cIdComponents = depthComponents[cId];

            for (int entity{}; entity < cIdComponents.size(); ++entity) {
                if (cIdComponents[entity].has_value()) {
                    func(entity, depth);
                }
            }
        }
    }
}

template <typename... TComponents>
void Registry::view(const std::function<void(int entity)>& func) const {
    view<TComponents...>([&func](int entity, int depth) { func(entity); });
}

template <typename TComponent>
size_t Registry::componentId() const {
    static size_t cId{nextComponentId_++};
    return cId;
}
