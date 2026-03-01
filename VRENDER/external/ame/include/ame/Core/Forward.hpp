#ifndef MATH_CORE_FORWARD_H
#define MATH_CORE_FORWARD_H

#include <cstddef>

namespace ame
{
	// Forward Declarations
	template <typename T, std::size_t N>
	class Vector;

	template <typename T, std::size_t N, std::size_t M>
	class Matrix;

	// Type Aliases
	using vec2f = Vector<float, 2>;
	using vec3f = Vector<float, 3>;
	using vec4f = Vector<float, 4>;

	using mat2f = Matrix<float, 2, 2>;
	using mat3f = Matrix<float, 3, 3>;
	using mat4f = Matrix<float, 4, 4>;

	// Templated Aliases
	template <typename T> using vec2 = Vector<T, 2>;
	template <typename T> using vec3 = Vector<T, 3>;
	template <typename T> using vec4 = Vector<T, 4>;

	template <typename T> using mat3 = Matrix<T, 3, 3>;
	template <typename T> using mat4 = Matrix<T, 4, 4>;
}

#endif