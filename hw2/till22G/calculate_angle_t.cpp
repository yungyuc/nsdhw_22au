#include<vector>
#include<iostream>
#include<cmath>
#include "calculate_angle.h"

int main(int argc, char* argv[]){

std::vector<float> x{-2, 5}; 
std::vector<float> y{4, 3};

float radians = calculate_angle(x,y);
std::cout << radians << std::endl; 
return 0;
}

