# os-dev-project
Expanding my skills by developing a basic CLI operating system

Requirements
----
To compile you must have a cross-compiler called `x86_64-elf-gcc` in your system's `PATH` variable.
To configure this you must use the following options for binutils:
```bash
export PREFIX="$HOME/opt/cross"
export TARGET=x86_64-elf
export PATH="$PREFIX/bin:$PATH"
./binutils-x.y.z/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
```
and for gcc:

Create the following file and save it as t-x86_64-elf inside gcc/config/i386/ under your GCC sources.
```
# Add libgcc multilib variant without red-zone requirement
 
MULTILIB_OPTIONS += mno-red-zone
MULTILIB_DIRNAMES += no-red-zone
```
By default this new configuration will not be used by GCC unless it's explicitly told to. Open gcc/config.gcc in your favorite editor and search for case block like this:
```
 x86_64-*-elf*)
 	tm_file="${tm_file} i386/unix.h i386/att.h dbxelf.h elfos.h newlib-stdint.h i386/i386elf.h i386/x86-64.h"
 	;;
```
This is the target configuration used when creating a GCC Cross-Compiler for x86_64-elf. Modify it to include the new multilib configuration:
```
 x86_64-*-elf*)
	tmake_file="${tmake_file} i386/t-x86_64-elf" # include the new multilib configuration
 	tm_file="${tm_file} i386/unix.h i386/att.h dbxelf.h elfos.h newlib-stdint.h i386/i386elf.h i386/x86-64.h"
 	;;
```
then confnigure with:
```bash
../gcc-x.y.z/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
```

Other requirements to compile are `make` and `NASM`

To run this project you must have `qemu`, and to debug you must have `gdb` compiled for x86_64-elf platforms, and it must be called `x86_64-elf-gdb`

Compiling and Runing
----
To compile run:
```bash
$ make all
```
To run using QEMU, run:
```bash
$ make run
```
To debug using GDB and QEMU,run:
```bash
$ make debug
```
You can also run it on a QEMU VNC server by running
```bash
$ make run-vnc
```
Or debug with GDB on a QEMU VNC server by running
```bash
$ make debug-vnc
```

Future Plans
----
Short term objectives can be found [here](https://trello.com/b/iBwjiLt0/).
Long term plans are currently not decided, however the above Trello link will be the long term plans after a more consideration about what I really want out of this project.

