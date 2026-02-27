#include <include/Core/Vector.hpp>
#include <include/Core/Matrix.hpp>

namespace ame
{
	// Affine Transformations
	template <typename T>
	ame::Matrix<T, 4, 4> Translation(const ame::Vector<T, 3>& translation)
	{
		return ame::Matrix<T, 4, 4>{
			T{ 0 }, T{ 0 }, T{ 0 }, T{ translation[0] },
			T{ 0 }, T{ 0 }, T{ 0 }, T{ translation[1] },
			T{ 0 }, T{ 0 }, T{ 0 }, T{ translation[2] },
			T{ 0 }, T{ 0 }, T{ 0 }, T{ 0 }
		};
	};

	template <typename T>
	ame::Matrix<T, 4, 4> Scale(const ame::Vector<T, 3>& scale)
	{
		return ame::Matrix<T, 4, 4>{
			T{ scale[0] }, T{ 0 }, T{ 0 }, T{ 0 },
			T{ 0 }, T{ scale[1] }, T{ 0 }, T{ 0 },
			T{ 0 }, T{ 0 }, T{ scale[2] }, T{ 0 },
			T{ 0 }, T{ 0 }, T{ 0 }, T{ 0 }
		};
	};

	template <typename T>
	ame::Matrix<T, 4, 4> Rotation(T& phi, T& theta, T& psi)
	{
		T cph = std::cos(phi);
		T cth = std::cos(theta);
		T cps = std::cos(psi);

		T sph = std::sin(phi);
		T sth = std::sin(theta);
		T sps = std::sin(psi);
		return ame::Matrix<T, 4, 4>{
			cps*cth,	cps*sth*sph - sps*cph,		cps*sth*cph + sps*sph,		0,
			sps*cth,	sps*sth*sph + cps*cph,		sps*sth*cph - cps*sph,		0,
			-sth,		cth*sph,					cth*cph,					0,
			0,			0,							0,							1
		};
	};

	template <typename T>
	ame::Matrix<T, 4, 4> TRS(
		const ame::Vector<T, 3>& translation, 
		const ame::Vector<T, 3>& rotation, 
		const ame::Vector<T, 3>& scale
	)
	{
		return
			ame::Translation(translation) *
			ame::Rotation(rotation[0], rotation[1], rotation[2]) *
			ame::Scale(scale);
	}

	template <typename T>
	ame::Matrix<T, 4, 4> SRT(
		const ame::Vector<T, 3>& scale,
		const ame::Vector<T, 3>& rotation,
		const ame::Vector<T, 3>& translation
	)
	{
		return
			ame::Scale(scale) *
			ame::Rotation(rotation[0], rotation[1], rotation[2]) *
			ame::Scale(scale);
	}

	// Normal Matrix

	// Misc. Transforms
	template <typename T>
	ame::Matrix<T, 4, 4> LookAt(
		const ame::Vector<T, 3>& eye,
		const ame::Vector<T, 3>& target,
		const ame::Vector<T, 3>& up
	)
	{
		return Matrix<T, 4, 4>{
			
		};
	}

	// Perspective Projection
	
	// Orthographic Projection
	
	// Rigid Transform Type

	// Dual Quaternions

	// Reflection Matrices

	// Shadow Projection Matrix


}