#ifndef FIXED_ARENA
#define FIXED_ARENA

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <concepts>
#include <algorithm>
#include <memory>

using s64 = std::int64_t;
using s32 = std::int32_t;
using s16 = std::int16_t;
using s8  = std::int8_t;
using u64 = std::uint64_t;
using u32 = std::uint32_t;
using u16 = std::uint16_t;
using u8  = std::uint8_t;

constexpr u64 KiB(u64 n) noexcept { return n << 10; }
constexpr u64 MiB(u64 n) noexcept { return n << 20; }
constexpr u64 GiB(u64 n) noexcept { return n << 30; }

template<auto fn>
concept ValidSizingFunction = (fn == KiB) || (fn == MiB) || (fn == GiB);
		
template<auto va>
concept ValidAlignment = (va == 1) || (va == 2) || (va == 4) || (va == 8) || (va == 16) || (va == 32) || (va == 64);

template <auto SizingFunction, auto MemoryAlignment>
requires ValidSizingFunction<SizingFunction> && ValidAlignment<MemoryAlignment>
struct Fixed_arena {
	Fixed_arena() = delete;								  // Delete the default constructor
	Fixed_arena(const Fixed_arena&) = delete;			  // Delete the copy constructor
	Fixed_arena& operator=(const Fixed_arena&) = delete;  // Delete the copy assignment operator
	Fixed_arena& operator+(const Fixed_arena&) = delete;
	Fixed_arena& operator-(const Fixed_arena&) = delete;
	Fixed_arena& operator*(const Fixed_arena&) = delete;
	Fixed_arena& operator/(const Fixed_arena&) = delete;
	Fixed_arena& operator+=(const Fixed_arena&) = delete;
	Fixed_arena& operator-=(const Fixed_arena&) = delete;
	Fixed_arena& operator*=(const Fixed_arena&) = delete;
	Fixed_arena& operator/=(const Fixed_arena&) = delete;
	explicit operator bool() const noexcept { return arena != nullptr; }
	
	explicit Fixed_arena(std::unsigned_integral auto n) : capacity(SizingFunction(n)), 
														  position(0) {
		arena = static_cast<std::byte*>(std::aligned_alloc(MemoryAlignment, capacity));
		if (arena == nullptr) {
			capacity = 0;
		}
	}
	
	~Fixed_arena() {
		std::free(arena);
	}
	
	Fixed_arena(Fixed_arena&& other) noexcept : capacity(other.capacity), 
												position(other.position), 
												arena(other.arena)
	{
		other.capacity = 0;
		other.position = 0;
		other.arena = nullptr;
	}
	
	Fixed_arena& operator=(Fixed_arena&& other) noexcept {
		if (this != &other) {
			std::free(arena);
			capacity = other.capacity;
			position = other.position;
			arena = other.arena;
			other.capacity = 0;
			other.position = 0;
			other.arena = nullptr;
		}
		return *this;
	}
	
	template <typename T>
	T *push_struct_z() {
		return static_cast<T*>(push(sizeof(T), alignof(T), false)); // will zero the memory
	}
	
	template <typename T>
	T *push_struct_nz() {
		return static_cast<T*>(push(sizeof(T), alignof(T), true)); // will leave the memory empty
	}
	
	template <typename T>
	T *push_array_z(u64 n) {
		return static_cast<T*>(push(sizeof(T) * n, alignof(T), false));
	}
	
	template <typename T>
	T *push_array_nz(u64 n) {
		return static_cast<T*>(push(sizeof(T) * n, alignof(T), true));
	}
	
	void *push(u64 size, u64 alignment, bool non_zero) {
		u64 position_aligned = (position + (alignment - 1)) & ~(alignment - 1);
		u64 new_position = position_aligned + size;
		
		if (new_position > capacity) {
			return nullptr;
		}
		
		position = new_position;
		std::byte *out = arena + position_aligned;
		
		if (!non_zero) {
			std::memset(out, 0, size);
		}
		
		return out;
	}
	
	void pop(u64 size) {
		size = std::min(size, position);
		position -= size;
	}
	
	void pop_to(u64 position_to_pop_to) {
		if (position_to_pop_to < position) {
			position = position_to_pop_to;
		}
	}
	
	void clear() {
		position = 0;
	}
	
	u64 capacity;
	u64 position;
	std::byte *arena{nullptr};
};

#endif //FIXED_ARENA
