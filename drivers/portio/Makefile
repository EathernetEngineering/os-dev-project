SOURCES := $(wildcard src/*.cpp)
HEADERS := $(wildcard src/*.hpp)

OBJECTS := $(patsubst %.cpp,obj/%.o,$(notdir $(SOURCES)))

CXXFLAGS := -g -std=c++17 -mcmodel=large -mno-red-zone -mno-sse -mno-sse2 \
			-mno-mmx -fno-exceptions -fasynchronous-unwind-tables \
			-I$(CURDIR)/inc -include stdint.h -Wall -Wextra -Werror \
			-fno-rtti -fno-use-cxa-atexit

LINKFLAGS := -nostdlib

.PHONY: all
all: bin/portio.so

bin/portio.so: $(OBJECTS)
	$(LD) -shared -o $@ $^

obj/%.o: */%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(DEFINES) -ffreestanding -c $< -o $@

.PHONY: clean
clean:
	rm -f bin/* obj/*

