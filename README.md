# Vehicle Contorl Unit 🏎️
## Overview
This is a custom implementation of a Vehicle Processor written by Rensselaer Motorsport to be used on FSAE vehicles through the STM32F767Zi microprocessor. It implements several systems and safety features to provide a secure and reliable method of operating datalogging, controls, and component simulation operations on a vehicle.

While our implementation is configured to be a central car computer and controller, this same code can be used to make something like a local CAN node or a small datalogging unit on a less powerful STM chip (i.e. F1 or F3) by reconfiguring code through STM32 Cube MX.

This software is intended to be run on a real time operating system (i.e. FreeRTOS).

### Documentation:
For code documentation, please see our Doxygen page: [RM-VCU Doxygen](https://rensselaermotorsport.github.io/VehicleControlUnit/files.html)

### Folder Structure
```
VehicleControlUnit/
├── Core/
│   ├── Inc/                  # Header files
│   │   └──                    # Similar to Src/
│   ├── Src/                  # Source files
│   │   ├── Outputs/          # Digital and Analog Outputs
│   │   ├── Scheduler/        # Scheduling service for VCU
│   │   ├── Sensors/          # Digital and Analog Inputs
│   │   ├── Systems/          # Control, Safety, & Comm Systems
│   │   └── Utils/            # Helper files
│   ├── Tests/                # Unit tests
│   └── Makefile              # makefile
├── Hardware/                 # KiCAD files for hardware dev
├── Documentation/            # Project documentation resources
├── Drivers/                  # Hardware abstraction layer drivers
│   ├── CMSIS/                # CMSIS files
│   └── STM32F7xx_HAL_Driver/ # STM32 HAL drivers
├── .gitignore                # Git ignore file
├── README.md                 # Project readme
└── VehicleControlUnit.ioc    # Cube MX configuration file
```

### General Program Structure



## Software Compilation on VS Code

### Basic Setup for Software Compilation
To compile purely software using cmake

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
    sudo apt install make cmake
    ```
7. cd to the repository using WSL:
    ```
    cd /mnt/c/Users/yourusername/STM32CubeIDE/workspace_x.xx.x/VehicleControlUnit
    ```
8. Open repository with WSL in VS Code:
    ```
    code .
    ```
9. cd to the directory that houses the makefile:
    ```
    cd Core
    ```
10. Build the project using cmake
    ```
    make clean 
    make all
    make test?
    ```
11. TO DO: Write what to expect

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


## Software Compilation to Hardware (STM 32 Cube IDE)

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
7. Connect STM32 board to computer. Flash firmware by running the project in debug mode. Refer to [HIL](#hil-hardware-testing) and [PIL](#pil-processor-testing) testing sections for more details

### Developing Tips on the IDE
#### File Creation / Duplication
- New project with **existing .ioc file**:

    File > New > STM32 Project from an Existing STM32CubeMX Configuration File (.ioc), then select the pre-existing file

- New project with unintialized .ioc file: 

    File > New > STM32 Project, then enter board name into "Commercial Part Number"

#### Code generation with .ioc file 

HAL intialization code for peripherals can be automatically generated into main.c once configured in the .ioc file. Any changes in the .ioc file will require code generation (to update main.c) - right click the .ioc file then click "Code Generation". 


> [!CAUTION]
> Code generation rewrites files such as main.c/.h, stm32*.c/.h, and so. Make sure any custom code is inbetween the comments labelled "BEGIN/END USER CODE." Otherwise, it will overwrite any code you wrote outside these blocks. Anything inside of the Scheduler, Sensors, System, Outputs, and Utils folders is safe.
#### Useful Documentation
TO DO: Add links to documents

### HIL (Hardware) Testing
It may be useful to first setup a test bench to simulate your car's hardware. The bench below simulates the following:

- CAN Bus activity on Bus 1
- APPs input (double)
- Brake input
- RTD input
- Throttle Signal output
- Brake Light output
- RTD status indicator

![HIL test bench](Documentation/shabbyHILsetup.jpg)


### PIL (Processor) Testing 
TO DO

## Useful Git Commands and Tools

### Command Line Git
#### git clean -xfd

Removes all untracked files and directories from your working directory.Essentially, prevents files like build artifacts, generated binary files, IDE-specific temporary files and debug outputs from getting commited.

- x: Removes ignored files
- f: Forces the clean operation
- d: Removes untracked directories

TO DO: Add more useful cli commands

### Tools
#### VS Code Source Control

Intuitive and accessible GUI for committng, creating pull requests, creating branches... essentially replaces the need to use command line git. 

#### [Github Pull Request Extension](https://marketplace.visualstudio.com/items?itemName=GitHub.vscode-pull-request-github)

Allows reviewing and managing of GitHub pull requests and issues directly in VS Code.

#### [Github Desktop](https://desktop.github.com/download/)

Intuitive GUI for cloning repositories to user specified directories, switching branches, fetching/pushing from origin.

#### Pretty Graphs
Run this command in the terminal to generate pretty git commit graphs
```git log --graph --abbrev-commit --decorate --date=relative --all```

#### STM32 Cube IDE
Useful for compiling code onto STM boards, and to also debug in real time on the hardware. You can put in breakpoints, view memory regions, and CPU utilization to name some useful tools.

#### STM32 Cube MX
Useful for configuring clocks and GPIO on STM boards using HAL (Hardware Abstraction Layer). It can then generate this code into the project for you.

</br>

> [!CAUTION]
> Code generation rewrites files such as main.c/.h, stm32*.c/.h, and so. Make sure any custom code is inbetween the comments labelled "BEGIN/END USER CODE." Otherwise, it will overwrite any code you wrote outside these blocks. Anything inside of the Scheduler, Sensors, System, Outputs, and Utils folders is safe.

</br>
STM 32 Cube MX interface for GPIO:

![STM 32 Cube MX interface for GPIO](Documentation/CubeMXpic1.png)

</br>
STM 32 Cube MX interface for Clocks:

![STM 32 Cube MX interface for Clocks](Documentation/CubeMXpic2.png)