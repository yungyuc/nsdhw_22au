# include "Line.hpp"

// copy constructor 
Line::Line(Line const & in)
{
   // pass the value 
   p_x = in.p_x;
   p_y = in.p_y;
   p_s = in.p_s;
}

// move constructor 
Line::Line(Line && in)
{
   // using std::move 
   p_x = move(in.p_x);
   p_y = move(in.p_y);
   p_s = move(in.p_s);
}

// copy operator
Line & Line::operator=(Line const & in )
{
   if (this == &in) { return *this; } 
   p_x = in.p_x;
   p_y = in.p_y;
   p_s = in.p_s;
   return *this;
}

// move operator
Line & Line::operator=(Line && in)
{
   p_x = move(in.p_x);
   p_y = move(in.p_y);
   p_s = move(in.p_s);
        
   return *this;
}

// constructor
Line::Line(size_t size){
   p_x.resize(size);
   p_y.resize(size);
   p_s = size;
}

Line::~Line(){
   p_x.clear();
   p_x.shrink_to_fit();

   p_y.clear();
   p_y.shrink_to_fit();
}

size_t Line::size() const{
   return p_s;
}

//getter x
float const & Line::x(size_t it) const{
   return p_x.at(it);
}

//setter x
float & Line::x(size_t it){
   return p_x.at(it);
}

float const & Line::y(size_t it) const{
   return p_y.at(it);
}

float & Line::y(size_t it){
   return p_y.at(it);
}

