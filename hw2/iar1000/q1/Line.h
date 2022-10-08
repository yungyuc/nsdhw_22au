#include <stddef.h>
#include <string>
#include <vector>
#include <limits>

class Line
  {
  public:
      // Basic constructors.
      Line(); // default constructor.
      Line(const size_t size);
      size_t size() const;
      float x(size_t it) const;
      float & x(size_t it);
      float y(size_t it) const;
      float & y(size_t it);
  private:
      std::vector<float> xs;
      std::vector<float> ys;
  }; 
