# include <iostream>
# include <vector>
# include <stddef.h>

using namespace std;
class Line
{
    public:
    Line();
    Line(size_t size);
    Line(Line const & );
    Line(Line       &&);
    Line & operator=(Line const & );
    Line & operator=(Line       &&);
    ~Line();
    size_t size() const;
    float & x (size_t it);
    float const & x (size_t it) const;
    float & y (size_t it);
    float const & y (size_t it) const;

    private:
    vector <float> x_vec;
    vector <float> y_vec;
    size_t len;

};

Line::Line(){
    len = 0;
}

Line::Line(Line const &line){
    x_vec = line.x_vec;
    y_vec = line.y_vec;
    len = line.len;

}

Line::Line(Line &&line){
    x_vec = move(line.x_vec);
    y_vec = move(line.y_vec);
    len = move(line.len);

}




Line::~Line(){
    x_vec.erase(x_vec.begin(), x_vec.end());
    y_vec.erase(y_vec.begin(), y_vec.end());
    x_vec.shrink_to_fit();
    y_vec.shrink_to_fit();
}

float & Line::x(size_t it){
    return x_vec[it];
}

float & Line::y(size_t it){
    return y_vec[it];
}

float const & Line::x(size_t it) const{
    return x_vec[it];
}

float const & Line::y(size_t it) const{
    return y_vec[it];
}

size_t Line::size() const{
    return len;
}

Line::Line(size_t size){
    this->len = size;
    this->x_vec.resize(size, 0);
    this->y_vec.resize(size, 0);
}


Line & Line::operator= (Line const &line){
    this->x_vec = line.x_vec;
    this->y_vec = line.y_vec;
    this->len = line.len;

    return *this;
}

Line & Line::operator= (Line &&line){
    this->x_vec = move(line.x_vec);
    this->y_vec = move(line.y_vec);
    this->len = move(line.len);

    return *this;
}





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