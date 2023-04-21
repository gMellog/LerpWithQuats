	#pragma once

	#define _USE_MATH_DEFINES

	#include <GL/glew.h>
	#include <GL/freeglut.h> 
	#include <iostream>
	#include <math.h>
	#include <random>
	#include <memory>

	struct Vector
	{
	Vector(float pX = 0.f, float pY = 0.f, float pZ = 0.f)
		:
		X{ pX },
		Y{ pY },
		Z{ pZ }
	{

	}

	float length() const noexcept
	{
		return std::sqrt(X * X + Y * Y + Z * Z);
	}

	Vector& operator+=(const Vector& rhs) noexcept
	{
		return *this = *this + rhs;;
	}

	Vector& operator-=(const Vector& rhs) noexcept
	{
		return *this = *this - rhs;;
	}

	Vector operator+(const Vector& rhs) const noexcept
	{
		return { X + rhs.X, Y + rhs.Y, Z + rhs.Z };
	}

	Vector operator-(const Vector& rhs) const noexcept
	{
		return { X - rhs.X, Y - rhs.Y, Z - rhs.Z };
	}

	Vector operator*(float v) const noexcept
	{
		return { X * v, Y * v, Z * v };
	}

	bool operator==(const Vector& rhs) const noexcept
	{
		return (X == rhs.X) && (Y == rhs.Y) && (Z == rhs.Z);
	}

	bool operator!=(const Vector& rhs) const noexcept
	{
		return !(*this == rhs);
	}

	float X;
	float Y;
	float Z;
	};

		struct Area
	{
		Vector SW;
		Vector NW;
		Vector NE;
		Vector SE;
	};

	inline Vector normalize(const Vector& v)
	{
	return { v.X / v.length(), v.Y / v.length(), v.Z / v.length() };
	}

	inline float dotProduct(const Vector& lhs, const Vector& rhs)
	{
		return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z;
	}

	inline bool inRange(float min, float max, float v)
	{
	return (min <= v) && (v <= max);
	}

	inline float getAngleBetweenVectors(const Vector& lhs, const Vector& rhs)
	{
	const auto scalarProduct = dotProduct(lhs, rhs);
	const auto lengthsMult = lhs.length() * rhs.length();
	
	return acosf(scalarProduct / lengthsMult) / M_PI * 180;
	
	}

	inline Vector getUnitVector(const Vector& v)
	{
		return {
			v.X / v.length(),
			v.Y / v.length(),
			v.Z / v.length()
		};
	}

	template<typename T>
	inline T clamp(T min, T max, T val)
	{
	return min > val ? min : val > max ? max : val;
	}

	inline bool isNearlyEqual(float a, float b, float e)
	{
	return std::abs(a - b) < e;
	}

	inline std::ostream& operator<<(std::ostream& os, const Vector& v)
	{
	os << "X: " << v.X << " Y: " << v.Y << " Z: " << v.Z;
	return os;
	}

	inline float toDegrees(float radians)
	{
		return 180 / M_PI * radians;
	}

	inline float toRadians(float degrees)
	{
		return M_PI / 180 * degrees;
	}

	inline float getAngleBasedOnQuadrant(const Vector& uv)
	{
		float r{};
		const auto firstQuadrantAngle = acos(std::abs(uv.X));

		const auto X = uv.X;
		const auto Z = -uv.Z;

		if((X >= 0) && (Z >= 0))
		{
			r = firstQuadrantAngle;
		}
		else if((X <= 0) && (Z >= 0))
		{
			r = M_PI - firstQuadrantAngle;
		}
		else if((X <= 0) && (Z <= 0))
		{
			r = M_PI + firstQuadrantAngle;
		}
		else if((X >= 0) && (Z <= 0))
		{	
			r = 2 * M_PI - firstQuadrantAngle;
		}
		else
		{
			std::cerr << "Unhandled case in getAngleBasedOnQuadrant()" << std::endl;
			std::cerr << "uv: " << uv << std::endl;
		}

		return r;
	}

	inline float getXZAngle(const Vector& dir)
	{
		const auto uv = getUnitVector(dir);
		return getAngleBasedOnQuadrant(uv);
	}
	
	struct RotationMatrix
	{
		explicit RotationMatrix(const std::array<float, 16> pMatrixInColumnForm = std::array<float,16>()) 
		:
		matrixInColumnForm{pMatrixInColumnForm}
		{

		}

		std::array<float, 16> matrixInColumnForm;
	};

	struct EulerAngles
	{
		EulerAngles(float pAlpha = 0.f, float pBeta = 0.f, float pGamma = 0.f)
		:
		alpha{pAlpha}, beta{pBeta}, gamma{pGamma}
		{

		}

		float alpha;
		float beta;
		float gamma;
	};

	struct Quaternion
	{
		Quaternion(float pW = 0.f, float pX = 0.f, float pY = 0.f, float pZ = 0.f)
		:
		w{pW}, x{pX}, y{pY}, z{pZ}
		{

		}

		Quaternion operator*(const Quaternion& rhs) const noexcept
		{
			Quaternion r;

			const float w1 = w;
			const float x1 = x;
			const float y1 = y;
			const float z1 = z;

			const float w2 = rhs.w;
			const float x2 = rhs.x;
			const float y2 = rhs.y;
			const float z2 = rhs.z;

			r.w = w1*w2 - x1*x2 - y1*y2 - z1*z2;
			r.x = w1*x2 + x1*w2 + y1*z2 - z1*y2;
			r.y = w1*y2 + y1*w2 + z1*x2 - x1*z2;
			r.z = w1*z2 + z1*w2 + x1*y2 - y1*x2;

			return r;
		}

		Quaternion operator*(float scalar) const noexcept
		{
			return Quaternion{w * scalar, x * scalar, y * scalar, z * scalar};
		}

		RotationMatrix getRotMatrix() const noexcept
		{
			std::array<float, 16> matrixInColForm;
			auto& m = matrixInColForm;

			m[0] = w*w + x*x - y*y - z*z;
			m[1] = 2.f*x*y + 2.f*w*z;
			m[2] = 2.f*x*z - 2.f*w*y;
			m[3] = 0.0;

			m[4] = 2.f*x*y - 2.f*w*z;
			m[5] = w*w - x*x + y*y - z*z;
			m[6] = 2.f*y*z + 2.f*w*x;
			m[7] = 0.0;

			m[8] = 2.f*x*z + 2.f*w*y;
			m[9] = 2.f*y*z - 2.f*w*x;
			m[10] = w*w - x*x - y*y + z*z;
			m[11] = 0.0;

			m[12] = 0.f;
			m[13] = 0.f;
			m[14] = 0.f;
			m[15] = 1.0;

			return RotationMatrix(matrixInColForm);
		}

		float w;
		float x;
		float y;
		float z;
	};

	inline Quaternion convertEulerAnglesToQuat(const EulerAngles& e)
	{
		const float alpha = toRadians(e.alpha);
		const float beta = toRadians(e.beta);
		const float gamma = toRadians(e.gamma);

		const auto qAlpha = Quaternion(cos(alpha / 2.0), sin(alpha / 2.0));
		const auto qBeta = Quaternion(cos(beta / 2.0), 0.f, sin(beta / 2.0));
		const auto qGamma = Quaternion(cos(gamma / 2.0), 0.f, 0.f, sin(gamma / 2.0));
		
		return qAlpha * qBeta * qGamma;
	}

	inline float QuaternionDotProduct(const Quaternion& q1, const Quaternion& q2)
	{
		return (q1.w * q2.w + 
			q1.x * q2.x + q1.y * q2.y + 
			q1.z * q2.z);
	}

	inline Quaternion slerp(const Quaternion& from, const Quaternion& to, float t)
	{
		const auto dotProduct = QuaternionDotProduct(from, to);
	
		const float theta = acos(dotProduct);

		const auto edgeTheta = 0.000001;

		float mult1;
		float mult2;

		if(theta > edgeTheta)
		{
			mult1 = sin((1 - t) * theta) / sin(theta);
			mult2 = sin(t * theta) / sin(theta);
		}
		else
		{
			mult1 = 1 - t;
			mult2 = t;
		}

		Quaternion r;
		
		const float toMult = (dotProduct < 0.f) ? -1.f : 1.f;
		const auto& q1 = from;
		const auto q2 = to * toMult;

		r.w = mult1*q1.w + mult2*q2.w;
		r.x = mult1*q1.x + mult2*q2.x;
		r.y = mult1*q1.y + mult2*q2.y;
		r.z = mult1*q1.z + mult2*q2.z;

		return r;
	}

	struct Color
	{
	Color(float pR = 0.f, float pG = 0.f, float pB = 0.f)
		:
		R{ pR },
		G{ pG },
		B{ pB }
	{

	}

	float R;
	float G;
	float B;
	};

	struct Rotation
	{
	Rotation(float pAngle = 0, const Vector& pDirs = Vector{})
		:
		angle{ pAngle },
		dirs{ pDirs }
	{

	}

	float angle;
	Vector dirs;
	};

	struct Transform
	{
	Transform(const Vector& pTranslation = {},
		const Vector& pScale = {},
		const Rotation& pRotation = {}
	)
		:
		translation{ pTranslation },
		scale{ pScale },
		rotation{ pRotation }
	{

	}

	Vector translation;
	Vector scale;
	Rotation rotation;
	};

	struct Random
	{
	
	float getRandomFloat(float from, float to)
	{
		std::uniform_real_distribution<float> di(from, to);
		return di(mt);
	}

	int getRandomInt(int from, int to)
	{
		std::uniform_int_distribution<int> di(from, to);
		return di(mt);
	}

	static Random& get()
	{
		static Random random;
		return random;
	}

	private:
	Random()
		:
		mt{static_cast<unsigned int>(time(nullptr))}
	{

	}
	
	std::mt19937 mt;
	};

	inline Color getRandomColor()
	{
	const std::vector<Color> colors
	{
		{0.f, 0.f, 1.f},
		{0.f, 1.f, 0.f},
		{0.f, 1.f, 1.f},
		{1.f, 0.f, 0.f},
		{1.f, 0.f, 1.f},
		{1.f, 1.f, 0.f},
		{1.f, 1.f, 1.f}
	};

	return colors[Random::get().getRandomInt(0,colors.size() - 1)];
	}

	inline void writeBitmapString(void* font, const std::string& str)
	{
	for (const auto ch : str) glutBitmapCharacter(font, ch);
	}

	inline bool checkSphereCollision(const Vector& sph1Loc, float r1, const Vector& sph2Loc, float r2)
	{
		const auto diff = sph2Loc - sph1Loc;
		return  (diff.X * diff.X + diff.Y * diff.Y + diff.Z * diff.Z) <= ((r1 + r2) * (r1 + r2));
	}


	template<typename T1, typename T2>
	inline T1 lerp(const T1& a, const T1& b, T2 t)
	{
	return a * (1 - t) + b * t;
	}
