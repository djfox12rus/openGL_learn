#include "../headers/test_vec_mat.h"

using namespace vm;
vm::vec4::vec4() : x(), y(), z(), w()
{

}

vm::vec4::vec4(float _all) : x(_all), y(_all), z(_all), w(_all)
{
}

vm::vec4::vec4(float _x, float _y, float _z) : x(_x), y(_y), z(_z), w()
{
}

vm::vec4::vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w)
{
}

vm::vec4::vec4(const vec4 &_v) : x(_v.x), y(_v.y), z(_v.z), w(_v.w)
{
}

vm::vec4::~vec4()
{
}

vec4 & vm::vec4::operator=(float _all)
{
	this->x = _all;
	this->y = _all;
	this->z = _all;
	this->w = _all;
	return *this;
}

vec4 & vm::vec4::operator=(const vec4 & _v)
{
	if (this == &_v) return *this;
	this->x = _v.x;
	this->y = _v.y;
	this->z = _v.z;
	this->w = _v.w;
	return *this;
}

vec4 & vm::vec4::operator+=(float _scalar)
{
	this->x += _scalar;
	this->y += _scalar;
	this->z += _scalar;
	this->w += _scalar;
	return *this;
}

vec4 & vm::vec4::operator+=(const vec4 & _v)
{
	this->x += _v.x;
	this->y += _v.y;
	this->z += _v.z;
	this->w += _v.w;
	return *this;
}

vec4 & vm::vec4::operator-=(float _scalar)
{
	this->x -= _scalar;
	this->y -= _scalar;
	this->z -= _scalar;
	this->w -= _scalar;
	return *this;
}

vec4 & vm::vec4::operator-=(const vec4 & _v)
{
	this->x -= _v.x;
	this->y -= _v.y;
	this->z -= _v.z;
	this->w -= _v.w;
	return *this;
}

vec4 & vm::vec4::operator*=(float _scalar)
{
	this->x *= _scalar;
	this->y *= _scalar;
	this->z *= _scalar;
	this->w *= _scalar;
	return *this;
}

vec4 & vm::vec4::operator*=(const vec4 & _v)
{
	this->x *= _v.x;
	this->y *= _v.y;
	this->z *= _v.z;
	this->w *= _v.w;
	return *this;
}

vec4 & vm::vec4::operator/=(float _scalar)
{
	this->x /= _scalar;
	this->y /= _scalar;
	this->z /= _scalar;
	this->w /= _scalar;
	return *this;
}

vec4 & vm::vec4::operator/=(const vec4 & _v)
{
	this->x /= _v.x;
	this->y /= _v.y;
	this->z /= _v.z;
	this->w /= _v.w;
	return *this;
}

float & vm::vec4::operator[](const int _pos)
{
	switch (_pos) {
	case 0:return this->x;
	case 1: return this->y;
	case 2:return this->z;
	case 3:return this->w;
	default: {
		if (_pos > 3) return this->w;
		if (_pos < 0) return this->x;
	}
	}
	return this->x;
}

vm::vec4::operator float()
{
	return this->x + this->y + this->z + this->w;
}

vec4 vm::operator+(const vec4 & _v)
{
	//вроде ничего не надо делать
	return vec4(_v.x, _v.y, _v.z, _v.w);
}

vec4 vm::operator-(const vec4 & _v)
{
	return vec4(-_v.x, -_v.y, -_v.z, -_v.w);
}

vec4 vm::operator+(const vec4 & _v, float _scalar)
{
	return vec4(_v) += _scalar;
}

vec4 vm::operator+(float _scalar, const vec4 & _v)
{
	return vec4(_v) += _scalar;
}

vec4 vm::operator+(const vec4 & _v1, const vec4 & _v2)
{
	return vec4(_v1) += _v2;
}

vec4 vm::operator-(const vec4 & _v, float _scalar)
{
	return vec4(_v) -= _scalar;
}

vec4 vm::operator-(float _scalar, const vec4 & _v)
{
	return vec4(_v) -= _scalar;
}

vec4 vm::operator-(const vec4 & _v1, const vec4 & _v2)
{
	return vec4(_v1) -= _v2;
}

vec4 vm::operator*(const vec4 & _v, float _scalar)
{
	return vec4(_v) *= _scalar;
}

