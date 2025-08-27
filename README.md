# stmIpcApp

A STM32 project for NUCLEO-U575ZI-Q to toggle LEDs within the board and when user button is pressed using FreeRtos and IPC.
Task 1 (Poller) sends request to Task 2 (Receiver) to change LED status then Task 2 changes the state and sends acknowledgment message to Task 1.

## Features

Organized source code in multiple directories  
Build files generated for __Binaries__ and __Debug__

## Prerequisites

STM32CubeIDE ≥ 1.18.1  
STM32CubeMX  ≥ 6.14.1  
EGit plugin	 ≥ 7.3  

## Building with Make

### 1. Build all

$ make -j8 all

Builds Binaries and Debug target

### 2. Clean Build Artifacts

$ make -j8 clean

Removes all build files