#include "GraphicComponents.h"
#include <stdio.h>
#include <string>
#include <ctime>

using namespace GraphicComponents;


Agenda::Agenda (const int window, const int x, const int y): Leaf(window, x, y) {
  _name = new Text(window, x, y, "Employe 1");  
  _hourly = new Table(window, x, y+2, 12, 2);  

  tab t = _hourly->getTab();
  char val[6];

  for(unsigned int i = 7; i <= 18; i++) {
    sprintf(val, "%02d:00", i);
    unsigned int r = i-7;
    _hourly->setValue(r, 1, val);
    _hourly->setValue(r, 2, "vide");
  }
}

Selector Agenda::getDay (){return *_day;}
Selector Agenda::getMonth (){return *_month;}
Selector Agenda::getYear (){return *_year;}
Table Agenda::getHourly (){return *_hourly;}
Text Agenda::getName (){return *_name;}
