#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <iostream>
#include <wiringPi.h>
#include <wiringSerial.h>

class Uart
{
    int serialDeviceId = 0;
    int pin = 7;

public:

    Uart() {
        using namespace std;
        serialDeviceId = serialOpen("/dev/ttyACM1",9600);
        if(serialDeviceId == -1) {
             cout << "Unable to open serial device" << endl;
        }
        if(wiringPiSetup() == -1) {
        }
        pinMode(pin,OUTPUT); // designing pin as an output
    }

    void test() {
        digitalWrite(pin, 0);
        delay(500);
        digitalWrite(pin, 1);
        delay(500);
    }
};

#endif // SERIAL_PORT_H
