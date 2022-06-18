SOURCES := $(wildcard $(CURDIR)/src/*)
HEADERS := $(wildcard $(CURDIR)/inc/*)

OBJECTS := $(patsubst %.cpp,obj/%.o,$(notdir $(SOURCES)))

TARGET := x86_64

CXX := /usr/x86_64elfgcc/bin/x86_64-elf-g++
AS := /usr/x86_64elfgcc/bin/x86_64-elf-as
AR := /usr/x86_64elfgcc/bin/x86_64-elf-ar
LD := /usr/x86_64elfgcc/bin/x86_64-elf-ld

OBJCOPY := /usr/x86_64elfgcc/bin/x86_64-elf-objcopy

GDB := /usr/x86_64elfgcc/bin/x86_64-elf-gdb

CXXFLAGS := -g -std=c++17 -mcmodel=large -mno-red-zone -mno-sse -mno-mmx \
			-mno-sse2 -fexceptions -fasynchronous-unwind-tables \
			-I$(CURDIR)/inc -Wall -Wextra -Werror


DEFINES := -D __PHYSICAL_ADDRESS_EXTENSION__=1

.PHONY: all
all: bin/libstdc++.a

bin/libstdc++d.a: $(OBJECTS)
	$(AR) rcs $@ $^

bin/libstdc++.a: bin/libstdc++d.a
	$(OBJCOPY) -S $^ $@

obj/%.o: */%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(DEFINES) -ffreestanding -c $< -o $@

.PHONY: clean
clean:
	rm -f bin/* obj/*

