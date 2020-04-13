#pragma once
#include "../Core.h"
#include <math.h>
#include <random> 
#include <intrin.h>
#include <xmmintrin.h>

//============       NOES       ============//

//TODO all matrices and vectors are floats/real32
//TODO check_orthogonal could be faster
//TODO nothing is tested
//TODO Nomalize/trapsose have SIMD operators, case 
//TODO SIMD more ??
//TODO Single header file full of operators ?!?!?
//TODO Vectors still contain more data than need (vec2)
//============     Constants     ============//
#define PI 3.14159265359

//============Forward Declerations============//
namespace cm
{
	struct  Vec2;	
	struct  Vec3;
	struct  Vec4;
	struct  vecN;

	struct  Mat2;
	struct  Mat3;
	struct  Mat4;

	struct Quat;
	struct Basis;	
	struct Polar_coord;

	class Transform;
	struct Ray;	

	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	enum class Space
	{
		None = 0, Object, World, View, Projection
	};

	//============Inline Function Declerations============//

	inline  uint32_t SafeTruncateUint64(uint64_t Value)
	{
		Assert(Value <= 0xFFFFFFFF);
		uint32_t Result = static_cast<uint32_t>(Value);
		return(Result);
	}

	inline  float SafeTruncateDouble(double Value)
	{
		Assert(Value <= 0xFFFFFFFF);
		float Result = static_cast<float>(Value);
		return(Result);
	}

	inline  float DegToRad(float degrees)
	{
		return SafeTruncateDouble((PI * degrees) / 180);
	}

	inline  float RadToDeg(float radians)
	{
		return SafeTruncateDouble((180 * radians) / PI);
	}

	inline float Round(float val)
	{
		return std::roundf(val * 1000000.0f) / 1000000.0f;
	}

	inline real Floor(const real &val)
	{
		return std::floor(val);
	}

	inline real Ceil(const real &val)
	{
		return std::ceil(val);
	}

	inline int32 Sign(const real &val)
	{
		return (0 < val) - (val < 0);
	}

	inline float FloatChop(float val, float decimal_count)
	{
		float d_amout = powf(10, decimal_count);
		return static_cast<float>(static_cast<int>(val * d_amout)) / d_amout;
	}

	inline float Clamp(float value, float lowerBound, float upperBound)
	{
		return std::clamp(value, lowerBound, upperBound);	
	}
	
	inline bool fequal(const float &a, const float &b, const float &epsilon = FLOATING_POINT_ERROR_PRESCION)
	{
		return (abs(a - b) < epsilon) ? true : false;
	}

	inline bool fnequal(const float &a, const float &b, const float &epsilon = FLOATING_POINT_ERROR_PRESCION)
	{
		return !fequal(a, b);
	}

	inline bool dequal(const double &a, const double &b, const double &epsilon = FLOATING_POINT_ERROR_PRESCION)
	{
		return (abs(a - b) < epsilon) ? true : false;
	}
	
	inline bool dnequal(const double &a, const double &b, const double &epsilon = FLOATING_POINT_ERROR_PRESCION)
	{
		return !dequal(a, b);
	}

	inline bool requal(const real &a, const real &b, const real &epsilon = FLOATING_POINT_ERROR_PRESCION)
	{
		return (abs(a - b) < epsilon) ? true : false;
	}

	inline bool rnequal(const real &a, const real &b, const real &epsilon = FLOATING_POINT_ERROR_PRESCION)
	{
		return !requal(a, b);
	}

	inline float RadicalInverse(uint32 a)
	{
		float x = 0.0;
		float f = 0.5;
		while (a)
		{
			x += f * (float)(!a & 1);
			a /= 2;
			f *= 0.5;
		}
		return x;
	}
	
	inline real RandomUnillateral() // 0 to 1
	{
		real res = (real)rand() / (real)RAND_MAX;
		return res;
	}

