# Computer Graphics Basics (OpenGL)

Interactive 3D rendering application built in **C++ with OpenGL** as part of a university computer graphics course.

The project implements a scene-based architecture, multiple lighting models, object picking using the stencil buffer, and real-time camera control.

---

## Features

- Multiple switchable scenes (1–6)
- Free camera movement (WASD + mouse look)
- Object picking via stencil + depth buffer
- World coordinate calculation using `glm::unProject`
- Dynamic object placement and removal
- Multiple shading models:
  - Lambert
  - Phong
  - Blinn-Phong
- Skybox rendering
- Depth & stencil testing

---

## Architecture

High-level structure:

Application

├── Scene (base)
├── DrawableObject
├── Controller (input)
├── ResourceManager
└── Shader system


The application separates:
- scene logic
- rendering
- resource loading
- input handling

---

## Controls

| Key | Action |
|------|--------|
| 1–6 | Switch scenes |
| W A S D | Move camera |
| RMB (hold) | Mouse look |
| LMB | Interact |
| Shift + LMB | Add object |
| Ctrl + LMB | Add textured object |
| F | Toggle flashlight |
| F1/F2/F3 | Change FOV |

---

## Tech Stack

- C++
- OpenGL
- GLFW
- GLEW
- GLM
- GLSL shaders
- Visual Studio solution

---

## What This Project Demonstrates

- 3D transformations & coordinate systems
- Lighting models implementation
- Depth & stencil buffer usage
- Scene-based rendering design
- Interactive 3D object manipulation
