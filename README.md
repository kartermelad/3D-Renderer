# 3D-Renderer
A from-scratch 3D renderer in C with no OpenGL, just math, pixels, and pure graphics fun.


3d-renderer/
│
├── assets/               # Textures, models, shaders (if any), test scenes
│
├── include/              # Public headers for your modules
│   ├── math/             # Linear algebra (vec3.h, mat4.h, etc.)
│   ├── render/           # Rendering interfaces (camera.h, renderer.h)
│   └── core/             # Core utilities (config.h, types.h)
│
├── src/                  # Source files (implementations)
│   ├── math/             # vec3.c, mat4.c
│   ├── render/           # renderer.c, rasterizer.c, camera.c
│   ├── core/             # main.c, utils.c, logging.c
│   └── main.c            # Entry point
│
├── tests/                # Unit tests and test scenes
│
├── build/                # Build output (you can gitignore this)
│
├── Makefile              # Or CMakeLists.txt
├── README.md
└── LICENSE