	inline real RandomBillateral() // -1 to 1
	{
		real res = 2.0f * RandomUnillateral() - 1;
		return res;
	}
	
	inline real RandomReal(const real &min, const real &max) // min to max - 1
	{
		real c = max - min;
		return  c == 0 ? min : min + static_cast <real> (rand()) / (static_cast <real> (RAND_MAX / c));		
	}

	inline uint32 RandomUInt(const uint32 &min, const uint32 &max) // min to max - 1
	{
		uint32 c = max - min;
		return c == 0 ? min : rand() % (max - min) + min;
	}

	inline real Fit(const real &value, const real &min, const real &max)
	{		
		return (value - min) / (max - min);
	}
	
	inline real Lerp(const real &a, const real &b, const real &t)
	{
		return a *(1 - t) + b * t;
	}

	//************************************
	// Other Functions
	//************************************

	Polar_coord Canonical(real32 r, real32 theta, real32 z);

	uint32 GetShaderDataTypeComponentCount(ShaderDataType type);
	
	uint32 GetShaderDataTypeSize(ShaderDataType type);
		
	Mat4  Perspective(const real32 &dfovy, const real32 &aspect, const real32 &fnear, const real32 &ffar);

	Mat4 Orthographic(const real32 &left, const real32 &right, const real32 &top, const real32 &bottom, const real32 &_near, const real32 &_far);
	   	 
	Mat4 LookAt(const Vec3 &position, const Vec3 &target, const Vec3 &up);
	
	//************************************
	// Quaterion Orientation Functions
	//************************************

	Quat EulerToQuat(const Vec3 &euler_angle);
	
	Quat Slerp(const Quat &a, const Quat &b, const real32 &t);

	Quat Conjugate(const Quat &a);

	Quat Normalize(const Quat &a);

	Mat4 QuatToMat4(const Quat &q);

	float Mag(const Quat &a);

	std::string ToString(const Quat &a);

	Vec3 Rotate(const Quat &r, const Vec3 &point);

	Vec3 Rotate(const real32 &d_angle, const Vec3 &point, const Vec3 &axis);

	//************************************
	// Coordinate Spaces
	//************************************

	Vec4 GetNormalisedDeviceCoordinates(const real32 &window_width, const real32 &window_height, const real32 &mouse_x, const real32 &mouse_y);

	Vec4 ToViewCoords(const Mat4 &projection_matrix, const Vec4 &viewCoords);

	Vec3 ToWorldCoords(const Mat4 &view_matrix, const Vec4 &viewCoords);

	//************************************
	// Vector2 
	//************************************

	float Mag(const Vec2 &a);

	Vec2 Normalize(const Vec2 &a);

	//************************************
	// Vector3
	//************************************

	float  Mag(const Vec3 &a);
	
	float  Dot(const Vec3 &a, const Vec3 &b);

	float SqrdDistance(const Vec3 &a, const Vec3 &b);
	
	Vec3 Cross(const Vec3 &a, const Vec3 &b);
	
	Vec3 Normalize(const Vec3 &a);

	Vec3 Vec4ToVec3(const Vec4 &a);	

	bool CompareVec(const Vec3 &a, const Vec3 &b, const real &epsilon = FLOATING_POINT_ERROR_PRESCION);

	std::string ToString(const Vec3 &a);

	//************************************
	// Vector4
	//************************************

	float Mag(const Vec4 &a);
	
	float Dot(const Vec4 &a, const Vec4 &b);
	
	Vec4 Normalize(const Vec4 &a);

	Vec4 Vec3ToVec4(const Vec3 &a, const real32  &w);

	std::string ToString(const Vec4 &a);

	//************************************
	// Matrix 3x3
	//************************************
	
	real32 Det(const Mat3 &a);

	Mat3 Mat4ToMat3(const Mat4 &a);

	//************************************
	// Matrix 4x4
	//************************************

	real32 Get(const Mat4 &a, const int32 &row, const int32 &col);
	
