# RoomCraft 3D — Interactive Interior Designer
### C++ & OpenGL Computer Graphics Mini Project

**RoomCraft 3D** is an intermediate-level, interactive 3D Computer Graphics application developed using **C++**, **OpenGL (classic fixed-function pipeline)**, and **FreeGLUT / GLU**. It allows users to design, furnish, customize, and inspect a 3D architectural room in real time.

---

## 🏛️ Project Features

- **Architectural Room Construction:**
  - Subdivided hardwood parquet floor with natural alternating wood grain tones.
  - Back wall featuring an architectural door casing, recessed wood door panel, and brass handle.
  - Left wall featuring a window opening with dark charcoal casing, cross mullions, and transparent glass.
  - Right accent wall with a geometric modern art canvas.
  - Perimeter baseboard trims and ceiling hanging pendant light fixture.
- **Parametric Furniture Library (6 Distinct Models):**
  - **King Bed:** Solid wood frame, tall cushioned headboard, mattress, folded duvet, and dual pillows.
  - **Dining / Lounge Chair:** Tapered legs, upholstered seat cushion, backrest spindles, and top rail.
  - **Dining / Work Table:** Beveled wood tabletop, solid legs, and apron support frame.
  - **Lounge Sofa:** Multi-cushion design, angled peg feet, plush armrests, backrest, and accent throw pillows.
  - **Wardrobe:** Tall double-door cabinet, recessed center divider, top crown, bottom plinth, and brass handles.
  - **Floor Lamp:** Weighted brass disk base, slender vertical pole, translucent linen shade, and glowing bulb.
- **Interactive Object Manipulation:**
  - Real-time placement, cycling selection (`[TAB]`, `[SPACE]`), translation (`[WASD]` or Arrow Keys), rotation (`[Q]`/`[E]`), scaling (`[` / `]`), and deletion (`[Del]`).
  - Palette customizer (`[C]`): Instantly cycles object fabric/wood between 8 designer tones (Navy, Terracotta, Sage Green, Warm Wood, Walnut, Charcoal, Warm Beige, Cream).
  - Object duplication (`[U]`) with automatic offset.
- **Dual Camera Systems:**
  - **3D Perspective Orbit View:** Smooth spherical coordinate mouse orbit (`Left-Click Drag`), pan (`Right-Click Drag`), and zoom (`Mouse Wheel` / `+/-`).
  - **Top-Down Orthographic Blueprint View (`[V]`):** Architectural 2D floor plan view without perspective distortion.
  - Elevation presets (Front Elevation and Side Elevation).
- **Collision & Boundary Enforcement:**
  - Axis-Aligned Bounding Box (AABB) collision warning when furniture overlaps.
  - Wall boundary clamping preventing items from leaving the room.
- **On-Screen HUD & Object Inspector:**
  - Top-left system status banner.
  - Top-right live property inspector displaying active object coordinates, rotation angle, scale percentage, color swatch, and collision status.
  - Bottom controls cheat sheet.
- **Serialization (Save & Load):**
  - Clean, human-readable text file format (`layouts/sample_room.txt` and `layouts/saved_room.txt`).
  - Hotkeys: `[K]` to Save, `[O]` to Load, `[P]` to Reload Sample Showroom, `[N]` for New Empty Room.

---

## 📁 Project Structure

```
RoomCraft3D/
├── CMakeLists.txt              # Standard CMake build configuration (MSVC / MinGW)
├── build.bat                   # 1-Click build script (compiles with g++/MinGW64)
├── run.bat                     # 1-Click launcher script
├── README.md                   # Complete documentation and viva guide
├── libfreeglut.dll             # FreeGLUT runtime library
├── external/
│   └── freeglut/               # Portable, self-contained FreeGLUT SDK
│       ├── include/GL/         # freeglut.h, glut.h, freeglut_ext.h
│       ├── lib/                # libfreeglut.dll.a, libfreeglut_static.a
│       └── bin/                # libfreeglut.dll
├── include/
│   ├── camera.h                # Camera orbit, spherical math, and projection modes
│   ├── room.h                  # 3D room architectural geometry and trims
│   ├── furniture.h             # 6 parametric furniture classes and renderers
│   ├── scene.h                 # Object list, selection state, and collisions
│   ├── renderer.h              # OpenGL primitives, lighting, colors, and 2D HUD
│   ├── input.h                 # Keyboard and mouse dispatching
│   └── save_load.h             # Text file serialization and deserialization
├── src/
│   ├── main.cpp                # Application entry point, HUD, and GLUT callbacks
│   ├── camera.cpp              # Camera transformations and gluLookAt
│   ├── room.cpp                # Floor, walls, door, window, and fixture drawing
│   ├── furniture.cpp           # Detailed geometric modeling for all 6 items
│   ├── scene.cpp               # Scene graph, boundary checks, and AABB collision
│   ├── renderer.cpp            # Normal calculations, GLU quadrics, 2D overlay
│   ├── input.cpp               # Event handler implementations
│   └── save_load.cpp           # File I/O parsing and writing
└── layouts/
    └── sample_room.txt         # Pre-configured sample furnished Scandinavian room
```

