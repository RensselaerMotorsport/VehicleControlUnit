# VehicleContorlUnit
## Overview
This is the c library for controlling the car.

This software is intended to be run on a real time operating system

##  Pretty graphs: 
```git log --graph --abbrev-commit --decorate --date=relative --all```

## Software Compilation on VS Code



## Software Compilation to Hardware 

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
- New project with existing .ioc file:

    File > New > STM32 Project from an Existing STM32CubeMX Configuration File (.ioc), then select the pre-existing file

- New project with unintialized .ioc file: 

    File > New > STM32 Project, then enter board name into "Commercial Part Number"



#### Code generation with .ioc file 

HAL intialization code for peripherals can be automatically generated into main.c once configured in the .ioc file. Any changes in the .ioc file will require code generation (to update main.c) - right click the .ioc file then click "Code Generation". 

> [!CAUTION]
> Code generation rewrites the main.c such that all user code (denoted by the user code begin and end comments) will be GONE. Be sure to save the user code elsewhere. 

### HAL documentation


## Useful Git Commands and Tools

- 



## Running Tests on Windows

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