	Vec4 GetColumn(const Mat4 &a, const uint8 &col);

	std::string ToString(const Mat4 &a);

	bool CheckOrthogonal(const Mat4 &a, const real32 tolerance = 0.01);

	Mat3 Adjoint(const Mat4 &a, const int32 &row, const int32 &col);
	
	Mat4 Inverse(const Mat4 &a);

	Mat4 Transpose(Mat4 a);

	real32 Det(const Mat4 &a);

	Mat4 Mat3ToMat4(const Mat3 &a, const Vec4 &b);

	Mat4 Translate(Mat4 a, Vec3 translation);

	Mat4 Translate(Mat4 a, real32 length, real32 d_angle, real32 z);

	Mat4 Translate(Mat4 a, Polar_coord p_coord);

	Mat4 Rotate(Mat4 a, real32 dangle, Vec3 axis, bool should_normalize = true);

	Mat4 ScaleDirection(Mat4 a, float k, Vec3 unit_direction, bool should_normalize = true);

	Mat4 ScaleCardinal(Mat4 a, Vec3 direction);
	
	Mat4 CalculateTransformMatrix(const Vec3 &position, const Vec3 &scale, const Quat &rotation);
	
	void Print(const Mat4 &m);

	void PrintPretty(const Mat4 &m);
			
	struct  Vec2
	{
		union
		{
			float arr[2];
			struct
			{
				float x;
				float y;
			};
		};
		Vec2()
		{
			x = 0; y = 0;
		}
		Vec2(float x, float y)
		{
			this->x = x; this->y = y;
		}
		Vec2(float all)
		{
			x = all;
			y = all;
		}
	};

	struct  Vec3
	{
		union
		{
			__m128 data;
			float arr[4];
			struct
			{
				float x;
				float y;
				float z;			
				float pad;
			};
		};
		Vec3()
		{
			data = _mm_set_ps(0, 0,0,0);
		}
		Vec3(float x, float y, float z)
		{
			data = _mm_set_ps(0, z, y, x);
		}
		Vec3(float all)
		{
			data = _mm_set_ps(0, all, all, all);
		}
		Vec3(__m128 _data)
		{
			data = _data;
		}
	};

	struct  Vec4
	{
		union
		{
			__m128 data;
			float arr[4];
			struct
			{
				union 
				{
					float x;
					float r;
				};
				union
				{
					float y;
					float g;
				};
				union
				{
					float z;
					float b;
				};
				union
				{
					float w;
					float a;
				};
			};
		};
		Vec4()
		{
			data = _mm_set_ps(0, 0, 0, 0);
		}
		Vec4(float x, float y, float z, float w)
		{
			data = _mm_set_ps(w, z, y, x);
		}
		Vec4(float all)
		{
			data = _mm_set_ps(all, all, all, all);
		}
		Vec4(Vec3 xyz, float _w)
		{
			x = xyz.x;
			y = xyz.y;
			z = xyz.z;
			w = _w;
		}
		Vec4(__m128 _data)
		{
			data = _data;
		}
	};
	struct Quat
	{
		union
		{
			Vec3 vec;
			__m128 data;
			float arr[4];
			struct
			{
				float x;
				float y;
				float z;
				float w;
			};
		};

		Quat()
		{
			data = _mm_set_ps(1, 0, 0, 0);
		}
		
		Quat(float x, float y, float z, float w)
		{
			data = _mm_set_ps(w, z, y, x);
		}

		Quat(Vec3 xyz, float _w)
		{
			x = xyz.x;
			y = xyz.y;
			z = xyz.z;
			w = _w;
		}

		Quat(__m128 _data)
		{
			data = _data;
		}
	};

