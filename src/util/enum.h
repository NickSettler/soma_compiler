/**
 * Enum helper macros
 * @file: enum.h
 * @date: 13.12.2022
 */

#ifndef SOMA_COMPILER_ENUM_H
#define SOMA_COMPILER_ENUM_H

#define ENUM_BIT_CASTING(type)                                                                                         \
    inline constexpr type operator&(type a, type b) {                                                                  \
        return static_cast<type>(static_cast<int>(a) & static_cast<int>(b));                                           \
    }                                                                                                                  \
                                                                                                                       \
    inline constexpr type operator|(type a, type b) {                                                                  \
        return static_cast<type>(static_cast<int>(a) | static_cast<int>(b));                                           \
    }                                                                                                                  \
                                                                                                                       \
    inline constexpr type operator~(type a) { return static_cast<type>(~static_cast<int>(a)); }                        \
                                                                                                                       \
    inline constexpr type operator|=(type &a, type b) { return a = a | b; }                                            \
                                                                                                                       \
    inline constexpr type operator&=(type &a, type b) { return a = a & b; }

#endif// SOMA_COMPILER_ENUM_H