vec4 vm::operator*(float _scalar, const vec4 & _v)
{
	return vec4(_v) *= _scalar;
}

vec4 vm::operator*(const vec4 & _v1, const vec4 & _v2)
{
	return vec4(_v1) *= _v2;
}

vec4 vm::operator/(const vec4 & _v, float _scalar)
{
	return vec4(_v) /= _scalar;
}

vec4 vm::operator/(float _scalar, const vec4 & _v)
{
	return vec4(_v) /= _scalar;
}

vec4 vm::operator/(const vec4 & _v1, const vec4 & _v2)
{
	return vec4(_v1) /= _v2;
}

bool vm::operator==(const vec4 & _v1, const vec4 & _v2)
{
	return _v1.x == _v2.x&&_v1.y == _v2.y&&_v1.z == _v2.z&&_v1.w == _v2.w;
}

bool vm::operator!=(const vec4 & _v1, const vec4 & _v2)
{
	return !(_v1 == _v2);
}

vec4 vm::cross(const vec4 & _v1, const vec4 & _v2, float _w)
{
	return vec4(_v1.y*_v2.z - _v1.z*_v2.y, _v1.y*_v2.x - _v1.x*_v2.z, _v1.x*_v2.y - _v1.y*_v2.x, _w);
}

vm::mat4::mat4()
{
	for (int i = 0; i < 4; i++) {
		this->value[i][i] = 1.0;
	}
}

vm::mat4::mat4(float _val)
{
	for (int i = 0; i < 4; i++) {
		this->value[i] = _val;
	}
}

vm::mat4::mat4(col_type & _v)
{
	for (int i = 0; i < 4; i++) {
		this->value[i] = _v;
	}
}

vm::mat4::mat4(col_type const & v0, col_type const & v1, col_type const & v2, col_type const & v3)
{
	value[0] = v0;
	value[1] = v1;
	value[2] = v2;
	value[3] = v3;
}

vm::mat4::mat4(float const & x0, float const & y0, float const & z0, float const & w0, float const & x1, float const & y1, float const & z1, float const & w1, float const & x2, float const & y2, float const & z2, float const & w2, float const & x3, float const & y3, float const & z3, float const & w3)
{
	value[0][0] = x0; value[1][0] = x1; value[2][0] = x2; value[3][0] = x3;
	value[0][1] = y0; value[1][1] = y1; value[2][1] = y2; value[3][1] = y3;
	value[0][2] = z0; value[1][2] = z1; value[2][2] = z2; value[3][2] = z3;
	value[0][3] = w0; value[1][3] = w1; value[2][3] = w2; value[3][3] = w3;

}

vm::mat4::mat4(const mat4 & _m)
{
	for (int i = 0; i < 4; i++) {
		this->value[i] = _m.value[i];
	}
}

vm::mat4::~mat4()
{
}

mat4 & vm::mat4::operator=(float _all)
{
	for (int i = 0; i < 4; i++) {
		this->value[i] = _all;
	}
	return *this;
}

mat4 & vm::mat4::operator=(const mat4 & _m)
{
	if (this == &_m)return *this;
	for (int i = 0; i < 4; i++) {
		this->value[i] = _m.value[i];
	}
	return *this;
}

mat4 & vm::mat4::operator+=(float _scalar)
{
	for (int i = 0; i < 4; i++) {
		this->value[i] += _scalar;
	}
	return *this;
}

mat4 & vm::mat4::operator+=(const mat4 & _m)
{
	for (int i = 0; i < 4; i++) {
		this->value[i] += _m.value[i];
	}
	return *this;
}

mat4 & vm::mat4::operator-=(float _scalar)
{
	for (int i = 0; i < 4; i++) {
		this->value[i] -= _scalar;
	}
	return *this;
}

mat4 & vm::mat4::operator-=(const mat4 & _m)
{
	for (int i = 0; i < 4; i++) {
		this->value[i] -= _m.value[i];
	}
	return *this;
}

mat4 & vm::mat4::operator*=(float _scalar)
{
	for (int i = 0; i < 4; i++) {
		this->value[i] *= _scalar;
	}
	return *this;
}

mat4 & vm::mat4::operator*=(const mat4 & _m)
{
	mat4 mtr = this->transpose();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->value[i][j] = mtr.value[i] * _m.value[j];

		}
	}	
	return*this;

}

