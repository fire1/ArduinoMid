//
// Created by Angel Zaprianov on 4.11.2016 г..
//

#include <iostream>
#include <string>

long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main() {


    int value =(int) map(870, 780, 850, 1, 100);

    std::cout << "TankL: " << value << "% \n";
}