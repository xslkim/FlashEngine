//#include "utils.h"
#include "Matrix.h"
#include "FlashUtils.h"

#define FLATE_PIXEL_RATE 20.f
#define TWIPS_TO_PIXELS(x)	((x) / FLATE_PIXEL_RATE)
#define PIXELS_TO_TWIPS(x)	((x) * FLATE_PIXEL_RATE)

bool	Point::bitwise_equal(const Point& p) const
	// Bitwise comparison; return true if *this is bitwise
	// identical to p.
{
	return memcmp(this, &p, sizeof(p)) == 0;
}

float Point::get_length() const
{
	return sqrt( x * x + y * y );
}

void Point::twips_to_pixels()
{
	x = TWIPS_TO_PIXELS(x);
	y = TWIPS_TO_PIXELS(y);
}

void Point::pixels_to_twips()
{
	x = PIXELS_TO_TWIPS(x);
	y = PIXELS_TO_TWIPS(y);
}

void Point::setCocos2dPoint( cocos2d::ccVertex3F& point)
{
	point.x = x;
	point.y = y;
	point.z = 0.f;
}


//
// matrix
//

Matrix	Matrix::identity;

Matrix::Matrix()
{
	// Default to identity.
	set_identity();
}

Matrix::Matrix(float aa, float bb, float cc, float dd, float txx, float tyy)
{
	a = aa;
	b = bb;
	c = cc;
	d = dd;
	tx = txx;
	ty = tyy;
}


//	bool	matrix::is_valid() const
//	{
//		return isfinite(a)
//			&& isfinite(b)
//			&& isfinite(tx)
//			&& isfinite(c)
//			&& isfinite(d)
//			&& isfinite(ty);
//	}


void	Matrix::set_identity()
	// Set the matrix to identity.
{
	//memset(&m_[0], 0, sizeof(m_));
	a = 1;
	b = 0;
	c = 0;
	d = 1;
	tx = 0;
	ty = 0;
}


void	Matrix::concatenate(const Matrix& m)
	// Concatenate m's transform onto ours.  When
	// transforming points, m happens first, then our
	// original xform.
{
	Matrix	t;
	//t.a = a * m.a + b * m.c;
	//t.b = a * m.b + b * m.d;
	//t.c = c * m.a + d * m.c;
	//t.d = c * m.b + d * m.d;
	////t.tx = a * m.tx + b * m.ty + tx;
	////t.ty = c * m.tx + d * m.ty + ty;
	//t.tx = tx*m.a + ty*m.c + m.tx;
	//t.ty = tx*m.b + ty*m.d + m.ty;

	t.a = m.a * a + m.b * c;
	t.b = m.a * b + m.b * d;
	t.c = m.c * a + m.d * c;
	t.d = m.c * b + m.d * d;
	t.tx = m.tx*a + m.ty*c + tx;
	t.ty = m.tx*b + m.ty*d + ty;

	

	*this = t;
}


void	Matrix::concatenate_translation(float tx_, float ty_)
	// Concatenate a translation onto the front of our
	// matrix.  When transforming points, the translation
	// happens first, then our original xform.
{
	tx += a * tx_ + b * ty_;
	ty += c * tx_ + d * ty_;
}


void	Matrix::concatenate_scale(float scale)
	// Concatenate a uniform scale onto the front of our
	// matrix.  When transforming points, the scale
	// happens first, then our original xform.
{
	a *= scale;
	b *= scale;
	c *= scale;
	d *= scale;
}


void	Matrix::set_lerp(const Matrix& m1, const Matrix& m2, float t)
	// Set this matrix to a blend of m1 and m2, parameterized by t.
{
	/*a = flerp(m1.a, m2.a, t);
	c = flerp(m1.c, m2.c, t);
	b = flerp(m1.b, m2.b, t);
	d = flerp(m1.d, m2.d, t);
	tx = flerp(m1.tx, m2.tx, t);
	ty = flerp(m1.ty, m2.ty, t);*/
}


