#pragma once

#include <iostream>

class Point
{
public:
	Point();
	Point(double x, double y, double z);
	~Point();
	Point(const Point& obj);
	Point& operator= (const Point& obj);

	void setPoint(double x, double y, double z);

	void translate(const Point& vector);
	void scalarMulti(double scalar);

	double getX() const;
	double getY() const;
	double getZ() const;

	void read(std::istream& stream);

	void print(std::ostream& stream) const;

private:
	double x;
	double y;
	double z;
};

//////////////////////////////////////////////////////////////

double distance(const Point& p1, const Point& p2);

double scalarMulti(const Point& vec1, const Point& vec2);

Point vecMulti(const Point& vec1, const Point& vec2);

Point vector(const Point& A, const Point& B);

//////////////////////////////////////////////////////////////