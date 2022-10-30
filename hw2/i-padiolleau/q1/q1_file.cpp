#include <iostream>
#include <cstdint>
#include <stdexcept>
#include <vector>

class Line{ 
public:
    Line(const int size){ //initializer
        for(int i = 0; i<size; i++){
          xl.push_back(0);
          yl.push_back(0);
        };
    };
    int size(){
        return xl.size();
    }
    float & x(int it){
        return xl.at(it);
    }
    float & y(int it){
        return yl.at(it);
    }
private:
    std::vector<float> xl;
    std::vector<float> yl;
};

int main(int, char **)
{
    Line line(3);
    line.x(0) = 0; line.y(0) = 1;
    line.x(1) = 1; line.y(1) = 3;
    line.x(2) = 2; line.y(2) = 5;

    Line line2(line);
    line2.x(0) = 9;

    std::cout << "line: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line.x(it)
                  << " y = " << line.y(it) << std::endl;
    }

    std::cout << "line2: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line2.x(it)
                  << " y = " << line2.y(it) << std::endl;
    }

    return 0;
}