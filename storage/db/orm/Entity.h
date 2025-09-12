#pragma once

#include <tuple>
#include <string>
#include <type_traits>

// Base class for all ORM entities. Provides alias `Self` for macros.
template <typename T>
struct Entity {
    using Self = T;
};

// Macro to declare table name in an entity class.
#define ENTITY_TABLE(table_name) \
    static std::string TableName() { return table_name; }

// Macro to declare fields in an entity class.
#define ENTITY_FIELDS(...) \
    static auto Fields() { \
        return std::make_tuple(__VA_ARGS__); \
    }

// Helper macro to create field metadata. Requires `Entity<T>` base.
#define FIELD(field_name) std::make_pair(#field_name, &Self::field_name)

