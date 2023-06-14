/* ------------------------------------------------- */

#pragma once

#ifndef FACTORY_H
#define FACTORY_H

#include <stdint.h>
#include <Adafruit_NeoPixel.h>
#include "highway.h"
#include "vehicle.h"

/* ------------------------------------------------- */

class Highway;
class Vehicle;

class Factory {
  private:
    Highway *toHighway;
    int ticker;

    Vehicle *findAvailableVehicle();
    boolean makeVehicle();

  public:
    Factory();
    void setup(Highway *toHighway);
    void tick();
    void recyle(Vehicle *toVehicle);
};

#endif