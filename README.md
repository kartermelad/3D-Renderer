# 3D Software Renderer in C

This is a custom-built software 3D renderer written in C, utilizing a pixel buffer for rendering and `GLFW` for window and OpenGL context management. The renderer performs 3D transformations, triangle rasterization, and depth buffering entirely in software.

## Features
- Custom built math library (matrices, vectors)
- Software rasterization with depth buffering
- MVP matrix based triangle rendering
- Camera system with view and projection matrices
- Rendering results displayed via OpenGL texture on a quad

## Dependencies
- GLFW: Window creation and context
- OpenGL: To display the final rendered texture

### Install GLFW

#### On Linux (Ubuntu/Debian):
```bash
sudo apt update
sudo apt install libglfw3-dev
```

## Run Instructions
- Build:
  ```bash
  make
  ```
- Clean Build Files:
  ```bash
  make clean
  ```
- Run the Program:
  ```
  ./3d-renderer
  ```
  
