#include <vector>
#include <iostream>
class Line
{
private:
    // Member data.
    std::vector<float> x_positions;
    std::vector<float> y_positions;
public:
    // default constructor
    Line(){};
    // copy constructor
    Line(Line const & old_line){
        x_positions = old_line.x_positions;
        y_positions = old_line.y_positions;    
    }
    // move constructor
    Line(Line       && old_line){
        std::cout << "Moved" << std::endl;
        x_positions = std::move(old_line.x_positions);
        y_positions = std::move(old_line.y_positions);  
    }
    // copy assign operator
    Line & operator=(Line const & old_line){
        // std::cout << "Assign Moved" << std::endl;
        if(&old_line!=this){
            // del the original data
            x_positions.clear();
            x_positions.shrink_to_fit();
            y_positions.clear();
            y_positions.shrink_to_fit();
            
            x_positions = std::move(old_line.x_positions);
            y_positions = std::move(old_line.y_positions);
        }
        return *this;
    }
    // move assign operator
    Line & operator=(Line       && old_line){
        // std::cout << "copy assign" << std::endl;
        x_positions = old_line.x_positions;
        y_positions = old_line.y_positions;
        return *this;
    }
    // constructor
    Line(size_t const size){
        for(size_t i = 0; i < size; i ++){
            x_positions.push_back(0);
            y_positions.push_back(0);
        }
    }
    // destructor
    ~Line(){};
    // size function
    size_t size() const{
        return x_positions.size();
    }
    // const x getter
    float const & x(size_t it) const{
        // std::cout << "read-only x" << std::endl;
        return x_positions.at(it);
    }
    // setter x
    float & x(size_t it){
        // std::cout << "setter x" << std::endl;
        return x_positions.at(it);
    }
    // const y getter
    float const & y(size_t it) const{
        // std::cout << "read-only y" << std::endl;
        return y_positions.at(it);
    }
    // setter y
    float & y(size_t it){
        // std::cout << "setter y" << std::endl;
        return y_positions.at(it);
    }
}; /* end class Line */

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