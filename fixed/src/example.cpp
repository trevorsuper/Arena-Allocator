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
	//Fixed_arena<sizing_function_that_will_not_compile> does_not_compile(70u); // 490 bytes
	std::cout << "Allocated " << small_arena.capacity << " bytes\n";
	std::cout << "Allocated " << medium_arena.capacity << " bytes\n";
	std::cout << "Allocated " << large_arena.capacity << " bytes\n";
	//std::cout << "Allocated " << does_not_compile.capacity << "bytes\n";
	
	
	/* 
$ make debug
clang++ -Wall -Wextra -Werror -Wpedantic -MMD -MP -std=c++23  -O0 -g -c src/example.cpp -o dbg/example.o
src/example.cpp:13:2: error: constraints not satisfied for class template 'Fixed_arena' [with SizingFunction = &allo]
   13 |         Fixed_arena<allo> should_not_compile(70u); // 490 bytes
      |         ^~~~~~~~~~~~~~~~~
src/fixed_arena.hpp:29:10: note: because &allo does not satisfy 'ValidSizingFunction'
   29 | requires ValidSizingFunction<SizingFunction>
      |          ^
src/fixed_arena.hpp:26:32: note: because '&allo == KiB' evaluated to false
   26 | concept ValidSizingFunction = (F == KiB) || (F == MiB) || (F == GiB);
      |                                ^
src/fixed_arena.hpp:26:46: note: and '&allo == MiB' evaluated to false
   26 | concept ValidSizingFunction = (F == KiB) || (F == MiB) || (F == GiB);
      |                                              ^
src/fixed_arena.hpp:26:60: note: and '&allo == GiB' evaluated to false
   26 | concept ValidSizingFunction = (F == KiB) || (F == MiB) || (F == GiB);
      |                                                            ^
1 error generated.
make: *** [Makefile:85: dbg/example.o] Error 1
	 */
	return 0;
}
