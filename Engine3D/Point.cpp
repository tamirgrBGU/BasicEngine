#include "Point.h"
#include <math.h>
double Point::Distance(Point point)
{
    return sqrt(pow((this->x - point.x),2) +  pow((this->y - point.y),2) + pow((this->z - point.z),2)  );
}

double Point::DistanceX(Point point)
{
    return sqrt(pow((this->x - point.x), 2));
}

double Point::DistanceY(Point point)
{
    return sqrt(pow((this->y - point.y), 2));
}

double Point::DistanceZ(Point point)
{
    return sqrt(pow((this->z - point.z), 2));
}

Point Point::Normalised()
{
    double magnitude = Distance(Point(0, 0, 0));
    return Point(x/magnitude, y/magnitude, z/magnitude);
}

Point Point::Scale(double scale)
{
    return Point(x*scale, y*scale, z*scale);
}

Point Point::Add(Point point)
{
    return Point(x+point.x, y+point.y, z+point.z);
}

Point Point::Sub(Point point)
{
    return Point(x-point.x, y-point.y, z-point.z);
}

double Point::Dot(Point point)
{
    return x*point.x + y*point.y + z*point.z;
}

double Point::Magnitude()
{
    return Distance(Point(0,0,0));
}

