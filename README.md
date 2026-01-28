# DW3Arduino - Dragon Warrior (Quest) 3 for Arduino

This is a remake of the original [Dragon Quest 3](https://en.wikipedia.org/wiki/Dragon_Quest_III) for the NES ("Dragon Warrior 3" in the US), built specifically for the Arduino Giga R1 board. After all the fun I had making [DragonQuestino](https://github.com/elgasste/DragonQuestino), I wanted to up the ante and go for a much bigger game.

## Hardware

This project is designed to use the following hardware:

- [Arduino Giga R1 Wifi board](https://store-usa.arduino.cc/products/giga-r1-wifi)
- [Arduino Giga R1 Display Shield](https://store-usa.arduino.cc/products/giga-display-shield)
- [Arduino_KVStore](https://github.com/arduino-libraries/Arduino_KVStore)
- Nintendo Entertainment System controller (any knockoff will do), plus a cord extender

## Wiring

The wiring is quite simple. The display shield is designed to attach directly to the back of the circuit board, and the controller only has five wires that need to be connected. The following photos match with the corresponding colos in the diagram:

![479288680-15e820b8-7181-4045-a097-2f1c28f2381e](https://github.com/user-attachments/assets/0b606e75-46b9-4017-8d4c-d30c3246df21)
![479279294-b2f8f18c-1761-40b2-8110-e61edcba1c52](https://github.com/user-attachments/assets/5292a95f-dff0-44de-9388-32e20664fb23)
![479279325-0f7103da-448b-4b8b-8690-0ae9cf102560](https://github.com/user-attachments/assets/6742210f-ace3-44df-bccf-7a58478ad777)
![490280252-7e3ed6b9-12b9-4a63-8227-ea1412f8d02f](https://github.com/user-attachments/assets/2da488d7-59d7-4451-8fea-e3769b113860)

## How To Build

### Arduino IDE

All the source and sketch files are located in the `DW3Arduino` directory. You'll need to install the following packages in the IDE:

- Arduino Giga R1 board
- Arduino_GigaDisplay library v1.0.2
- Adafruit GFX library v1.12.4 (possibly, I can't remember if this comes with the Giga Display library)

### Visual Studio 2022

There are two Visual Studio projects available for development in Windows:

- The DW3 Windows development project is located in the `DW3ArduinoWinDev` directory. This will run the actual game, and is very useful for quick development, debugging, and running diagnostics.
- The DW3 Editor tool is located in the `DW3ArduinoEditor` directory. This is a WPF project for editing the game data (maps, sprite sheets, NPC locations, etc).

## Windows Diagnostics and Keyboard Controls

When running `DW3ArduinoWinDev`, pressing `F8` brings up a diagnostics window with some useful information, as well as some debug keys. These do the following:

- Pressing 1 toggles no-clip mode, allowing the player to walk through anything.
- Pressing 2 toggles fast-walk mode, making the player move very fast (useful for testing different areas of tile maps).
- Pressing 3 toggles visible hit boxes, which will show all entities' hit boxes as red rectangles.

As far as controlling the game, the button mappings are as follows:

- NES A: keyboard X
- NES B: keyboard Z
- NES Select: keyboard Shift
- NES Start: keyboard Enter
- NES D-Pad: keyboard arrow keys

## More Information

Coming soon in the wiki!
