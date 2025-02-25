# VehicleContorlUnit
## Overview
This is the c library for controlling the car.

This software is intended to be run on a real time operating system

##  Pretty graphs: 
```git log --graph --abbrev-commit --decorate --date=relative --all```

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
 10. Define the environment variable `TEST_MODE` (e.g. `export TEST_MODE=1`)
 11. Open terminal and run: `make "test file directory".out && ./"test file directory".out` (e.g. `make Tests/TorqueControlActuatorTest.out && ./Tests/TorqueControlActuatorTest.out`)

## Running Tests on Linux

1. Update and install dependencies: (e.g. `sudo apt-get update && sudo apt-get upgrade && sudo apt-get install gcc -y`)
2. Clone repo into your working directory (`git clone https://github.com/RensselaerMotorsport/VehicleControlUnit.git`)
3. Change directory to `Core` (`cd VehicleControlUnit/Core`)
4. Define the environment variable `TEST_MODE` (`export TEST_MODE=1`)
5. Build the desired test with `make` (e.g. `make Tests/Utils/LUT.out`)
6. Run the test and examine output (e.g. `./Tests/Utils/LUT.out`)