	//Matrices
	struct  Mat3
	{
		union
		{
			float arr[9];
			Vec3 data[3];
			struct
			{
				Vec3 row0;
				Vec3 row1;
				Vec3 row2;
			};
		};
		Mat3(float a)
		{
			row0 = Vec3(a, 0, 0);
			row1 = Vec3(0, a, 0);
			row2 = Vec3(0, 0, a);
		}
		Mat3(Vec3 _row0, Vec3 _row1, Vec3 _row2)
		{
			row0 = _row0;
			row1 = _row1;
			row2 = _row2;
		}
	};

	struct  Mat4
	{
		union
		{
			float arr[16];
			Vec4 data[4];
			struct
			{
				Vec4 row0;
				Vec4 row1;
				Vec4 row2;
				Vec4 row3;
			};
		};
		Mat4()
		{
			row0 = Vec4(1, 0, 0, 0);
			row1 = Vec4(0, 1, 0, 0);
			row2 = Vec4(0, 0, 1, 0);
			row3 = Vec4(0, 0, 0, 1);
		}

		Mat4(float a)
		{
			row0 = Vec4(a, 0, 0, 0);
			row1 = Vec4(0, a, 0, 0);
			row2 = Vec4(0, 0, a, 0);
			row3 = Vec4(0, 0, 0, a);
		}
		Mat4(Vec4 _row0, Vec4 _row1, Vec4 _row2, Vec4 _row3)
		{
			row0 = _row0;
			row1 = _row1;
			row2 = _row2;
			row3 = _row3;
		}
		
	};

	struct  Polar_coord
	{
		real r;
		real theta;
		real z;		
	};

	struct Basis
	{	
		Vec3 forward = Vec3(0,0,1);
		Vec3 upward	 = Vec3(0,1,0);
		Vec3 right	 = Vec3(1,0,0);
	};

	class Transform //@TODO: Move defintions into own cpp file
	{
	public:
		Vec3 position = Vec3(0,0,0);
		Vec3 scale = Vec3(1, 1, 1);
		Quat rotation = Quat(0,0,0,1);
		Basis basis;

	public:
		Mat4 CalcTransformMatrix() const
		{
			return CalculateTransformMatrix(position, scale, rotation);
		}
	};


	struct Ray
	{
		Vec3 origin;
		Vec3 direction;
	};


	////============Vector2 Operators============////

	inline  Vec2 operator  +(const Vec2 &a, const Vec2 &b)
	{
		return Vec2(a.x + b.x, a.y + b.y);
	}

	inline  Vec2 operator  -(const Vec2 &a, const Vec2 &b)
	{
		return Vec2(a.x - b.x, a.y- b.y);
	}

	inline  Vec2 operator  /(const Vec2 &a, float b)
	{
		return Vec2(a.x / b, a.y / b);
	}

	inline  bool operator  ==(const Vec2 &a, const Vec2 &b)
	{
		return (a.x == b.x && a.y == b.y);
	}

	////============Vector3 Operators============////

	inline  Vec3 operator  +(const Vec3 &a, const Vec3 &b)
	{
		return Vec3(_mm_add_ps(a.data, b.data));
	}

	inline  Vec3 operator  -(const Vec3 &a, const Vec3 &b)
	{
		return Vec3(_mm_sub_ps(a.data, b.data));
	}

	inline  Vec3 operator  *(const Vec3 &a, const Vec3 &b)
	{
		return Vec3(_mm_mul_ps(a.data, b.data));
	}

	inline  Vec3 operator  /(const Vec3 &a, const Vec3 &b)
	{
		return Vec3(_mm_div_ps(a.data, b.data));
	}

	inline  Vec3 operator *(const Vec3 &a, const float b)
	{
		return _mm_mul_ps(a.data, _mm_set1_ps(b));
	}

	inline  Vec3 operator *(const float a, const Vec3 &b)
	{
		return _mm_mul_ps(b.data, _mm_set1_ps(a));
	}

	inline  Vec3 operator /(const Vec3 &a, const float b)
	{
		return _mm_div_ps(a.data, _mm_set1_ps(b));
	}

