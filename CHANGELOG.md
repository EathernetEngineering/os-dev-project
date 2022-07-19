# Changelog


## Changes:



* Moved `ZeroMemory` to function.hpp
* Moved structures related to memory maps to mmap.hpp
* Renamed ‘libc’ to ‘klibc’
* `setupPaging()` now maps 511MiB instead of 16MiB 


## Additions:



* Added changelog
* Added function `swap`. Templated to take any type. Both parameters ust be the same type
* Added function `sort` to sort an array of integers. Internally uses Quicksort
* Added mmap.hpp and mmap.cpp
    * Added function to sort e820 map
* Added file ‘init.hpp’ to klibc, with an inline function to call init functions for all other files in klibc which need to be initalized
* Added `malloc`
* Added linking to libgcc
* Added `kentry()` and moved all critical initialization functions there, and renamed the original `kentry` `kmain`


## Deletions:



* 


## Bug fixes:



* Fixed missing colon on label in boot/memoryMap.asm
* Fixed bug causing the first 32 k of the disk being read twice


---


# Known bugs:



* Makefile does not recompile bootSecondStage if dependencies are changed
* Reqires specific gcc version, and libgcc at specific path

