## Table of contents
* [General info](#general-info)
* [Technologies](#technologies)
* [Setup](#setup)

## General info
This is prototype of autonomous plant incubator based on STM32F746ZG microcontroller, connected to RPI based server.
	
## Technologies
Project is created with:
* C
* Python
* PHP
* JavaScript
* HTML/CSS
* Raspbian
* STM32CubeIDE version: 1.3.0

## Used Hardware
Project is created with:
* RPI4 4GB RAM
* STM32F746ZG Nucleo-144
* Light sensor BH1750 - github.com/lamik library here
* Temperature & Pressure sensor BMP280 -  github.com/LonelyWolf library here
* Hummidity sensor MOD-01588
* Aquarium Water Pump 12V
* LED light 
* 2x 5V FAN
* 2x16 LCD Display - github.com/lamik library here
	
## Setup
To run this project, install STM32CubeIDE version atleast 1.3.0, load project and connect to STM.
RPI files can be found here https://github.com/AvajWiktor/Autonomic-incubator-server, paste it to your server folder path and connect RPI to stm32 by usb, RPI has to be connected with your wifi aswell.