	inline  bool operator  ==(const Vec3 &a, const Vec3 &b)
	{
		int *compared = reinterpret_cast<int*>(&_mm_cmpeq_ps(a.data, b.data));
		return(compared[0] & compared[1] & compared[2] & compared[3]);
	}

	inline  bool operator  !=(const Vec3 &a, const Vec3 &b)
	{
		int *compared = reinterpret_cast<int*>(&_mm_cmpneq_ps(a.data, b.data));
		return(compared[0] | compared[1] | compared[2] | compared[3]);
	}

	inline  void operator +=(Vec3 &a, const Vec3 &b)
	{
		a = Vec3(_mm_add_ps(a.data, b.data));
	}

	inline  void operator -=(Vec3 &a, const Vec3 &b)
	{
		a  = Vec3(_mm_sub_ps(a.data, b.data));
	}

	////============Vector4 Operators============////

	inline  Vec4 operator  +(const Vec4 &a, const Vec4 &b)
	{
		return Vec4(_mm_add_ps(a.data, b.data));
	}

	inline  Vec4 operator  -(const Vec4 &a, const Vec4 &b)
	{
		return Vec4(_mm_sub_ps(a.data, b.data));
	}

	inline  Vec4 operator  *(const Vec4 &a, const Vec4 &b)
	{
		return Vec4(_mm_mul_ps(a.data, b.data));
	}

	inline  Vec4 operator  /(const Vec4 &a, const Vec4 &b)
	{
		return Vec4(_mm_div_ps(a.data, b.data));
	}

	inline  Vec4 operator *(const Vec4 &a, const float &b)
	{
		return _mm_mul_ps(a.data, _mm_set1_ps(b));
	}

	inline  Vec4 operator /(const Vec4 &a, const float &b)
	{
		return _mm_div_ps(a.data, _mm_set1_ps(b));
	}

	inline  bool operator  ==(const Vec4 &a, const Vec4 &b)
	{
		int *compared = reinterpret_cast<int*>(&_mm_cmpeq_ps(a.data, b.data));
		return(compared[0] & compared[1] & compared[2] & compared[3]);
	}

	inline  bool operator  !=(const Vec4 &a, const Vec4 &b)
	{
		int *compared = reinterpret_cast<int*>(&_mm_cmpneq_ps(a.data, b.data));
		return(compared[0] | compared[1] | compared[2] | compared[3]);
	}

	////============Matrix Operators============////
	
	Mat4  operator /(Mat4 a, float b);

	Mat4  operator *(const Mat4 &a, const Mat4&b);

	Mat4 operator +(const Mat4 &a, const Mat4 &b);
	
	Vec4  operator *(const Vec4 &a, const Mat4 &b);
			
	////============Quaterion Operators============////

	inline Quat operator *(const Quat &q, const Quat &p)
	{
		float w = p.w * q.w		 - p.x * q.x		- p.y * q.y			- p.z * q.z;
		float x = p.w * q.x		 + p.x * q.w		+ p.y * q.z			- p.z * q.y;
		float y = p.w * q.y		 + p.y * q.w		+ p.z * q.x			- p.x * q.z;
		float z = p.w * q.z		 + p.z * q.w		+ p.x * q.y			- p.y * q.x;
		return Quat(x, y, z, w);
	}

	inline Quat operator *(const Quat &a, const Vec3 &b)
	{
		float w = -a.x * b.x - a.y - b.y - a.z * b.z;
		float x =  a.w * b.x + a.y *b.z - a.z * b.y;
		float y =  a.w * b.y + a.z *b.x - a.x * b.z;
		float z =  a.w * b.z + a.x *b.y - a.y * b.x;
		return Quat(x, w, z, w);
	}

	inline Vec3 Lerp(Vec3 a, Vec3 b, float t)
	{
		Vec3 r = Vec3(
			a.x + t * (b.x - a.x),
			a.y + t * (b.y - a.y),
			a.z + t * (b.z - a.z)
		);
		return r;
	}

	typedef Vec4 Colour;
}