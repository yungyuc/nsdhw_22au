#include<vector>
#include<iostream>
#include<cmath>

float const calculate_magnitude(std::vector<float> &x);
float const calculate_angle(std::vector<float> &x, std::vector<float> &y);

int main(int argc, char* argv[]){

std::vector<float> x{-2, 5}; 
std::vector<float> y{4, 3};

float degree = calculate_angle(x,y);

std::cout << degree << std::endl;

return 0;
}

float const calculate_angle (std::vector<float> &x, std::vector<float> &y) {

	// ceck dimensions of vectors
	if ( x.size() != x.size()){
		std::cout << "Size does not match, returned 0" << std::endl;
		return 0;	
   	}	

	// calculate dot product
	float dot_prod = 0;
	for(int i = 0; i < x.size(); i++){
		dot_prod += x[i] * y[i]; 
	}
	
	// calculate magnitude
	float mag_x = calculate_magnitude(x);	
	float mag_y = calculate_magnitude(y);

	// calculate radiance
	float tmp = dot_prod / (mag_x * mag_y);

	// c++ acos returns radiance by default
	return acos((tmp));
}

float const calculate_magnitude(std::vector<float> &x){
	
	float magnitude_sq = 0;
	int size =x.size();
	
	if(size == 0) {
		return 0;
	}

	for(int i = 0; i < size; i++){
		magnitude_sq += pow(x[i], 2); 
	}	

	return sqrt(magnitude_sq);
	}
