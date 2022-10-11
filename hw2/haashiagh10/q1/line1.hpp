class Line
{
public:
    Line(size_t size): mPoints(size, {0, 0}) {}

    size_t size() const
    {
        return mPoints.size();
    }

    float const & x(size_t it) const
    {
        assert(mPoints.size() > it && mPoints[it].size() > 0);
        return mPoints[it][0];
    }

    float & x(size_t it)
    {
        assert(mPoints.size() > it && mPoints[it].size() > 0);
        return mPoints[it][0];
    }

    float const & y(size_t it) const
    {
        assert(mPoints.size() > it && mPoints[it].size() > 1);
        return mPoints[it][1];
    }

    float & y(size_t it)
    {
        assert(mPoints.size() > it && mPoints[it].size() > 1);
        return mPoints[it][1];
    }
private:
    std::vector<std::vector<float>> mPoints;
};
