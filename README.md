# FLTK-ShapeDrawer

# Shape Drawer with FLTK

A graphical application built using the **FLTK (Fast Light Toolkit)** library to draw various shapes (lines, rectangles, and circles) on a canvas. The application supports both **static** and **random** shape generation with customizable border thickness.

---

## Features

- **Static Shape Drawing:**
  - Input coordinates and dimensions manually to draw shapes.
  - Available shapes: **Line**, **Rectangle**, and **Circle**.
- **Random Shape Generation:**
  - Automatically generate shapes with random sizes and positions.
  - Border thickness cycles between 1 and 5 for each new shape.
- **Customizable Thickness:**
  - Adjust thickness for static shapes using a slider.

---

## Requirements

- **C++ Compiler:** A compiler supporting C++11 or later.
- **FLTK Library:** Version 1.3.x or higher.
- **Make Utility:** To use the provided `Makefile`.

---

## Installation

### Step 1: Install FLTK

On Ubuntu or Debian-based systems:
```bash
sudo apt update
sudo apt install libfltk1.3-dev

