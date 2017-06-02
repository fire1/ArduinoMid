//
// Created by Angel Zaprianov on 2.6.2017 г..
//

#ifndef ARDUINO_MID_GRAP_H
#define ARDUINO_MID_GRAP_H

#include <U8g2lib.h>


#ifndef _U8G2LIB_HH
// Some IDE syntax mishmash fixer
#include "../../libraries/U8g2/src/U8g2lib.h"

#endif

double ox , oy ;
/*

 @author Kris Kasprzak

  function to draw a cartesian coordinate system and plot whatever data you want
  just pass x and y and the graph will be drawn

  huge arguement list
  &d name of your display object
  x = x data point
  y = y datapont
  gx = x graph location (lower left)
  gy = y graph location (lower left)
  w = width of graph
  h = height of graph
  xlo = lower bound of x axis
  xhi = upper bound of x asis
  xinc = division of x axis (distance not count)
  ylo = lower bound of y axis
  yhi = upper bound of y asis
  yinc = division of y axis (distance not count)
  title = title of graph
  xlabel = x asis label
  ylabel = y asis label
  gcolor = graph line colors
  acolor = axi ine colors
  pcolor = color of your plotted data
  tcolor = text color
  bcolor = background color
  &redraw = flag to redraw graph on fist call only
*/


void Graph(U8G2 *d, double x, double y, double gx, double gy, double w, double h, double xlo, double xhi, double xinc, double ylo, double yhi, double yinc, String title, String xlabel, String ylabel, boolean &redraw) {

    double ydiv, xdiv;
    // initialize old x and old y in order to draw the first point of the graph
    // but save the transformed value
    // note my transform funcition is the same as the map function, except the map uses long and we need doubles
    //static double ox = (x - xlo) * ( w) / (xhi - xlo) + gx;
    //static double oy = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
    double i;
    double temp;
    int rot, newrot;

    if (redraw == true) {

        redraw = false;
        ox = (x - xlo) * ( w) / (xhi - xlo) + gx;
        oy = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
        // draw y scale
        for ( i = ylo; i <= yhi; i += yinc) {
            // compute the transform
            temp =  (i - ylo) * (gy - h - gy) / (yhi - ylo) + gy;

            if (i == 0) {
                d->drawLine(gx, temp, gx + w, temp);
            }
            else {
                d->drawLine(gx, temp, gx + w, temp);
            }

//            d->setTextSize(1);
//            d->setTextColor(tcolor, bcolor);
//            d->setCursor(gx - 40, temp);
//            // precision is default Arduino--this could really use some format control
//            d->println(i);
        }
        // draw x scale
        for (i = xlo; i <= xhi; i += xinc) {

            // compute the transform

            temp =  (i - xlo) * ( w) / (xhi - xlo) + gx;
            if (i == 0) {
                d->drawLine(temp, gy, temp, gy - h);
            }
            else {
                d->drawLine(temp, gy, temp, gy - h);
            }

//            d->setTextSize(1);
//            d->setTextColor(tcolor, bcolor);
//            d->setCursor(temp, gy + 10);
//            // precision is default Arduino--this could really use some format control
//            d->println(i);
        }

        //now draw the labels
//        d->setTextSize(2);
//        d->setTextColor(tcolor, bcolor);
//        d->setCursor(gx , gy - h - 30);
//        d->println(title);
//
//        d->setTextSize(1);
//        d->setTextColor(acolor, bcolor);
//        d->setCursor(gx , gy + 20);
//        d->println(xlabel);
//
//        d->setTextSize(1);
//        d->setTextColor(acolor, bcolor);
//        d->setCursor(gx - 30, gy - h - 10);
//        d->println(ylabel);


    }

    //graph drawn now plot the data
    // the entire plotting code are these few lines...
    // recall that ox and oy are initialized as static above
    x =  (x - xlo) * ( w) / (xhi - xlo) + gx;
    y =  (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
    d->drawLine(ox, oy, x, y);
    d->drawLine(ox, oy + 1, x, y + 1);
    d->drawLine(ox, oy - 1, x, y - 1);
    ox = x;
    oy = y;

}


#endif //ARDUINOMID_GRAPH_H