---

## 🚀 How to Build and Run

### Option 1: Direct 1-Click Build (Fastest — No setup required)
The repository includes a ready-to-use build script that automatically detects the installed MinGW-w64 compiler:

1. Open PowerShell or Command Prompt in `RoomCraft3D`:
   ```cmd
   cd RoomCraft3D
   .\build.bat
   ```
2. Launch the application:
   ```cmd
   .\run.bat
   ```
   *(or run `.\bin\RoomCraft3D.exe` directly)*

### Option 2: Building with CMake (MSVC x64 or MinGW)
If you have CMake installed on your machine:
```cmd
cd RoomCraft3D
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

---

## 🎮 Interactive Controls Reference

| Action | Control | Description |
|---|---|---|
| **Add Bed** | `[1]` | Places King Bed into room |
| **Add Chair** | `[2]` | Places Dining Chair into room |
| **Add Table** | `[3]` | Places Dining / Work Table into room |
| **Add Sofa** | `[4]` | Places Lounge Sofa into room |
| **Add Wardrobe** | `[5]` | Places Double-door Wardrobe into room |
| **Add Floor Lamp** | `[6]` | Places Standing Floor Lamp into room |
| **Cycle Selection** | `[TAB]` or `[SPACE]` | Selects next furniture item |
| **Previous Selection** | `[ ` ]` (Tilde) | Selects previous furniture item |
| **Deselect All** | `[X]` | Clears active selection |
| **Move Object** | `[W] [A] [S] [D]` or `[Arrow Keys]` | Translates selected object along X/Z floor plane |
| **Rotate Object** | `[Q]` / `[E]` | Rotates selected item by ±15° around vertical Y-axis |
| **Scale Object** | `[` / `]` | Scales selected item down/up (50% to 220%) |
| **Change Color** | `[C]` | Cycles selected object through 8 designer color palettes |
| **Duplicate** | `[U]` | Clones active object with offset |
| **Delete Object** | `[Backspace]` or `[Delete]` | Removes selected object from scene |
| **Orbit Camera** | `Mouse Left-Click Drag` | Rotates viewing angle around room center |
| **Pan Camera** | `Mouse Right-Click Drag` | Pans camera target point |
| **Zoom In / Out** | `Mouse Wheel` or `[+] / [-]` | Adjusts camera distance / field of view |
| **Cycle Camera View** | `[V]` | Switches: 3D Orbit → Top-Down 2D Blueprint → Front → Side |
| **Reset Camera** | `[R]` | Restores default camera viewing angle |
| **Toggle Ceiling Light** | `[L]` | Turns room ceiling light ON / OFF |
| **Toggle Floor Grid** | `[G]` | Shows / hides floor alignment grid |
| **Toggle Front Wall** | `[F]` | Toggles front wall visibility (cutaway view) |
| **Save Room** | `[K]` | Saves layout to `layouts/saved_room.txt` |
| **Load Room** | `[O]` | Loads previously saved layout |
| **Sample Preset** | `[P]` | Reloads the default showroom layout |
| **Clear Room** | `[N]` | Clears all furniture (starts blank room) |
| **Toggle HUD** | `[H]` | Toggles on-screen UI overlay |
| **Exit** | `[ESC]` | Closes the application |

---

## 🎓 Viva & Technical Interview Explanation Guide

### 1. Coordinate System Convention
- **X-Axis (Horizontal / Red):** Left (-) to Right (+) across the room width.
- **Y-Axis (Vertical / Green):** Floor ($Y = 0$) to Ceiling ($Y = 3.6$ m) upwards.
- **Z-Axis (Depth / Blue):** Back wall ($Z < 0$) to Front wall ($Z > 0$).
- Standard Right-Handed Cartesian coordinate system where the floor lies on the $XZ$-plane.

### 2. Camera Transformations & Spherical Coordinates
- In `camera.cpp`, the camera orbits around a target point $(X_t, Y_t, Z_t)$ using spherical coordinates:
  $$\text{Eye}_X = X_t + R \cdot \cos(\text{pitch}) \cdot \sin(\text{yaw})$$
  $$\text{Eye}_Y = Y_t + R \cdot \sin(\text{pitch})$$
  $$\text{Eye}_Z = Z_t + R \cdot \cos(\text{pitch}) \cdot \cos(\text{yaw})$$
- `gluLookAt(eyeX, eyeY, eyeZ, targetX, targetY, targetZ, upX, upY, upZ)` sets up the viewing matrix by converting world coordinates into camera-relative coordinates.
- In **Top-Down Orthographic View**, the camera is placed directly above $(0, 25, 0)$ looking straight down at $(0, 0, 0)$ with Up Vector $(0, 0, -1)$.

### 3. Projection Matrices
- **Perspective Projection (`gluPerspective`):** Objects further away appear smaller, simulating human vision. Used in 3D orbit mode.
- **Orthographic Projection (`glOrtho` / `gluOrtho2D`):** Parallel projection lines where objects maintain equal size regardless of distance. Used in Top-Down architectural blueprint mode and 2D UI overlays.

### 4. Transformation Stack (`glPushMatrix` / `glPopMatrix`)
- OpenGL uses a matrix stack to isolate transformations for individual hierarchical components:
  1. `glPushMatrix()` saves the current transformation state.
  2. `glTranslatef()`, `glRotatef()`, and `glScalef()` place, orient, and size the piece of furniture.
  3. Primitive components (legs, cushions, panels) are drawn relative to this local coordinate system.
  4. `glPopMatrix()` restores the previous matrix so subsequent objects are unaffected.

### 5. Lighting, Surface Normals, and Shading
- **Lighting Model:** Classic Phong/Blinn-Phong reflection model with Ambient, Diffuse, and Specular components.
  - `GL_LIGHT0`: Ceiling light positioned at ceiling center with quadratic distance attenuation.
  - `GL_LIGHT1`: Gentle directional fill light preventing harsh black shadows.
- **Surface Normals (`glNormal3f`):** Every face of our cuboids and cylinders specifies a perpendicular unit vector $(\hat{N})$. The lighting engine computes $\vec{N} \cdot \vec{L}$ (Lambert's cosine law) to determine face brightness.
- **Smooth Shading (`glShadeModel(GL_SMOOTH)`):** Vertex colors and lighting intensities are bilinearly interpolated across each polygon polygon (Gouraud shading).

### 6. Depth Buffering (`Z-Buffering`)
- Enabled via `glEnable(GL_DEPTH_TEST)` and cleared every frame with `glClear(GL_DEPTH_BUFFER_BIT)`.
- For each pixel, the GPU compares the incoming fragment's depth value with the stored depth buffer. Only fragments closer to the camera are rendered, correctly handling object occlusion.

### 7. Double Buffering
- Initialized with `GLUT_DOUBLE`.
- While one buffer (Front Buffer) is displayed on screen, the GPU renders the next frame into the hidden Back Buffer. `glutSwapBuffers()` instantly flips them, completely eliminating flickering and visual tearing.

### 8. Collision Detection Algorithm
- Uses **Axis-Aligned Bounding Box (AABB)** overlap on the $XZ$ floor plane:
  $$\text{Overlap}_X = |\text{posA}_X - \text{posB}_X| < (\text{halfW}_A + \text{halfW}_B)$$
  $$\text{Overlap}_Z = |\text{posA}_Z - \text{posB}_Z| < (\text{halfD}_A + \text{halfD}_B)$$
- If both conditions hold true, an on-screen warning alert is triggered.
