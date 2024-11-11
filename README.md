# 3D LED Cube Display Project

## Overview
This project is an interactive 3D LED cube display, developed by **Group D** (Diogo Chaves, Joana Pereira, Lucas Freitas) as part of an embedded systems course. The system uses a 6x6x6 LED matrix controlled by a Raspberry Pi 4 to produce dynamic animations and patterns. The cube's LEDs can create complex visual effects, allowing user-selected animations via physical buttons.

## Vision
Our goal is to create an interactive LED cube that showcases visual animations in a 3D grid format. By controlling each LED individually, we can generate diverse effects, from basic patterns to intricate 3D animations. This project blends hardware integration and software development to enable smooth, real-time transitions and responsive user interactions.

## Key Objectives and Requirements
- **3D LED Cube Display**: Create a 6x6x6 LED cube matrix capable of displaying various animations.
- **Raspberry Pi Control**: Manage LED operations through a Raspberry Pi 4, which handles animation sequences.
- **Real-Time Interactivity**: Enable user control of animations using physical buttons.
- **Optimization**: Ensure the system's responsiveness, smooth transitions, and performance optimization.

## Hardware Components
The project requires the following hardware:
- **Raspberry Pi 4** - The main control unit.
- **LED Tower (6x6x6)** - The display matrix.
- **MCP23017 I/O Expanders** (x3) - For additional I/O control.
- **74HC164 Shift Registers** (x2) - To control LED rows.
- **Resistors**:
  - 36x 1kΩ Resistors
  - 36x 120Ω Resistors
- **BC557C Transistors** (x36) - For driving the LEDs.
- **NPN Transistors** (x6)
- **Buttons** (x12) - For user input.

## Software
The software manages:
- **Animation Sequences**: Pre-programmed patterns that can be triggered by users.
- **Real-Time Control**: Real-time processing to maintain smooth transitions and responsive interactions.
- **User Interface**: Simple button-based interface for selecting animations.

## Usage
1. **Setup**: Connect the Raspberry Pi to the LED matrix.
2. **Launch**: Run the control software on the Raspberry Pi to initialize the LED cube.
3. **Interaction**: Use the physical buttons to switch between different animations and adjust settings as needed.
