# SDMMC / SD Logging


## Overview
This module handles interfacing with the STM32 SDMMC peripheral and managing data logging to the SD card using SDIO/SDMMC, RTOS, and DMA.  
It is responsible for initializing the SD card interface, mounting the filesystem, and handling high-speed data writes from various subsystems.

## Hardware Interface
- STM32 SDMMC1 peripheral
- microSD with a FAT filesystem

## Pinout
[ST Nucleo-F767ZI Dev-Board Pins](https://os.mbed.com/platforms/ST-Nucleo-F767ZI/)

PC_8 -> D0  
PC_9 -> D1  
PC_10 -> D2  
PC_11 -> D3  
PC_12 -> CLK  
PD2 -> CMD  
(We haven’t made use of the DET pin)

## Logging Behavior
- Logs are written as CSV files (`log_XXXX.csv`).
- Each line formatted as: `<RTC>,<signal_name>,<value>`
- A new file is created every time the system is reset.
- If you remove the SD card before pausing or shutting off the VCU, you may lose some data.

## SDMMC Configuration
Make sure the RCC clock is configured already.  
In Connectivity, enable SD 4-bit Wide Bus.  
Since RTOS forces DMA, the DMA streams will be auto-generated, but you must also enable the SDMMC global interrupt for the other interrupts to work properly.  
Enable SDIO hardware control flow to ensure stable logging.  
Make sure the SDIO clock is below 48 MHz.  
In FatFs, enable SD Card.  
I'm not 100% sure on this, but RTC may also need to be configured for FatFs to write files.

## SDMMC Coding Notes
In `MX_SDMMCn_SD_Init`, in 4-bit mode, the system will try to initialize the SDMMC in 1-bit mode, but RTOS doesn’t allow this, so you must override it in the user-generated code. It will still be a 4-bit bus.

```c
hsd1.Init.BusWide = SDMMC_BUS_WIDE_1B;
```

SDMMC can’t initialize before the OS is initialized, meaning that mounting, writing, etc. on the SD card has to occur in an RTOS task.
