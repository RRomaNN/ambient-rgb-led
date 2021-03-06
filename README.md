# Ambient backlight with addressable RGB strip [WIP]

This is a **prototype** of the LED backlight controller.

in short:

you can upload an image (ie pattern) from the computer to the device. The pattern will be read row by row by the controller and rows will be sequentially displayed on the addressable LED strip using the selected color schema with preselected speed and smooth color transitions between the rows.

Features:

- Supports up to 16 patterns, stored in 4x AT24C256 EEPROMs. You can interactively select one of them at a time.
- Supports up to 32 custom color schemas. You can create/edit/save them to EEPROM directly on the device using buttons, LCD and the short *preview* led strip.
- Supports different pattern playback speeds. From ~3 smooth transitions/second to 1 transition per 2 minutes. You can select the one you need.
- Measures amperage of the LED strip and displays it on the LCD in the operating mode. 
- TODO: Add feature, which allows limiting max current draw by LED strip to prevent damage of DC power supply / PCB.

- Load current - up to 10A.
- Distance between the controller and LED strip - up to 5 meters (the device contains repeater of the output digital signal)
- Protection from the minor static discharges over +5V, DOUT, GND lines (no TVS diodes though)

Schema:

![alt text](https://github.com/RRomaNN/ambient-rgb-led/blob/main/readme/schema.png?raw=true)

PCB front view:

![alt text](https://github.com/RRomaNN/ambient-rgb-led/blob/main/readme/pcb_front.png?raw=true)

PCB back view:

![alt text](https://github.com/RRomaNN/ambient-rgb-led/blob/main/readme/pcb_back.png?raw=true)

Firmware:

[controller.ino](https://github.com/RRomaNN/ambient-rgb-led/blob/main/controller_c/controller.ino)



