SOURCES := $(wildcard src/*.cpp)
HEADERS := $(wildcard src/*.hpp)

OBJECTS := $(patsubst %.cpp,obj/%.o,$(notdir $(SOURCES)))

CXXFLAGS := -g -std=c++17 -mcmodel=large -mno-red-zone -mno-sse -mno-sse2 \
			-mno-mmx -fexceptions -fasynchronous-unwind-tables \
			-I$(CURDIR)/inc/ -Wall -Wextra -Werror

DEFINES := -D __PHYSICAL_ADDRESS_EXTENSION__=1
all:

.PHONY: all

.PHONY: clean
clean:
	rm -f bin/* obj/*

