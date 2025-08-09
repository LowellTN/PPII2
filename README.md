# PPII2

Small prototype platformer written in C with SDL2. The game explores two gameplay phases:
- Platformer phase when you have HP remaining.
- A more rhythmic, experimental phase that currently triggers when your HP drop to 0.

Developed and tested on Linux.

## Requirements

- gcc, make
- SDL2, SDL2_image, SDL2_mixer, SDL2_ttf development packages

Install on common distros:
- Debian/Ubuntu:
  ```
  sudo apt update
  sudo apt install build-essential libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
  ```
- Arch:
  ```
  sudo pacman -S base-devel sdl2 sdl2_image sdl2_mixer sdl2_ttf
  ```

## Build

From the repository root:
```
make -j
```

## Run

Run from the repo root so asset paths resolve:
```
./PPII2   # or the binary name defined in the Makefile
```
Check the Makefile for the exact output binary name/location.

Tip (VS Code): Terminal > Run Build Task… > make.

## Controls

- Dash: J


## Features (current state)

- Window size: 1280x720
- Side-scrolling camera that follows the player
- Dash mechanic (J)
- Background drawing handled via `drawBackground` in `map.c`

## Maps and data

Maps live in `map1/`, `map2/`, `mapBoss1/` with:
- `data.txt`, `data_1.txt` (tiles)
- `metadata.txt` (map metadata)

Metadata format:
- First line: number of tiles (height), number of tiles (width), start X, start Y, goal X, goal Y, number of checkpoints
- Next N lines: checkpoint positions
  - Line i (1-based) after the first line: X of checkpoint (i-1), Y of checkpoint (i-1)

Example (conceptual):
```
H W startX startY goalX goalY checkpointsCount
cp1X cp1Y
cp2X cp2Y
...
```

## Project layout

- C source/headers at repo root (`*.c`, `*.h`)
- `asset/`:
  - `spritesheet/`, `background/`, `sounds/`, `Music/`, `Fonts/`, etc.
- `map*/`:
  - Map tiles and metadata
- `Makefile`: build configuration

## Notes

- Prototype quality; the 0 HP “rhythmic” phase is experimental.
- Keep working directory at the repo root when running, so relative asset paths load