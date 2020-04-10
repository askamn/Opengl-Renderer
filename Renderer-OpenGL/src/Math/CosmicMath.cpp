#pragma once
#include "CosmicMath.h"
namespace cm
{	   	 
	//============Vector2 Functions============//

	float Mag(const Vec2 &a)
	{
		return sqrt(a.x*a.x + a.y*a.y);
	}

	Vec2 Normalize(const Vec2 &a)
	{
		float magA = Mag(a);
		return a / magA;
	}

	//============Vector3 Functions============//

	float Dot(const Vec3 a, const Vec3 b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	float SqrdDistance(const Vec3 &a, const Vec3 &b)
	{
		return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z);
	}

	float Mag(Vec3 a)
	{
		return sqrt(a.x*a.x + a.y * a.y + a.z * a.z);
	}

	Vec3 Cross(Vec3 a, Vec3 b)
	{
		float x = a.y * b.z - b.y * a.z;
		float y = a.z * b.x - b.z * a.x;
		float z = a.x * b.y - b.x * a.y;
		return Vec3(x, y, z);
	}

	Vec3 Normalize(Vec3 a)
	{
		float magA = Mag(a);
		//Assert(magA != 0);
		__m128 div = _mm_div_ps(a.data, _mm_set1_ps(magA));
		return Vec3(div);
	}

	Vec3 Vec4ToVec3(Vec4 &a)
	{
		return Vec3(a.x, a.y, a.z);
	}


	bool CompareVec(const Vec3 &a, const Vec3 &b, const real &epsilon /*= FLOATING_POINT_ERROR_PRESCION*/)
	{		
		bool resx = requal(a.x, b.x, epsilon);
		bool resy = requal(a.y, b.y, epsilon);
		bool resz = requal(a.z, b.z, epsilon);
		return resx && resy && resz;
	}

	std::string ToString(Vec3 a)
	{
		std::stringstream ss;
		ss << '(' << a.x << ", " << a.y << ", " << a.z << ')';
		return ss.str();
	}

	//============Vector4 Functions============//

