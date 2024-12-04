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
```

### Step 2: Clone the Repository
Clone the repository to your local machine:
```bash
git clone https://github.com/yourusername/FLTK-ShapeDrawer.git
cd FLTK-ShapeDrawer
```

### Step 3: Build the Project
Use the included Makefile to compile the program:
```bash
sudo make
```
This will generate an executable file named fltk-shapedrawer.

### Step 4: Run the Program
Execute the program with:

```bash
./fltk-shapedrawer
```

## Usage Instructions
### Static Shape Drawing

Click on buttons labeled Static Line, Static Rectangle, or Static Circle.
Enter the required details (coordinates, dimensions).
Use the slider to set the thickness.
Click Submit to draw the shape.

### Random Shape Drawing
Click on buttons labeled Random Line, Random Rectangle, or Random Circle.
A shape with random dimensions and position will appear on the canvas.

### Cleaning Up
To remove the compiled files and reset the project directory:
```bash
sudo make clean
```

