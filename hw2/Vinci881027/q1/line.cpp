#include <iostream>
#include <vector>

using namespace std;

class Line
{
public:
    Line();
    Line(Line const &line) : m_x(line.m_x), m_y(line.m_y){};
    Line(Line &&line) : m_x(move(line.m_x)), m_y(move(line.m_y)){};
    Line &operator=(Line const &);
    Line &operator=(Line &&);
    Line(size_t size) : m_x(size, 0), m_y(size, 0){};
    ~Line()
    {
        m_x.clear();
        m_y.clear();
    };
    size_t size() const { return m_x.size(); };
    float const &x(size_t it) const { return m_x.at(it); };
    float &x(size_t it) { return m_x.at(it); };
    float const &y(size_t it) const { return m_y.at(it); };
    float &y(size_t it) { return m_y.at(it); };

private:
    // Member data.
    vector<float> m_x;
    vector<float> m_y;
}; /* end class Line */

int main(int, char **)
{
    Line line(3);
    line.x(0) = 0;
    line.y(0) = 1;
    line.x(1) = 1;
    line.y(1) = 3;
    line.x(2) = 2;
    line.y(2) = 5;

    Line line2(line);
    line2.x(0) = 9;

    cout << "line: number of points = " << line.size() << endl;
    for (size_t it = 0; it < line.size(); ++it)
    {
        cout << "point " << it << ":"
             << " x = " << line.x(it)
             << " y = " << line.y(it) << endl;
    }

    cout << "line2: number of points = " << line.size() << endl;
    for (size_t it = 0; it < line.size(); ++it)
    {
        cout << "point " << it << ":"
             << " x = " << line2.x(it)
             << " y = " << line2.y(it) << endl;
    }

    return 0;
}
