#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <math.h>
#include "cocos2d.h"


struct MyPoint
{
	float	x, y;

	MyPoint() : x(0), y(0) {}
	MyPoint(float x_, float y_) : x(x_), y(y_) {}

	void	set_lerp(const MyPoint& a, const MyPoint& b, float t)
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

	bool operator==(const MyPoint& p) const { return x == p.x && y == p.y; }

	void setCocos2dPoint( cocos2d::Vec3& point);

	bool	bitwise_equal(const MyPoint& p) const;

	float get_length() const;

	void twips_to_pixels();
	void pixels_to_twips();
};

struct Matrix
{
	//float	m_[2][3];
	float a, b, c, d;
	float tx, ty;

	static Matrix	identity;

	Matrix();
	Matrix(float aa, float bb, float cc, float dd, float txx, float tyy);
	void	set_identity();
	void	concatenate(const Matrix& m);
	void	concatenate_translation(float tx_, float ty_);
	void	concatenate_scale(float s);
	void	set_lerp(const Matrix& m1, const Matrix& m2, float t);
	void	set_scale_rotation(float x_scale, float y_scale, float rotation);
	void	set_scale(float x_scale, float y_scale);
	void	print() const;
	void	transform(MyPoint* result, const MyPoint& p) const;
	//void	transform(rect* bound) const;
	void	transform_vector(MyPoint* result, const MyPoint& p) const;
	void	transform_by_inverse(MyPoint* result, const MyPoint& p) const;
	//void	transform_by_inverse(rect* bound) const;
	void	set_inverse(const Matrix& m);
	bool	does_flip() const;	// return true if we flip handedness
	float	get_determinant() const;	// determinant of the 2x2 rotation/scale part only
	float	get_max_scale() const;	// return the maximum scale factor that this transform applies
	float	get_x_scale() const;	// return the magnitude scale of our x coord output
	float	get_y_scale() const;	// return the magnitude scale of our y coord output
	float	get_rotation() const;	// return our rotation component (in radians)
	void	calculatePara();
	//float  getBiasX();
	//float  getBiasY();
	//void setScaleBias(float xScale, float yScale, float xBias, float yBias);

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