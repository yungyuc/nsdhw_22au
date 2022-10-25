class Vector {
public:
    Vector(double dx, double dy);

    friend double get_angle(Vector const& v1, Vector const& v2);

private:
    double _dx;
    double _dy;
};

double get_angle(Vector const& v1, Vector const& v2);