	float Dot(Vec4 a, Vec4 b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	float Mag(Vec4 a)
	{
		return sqrt(a.x*a.x + a.y * a.y + a.z * a.z + a.w *a.w);
	}

	Vec4 Normalize(Vec4 a)
	{
		float magA = Mag(a);
		if (magA != 1)
		{
			__m128 div = _mm_div_ps(a.data, _mm_set1_ps(magA));
			return Vec4(div);
		}
		return a;
	}

	Vec4 Vec3ToVec4(Vec3 &a, float w)
	{
		return Vec4(a.x, a.y, a.z, w);
	}

	std::string ToString(Vec4 a)
	{
		std::stringstream ss;
		ss << '(' << a.x << ", " << a.y << ", " << a.z << ", " << a.w << ')';
		return ss.str();

	}

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	//============Mat3 Functions==============//

	float Det(Mat3 a)
	{
		float f = a.arr[0] * (a.arr[4] * a.arr[8] - a.arr[7] * a.arr[5]);
		float b = a.arr[1] * (a.arr[3] * a.arr[8] - a.arr[6] * a.arr[5]);
		float c = a.arr[2] * (a.arr[3] * a.arr[7] - a.arr[6] * a.arr[4]);
		return f - b + c;
	}

	Mat3 Mat4ToMat3(Mat4 &a)
	{
		Mat3 result(1);
		result.row0 = Vec4ToVec3(a.row0);
		result.row1 = Vec4ToVec3(a.row1);
		result.row2 = Vec4ToVec3(a.row2);
		return result;
	}

	Mat4 CalculateTransformMatrix(const Vec3 & position, const Vec3 & scale, const Quat & rotation)
	{
		// @NOTE: This is what stack overflow/people recommend. The order doesn't much matter in terms of major, but in term of functionality
		//		: Example, scaling first require higher translation values.
		Mat4 trans(1);
		Mat4 rot(1);
		Mat4 scl(1);
		trans = Translate(trans, position);	
		rot = QuatToMat4(Conjugate(rotation));
		scl= ScaleCardinal(scl, scale);
		return scl * rot * trans;
	}

	void Print(const Mat4 &m)
	{
		for (int32 i = 0; i < 16; i++)
		{
			std::cout << m.arr[i] << " ";
		}
		std::cout << std::endl;
	}
	
	void PrintPretty(const Mat4 &m)
	{
		std::cout << ToString(m) << std::endl;
	}

	float Get(Mat4 a, int row, int col)
	{
		return a.arr[4 * row + col];
	}

	Vec4 GetColumn(Mat4 a, uint8 col)
	{
		Vec4 column(0, 0, 0, 0);
		column.x = a.arr[4 * 0 + col];
		column.y = a.arr[4 * 1 + col];
		column.z = a.arr[4 * 2 + col];
		column.w = a.arr[4 * 3 + col];
		return column;
	}

	std::string ToString(Mat4 a)
	{
		std::stringstream ss;
		std::string space = "            ";
		ss << "| " << a.arr[0] << space << a.arr[1] << space << a.arr[2] << space << a.arr[3] << " |" << '\n';
		ss << "| " << a.arr[4] << space << a.arr[5] << space << a.arr[6] << space << a.arr[7] << " |" << '\n';
		ss << "| " << a.arr[8] << space << a.arr[9] << space << a.arr[10] << space << a.arr[11] << " |" << '\n';
		ss << "| " << a.arr[12] << space << a.arr[13] << space << a.arr[14] << space << a.arr[15] << " |" << '\n';
		return ss.str();
	}

	Mat3 Adjoint(Mat4 a, int row, int col)
	{
		Mat3 result(1);
		int index = 0;
		for (int r = 0; r < 4; r++)
		{
			if (row == r)
				continue;
			for (int c = 0; c < 4; c++)
			{
				if (c == col || c == col + 4 || c == col + 8 || c == col + 12)
					continue;
				result.arr[index++] = Get(a, r, c);
			}
		}
		return result;
	}

	float Det(Mat4 a)
	{
		float f = a.arr[0] * (
			a.arr[5] * (a.arr[10] * a.arr[15] - a.arr[11] * a.arr[14]) +
			a.arr[6] * (a.arr[11] * a.arr[13] - a.arr[9] * a.arr[15]) +
			a.arr[7] * (a.arr[9] * a.arr[14] - a.arr[10] * a.arr[13]));

		float b = a.arr[1] * (
			a.arr[4] * (a.arr[10] * a.arr[15] - a.arr[11] * a.arr[14]) +
			a.arr[6] * (a.arr[11] * a.arr[12] - a.arr[8] * a.arr[15]) +
			a.arr[7] * (a.arr[8] * a.arr[14] - a.arr[10] * a.arr[12]));

		float c = a.arr[2] * (
			a.arr[4] * (a.arr[9] * a.arr[15] - a.arr[11] * a.arr[13]) +
			a.arr[5] * (a.arr[11] * a.arr[12] - a.arr[8] * a.arr[15]) +
			a.arr[7] * (a.arr[8] * a.arr[13] - a.arr[9] * a.arr[12]));

		float d = a.arr[3] * (
			a.arr[4] * (a.arr[9] * a.arr[14] - a.arr[10] * a.arr[13]) +
			a.arr[5] * (a.arr[10] * a.arr[12] - a.arr[8] * a.arr[14]) +
			a.arr[6] * (a.arr[8] * a.arr[13] - a.arr[9] * a.arr[12]));

		return f - b + c - d;
	}

	Mat4 Mat3ToMat4(Mat3 & a, Vec4 & b)
	{
		Mat4 result(1);
		result.row0 = Vec3ToVec4(a.row0, 0);
		result.row1 = Vec3ToVec4(a.row1, 0);
		result.row2 = Vec3ToVec4(a.row2, 0);
		result.row3 = b;
		return result;
	}

	bool ChecOorthogonal(Mat4 a, float tolerance)
	{
		Mat4 result = a * Transpose(a);
		for (int i = 0; i < 4; i++)
		{
			if (abs(1 - abs(result.arr[i * 5])) > tolerance)
				return false;
		}
		return true;
	}

	Mat4 Transpose(Mat4 a)
	{
		_MM_TRANSPOSE4_PS(a.row0.data, a.row1.data, a.row2.data, a.row3.data);
		return a;
	}

	Mat4 Inverse(Mat4 a)
	{
		if (ChecOorthogonal(a))
		{
			return Transpose(a);
		}
		Mat4 result(1);
		Mat3 ad(1);
		int index = 0;
		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				if ((row + col) % 2)
				{
					ad = Adjoint(a, row, col);
					float i = -Det(ad);
					result.arr[index++] = i;
				}
				else
				{
					ad = Adjoint(a, row, col);
					float i = Det(ad);
					result.arr[index++] = i;
				}
			}
		}

