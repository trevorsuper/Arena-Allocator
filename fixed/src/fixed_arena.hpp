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

//std::min(x, y)
//std::max(x, y)

template <auto SizingFunction>
struct Fixed_arena {
	Fixed_arena() = delete;								  // Delete the default constructor
	Fixed_arena(const Fixed_arena&) = delete;			  // Delete the copy constructor
	Fixed_arena& operator=(const Fixed_arena&) = delete;  // Delete the copy assignment operator
	
	explicit Fixed_arena(std::unsigned_integral auto n) : capacity(SizingFunction(n)), position(0) {
		arena = static_cast<std::byte*>(std::malloc(capacity));
		
		if (arena == nullptr) { // FIX LATER: bad if statement
			capacity = 0;
		}
	}

	~Fixed_arena() {
		std::free(arena);
	}
	
	void *arena_push(Fixed_arena *arena, u64 size, bool non_zero);
	void arena_pop(Fixed_arena *arena, u64 size);
	void arena_pop_to(Fixed_arena *arena, u64 position);
	void arena_clear(Fixed_arena *arena);
	
	u64 capacity;
	u64 position;
	std::byte *arena{nullptr};
};

#endif //ARENA
