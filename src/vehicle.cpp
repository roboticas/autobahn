// Represents a vehicle that can drive on the highway

#include "vehicle.h"
#include <Arduino.h>
#include "highway.h"

const uint32_t BACKLIGHTCOLOR = Adafruit_NeoPixel::Color(2, 0, 0);
const uint32_t BACKLIGHTBRAKECOLOR = Adafruit_NeoPixel::Color(12, 0, 0);
const uint32_t FRONTLIGHTCOLOR = Adafruit_NeoPixel::Color(8, 8, 8);
const uint32_t FLASHHIGHCOLOR = Adafruit_NeoPixel::Color(0, 0, 20);
const uint32_t FLASHLOWCOLOR = Adafruit_NeoPixel::Color(0, 0, 2);
const int MINIMUMSPEEDDELAY = 1000; // ms

Vehicle::Vehicle()
{
    position = 0;
    setLength(length);
    setColor(color);
    setBrakeDelay(0);
    active = false;
    target = 0;
    speedDelay = 500; // default
    hasPriority = false;
}

void Vehicle::setHasPriority() {
    hasPriority = true;
}

void Vehicle::clearHasPriority() {
    hasPriority = false;
}

void Vehicle::setFlash(boolean flash)
{
    this->flash = flash;
}

boolean Vehicle::getHasPriority() {
    return hasPriority;
}

void Vehicle::setLength(uint l) {
    length = l;
}

void Vehicle::setSpeedDelay(uint32_t s)
{
    speedDelay = s;
}

void Vehicle::setBrakeDelay(uint32_t b) {
    brakeDelay = b;
}

void Vehicle::setColor(uint32_t c) {
    color = c;
}

int16_t Vehicle::getPosition()
{
    return position;
}

void Vehicle::setPosition(int16_t newPosition) {
    position = newPosition;
}

boolean Vehicle::isActive() {
    return active;
}

void Vehicle::activate() {
    active = true;
    toHighway->increaseNrOfVehicles();
}

void Vehicle::deactivate() {
    active = false;
    toHighway->decreaseNrOfVehicles();
}

void Vehicle::updatePosition(uint32_t msCounter, boolean road[]) {
    if (brakeDelay > 0) {
        brakeDelay--; // decrease every ms
    }
    if (target < msCounter) {
        target = msCounter + speedDelay + brakeDelay;
        // check if next position is free, if driving on the road
        uint16_t lookaheadPosition = position+length+5;
        if ((lookaheadPosition >=0) && (lookaheadPosition < toHighway->getSize())) {
            if ((road[lookaheadPosition] == false) || (hasPriority)) {
                position = position + 1;
                // Speed up to maximum speed
                if (speedDelay > 50)
                    speedDelay = speedDelay - 3;
                braking = false;
            } else {
                // Slow down a little, we need to break
                speedDelay = speedDelay + 10;
                brakeDelay = 150;
                braking = true;
            } 
        } else
            position = position + 1;

        if (position == 0) {
            toHighway->clearProductionAreaFilled();
        }
        // No need to claim road for priority vehicles
        if (!hasPriority) {
            // claim road space
            if (((position-1) >= 0) && ((position-1) < toHighway->getSize()))
                road[position-1] = false;
            for(int index = position; index < (position+length+2); index++) {
                if ( (index >= 0) && (index < toHighway->getSize()) ) {
                    road[index] = true;
                }
            }
        }
    }
}

void Vehicle::setup(Highway *toHighway) {
    this->toHighway = toHighway; 
}

void Vehicle::tick() {
}

void Vehicle::drawOnHighway() {
    if (length > 0) {
        // draw back lights
        int ledPosition = (int) std::round(position);
        if (braking)
            toHighway->setPixel(ledPosition, BACKLIGHTBRAKECOLOR);
        else
            toHighway->setPixel(ledPosition, BACKLIGHTCOLOR);
        // draw body of vehicle
        if (hasPriority) {
            // draw priority vehicle body with flashlight
            for(int index = 1; index <= (length-1); index++) {
                toHighway->setPixel(ledPosition+index, color);       
            }
            if (flash) {
                toHighway->setPixel(ledPosition+length, FLASHHIGHCOLOR);       
            } else {
                toHighway->setPixel(ledPosition+length, FLASHLOWCOLOR);       
            }
        } else {
            for(int index = 1; index <= length; index++) {
                toHighway->setPixel(ledPosition+index, color);       
            }
        }
        // draw head lights
        toHighway->setPixel(ledPosition+length+1, FRONTLIGHTCOLOR);
    }
}