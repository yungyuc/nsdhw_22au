#include <iostream>
#include <vector>
using namespace std;

class Line {
public:
    Line() = default;
    Line(Line const&) = default;
    Line(Line&&) = default;
    Line& operator=(Line const&) = default;
    Line& operator=(Line&&) = default;
    ~Line() = default;

    Line(size_t size) {
        _x.resize(size);
        _y.resize(size);
    }

    size_t size() const { 
        return _x.size(); 
    }

    float& x(size_t it) {
        _check_safety(it);
        return _x[it];
    }

    float const& x(size_t it) const {
        _check_safety(it);
        return _x[it];
    }

    float& y(size_t it) {
        _check_safety(it);
        return _y[it];
    }

    float const& y(size_t it) const {
        _check_safety(it);
        return _y[it];
    }

private:
    vector<float> _x, _y;

    bool _check_safety(size_t it) const {
        if (it < _x.size())
            return true;
        throw out_of_range("Index out of range.");
    }
}; 

int main(int, char **) {
    Line line(3);
    line.x(0) = 0; line.y(0) = 1;
    line.x(1) = 1; line.y(1) = 3;
    line.x(2) = 2; line.y(2) = 5;

    Line line2(line);
    line2.x(0) = 9;

    std::cout << "line: number of points = " << line.size() << std::endl;
    for (size_t it = 0; it < line.size(); it++) {
        std::cout << "point " << it << ":"
                  << " x = " << line.x(it)
                  << " y = " << line.y(it) << std::endl;
    }

    std::cout << "line2: number of points = " << line.size() << std::endl;
    for (size_t it = 0; it < line.size(); it++) {
        std::cout << "point " << it << ":"
                  << " x = " << line2.x(it)
                  << " y = " << line2.y(it) << std::endl;
    }

    return 0;
}