mat4 & vm::mat4::operator/=(float _scalar)
{
	for (int i = 0; i < 4; i++) {
		this->value[i] /= _scalar;
	}
	return *this;
}

const vec4 & vm::mat4::operator[](const int _pos)const
{
	if (_pos > 3) return this->value[3];
	if (_pos < 0) return this->value[0];
	return this->value[_pos];	
}

mat4 vm::mat4::transpose()const
{
	return  mat4(this->value[0].x, this->value[1].x, this->value[2].x, this->value[3].x,
		this->value[0].y, this->value[1].y, this->value[2].y, this->value[3].y,
		this->value[0].z, this->value[1].z, this->value[2].z, this->value[3].z,
		this->value[0].w, this->value[1].w, this->value[2].w, this->value[3].w);;
}

vm::mat4::operator float()
{
	float out = 0;
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (j == i) continue;
			for (int k = 0; k < 4; k++) {
				if (k == i || k == j)continue;
				for (int l = 0; l < 4 ; l++) {
					if (l == i || l == j||l==k)continue;
					out += value[0][i] * value[1][j] * value[2][k] * value[3][l];
				}
			}
		}
	}
	return out;
}

float vm::mat4::determinant()
{
	return *this;
}

void vm::mat4::swap(int _i, int _j)
{
	if (_i > 3 || _j > 3 || _i < 0 || _j < 0) return;
	vec4 buf = this->value[_i];
	this->value[_i] = this->value[_j];
	this->value[_j] = buf;
}



mat4 vm::inverse(const mat4 & _m)
{
	int i;
	float coef = 0;
	for (i = 0; i < 4; i++) {
		if (_m.value[i] == vec4(0.0)) return mat4(INFINITY);
	}
	mat4 inv;
	mat4 m = _m;
	for (int count = 0; count < 4; count++) {
		if (m.value[count][count] == 0.0) {
			for (i = count+1; i < 4; i++) {
				if (m.value[i][count] != 0) break;
			}
			if (i == 4) return mat4(INFINITY);
			m.swap(count, i);
			inv.swap(count, i);
		}
		m.value[count] /= m.value[count][count];		
		for (i = count + 1; i < 4; i++) {			
			for (int j = 0; j < 4; j++) {
				m.value[i][j] -= m.value[i][count] * m.value[count][j];
				inv.value[i][j] -= m.value[i][count] * inv.value[count][j];
			}
			m.value[i][count] = 0;
		}
	}
	return inv;
}

mat4 vm::operator+(const mat4 & _m)
{
	return mat4(_m[0], _m[1], _m[2], _m[3]);
}

mat4 vm::operator-(const mat4 & _m)
{
	return mat4(-_m[0], -_m[1], -_m[2], -_m[3]);
}

mat4 vm::operator+(const mat4 & _m, float _scalar)
{
	return mat4(_m)+= _scalar;
}

mat4 vm::operator+(float _scalar, const mat4 & _m)
{
	return mat4(_m) += _scalar;
}

mat4 vm::operator+(const mat4 & _m1, const mat4 & _m2)
{
	return mat4(_m1)+=_m2;
}

mat4 vm::operator-(const mat4 & _m, float _scalar)
{
	return mat4(_m) -= _scalar;
}

mat4 vm::operator-(float _scalar, const mat4 & _m)
{
	return mat4(_m) -= _scalar;
}

mat4 vm::operator-(const mat4 & _m1, const mat4 & _m2)
{
	return mat4(_m1) -= _m2;
}

mat4 vm::operator*(const mat4 & _m, float _scalar)
{
	return mat4(_m)*=_scalar;
}

mat4 vm::operator*(float _scalar, const mat4 & _m)
{
	return mat4(_m) *= _scalar;
}

mat4 vm::operator*(const mat4 & _m1, const mat4 & _m2)
{
	return mat4(_m1)*=_m2;
}

mat4 vm::operator/(const mat4 & _m, float _scalar)
{
	return mat4(_m) /= _scalar;
}

mat4 vm::operator/(float _scalar, const mat4 & _m)
{
	return mat4();
}

mat4 vm::operator/(const mat4 & _m1, const mat4 & _m2)
{
	return mat4();
}
