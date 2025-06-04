#include "GraphicComponents.h"
#include <stdio.h>
#include <string>
#include <ctime>

using namespace GraphicComponents;


Calendar::Calendar (const int window, const int x, const int y): Leaf(window, x, y) {
  time_t t1 = time(0); 
  tm* now = localtime(&t1);

  string Months[12] = {"Janvier", "Fevrier", "Mars", "Avril", "Mai", "Juin", "Juillet", "Aout", "Septembre", "Octobre", "Novembre", "Decembre"};

  int iyear = now->tm_year+1900;
  char s[5] = "0000";
  sprintf(s, "%d", iyear);
  string str = s;
  unsigned int imonth = now->tm_mon;
  unsigned int monthday = now->tm_mday;
  unsigned int startweekday = now->tm_wday-now->tm_mday%7;

  //_month = new Selector(window, x, y, "SEPTEMBRE");
  _month = new Selector(window, x, y, Months[imonth]);
  _year = new Selector(window, x+7, y, str);
  _daily = new Table(window, x, y+2, 7, 7);  // le mois de juin 2025 a 7 lignes
  _daily->setValue(0, 1, "Lu");
  _daily->setValue(0, 2, "Ma");
  _daily->setValue(0, 3, "Me");
  _daily->setValue(0, 4, "Je");
  _daily->setValue(0, 5, "Ve");
  _daily->setValue(0, 6, "Sa");
  _daily->setValue(0, 7, "Di");
  tab t = _daily->getTab();
  unsigned int cols = t.cols;
  char val[3];
  for(unsigned int i = 1; i <= 31; i++) {
    sprintf(val, "%02d", i);
    unsigned int r = (startweekday+i)/cols;
    unsigned int c = (startweekday+i)%cols;
    _daily->setValue(2+r, c, val);
    if(i==monthday)
      _daily->select(2+r, c);
  }
}

Selector Calendar::getMonth () {
  return *_month;
}

Selector Calendar::getYear () {
  return *_year;
}

Table Calendar::getDaily () {
  return *_daily;
}
