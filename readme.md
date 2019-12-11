# Computergraphic

> Course work for the module 'Computergrafik' at FH Bingen

**Seems like it does not render the world anymore**

## Building
### Requirements

On Arch Linux install:
```
pacman -S base-devel cmake xorg glew mesa
```

### Compile and run

```
git clone https://github.com/qqwa/sbi_game.git
cd sbi_game
git submodule update --init --recursive
```

When on Linux just running the `run.sh` script should be enough, when all depedencies are installed.
Windows is not supported.

> You need to make sure that the folder `res` is in the current working direction, before starting the game

![Screenshot of the Game](/screenshot_small.png)

## Controls
### Keyboard

* **Rotate:** wasd
* **Accelerate:** space
* **Brake:** shift

### Gamepad (Xbox360/Xbox-One)

* **Rotate:** Left Analog Stick
* **Accelerate:** A
* **Brake:** B

*note: if a gamepad is connected the keyboard controls won't work*

---
