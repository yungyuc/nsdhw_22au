#include <pybind11/stl.h>
#include <cmath>
using namespace std;

double angle(vector <double> v1, vector <double> v2){

    double dot = v1[0] * v2[0] + v1[1] * v2[1];
    double len_v1 = sqrt(v1[0] * v1[0] + v1[1] * v1[1] );
    double len_v2 = sqrt(v2[0] * v2[0] + v2[1] * v2[1]);
    
    if (len_v1 == 0.0 || len_v2 == 0.0 )
    {
        throw invalid_argument("error: input zero vector");
    }
    
    return acos(dot / (len_v1 * len_v2)) ;

}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "pybind11 - use vector to do caculation"; 
    m.def("angle", &angle, "A function that caculate angle between two vectors");
}
