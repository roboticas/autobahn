#include <Arduino.h>
#include "highway.h"
#include "factory.h"

static Highway highway;
static Factory factory;

void setup() {
  factory.setup(&highway);
  highway.setup(&factory);
}

void loop() {
  factory.tick();
  highway.tick();

  delay(10);
}

