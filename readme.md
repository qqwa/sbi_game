# Computergraphic

> Course work for the module 'Computergrafik' at FH Bingen

**Seems like it does not render the world anymore**

## Building
### Requirements
* cmake - version 2.9 or higher
* gcc - version 6.1.1 or higher (lower version might work, but i don't guarantee it)
* glew

### Compile

```
git clone https://github.com/qqwa/sbi_game.git
cd sbi_game
git submodule update --init --recursive
```

On Linux just run the `run.sh` script.

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
