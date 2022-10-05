#include <vector>

class Line
{
public:
    Line() : coord_(0) {}
    Line(Line const &);
    Line(Line &&);
    Line & operator=(Line const &);
    Line & operator=(Line &&);

    Line(size_t size);
    ~Line() = default;

    size_t size() const { return coord_.size(); }
    const float & x(size_t it) const { return coord_[it][0]; }
    float & x(size_t it) { return coord_[it][0]; }
    const float & y(size_t it) const { return coord_[it][1]; }
    float & y(size_t it) { return coord_[it][1]; }

private:
    std::vector<std::vector<float>> coord_;
};