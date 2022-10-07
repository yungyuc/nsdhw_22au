#include <stdexcept>
#include <vector>
class Line{
    public:
        Line() = default;
        Line(Line const & other) : _point(other._point) {}
        Line(Line && other) : _point(other._point) {}
        Line & operator=(Line const & other) {
            this->_point = other._point;
            return *this;
        }
        Line & operator=(Line && other) {
            this->_point = other._point;
            return *this;
        }
        Line(size_t size){
            _point.resize(size);
        }
        ~Line() {} 
        size_t size() const { return this->_point.size(); }
        float const & x(size_t it) const { this->check_range(it); return _point[it].first;}
        float & x(size_t it) { this->check_range(it); return _point[it].first; }
        float const & y(size_t it) const{ this->check_range(it); return _point[it].second; }
        float & y(size_t it){ this->check_range(it); return _point[it].second; }
    private:
        std::vector<std::pair<float, float> > _point;
        void check_range(size_t it) const {
            if(it >= this->size())
                throw std::out_of_range("Line index out of range");
        }
};
