/* ------------------------------------------------- */

#pragma once

#ifndef HIGHWAY_H
#define HIGHWAY_H

#include <stdint.h>
#include <Adafruit_NeoPixel.h>
#include <list>
#include "factory.h"
#include "vehicle.h"

/* ------------------------------------------------- */
class Vehicle;
class Factory;

const int MAXVEHICLES=25;
const int NUMLEDS=145;
const int LEDPIN =18;

class Highway {
  private:
    Adafruit_NeoPixel *toPixels;
    Factory *toFactory;
    boolean productionAreaFilled = false;
    int nrOfVehicles;

  public:
    Highway();
    void setup(Factory *toF);
    void tick();
    void addVehicle(Vehicle *toVehicle);
    void setPixel(int index, uint32_t color);
    int getNrOfVehicles();
    Vehicle *findAvailableVehicle();
    void setProductionAreaFilled();
    void clearProductionAreaFilled();
    boolean isProductionAreaFilled();
    void decreaseNrOfVehicles();
    void increaseNrOfVehicles();
    int getSize();
    uint32_t getMsCounter();
};

#endif