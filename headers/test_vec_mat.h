#pragma once
#ifndef TEST_VEC_MAT_H
#define TEST_VEC_MAT_H

#include"STD_commons.h"

namespace vm {


	struct vec4 {
	private:
		
	public:
		float x, y, z, w;
		vec4();
		vec4(float _all);
		vec4(float _x, float _y, float _z);
		vec4(float _x, float _y, float _z, float _w);
		vec4(const vec4& _v);
		~vec4();

		vec4& operator= (float _all);
		vec4& operator= (const vec4& _v);

		vec4& operator+=(float _scalar);
		vec4& operator+=(const vec4& _v);

		vec4& operator-=(float _scalar);
		vec4& operator-=(const vec4& _v);

		vec4& operator*=(float _scalar);
		vec4& operator*=(const vec4& _v);

		vec4& operator/=(float _scalar);
		vec4& operator/=(const vec4& _v);//не очень понятен смысл такого оператора, но в glm есть зачем-то
		
		float& operator[](const int _pos);

		operator float();
	};

	vec4 operator+(const vec4& _v);
	vec4 operator-(const vec4& _v);

	vec4 operator+(const vec4& _v, float _scalar);
	vec4 operator+(float _scalar,const vec4& _v);
	vec4 operator+(const vec4& _v1, const vec4& _v2);

	vec4 operator-(const vec4& _v, float _scalar);
	vec4 operator-(float _scalar, const vec4& _v);
	vec4 operator-(const vec4& _v1, const vec4& _v2);

	vec4 operator*(const vec4& _v, float _scalar);
	vec4 operator*(float _scalar, const vec4& _v);
	vec4 operator*(const vec4& _v1, const vec4& _v2);

	vec4 operator/(const vec4& _v, float _scalar);
	vec4 operator/(float _scalar, const vec4& _v);
	vec4 operator/(const vec4& _v1, const vec4& _v2);

	bool operator==(const vec4& _v1, const vec4& _v2);
	bool operator!=(const vec4& _v1, const vec4& _v2);

	vec4 cross(const vec4& _v1, const vec4& _v2, float _w = 0);

	class mat4 {
	private:
		friend mat4 inverse(const mat4& _m);;
		typedef vec4 col_type;
		typedef vec4 row_type;
		col_type value[4];
	public:
		
		mat4();
		mat4(float _val);
		mat4(col_type& _v);
		mat4(col_type const & v0,
			col_type const & v1,
			col_type const & v2,
			col_type const & v3);
		mat4(float const & x0, float const & y0, float const & z0, float const & w0,
			float const & x1, float const & y1, float const & z1, float const & w1,
			float const & x2, float const & y2, float const & z2, float const & w2,
			float const & x3, float const & y3, float const & z3, float const & w3);
		mat4(const mat4& _m);
		~mat4();

		mat4& operator= (float _all);
		mat4& operator= (const mat4& _m);

		mat4& operator+=(float _scalar);
		mat4& operator+=(const mat4& _m);

		mat4& operator-=(float _scalar);
		mat4& operator-=(const mat4& _m);

		mat4& operator*=(float _scalar);
		mat4& operator*=(const mat4& _m);		

		mat4& operator/=(float _scalar);
		mat4& operator/=(const mat4& _m);

		const vec4& operator[](const int _pos)const;

		mat4 transpose() const;

		operator float();

		float determinant();
		
		void swap(int _i, int _j);

	};

	mat4 inverse(const mat4& _m);

	mat4 operator+(const mat4& _m);
	mat4 operator-(const mat4& _m);

	mat4 operator+(const mat4& _m, float _scalar);
	mat4 operator+(float _scalar, const mat4& _m);
	mat4 operator+(const mat4& _m1, const mat4& _m2);

	mat4 operator-(const mat4& _m, float _scalar);
	mat4 operator-(float _scalar, const mat4& _m);
	mat4 operator-(const mat4& _m1, const mat4& _m2);

	mat4 operator*(const mat4& _m, float _scalar);
	mat4 operator*(float _scalar, const mat4& _m);
	mat4 operator*(const mat4& _m1, const mat4& _m2);

	mat4 operator/(const mat4& _m, float _scalar);
	mat4 operator/(float _scalar, const mat4& _m);
	mat4 operator/(const mat4& _m1, const mat4& _m2);

}





#endif // !TEST_VEC_MAT_H
