# 🏠 Interior Design of a Room — OpenGL 3D Scene

A fully interactive 3D room simulation built with OpenGL (GLUT). The scene includes various furniture, lighting, ceiling fan, AC, TV — all animated and controllable via keyboard.

---

## 👨‍💻 Authors

| Name | GitHub |
|------|--------|
| Md Naimur Rahman | [@MNR-Tushar](https://github.com/MNR-Tushar) |
| Md Shohanur Rahman | [@MSR-Shohan](https://github.com/MSR-Shohan) |


---

 
## 🎬 Demo Video
 
▶️ [Click here to watch the demo](https://shorturl.at/e9cCL)
 
---
 
## 📸 Screenshots
 
**View 1 — Living Area (TV, Sofa, Carpet, Fan)**
 
![Room View 1](screenshots/Picture.png)
 
**View 2 — Bedroom Area (Bed, Bookshelf, Study Table, AC, Window)**
 
![Room View 2](screenshots/Picture1.png)
 
---

## 📸 Features

### 🏗️ Room Structure
- Four walls, ceiling, and floor (with grid lines)
- **Back wall** — window with animated waving curtains
- **Left wall** — door with handle and light switch
- **Front wall** — wall-mounted TV, shelf, painting, power socket, flower pot

### 🛋️ Furniture
| Item | Description |
|------|-------------|
| Bed | With headboard, pillow, and blanket |
| Study Table | Monitor, keyboard, mouse, pen holder, notepad |
| Chair | Ergonomic office chair with armrests and star base |
| Bookshelf | 5 shelves with colorful books |
| Book Rack | 4 shelves, placed in a separate corner |
| Sofa | 3-seater with cushions and throw pillows |
| Coffee Table | Glass top with remote and cup |
| Side Table | With bedside lamp |
| Dressing Table | Mirror, drawers, and accessories |
| TV Cabinet | 3 doors with decorative items on top |
| Carpet | With border design |

### 💡 Animated Elements
- **Ceiling Fan** — rotates with adjustable speed
- **Ceiling Light** — toggle on/off
- **AC** — animated airflow streams
- **TV Screen** — UI glow and flicker effect
- **Curtains** — sine wave swaying animation
- **Digital Clock** — wall-mounted display

### 🌿 Decorations
- Wall art / painting on back wall
- Large floor corner flower pot
- Wall-mounted flower pot with blooms
- Small plants and vases on shelf
- Sunset landscape photo frame above sofa

---

## 🎮 Keyboard Controls

### Camera Movement
| Key | Action |
|-----|--------|
| `W` | Move forward |
| `S` | Move backward |
| `A` | Move left |
| `D` | Move right |
| `Q` | Move up |
| `E` | Move down |
| `Arrow Keys` | Look around |

### Device Controls
| Key | Action |
|-----|--------|
| `L` | Toggle light on/off |
| `F` | Toggle fan on/off |
| `+` / `=` | Increase fan speed |
| `-` | Decrease fan speed |
| `O` | Turn AC on |
| `P` | Turn AC off |
| `T` | Toggle TV on/off |
| `ESC` | Quit |

---

## 🛠️ Build & Run

### Prerequisites
- Linux / Windows (MinGW) / macOS
- OpenGL + GLUT libraries installed

### Install on Linux (Ubuntu/Debian)
```bash
sudo apt-get install freeglut3-dev
```

### Compile
```bash
g++ main.cpp -o room -lGL -lGLU -lglut -lm
```

### Run
```bash
./room
```

### Windows (MinGW)
```bash
g++ main.cpp -o room.exe -lfreeglut -lopengl32 -lglu32
```

---

## 📁 File Structure

```
├── main.cpp        # Complete source code (single file)
└── README.md       # This file
```

---

## 🧩 Code Structure

```
main.cpp
├── Global Variables     (camera position, animation states)
├── Helper Functions     (drawQuad, drawCylinder, drawCone)
├── Room & Walls         (drawRoom, drawDoor, drawSingleWindowWithCurtain)
├── Furniture            (drawBed, drawTable, drawChair, drawSofa, ...)
├── Decor                (drawFrontWallDecor, drawHangingWallFlower, ...)
├── Appliances           (drawFan, drawCeilingLight, drawAC, drawWallMountedTV)
├── Lighting Setup       (setupLighting — GL_LIGHT0 + GL_LIGHT1)
└── GLUT Callbacks       (display, idle, reshape, keyboard, specialKeys)
```

---

> Built with ❤️ using OpenGL / GLUT. All objects are constructed from custom geometry — no external 3D models used.
