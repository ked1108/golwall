# golwall - Game of Life Wallpaper
Its a simple dynamic wallpaper written in C for linux systems

The program uses [lodepng](https://github.com/lvandeve/lodepng) for encoding the background into a png file.  
Please star the repo to show them some love!

## Install 
***The program depends on nitrogen for setting the wallpaper to ensure an environment agnostic experience***
To build the program:
```bash
make
sudo make install #optional step for installing systemwide
```
I am currently working on implementing support for different DE's and WM's as of now it should be working on i3 and Gnome however Sway does not support nitrogen so I have created a separate branch for it.
