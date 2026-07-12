CC = clang++

TARGET_ARCH ?=
SYSROOT		?=
CROSS_FLAGS ?=
LDFLAGS		?=

ifneq ($(TARGET_ARCH),)
	CROSS_FLAGS += -target $(TARGET_ARCH)
	LDFLAGS		+= -target $(TARGET_ARCH) -fuse-ld+lld
endif

ifneq ($(SYSROOT),)
	CROSS_FLAGS += --sysroot=$(SYSROOT)
	LDFLAGS	+= --sysroot=$(SYSROOT)
endif

src_dir = src
debug_objects_dir = dbg
uba_objects_dir = uba
point_objects_dir = point
leak_objects_dir = leak
thread_objects_dir = thread
opt_objects_dir = opt

src = $(wildcard $(src_dir)/*.cpp)
debug_objects       = $(patsubst $(src_dir)/%.cpp,$(debug_objects_dir)/%.o,$(src))
debug_dependencies  = $(patsubst $(src_dir)/%.cpp,$(debug_objects_dir)/%.d,$(src))
uba_objects         = $(patsubst $(src_dir)/%.cpp,$(uba_objects_dir)/%.o,$(src))
uba_dependencies    = $(patsubst $(src_dir)/%.cpp,$(uba_objects_dir)/%.d,$(src))
point_objects       = $(patsubst $(src_dir)/%.cpp,$(point_objects_dir)/%.o,$(src))
point_dependencies  = $(patsubst $(src_dir)/%.cpp,$(point_objects_dir)/%.d,$(src))
leak_objects        = $(patsubst $(src_dir)/%.cpp,$(leak_objects_dir)/%.o,$(src))
leak_dependencies   = $(patsubst $(src_dir)/%.cpp,$(leak_objects_dir)/%.d,$(src))
thread_objects      = $(patsubst $(src_dir)/%.cpp,$(thread_objects_dir)/%.o,$(src))
thread_dependencies = $(patsubst $(src_dir)/%.cpp,$(thread_objects_dir)/%.d,$(src))
opt_objects         = $(patsubst $(src_dir)/%.cpp,$(opt_objects_dir)/%.o,$(src))
opt_dependencies    = $(patsubst $(src_dir)/%.cpp,$(opt_objects_dir)/%.d,$(src))

debug_flags = -Wall -Wextra -Werror -Wpedantic -O0 -g -MMD -MP -std=c++23 $(CROSS_FLAGS)
debug_uba_flags = -Wall -Wextra -Werror -Wpedantic -O0 -fsanitize=undefined,address -MMD -MP -std=c++23 $(CROSS_FLAGS)
debug_point_flags = -Wall -Wextra -Werror -Wpedantic -O0 -fsanitize=address,pointer-compare -MMD -MP -std=c++23 $(CROSS_FLAGS)
debug_leak_flags = -Wall -Wextra -Werror -Wpedantic -O0 -fsanitize=leak -MMD -MP -std=c++23 $(CROSS_FLAGS)
debug_thread_flags = -Wall -Wextra -Werror -Wpedantic -O0 -fsanitize=thread -MMD -MP -std=c++23 $(CROSS_FLAGS)
optimized_flags = -Wall -Wextra -Werror -Wpedantic -fPIE -O2 -MMD -MP -std=c++23 $(CROSS_FLAGS)

target = prg
debug_target = $(target)-dbg
uba_target = $(target)-uba
point_target = $(target)-point
leak_target = $(target)-leak
thread_target = $(target)-thread
opt_target = $(target)

.PHONY: debug uba point leak thread opt clean

define run_binary
	$(if $(TARGET_ARCH), @echo "Cross-compiled binary created. Skipping execution.",./$<)
endef

debug: $(debug_target)
	$(call run_binary)

$(debug_target): $(debug_objects)
	$(CC) $(LDFLAGS) -o $@ $^

$(debug_objects_dir)/%.o: $(src_dir)/%.cpp
	@mkdir -p $(debug_objects_dir)
	$(CC) $(debug_flags) -c $< -o $@

uba: $(uba_target)
	$(call run_binary)

$(uba_target): $(uba_objects)
	$(CC) $(LDFLAGS) -o $@ $^ -fsanitize=undefined,address

$(uba_objects_dir)/%.o: $(src_dir)/%.cpp
	@mkdir -p $(uba_objects_dir)
	$(CC) $(debug_uba_flags) -c $< -o $@

point: $(point_target)
	$(call run_binary)

$(point_target): $(point_objects)
	$(CC) $(LDFLAGS) -o $@ $^ -fsanitize=address,pointer-compare

$(point_objects_dir)/%.o: $(src_dir)/%.cpp
	@mkdir -p $(point_objects_dir)
	$(CC) $(debug_point_flags) -c $< -o $@

leak: $(leak_target)
	$(call run_binary)

$(leak_target): $(leak_objects)
	$(CC) $(LDFLAGS) -o $@ $^ -fsanitize=leak

$(leak_objects_dir)/%.o: $(src_dir)/%.cpp
	@mkdir -p $(leak_objects_dir)
	$(CC) $(debug_leak_flags) -c $< -o $@

thread: $(thread_target)
	$(call run_binary)

$(thread_target): $(thread_objects)
	$(CC) $(LDFLAGS) -o $@ $^ -fsanitize=thread

$(thread_objects_dir)/%.o: $(src_dir)/%.cpp
	@mkdir -p $(thread_objects_dir)
	$(CC) $(debug_thread_flags) -c $< -o $@

opt: $(opt_target)
	$(call run_binary)

$(opt_target): $(opt_objects)
	$(CC) $(LDFLAGS) -o $@ $^

$(opt_objects_dir)/%.o: $(src_dir)/%.cpp
	@mkdir -p $(opt_objects_dir)
	$(CC) $(optimized_flags) -c $< -o $@

clean:
	rm -rf $(debug_objects_dir) $(uba_objects_dir) $(point_objects_dir) $(leak_objects_dir) $(thread_objects_dir) $(opt_objects_dir) \
	$(debug_target) $(uba_target) $(point_target) $(leak_target) $(thread_target) $(opt_target)

-include $(debug_dependencies)
-include $(uba_dependencies)
-include $(point_dependencies)
-include $(leak_dependencies)
-include $(thread_dependencies)
-include $(opt_dependencies)
