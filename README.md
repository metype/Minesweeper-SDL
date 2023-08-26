# Minesweeper-SDL

## Build Instructions

Prerequisites:
* g++
* cmake
* SDL2
* SDL2_ttf

### Linux:
```
cd {Project Root Directory} 
cmake ./ 
make
```
To run the compiled executable:
```
cd {Project Root Directory}/bin
./Minesweeper
```

## Program Arguments

| Arg Name | Long Arg Name | Description                                                        | Default Value |
| -------- | ------------- | ------------------------------------------------------------------ | ------------- |
| -c       | --cellSize    | The size of each cell (square) in the game (measured in pixels).   | 64            |
| -b       | --bombCount   | The number of bombs that should be placed on the grid              | 24            |
| -v       | --vSync       | Enables or disables vSync on the render surface. (0=false, 1=true) | 0             |
| -w       | --width       | The width of the window in pixels                                  | 1280          |
| -h       | --height      | The height of the window in pixels                                 | 960           |