		result = Transpose(result);

		float determinant = Det(a);

		return result / determinant;

	}

	Mat4 Translate(Mat4 a, Vec3 translation)
	{
		a.row3 = Vec4(translation, 1) * a;
		return a;
	}

	Mat4 Translate(Mat4 a, float length, float d_angle, float z)
	{
		Polar_coord p_coord = Canonical(length, d_angle, z);
		a.row3 = Vec4(p_coord.r * cosf(p_coord.theta), p_coord.r * sinf(p_coord.theta), p_coord.z, 1) * a;
		return a;
	}

	Mat4 Translate(Mat4 a, Polar_coord p_coord)
	{
		a.row3 = Vec4(p_coord.r * cosf(p_coord.theta), p_coord.r * sinf(p_coord.theta), p_coord.z, 1) * a;
		return a;
	}

	Mat4 Rotate(Mat4 a, float d_angle, Vec3 axis, bool should_normalize)
	{
		if (should_normalize && Mag(axis) != 1)
		{
			axis = Normalize(axis);
		}
		float theata = DegToRad(d_angle);
		float cos_theata = cosf(theata);
		float sin_theata = sinf(theata);

		Vec4 iPrime(0, 0, 0, 0);
		iPrime.x = Round(axis.x *axis.x * (1 - cos_theata) + cos_theata);
		iPrime.y = Round(axis.x *axis.y * (1 - cos_theata) + axis.z * sin_theata);
		iPrime.z = Round(axis.x *axis.z * (1 - cos_theata) - axis.y * sin_theata);
		iPrime.w = 0;

		Vec4 jPrime(0, 0, 0, 0);
		jPrime.x = Round(axis.x *axis.y * (1 - cos_theata) - axis.z *sin_theata);
		jPrime.y = Round(axis.y *axis.y * (1 - cos_theata) + cos_theata);
		jPrime.z = Round(axis.y *axis.z * (1 - cos_theata) + axis.x *sin_theata);
		jPrime.w = 0;

		Vec4 kPrime(0, 0, 0, 0);
		kPrime.x = Round(axis.x *axis.z * (1 - cos_theata) + axis.y *sin_theata);
		kPrime.y = Round(axis.y *axis.z * (1 - cos_theata) - axis.x *sin_theata);
		kPrime.z = Round(axis.z *axis.z * (1 - cos_theata) + cos_theata);
		kPrime.w = 0;

		Vec4 wPrime(0, 0, 0, 1);

		Mat4 result(iPrime, jPrime, kPrime, wPrime);

		return result * a;
	}

	Mat4 ScaleDirection(Mat4 a, float k, Vec3 unit_direction, bool should_normalize)
	{
		if (should_normalize && Mag(unit_direction) != 1)
		{
			unit_direction = Normalize(unit_direction);
		}
		Vec4 iPrime(0, 0, 0, 0);
		iPrime.x = 1 + (k - 1) * unit_direction.x * unit_direction.x;
		iPrime.y = (k - 1) * unit_direction.x * unit_direction.y;
		iPrime.z = (k - 1) * unit_direction.x * unit_direction.z;

		Vec4 jPrime(0, 0, 0, 0);
		jPrime.x = (k - 1) * unit_direction.x * unit_direction.y;
		jPrime.y = 1 + (k - 1) * unit_direction.y * unit_direction.y;
		jPrime.z = (k - 1) * unit_direction.y * unit_direction.z;

		Vec4 kPrime(0, 0, 0, 0);
		kPrime.x = (k - 1) * unit_direction.x * unit_direction.z;
		kPrime.y = (k - 1) * unit_direction.y * unit_direction.z;
		kPrime.z = 1 + (k - 1) * unit_direction.z * unit_direction.z;

		Vec4 wPrime(0, 0, 0, 1);

		Mat4 result(iPrime, jPrime, kPrime, wPrime);
		return result * a;
	}

	Mat4 ScaleCardinal(Mat4 a, Vec3 direction)
	{
		a.row0 = a.row0 * direction.x;
		a.row1 = a.row1 * direction.y;
		a.row2 = a.row2 * direction.z;
		return a;
	}

	Mat4 Perspective(float fovy, float aspect, float fnear, float ffar)
	{
		Mat4 p(1);
		fovy = DegToRad(fovy);
		float half_tan_fovy = tan(fovy / 2);
		p.row0 = Vec4((1 / (aspect * half_tan_fovy)), 0, 0, 0);
		p.row1 = Vec4(0, 1 / half_tan_fovy, 0, 0);

		float a = -(ffar + fnear) / (ffar - fnear);
		float b = (-2 * ffar * fnear) / (ffar - fnear);
		p.row2 = Vec4(0, 0, a, -1);
		p.row3 = Vec4(0, 0, b, 0);
		return p;
	}

	Mat4 Orthographic(float left, float right, float top, float bottom, float _near, float _far)
	{
		Mat4 result(1);
		result.row0 = Vec4(2 / (right - left), 0, 0, 0);
		result.row1 = Vec4(0, 2 / (top - bottom), 0, 0);
		result.row2 = Vec4(0, 0, -2 / (_far - _near), 0);
		result.row3 = Vec4(-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(_far + _near) / (_far - _near), 1);
		return result;
	}

	Mat4 LookAt(Vec3 position, Vec3 target, Vec3 up)
	{

		Vec3 camera_reverse_direction = Normalize((target - position));

		Vec3 basis_right = Normalize(Cross(camera_reverse_direction, up));
		Vec3 basis_up = Cross(basis_right, camera_reverse_direction);

		Vec3 basis_forward = Normalize(Cross(basis_up, basis_right));

		//THIS IS CODE IS ACTUALLY COLUNM MAJOR
		//We just yeet it by transposing it at the end. Also care for order of multiplication order
		Mat4 rotation(1);

		rotation.row0 = Vec4(basis_right, 0);
		rotation.row1 = Vec4(basis_up, 0);
		rotation.row2 = Vec4(camera_reverse_direction * -1, 0);
		//rotation.row2 = vec4(camera_reverse_direction, 0);
		Mat4 translation(1);

		translation.row0 = Vec4(1, 0, 0, -position.x);
		translation.row1 = Vec4(0, 1, 0, -position.y);
		translation.row2 = Vec4(0, 0, 1, -position.z);

		return Transpose(rotation * translation);
	}

	Mat4 operator /(Mat4 a, float b)
	{
		a.row0 = a.row0 / b;
		a.row1 = a.row1 / b;
		a.row2 = a.row2 / b;
		a.row3 = a.row3 / b;
		return a;
	}

	Mat4 operator *(const Mat4 &a, const Mat4 &b)
	{
		Mat4 result(1);
		//Steps through the rows
		for (int i = 0; i < 4; i++)
		{
			//Steps through the columns
			for (int y = 0; y < 4; y++)
			{
				//Gets the column vector of the right hand side
				Vec4 col(0, 0, 0, 0);
				for (int x = 0; x < 4; x++)
				{
					col.arr[x] = Get(b, x, y);
				}
				//Adds to result
				result.arr[4 * i + y] = Dot(col, a.data[i]);
			}
		}
		return result;
	}

	Mat4 operator+(const Mat4 & a, const Mat4 & b)
	{
		Mat4 result(1);
		result.row0 = a.row0 + b.row0;
		result.row1 = a.row1 + b.row1;
		result.row2 = a.row2 + b.row2;
		result.row3 = a.row3 + b.row3;
		return result;
	}

	Vec4 operator *(const Vec4 &a, const Mat4 &b)
	{
		Vec4 result(0, 0, 0, 0);
		for (int i = 0; i < 4; i++)
		{
			Vec4 col = GetColumn(b, i);
			result.arr[i] = Dot(col, a);
		}
		return result;
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	Quat EulerToQuat(const Vec3 &euler_angle)
	{
		Vec3 c = Vec3(cos(DegToRad(euler_angle.x) / 2), cos(DegToRad(euler_angle.y) / 2), cos(DegToRad(euler_angle.z) / 2));
		Vec3 s = Vec3(sin(DegToRad(euler_angle.x) / 2), sin(DegToRad(euler_angle.y) / 2), sin(DegToRad(euler_angle.z) / 2));

		Quat q;
		q.x = s.x * c.y * c.z - c.x * s.y * s.z;
		q.y = c.x * s.y * c.z + s.x * c.y * s.z;
		q.z = c.x * c.y * s.z - s.x * s.y * c.z;
		q.w = c.x * c.y * c.z + s.x * s.y * s.z;

		return q;
	}
	
	Quat Slerp(const Quat &a, const Quat &b, const real32 &t)
	{
		Quat an = Normalize(a);
		Quat bn = Normalize(b);

		real32 d = an.x * bn.x + an.y * bn.y + an.z * bn.z + an.w * bn.w;
		real32 tinv = 1.0f - t;
		int32 ds = Sign(d);

		Quat result;		
		result.x = an.x * tinv + ds * t * bn.x;
		result.y = an.y * tinv + ds * t * bn.y;
		result.z = an.z * tinv + ds * t * bn.z;
		result.w = an.w * tinv + ds * t * bn.w;
		
		return Normalize(result);
	}

	Mat4 QuatToMat4(const Quat &q)
	{
		// @Verified
		Mat4 mat(1);

		float qxx = (q.x * q.x);
		float qyy = (q.y * q.y);
		float qzz = (q.z * q.z);
		float qxz = (q.x * q.z);
		float qxy = (q.x * q.y);
		float qyz = (q.y * q.z);
		float qwx = (q.w * q.x);
		float qwy = (q.w * q.y);
		float qwz = (q.w * q.z);

		mat.row0.x = 1 - (2) * (qyy + qzz);
		mat.row0.y = (2) * (qxy + qwz);
		mat.row0.z = (2) * (qxz - qwy);

		mat.row1.x = (2) * (qxy - qwz);
		mat.row1.y = (1) - (2) * (qxx + qzz);
		mat.row1.z = (2) * (qyz + qwx);

		mat.row2.x = (2) * (qxz + qwy);
		mat.row2.y = (2) * (qyz - qwx);
		mat.row2.z = (1) - (2) * (qxx + qyy);
		return mat;
	}

	Vec3 Rotate(const float &d_angle, const Vec3 &point, const Vec3 &axis)
	{
		//@Speed, normalizing to be safe
		Vec3 ax = Normalize(axis);

		float sh = sin(DegToRad(d_angle / 2));
		float ch = cos(DegToRad(d_angle / 2));

		Quat r(ax.x * sh,
			ax.y * sh,
			ax.z * sh,
			ch);

		Quat rc = Conjugate(r);
		Quat pp = Quat(point, 0);

		Quat res = (r * pp) * rc;
		return res.vec;
	}

	Vec3 Rotate(const Quat &r, const Vec3 &point)
	{
		//@Help: https://gamedev.stackexchange.com/questions/28395/rotating-vector3-by-a-quaternion
		//@Speed, normalizing to be safe		
		Quat rc = Conjugate(Normalize(r));
		Quat pp = Quat(point, 0);
		Quat res = (r * pp) * rc;
		return res.vec;
	}

	Quat Conjugate(const Quat &a)
	{
		return Quat(-a.x, -a.y, -a.z, a.w);
	}

	float Mag(const Quat & a)
	{
		return sqrt(a.x* a.x + a.y * a.y + a.z * a.z + a.w * a.w);
	}

	Quat Normalize(const Quat & a)
	{
		float m = Mag(a);
		return Quat(a.x / m, a.y / m, a.z / m, a.w / m);
	}

	std::string ToString(const Quat & a)
	{
		std::stringstream ss;
		ss << '(' << '{' << a.x << ", " << a.y << ", " << a.z << '}' << ' ' << a.w << ')';
		return ss.str();
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	Vec4 GetNormalisedDeviceCoordinates(const float & window_width, const float & window_height, const float & mouse_x,
		const float & mouse_y)
	{
		//This is actualy clip space when the vec4 with -1 and 1
		return Vec4(2 * mouse_x / window_width - 1, -(2 * mouse_y / window_height - 1), -1, 1);
	}

	Vec4 ToViewCoords(Mat4 & projection_matrix, Vec4 & viewCoords)
	{
		Mat4 invproj = Inverse(projection_matrix);
		return viewCoords * invproj;
	}

	Vec3 ToWorldCoords(Mat4 & view_matrix, Vec4 & viewCoords)
	{
		Mat4 invView = Inverse(view_matrix);
		Vec4 worldSpace = viewCoords * invView;
		return Vec3(worldSpace.x, worldSpace.y, worldSpace.z);
	}
		
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
		   
	//============Other Functions============//
	Polar_coord Canonical(float r, float theta, float z)
	{
		theta = DegToRad(theta);
		if (r == 0)
		{
			theta = 0;
		}
		else
		{
			if (r < 0.f)
			{
				r = -(r);
				theta += static_cast<float>(PI);
			}
			if (fabs(theta) > PI)
			{
				theta += static_cast<float>(PI);
				theta -= static_cast<float>(floor(theta / 2 * PI) * 2 * PI);
				theta -= static_cast<float>(PI);
			}
		}
		return { r, theta, z };
	}

	uint32 GetShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return 4;
		case ShaderDataType::Float2:   return 4 * 2;
		case ShaderDataType::Float3:   return 4 * 3;
		case ShaderDataType::Float4:   return 4 * 4;
		case ShaderDataType::Mat3:     return 4 * 3 * 3;
		case ShaderDataType::Mat4:     return 4 * 4 * 4;
		case ShaderDataType::Int:      return 4;
		case ShaderDataType::Int2:     return 4 * 2;
		case ShaderDataType::Int3:     return 4 * 3;
		case ShaderDataType::Int4:     return 4 * 4;
		case ShaderDataType::Bool:     return 1;
		}
		Assert(0);
		return 0;
	}

	uint32 GetShaderDataTypeComponentCount(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return 1;
		case ShaderDataType::Float2:   return 1 * 2;
		case ShaderDataType::Float3:   return 1 * 3;
		case ShaderDataType::Float4:   return 1 * 4;
		case ShaderDataType::Mat3:     return 1 * 3 * 3;
		case ShaderDataType::Mat4:     return 1 * 4 * 4;
		case ShaderDataType::Int:      return 1;
		case ShaderDataType::Int2:     return 1 * 2;
		case ShaderDataType::Int3:     return 1 * 3;
		case ShaderDataType::Int4:     return 1 * 4;
		case ShaderDataType::Bool:     return 1;
		}
		Assert(0);
		return 0;
	}


}