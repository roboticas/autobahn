// Makes vehicles

#include "factory.h"
#include "highway.h"
#include "vehicle.h"

#include <Arduino.h>

const int PRODUCTIONRATE=5;

Vehicle *Factory::findAvailableVehicle() {
    return toHighway->findAvailableVehicle();
}

// Returns true if vehicle has been produced
boolean Factory::makeVehicle()
{
    boolean result = false;

    // Determine how many vehicles are allowed to be created at this moment
    float a = ((float) ((toHighway->getMsCounter() & 0xffff))/(float) 0xffff)*2*PI;
    float s = sin(a)*0.4;
    
    int maxNrOfVehicles = (MAXVEHICLES/2.0)+(int) (MAXVEHICLES*s);
    if ( (toHighway->getNrOfVehicles() < maxNrOfVehicles) && (!toHighway->isProductionAreaFilled()) ){
        uint32_t  color;
        int carLength;
        int maxSpeed;    
        int acceleration;
        int speedDelay;
        uint8_t r;
        uint8_t g;
        uint8_t b;

        Vehicle *toVehicle = findAvailableVehicle();
        if (toVehicle != NULL) {
            if ((rand() % 10) > 8) {
                // Make priority vehicle
                color = Adafruit_NeoPixel::Color(0, 0, 5);
                carLength = 3;
                speedDelay = 30;
                toVehicle->setHasPriority();
            } else {
                // Make regular vehicle
                r = (rand() % 5)+1;
                g = (rand() % 5)+1;
                b = (rand() % 2);
                color = Adafruit_NeoPixel::Color(r, g, b);
                carLength = (rand() % 1)+2;
                speedDelay = 60-(rand() % 30);
                toVehicle->clearHasPriority();
            }

            toVehicle->setLength(carLength);
            toVehicle->setSpeedDelay(speedDelay);
            toVehicle->setPosition(-(carLength+1));
            toVehicle->setColor(color);
            toVehicle->activate();

            toHighway-> setProductionAreaFilled();
            result = true;
        }
    }
    return result;
}

Factory::Factory()
{
}

void Factory::setup(Highway *toHighway) {
    this->toHighway = toHighway;
}

void Factory::tick() {
    if (ticker > 0) {
        ticker--;
    } else {
        if (makeVehicle()) {
            ticker = PRODUCTIONRATE*((rand() % 5)+1);
        }
    }
}

void Factory::recyle(Vehicle *toVehicle)
{
    toVehicle->deactivate();
}
