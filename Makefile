# Files
CXXSources:=$(shell find src/ -name *.cpp -type f)

# Output
CXXObjects:=$(patsubst src/%.cpp,build/%.o,$(CXXSources))
OutputDirs:=build/ build/gui/ build/level/ build/level/gen/ build/level/tile/ build/level/entity/ build/level/item/ build/gfx/ build/assets/ build/util/

# Third-party
ThirdPartyObjects:=

# Compiler params
G++6X := $(shell command -v g++-6 2> /dev/null)
CXX:=$(if $(G++6X),g++-6,g++)
LD:=$(if $(G++6X),g++-6,g++)
CXX_FLAGS:=-std=c++14 --pedantic -O2 -Wall -Iinclude/ -Ilib/include/ `sdl2-config --cflags`
LD_FLAGS:=`sdl2-config --libs` `pkg-config --libs SDL2_image` `pkg-config --libs zlib`


all: build/roguelike

build/roguelike: makedirs tplibs $(CXXObjects)
	$(LD) -o $@ $(CXXObjects) $(ThirdPartyObjects) $(LD_FLAGS)

build/%.o: src/%.cpp
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

makedirs:
	@mkdir -p $(OutputDirs)
	
tplibs:

run: build/roguelike
	build/roguelike
	
run_valgrind: build/roguelike
	valgrind build/roguelike

clean:
	rm -rf build/
	rm -f $(ThirdPartyObjects)