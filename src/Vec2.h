#pragma once
#include <math.h>
#include <iostream>

template <class T>
class Vec2 {

private:

	T x, y;

public:

	Vec2(T x_, T y_) : x(x_), y(y_) {};
	Vec2() : x(0), y(0) {};
	Vec2(Vec2& vec_) { x = vec_.x; y = vec_.y; };

	bool operator ==(const Vec2& vec_) const 
	{
		return (x == vec_.x) && (y == vec_.y);

	};

	bool operator !=(const Vec2& vec_) const
	{
		return (x != vec_.x) || (y != vec_.y);
	};

	Vec2 operator +(const Vec2& vec_)
	{
		return Vec2(x + vec_.x, y + vec_.y);
	};

	Vec2 operator -(const Vec2& vec_)
	{
		return Vec2(x - vec_.x, y - vec_.y);
	};

	Vec2 operator /(T number) 
	{
		return Vec2(x / number, y / number);
	};

	Vec2 operator *(T number)
	{
		return Vec(x * number, y * number);
	};
	
	void operator =(const Vec2& vec_)
	{
		x = vec_.x;
		y = vec_.y;
	}

	T lenght()
	{
		return sqrt(((x * x) + (y * y)));	
	};

	void normalize() 
	{
		x /= lenght();
		y /= lenght();

	};

	void reverse(bool x_, bool y_ = false) {
		if (x_) {
			x = -x;
		}
		if (y_) {
			y = -y;
		}
	}

	const std::pair<T, T> getCords() const 
	{
		return std::pair<T, T>(x, y);
	};


};