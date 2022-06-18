# os-dev-project
Expanding my skills by developing a basic CLI operating system

Requirements
----
To compile you must have a cross-compiler called `x86_64-elf-gcc` in your system's `PATH` variable. This cross compiler must be compiled to compile for x86_64-elf platforms. You must also compile `binutils' with the same options.

Other requirements to compile are `make` and `NASM`

To run this project you must have `qemu`, and to debug you must have `gdb` compiled for x86_64-elf platforms, and it must be called `x86_64-elf-gdb`

Compiling and Runing
----
To compile simply type:
```bash
$ make all
```
To run using QEMU, type:
```bash
$ make run
```
To debug using GDB and QEMU, type:
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

