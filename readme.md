# Computergraphic

> Course work for the module 'Computergrafik' at FH Bingen

**Seems like it does not render the world anymore**

## Building
### Requirements
* cmake - version 2.9 or higher
* gcc - version 6.1.1 or higher (lower version might work, but i don't guarantee it)
* some other libraries like glm need to be installed

### Compile

1. run `mkdir -p build && cd build && cmake ../WyvernEngine && make`
2. the compiled game can be found at **./build/sbigame/sbi_game**

you need to make sure that the folder `res` is in the current working direction, before starting the game

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
