#include <cstdint>
#include <cstddef>
#include <cstring>
#include <algorithm>
#include <memory>
#include <iostream>
#include "fixed_arena.hpp"

int main() {
	Fixed_arena<KiB> small_arena(500u);
	Fixed_arena<MiB> medium_arena(6u);
	Fixed_arena<GiB> large_arena(1u);
	std::cout << "Allocated " << small_arena.capacity << " bytes\n";
	std::cout << "Allocated " << medium_arena.capacity << " bytes\n";
	std::cout << "Allocated " << large_arena.capacity << " bytes\n";
	
	return 0;
}
