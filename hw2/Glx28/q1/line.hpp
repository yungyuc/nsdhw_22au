#include <cstddef>
#include <cstdlib>
#include <vector>
using namespace std;

class Line {
public:
  Line();
  Line(const Line &ref);
  Line(Line &&ref);
  Line &operator=(Line const &ref);
  Line &operator=(Line &&ref);
  ~Line();
  Line(size_t size);
  size_t size() const;
  float const &x(size_t it) const;
  float &x(size_t it);
  float const &y(size_t it) const;
  float &y(size_t it);

private:
  vector<pair<float, float>> mPoints;
  size_t point_size;
};
