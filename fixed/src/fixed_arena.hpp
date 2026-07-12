#ifndef FIXED_ARENA
#define FIXED_ARENA

#include <cstdint>
#include <cstddef>
#include <cstring>
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

template <auto SizingFunction>
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
	
	template <typename T>
	T *push_struct_z() {
		return static_cast<T*>(push(sizeof(T), false)); // will zero the memory
	}
	
	template <typename T>
	T *push_struct_nz() {
		return static_cast<T*>(push(sizeof(T), true)); // will leave the memory empty
	}
	
	template <typename T>
	T *push_array_z() {
		return static_cast<T*>(push(sizeof(T), false));
	}
	
	template <typename T>
	T *push_array_nz() {
		return static_cast<T*>(push(sizeof(T), true));
	}
	
	explicit Fixed_arena(std::unsigned_integral auto n) : capacity(SizingFunction(n)), position(0) {
		arena = static_cast<std::byte*>(std::malloc(capacity));
		
		if (arena == nullptr) { // FIX LATER: bad if statement
			capacity = 0;
		}
	}
	
	~Fixed_arena() {
		std::free(arena);
	}
	
	void *push(u64 size, bool non_zero) {
		u64 position_aligned = (((u64)(position) + ((u64)(sizeof(void*)) - 1)) & (~((u64)(sizeof(void*)) - 1)));
		u64 new_position = position_aligned + size;
		
		if (new_position > capacity) {
			return nullptr;
		}
		
		position = new_position;
		std::byte *out = static_cast<std::byte*>(arena) + position_aligned;
		
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
