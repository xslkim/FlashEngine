#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <math.h>
#include "cocos2d.h"

struct Point
{
	double	x, y;

	Point() : x(0), y(0) {}
	Point(double x_, double y_) : x(x_), y(y_) {}

	void	set_lerp(const Point& a, const Point& b, double t)
		// Set to a + (b - a) * t
	{
		x = a.x + (b.x - a.x) * t;
		y = a.y + (b.y - a.y) * t;
	}

	void set_inverse()
	{
		x = -x;
		y = -y;
	}

	bool operator==(const Point& p) const { return x == p.x && y == p.y; }

	void setCocos2dPoint( cocos2d::ccVertex3F& point);

	bool	bitwise_equal(const Point& p) const;

	double get_length() const;

	void twips_to_pixels();
	void pixels_to_twips();
};

struct Matrix
{
	//double	m_[2][3];
	double a, b, c, d;
	double tx, ty;

	static Matrix	identity;

	Matrix();
	Matrix(double aa, double bb, double cc, double dd, double txx, double tyy);
	void	set_identity();
	void	concatenate(const Matrix& m);
	void	concatenate_translation(double tx_, double ty_);
	void	concatenate_scale(double s);
	void	set_lerp(const Matrix& m1, const Matrix& m2, double t);
	void	set_scale_rotation(double x_scale, double y_scale, double rotation);
	void	set_scale(double x_scale, double y_scale);
	void	print() const;
	void	transform(Point* result, const Point& p) const;
	//void	transform(rect* bound) const;
	void	transform_vector(Point* result, const Point& p) const;
	void	transform_by_inverse(Point* result, const Point& p) const;
	//void	transform_by_inverse(rect* bound) const;
	void	set_inverse(const Matrix& m);
	bool	does_flip() const;	// return true if we flip handedness
	double	get_determinant() const;	// determinant of the 2x2 rotation/scale part only
	double	get_max_scale() const;	// return the maximum scale factor that this transform applies
	double	get_x_scale() const;	// return the magnitude scale of our x coord output
	double	get_y_scale() const;	// return the magnitude scale of our y coord output
	double	get_rotation() const;	// return our rotation component (in radians)
	void	calculatePara();
	//double  getBiasX();
	//double  getBiasY();
	//void setScaleBias(double xScale, double yScale, double xBias, double yBias);

	bool operator==(const Matrix& m) const
	{
		return 
			a == m.a &&
			b == m.b &&
			tx == m.tx &&
			c == m.c &&
			d == m.d &&
			ty == m.ty;
	}

	bool operator!=(const Matrix& m) const
	{
		return ! (*this == m);
	}

};

#endif