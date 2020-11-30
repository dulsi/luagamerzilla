SRC=src/*.c

CPP_FLAGS=-Wall

LUAJIT_CFLAGS := $(shell pkg-config --cflags luajit, gamerzilla)
LUAJIT_LDFLAGS := $(shell pkg-config --libs luajit gamerzilla)

GNU_OUT=luagamerzilla.so
GNU_IPATHS=$(LUAJIT_CFLAGS)
GNU_FLAGS=$(GNU_IPATHS) $(LUAJIT_LDFLAGS)

.PHONY: all osx linux32 linux64 windows32 windows64

all:
	@echo "choose platform: linux64 | linux32 | windows32 | windows64 | osx"

linux32:
	$(CC) $(SRC) $(CPP_FLAGS) -m32 -o $(GNU_OUT) -shared -fPIC $(GNU_FLAGS)

linux64:
	$(CC) $(SRC) $(CPP_FLAGS) -o $(GNU_OUT) -shared -fPIC $(GNU_FLAGS)

ifeq ($(OS),Windows_NT)
# Windows stuff
SHELL=cmd
WINDOWS_IPATHS=-I./cache/include
WINDOWS_OPT=-LD -EHsc -Feluagamerzilla
VARSALL="C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"

luajit.zip:
	curl -sL -o luajit.zip http://luajit.org/download/LuaJIT-2.0.5.zip

cache: luajit.zip
	@echo "Downloading LuaJIT source"
	unzip -qo luajit.zip
	mkdir cache\include
	cp LuaJIT-2.0.5/src/*.h cache/include

cache/win32_lua51.lib:
	@echo "Compiling LuaJIT 32 bits"
	if not exist cache $(MAKE) cache
	cd LuaJIT-2.0.5/src && call $(VARSALL) x86 && call msvcbuild.bat
	cp LuaJIT-2.0.5/src/lua51.lib cache/win32_lua51.lib

cache/win64_lua51.lib:
	@echo "Compiling LuaJIT 64 bits"
	if not exist cache $(MAKE) cache
	cd LuaJIT-2.0.5/src && call $(VARSALL) x64 && call msvcbuild.bat
	cp LuaJIT-2.0.5/src/lua51.lib cache/win64_lua51.lib

windows32:
	if not exist cache\win32_lua51.lib $(MAKE) cache/win32_lua51.lib
	call $(VARSALL) x86 && cl $(SRC) cache/win32_lua51.lib $(WINDOWS_OPT) $(WINDOWS_IPATHS)

windows64:
	if not exist cache\win64_lua51.lib $(MAKE) cache/win64_lua51.lib
	call $(VARSALL) x64 && cl $(SRC) cache/win64_lua51.lib $(WINDOWS_OPT) $(WINDOWS_IPATHS)
endif
