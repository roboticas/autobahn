/* ------------------------------------------------- */

#pragma once

#ifndef VEHICLE_H
#define VEHICLE_H

#include <stdint.h>
#include <Adafruit_NeoPixel.h>
#include "highway.h"

/* ------------------------------------------------- */
class Highway;

class Vehicle {
  private:
    Highway *toHighway;

  protected:
    uint32_t color;
    uint length;
    // value of msCounter in the future to react upon
    uint32_t target; 
    uint32_t speedDelay;
    uint32_t brakeDelay;
    int16_t position;
    boolean active;
    boolean hasPriority;
    boolean braking;
    boolean flash;

  public:
    Vehicle();
    void setup(Highway *toHighway);
    void tick();
    void setBrakeDelay(uint32_t b);
    void setLength(uint l);
    void setSpeedDelay(uint32_t s);
    void setColor(uint32_t c);
    void drawOnHighway();
    int16_t getPosition();
    void setPosition (int16_t newPosition);
    void deactivate();
    boolean isActive();
    void activate();
    void updatePosition(uint32_t msCounter, boolean road[]);
    bool getHasPriority();
    void setHasPriority();
    void clearHasPriority();
    void setFlash(boolean flash);
};

#endif