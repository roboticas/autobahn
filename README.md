# autobahn
Visual highway simulation using a ledstrip and the esp32 mini microcontroller
This is the highway project (Autobahn). Concept developped by Ronald Lokker.
It represents a highway at night with all the cars wizzing along at different speeds.
A led strip is used where each car has one white led as headlight, some coloured leds for body and a red tail light. This tail light is also a brakelight when the car needs to brake.
At some random time interval a police priority car passes the regular cars with a blinking blue flashlight.
The whole system has a cycle of about one minute in which the number of cars increases and then decreases repeatedly.

It consists of an ESP32 mini microcontroller connected to a LED strip of 145 neopixels.
Make sure you supply the led strip with a large enough 5v powersupply for your neopixels.
One full lit neopixel uses 60ma, so estimate how many leds are on at the time.
The prototype uses a 3A 5v powersupply.

The used led strip has three colored wires. The red wire is the 5v, the white wire is the ground and the green wire is the data input that should be connected to the pin 18 of the ESP32 mini.

The software needs the adafruit neopixel library: https://github.com/adafruit/Adafruit_NeoPixel
The development environment used is Visual Code : https://code.visualstudio.com/
with the platformio extension.
Search for the extension name 'PlatformIO IDE' on : https://marketplace.visualstudio.com/search?term=platformio&target=VSCode&category=All+categories&sortBy=Relevance

It probably is also very possible to get this running in the Arduino IDE. Just make sure you have the neopixel library setup in the Arduino library manager. 
Remove the '#include <Arduino.h>' line if you use the Arduino IDE. 
