# Changelog


## Changes:



* Moved `ZeroMemory` to function.hpp
* Moved structures related to memory maps to mmap.hpp


## Additions:



* Added changelog
* Added function `sort` to sort an array of integers. Internally uses Quicksort
* Added mmap.hpp and mmap.cpp
    * Added function to sort e820 map


## Deletions:



* 


## Bug fixes:



* Fixed missing colon on label in boot/memoryMap.asm


---


# Known bugs:



* Makefile does not recompile bootSecondStage if included files are changed

