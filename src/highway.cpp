// Represents the highway to which vehicles can be added and shown on the ledstrip

#include "highway.h"

#include <Arduino.h>
#include "vehicle.h"
#include "factory.h"

static hw_timer_t *Timer0_Cfg = NULL;

const int LED    = 19;
const int LEDOFF = 0;
const int LEDON  = 1;

static volatile uint32_t msCounter = 0L;
static Vehicle  vehicles[MAXVEHICLES];
static boolean road[NUMLEDS];
static boolean flash;

// 1ms interrupt handler
void IRAM_ATTR Timer0_ISR()
{
  msCounter++;
  // Blink green LED every second 
  if ((msCounter % 1000) == 0) {
    digitalWrite(LED, !digitalRead(LED));
  }
  // flashlight frequency
  flash = ((msCounter & 0x100) == 0);

  // Update position for each active vehicle
  for (int index = 0; index < MAXVEHICLES; index++) {
    Vehicle *toVehicle = &vehicles[index];
    if (toVehicle->isActive()) {
        toVehicle->updatePosition(msCounter, road);
        toVehicle->setFlash(flash);
        // check if vehicle should get deactivated at end of highway
        if (toVehicle->getPosition() >= NUMLEDS) {
            toVehicle->deactivate();
        }
    }
  }
}

static void initializeTimer() {
  // Initialise timer to 1ms resolution
  Timer0_Cfg = timerBegin(0, 80, true);
  timerAttachInterrupt(Timer0_Cfg, &Timer0_ISR, true);
  // Timer 1 ms = 1000
  timerAlarmWrite(Timer0_Cfg, 1000, true);
  timerAlarmEnable(Timer0_Cfg);
}

Highway::Highway()
{
    // debug green LED
    pinMode(LED, OUTPUT);

    initializeTimer();
}

uint32_t Highway::getMsCounter() {
    return msCounter;
}

Vehicle *Highway::findAvailableVehicle() {
    Vehicle *toVehicle = NULL;

    for (int index = 0; index < MAXVEHICLES; index++) {
        Vehicle *toSearchVehicle = &vehicles[index];
        if (toSearchVehicle->isActive() == false) {
            toVehicle = toSearchVehicle;
        }
    }

    return toVehicle;
}

void Highway::setProductionAreaFilled()
{
    productionAreaFilled = true;
}

void Highway::clearProductionAreaFilled()
{
    productionAreaFilled = false;
}

boolean Highway::isProductionAreaFilled()
{
    return productionAreaFilled;
}

void Highway::increaseNrOfVehicles()
{
    nrOfVehicles++;
}

int Highway::getSize()
{
    return NUMLEDS;
}

void Highway::decreaseNrOfVehicles()
{
    nrOfVehicles--;
}

void Highway::setup(Factory *toF) {
    nrOfVehicles = 0;

    toPixels = new Adafruit_NeoPixel(NUMLEDS, LEDPIN, NEO_GRB + NEO_KHZ800);
    toPixels->begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

    toPixels->clear();
    toPixels->show();

    toFactory = toF;

    // clear the road
    uint32_t BLACK = Adafruit_NeoPixel::Color(0,0,0);
    for(int index = 0; index < NUMLEDS; index++) {
        road[index] = false;
        toPixels->setPixelColor(index, BLACK);
    }

    // Inform vehicles of this highway
    for (int index = 0; index < MAXVEHICLES; index++) {
        Vehicle *toVehicle = &vehicles[index];
        toVehicle->setup(this);
    }
}

void Highway::tick() {
    toPixels->clear();
    for (int index = 0; index < MAXVEHICLES; index++) {
        Vehicle *toVehicle = &vehicles[index];
        toVehicle->drawOnHighway();
    }
    toPixels->show();
}

void Highway::setPixel(int index, uint32_t color) {
    if ((index >= 0) && (index < toPixels->numPixels())) {
        toPixels->setPixelColor(index, color);
    }
}

int Highway::getNrOfVehicles()
{
    return nrOfVehicles;
}