void	Matrix::set_scale(float x_scale, float y_scale)
{
	a = x_scale;
	b = 0;
	c = 0;
	d = y_scale;
}


void	Matrix::print() const
	// Debug log.
{
	//debug_out("| %4.4f %4.4f %4.4f |\n", float(a), float(b), tx);
	//debug_out("| %4.4f %4.4f %4.4f |\n", float(c), float(d), ty);
}

void	Matrix::transform(Point* result, const Point& p) const
	// Transform point 'p' by our matrix.  Put the result in
	// *result.
{
	assert(result);
	assert(&p != result);

	result->x = a * p.x + c * p.y + tx;
	result->y = b * p.x + d * p.y + ty;
}

//void	Matrix::transform(rect* bound) const
//	// Transform bound our matrix.
//{
//	// get corners of transformed bound
//	point p[4];
//	transform(p + 0, point(bound->get_corner(0)));
//	transform(p + 1, point(bound->get_corner(1)));
//	transform(p + 2, point(bound->get_corner(2)));
//	transform(p + 3, point(bound->get_corner(3)));
//
//	// Build bound that covers transformed bound
//	bound->set_to_point(p[0]);
//	bound->expand_to_point(p[1]);
//	bound->expand_to_point(p[2]);
//	bound->expand_to_point(p[3]);
//}

void	Matrix::transform_vector(Point* result, const Point& v) const
	// Transform vector 'v' by our matrix. Doesn't apply translation.
	// Put the result in *result.
{
	assert(result);
	assert(&v != result);

	result->x = a * v.x + c * v.y;
	result->y = b * v.x + d * v.y;
}

void	Matrix::transform_by_inverse(Point* result, const Point& p) const
	// Transform point 'p' by the inverse of our matrix.  Put result in *result.
{
	// @@ TODO optimize this!
	Matrix	m;
	m.set_inverse(*this);
	m.transform(result, p);
}

//void	Matrix::transform_by_inverse(rect* bound) const
//	// Transform point 'p' by the inverse of our matrix.  Put result in *result.
//{
//	// @@ TODO optimize this!
//	matrix	m;
//	m.set_inverse(*this);
//	m.transform(bound);
//}

void	Matrix::set_inverse(const Matrix& m)
	// Set this matrix to the inverse of the given matrix.
{
	assert(this != &m);

	// Invert the rotation part.
	float	det = m.a * m.d - m.b * m.c;
	if (det == 0.0f)
	{
		// Not invertible.
		//assert(0);	// castano: this happens sometimes! (ie. sample6.swf)

		// Arbitrary fallback.
		set_identity();
		tx = -m.tx;
		ty = -m.ty;
	}
	else
	{
		float	inv_det = 1.0f / det;
		a = m.d * inv_det;
		d = m.a * inv_det;
		b = -m.b * inv_det;
		c = -m.c * inv_det;

		tx = -(a * m.tx + b * m.ty);
		ty = -(c * m.tx + d * m.ty);
	}
}


bool	Matrix::does_flip() const
	// Return true if this matrix reverses handedness.
{
	float	det = a * d - b * c;

	return det < 0.f;
}


float	Matrix::get_determinant() const
	// Return the determinant of the 2x2 rotation/scale part only.
{
	return a * d - c * b;
}


float	Matrix::get_max_scale() const
	// Return the maximum scale factor that this transform
	// applies.  For assessing scale, when determining acceptable
	// errors in tesselation.
{
	// @@ not 100% sure what the heck I'm doing here.  I
	// think this is roughly what I want; take the max
	// length of the two basis vectors.
	float	basis0_length2 = a * a + b * b;
	float	basis1_length2 = c * c + d * d;
	float	max_length2 = fmax(basis0_length2, basis1_length2);
	return sqrtf(max_length2);
}

