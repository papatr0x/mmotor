//
// Created by Patricio Palma on 25-02-26.
//

#ifndef MOTORCITO_REGISTRY_H
#define MOTORCITO_REGISTRY_H
#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <cstdint>

using Entity = std::uint32_t;

class ComponentPool {
public:
    virtual ~ComponentPool() = default;
    virtual void remove(Entity e) = 0;
};

template<typename T>
class TypedPool final : public ComponentPool {
    std::unordered_map<Entity, T> m_data;
public:
    template<typename... Args>
    T& emplace(Entity e, Args&&... args) {
        return m_data.emplace(e, T{std::forward<Args>(args)...}).first->second;
    }

    T* get(Entity e) {
        auto it = m_data.find(e);
        return it != m_data.end() ? &it->second : nullptr;
    }

    void remove(Entity e) override { m_data.erase(e); }

    auto begin() { return m_data.begin(); }
    auto end()   { return m_data.end(); }
};

class Registry {
    std::uint32_t m_nextEntity{1};
    std::unordered_map<std::type_index, std::unique_ptr<ComponentPool>> m_pools;

public:
    Entity create() { return m_nextEntity++; }

    void destroy(Entity e) {
        for (auto& [_, pool] : m_pools) pool->remove(e);
    }

    template<typename T, typename... Args>
    T& emplace(Entity e, Args&&... args) {
        return pool<T>().emplace(e, std::forward<Args>(args)...);
    }

    template<typename T>
    T* get(Entity e) { return pool<T>().get(e); }

    template<typename T>
    TypedPool<T>& view() { return pool<T>(); }

private:
    template<typename T>
    TypedPool<T>& pool() {
        auto key = std::type_index(typeid(T));
        auto it = m_pools.find(key);
        if (it == m_pools.end()) {
            m_pools[key] = std::make_unique<TypedPool<T>>();
        }
        return static_cast<TypedPool<T>&>(*m_pools.at(key));
    }
};

#endif //MOTORCITO_REGISTRY_H