#include <vector>
#include <cmath>

namespace dimension2
{

const float PI = 3.14159265;

float dot(const std::vector<float>& vec1, const std::vector<float>& vec2);
float len_of(const std::vector<float>& vec);
float angle_between(const std::vector<float>& vec1, const std::vector<float>& vec2);

}
