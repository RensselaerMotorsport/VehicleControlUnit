# Vehicle Contorl Unit 🏎️
## Overview
This is the c library for controlling the car.

This software is intended to be run on a real time operating system.

### Pretty Graphs
```git log --graph --abbrev-commit --decorate --date=relative --all```

## Software Compilation on VS Code

To compile purely software using .

1. You idealy would want to fetch the repository to the same directory that can be accessed by STM32 Cube IDE to ensure seamless workflow and updates from software to hardware, so follow step 1 to 4 from [Basic Setup for Hardware Compilation](#basic-setup-for-hardware-compilation)
2. Open the repository folder in VS Code. 
3. Install the [Cortex-Debug](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug), [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) and [WSL](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-wsl) extensions in VS Code. 
4. Install WSL from Microsoft Store.
5. Open WSL in terminal.
6. Install the required tools in WSL using the following commands:
    ```
    sudo apt update
    sudo apt install -y gcc-arm-none-eabi
    sudo apt install -y build-essential cmake
    sudo apt install -y ninja-build
    ```
7. cd to the repository using WSL:
    ```
    cd /mnt/c/Users/yourusername/STM32CubeIDE/workspace_x.xx.x/VehicleControlUnit
    ```
8. Open repository with WSL in VS Code:
    ```
    code .
    ```
9. 

### SIL (Software) Testing

 1. Install WSL from Microsoft Store
 2. Run terminal as administrator
 3. run `wsl --update` (If it's stuck at 0%, try `wsl --update --web-download`)
 4. Start wsl in terminal: `wsl`
 5. `sudo apt-get update && sudo apt-get upgrade -y`
 6. `sudo apt-get install gcc -y`
 7. git clone VCU repo in wsl
 8. Open in VScode `code .`
 9. Change directory to Core `cd Core`
 10. Open terminal and run: `make "test file directory".out && ./"test file directory".out` (e.g. `make Tests/TorqueControlActuatorTest.out && ./Tests/TorqueControlActuatorTest.out`)


## Software Compilation to Hardware 

### Basic Setup for Hardware Compilation

To compile software into firmware that is flashed onto the STM32 board. 

1. Install [STM32 Cube IDE](https://www.st.com/en/development-tools/stm32cubeide.html#:~:text=Read%20more-,Get%20Software,-Part%20Number) 
2. Upon intsallation, find the STM32CubeIDE directory. It should be at `C:\Users\yourusername\STM32CubeIDE` if the default installation instructions are followed
3. Depending on the installed version, a folder titled `workspace_x.xx.x` should be found
4. Fetch the entire repository into the folder. Go to your native terminal, then enter the command:
    ```
    git clone https://github.com/RensselaerMotorsport/VehicleControlUnit C:\Users\yourusername\STM32CubeIDE
    ```
5. A folder titled VehicleControlUnit should appear in your workspace. 
6. Open STM32 Cube IDE. Select `C:\Users\yourusername\STM32CubeIDE\workspace_x.xx.x` as the launch directory.
7. Connect STM32 board to computer. Flash firmware  by running the project in debug mode. ** have to test this agai

### Developing Tips on the IDE
#### File Creation / Duplication
- New project with **existing .ioc file**:

    File > New > STM32 Project from an Existing STM32CubeMX Configuration File (.ioc), then select the pre-existing file

- New project with unintialized .ioc file: 

    File > New > STM32 Project, then enter board name into "Commercial Part Number"



#### Code generation with .ioc file 

HAL intialization code for peripherals can be automatically generated into main.c once configured in the .ioc file. Any changes in the .ioc file will require code generation (to update main.c) - right click the .ioc file then click "Code Generation". 

> [!CAUTION]
> Code generation rewrites the main.c such that all user code (denoted by the user code begin and end comments) will be GONE. Be sure to save the user code elsewhere. 

#### Useful Documentation


### HIL (Hardware) Testing

### PIL (Processor) Testing 


## Useful Git Commands and Tools

Common errors and mitigation techniques (e.g. git clean -xfd)