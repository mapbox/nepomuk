#ifndef NEPOMUK_TOOL_TYPES_AND_TRAITS_STRONG_TYPEDEF_HPP
#define NEPOMUK_TOOL_TYPES_AND_TRAITS_STRONG_TYPEDEF_HPP

#include <cstddef>
#include <functional>
#include <iostream>
#include <type_traits>

// Tool to explicityly check for ID types
#define STRONG_ID_TYPE(base_type, id_type)                                                         \
    struct id_type final                                                                           \
    {                                                                                              \
        using value_type = base_type;                                                              \
        base_type __value;                                                                         \
                                                                                                   \
        /* explicit conversion */                                                                  \
        explicit operator base_type &() { return __value; }                                        \
        explicit operator base_type() const { return __value; }                                    \
                                                                                                   \
        base_type base() const { return __value; }                                                 \
                                                                                                   \
        /* comparison for basic relative checks */                                                 \
        bool operator<(const id_type z_) const                                                     \
        {                                                                                          \
            return __value < static_cast<const base_type>(z_);                                     \
        }                                                                                          \
        bool operator>(const id_type z_) const                                                     \
        {                                                                                          \
            return __value > static_cast<const base_type>(z_);                                     \
        }                                                                                          \
        bool operator<=(const id_type z_) const                                                    \
        {                                                                                          \
            return __value <= static_cast<const base_type>(z_);                                    \
        }                                                                                          \
        bool operator>=(const id_type z_) const                                                    \
        {                                                                                          \
            return __value >= static_cast<const base_type>(z_);                                    \
        }                                                                                          \
        bool operator==(const id_type z_) const                                                    \
        {                                                                                          \
            return __value == static_cast<const base_type>(z_);                                    \
        }                                                                                          \
        bool operator!=(const id_type z_) const                                                    \
        {                                                                                          \
            return __value != static_cast<const base_type>(z_);                                    \
        }                                                                                          \
        /* serialisable */                                                                         \
        friend std::ostream &operator<<(::std::ostream &stream, const id_type &inst);              \
    };                                                                                             \
    static_assert(std::is_arithmetic<id_type::value_type>(), "ID type requires arithmetic type."); \
    static_assert(std::is_trivial<id_type>(), #id_type " is not a trivial type");                  \
    static_assert(std::is_standard_layout<id_type>(), #id_type " is not a standart layout");       \
    static_assert(std::is_pod<id_type>(), #id_type " is not a POD layout");                        \
    inline std::ostream &operator<<(::std::ostream &stream, const id_type &inst)                   \
    {                                                                                              \
        return stream << inst.__value;                                                             \
    }

#define STRONG_ID_TYPE_HASH(base_type, id_type)                                                    \
    namespace std                                                                                  \
    {                                                                                              \
    template <> struct hash<id_type>                                                               \
    {                                                                                              \
        ::std::size_t operator()(const id_type key) const                                          \
        {                                                                                          \
            return ::std::hash<base_type>()(static_cast<const base_type>(key));                    \
        }                                                                                          \
    };                                                                                             \
    }

#endif // NEPOMUK_TOOL_TYPES_AND_TRAITS_STRONG_TYPEDEF_HPP