float	Matrix::get_x_scale() const
{
	float scale = sqrtf(a * a + c * c);
	if (get_determinant() < 0.f)
	{
		scale = -scale;
	}
	return scale;

	//Point srcPoint;
	//srcPoint.x = 1.f;
	//srcPoint.y = 0.f;
	//Point destPoint;
	//this->transform_vector(&destPoint, srcPoint);
	//float scaleX = sqrt( destPoint.x*destPoint.x + destPoint.y*destPoint.y);
	//if( destPoint.x < 0 )
	//{
	//	scaleX = -scaleX;
	//}
	//return scaleX;

}

float	Matrix::get_y_scale() const
{
	return sqrtf(d * d + b * b);
}

void	Matrix::set_scale_rotation(float x_scale, float y_scale, float angle)
	// Set the scale & rotation part of the matrix.
	// angle in radians.
{
	float	cos_angle = cos(angle);
	float	sin_angle = sin(angle);
	a = x_scale * cos_angle;
	b = y_scale * sin_angle;
	c = x_scale * -sin_angle;
	d = y_scale * cos_angle;
}

float	Matrix::get_rotation() const
{
	if (get_determinant() < 0.f)
	{
		float rotation = atan2(b, -a);
		return rotation;
	}
	else
	{
		float rotation = atan2(b, a);
		return rotation;
	}

	//Point srcPoint;
	//srcPoint.x = 1.f;
	//srcPoint.y = 0.f;
	//Point destPoint;
	//this->transform_vector(&destPoint, srcPoint);
	//float destRotate = 0;
	//if(destPoint.x == 0)
	//{
	//	if(destPoint.y > 0)
	//		destRotate = M_PI_2;
	//	else if(destPoint.y < 0)
	//		destRotate = M_PI_2 + M_PI;
	//	else
	//		CCAssert(false, "");
	//}
	//else
	//{
	//	if( destPoint.y == 0)
	//	{
	//		if( destPoint.x > 0 )
	//			destRotate = 0;
	//		else
	//			destRotate = M_PI;
	//	}
	//	else
	//	{
	//		destRotate = atan(destPoint.y/destPoint.x);
	//		//if( destPoint.x < 0 && destPoint.y > 0) //2ÏóÏÞ
	//		//	destRotate += M_PI;
	//		//else if(destPoint.x < 0 && destPoint.y < 0)
	//		//	destRotate += M_PI;
	//		//else if(destPoint.x > 0 && destPoint.y < 0)
	//		//	destRotate += M_PI*2;
	//	}
	//	
	//}

	//return destRotate;
}

void	Matrix::calculatePara()
{

}

//float  Matrix::getBiasX()
//{
//	Point xTest(1, 0);
//	Point xTestOut;
//	transform_vector(&xTestOut, xTest);
//	float xAngleChange = getAngle(xTestOut);
//	float xAngleChangeA = PiToAngle(xAngleChange);
//	return xAngleChange;
//}
//
//
//float  Matrix::getBiasY()
//{
//	Point yTest(0, 1);
//	Point yTestOut;
//	transform_vector(&yTestOut, yTest);
//	float yAngle = getAngle(yTestOut);
//	float yAngleA = PiToAngle(yAngle);
//	float yAngleChange = yAngle - M_PI_2;
//	if( yAngleChange < - M_PI)
//	{
//		yAngleChange = yAngleChange + (2*M_PI);
//	}
//	float yAngleChangeA = PiToAngle(yAngleChange);
//	return yAngleChange;
//}
//
//
//void Matrix::setScaleBias(float xScale, float yScale, float xBias, float yBias)
//{
//	Matrix matX, matY;
//	matX.a = cos(xBias);
//	matX.b = -sin(xBias);
//
//	matY.c = sin(yBias);
//	matY.d = cos(yBias);
//
//	Matrix biasMatrix = matX;
//	biasMatrix.c = matY.c;
//	biasMatrix.d = matY.d;
//	//biasMatrix.print();
//
//	Matrix scaleMatrix;
//	scaleMatrix.set_scale(abs(xScale), abs(yScale));
//
//	Matrix outMatrix = biasMatrix;
//	outMatrix.tx = tx;
//	outMatrix.ty = ty;
//	outMatrix.concatenate( scaleMatrix );
//	*this = outMatrix;
//
//}
