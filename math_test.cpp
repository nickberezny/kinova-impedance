#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <cmath>   

const double PI = 3.14159265358979323846;

int main() {


    double q0 = 340.0; //change to joint limit (avoid collisions!)
    double q1 = 380.0;
    double rate = 0.001; //rad/s
    double A = (q1-q0)/2.0;
    double A0 = (q1+q0)/2.0;
    double qd = A0;

    for(int i = 0; i < 100; i++)
    {
        std::cout << fmod(A*std::sin(rate*(double)i) + A0, 360.0f) << std::endl;
    }

    

}