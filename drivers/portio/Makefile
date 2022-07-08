SOURCES := $(wildcard src/*.cpp)
HEADERS := $(wildcard src/*.hpp)

OBJECTS := $(patsubst %.cpp,obj/%.o,$(notdir $(SOURCES)))

CXXFLAGS := -g -std=c++17 -mcmodel=large -mno-red-zone -mno-sse -mno-sse2 \
			-mno-mmx -fexceptions -fasynchronous-unwind-tables \
			-I$(CURDIR)/inc/ -Wall -Wextra -Werror -I$(PROJROOTDIR)/drivers/terminal/inc/

DEFINES := -D __PHYSICAL_ADDRESS_EXTENSION__=1

all: bin/portio.so
.PHONY: all

bin/portio.so: $(OBJECTS)
	$(LD) -shared -o $@ $^

obj/%.o: */%.cpp $(HEADERS)
	echo $(PROJROOTDIR)
	$(CXX) $(CXXFLAGS) $(DEFINES) -ffreestanding -c $< -o $@

.PHONY: clean
clean:
	rm -f bin/* obj/*

