#ifndef MATH_CORE_VECTOR_HPP
#define MATH_CORE_VECTOR_HPP

#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <initializer_list>

namespace ame
{
	template<typename T, std::size_t N>
	class Vector
	{
	public:
		// Contstructors
		Vector();
		explicit Vector(const T& value);
		Vector(std::initializer_list<T> values);

		// Component Access
		T& operator[](std::size_t index);
		const T& operator[](std::size_t index) const;

		// Arithmetic Operators
		Vector<T, N> operator+(const Vector<T, N>& addend) const;
		Vector<T, N> operator-(const Vector<T, N>& subtrahend) const;
		Vector<T, N> operator*(T multiplicand) const;
		Vector<T, N> operator/(T divisor) const;

		// Compound Assignment Operators
		Vector<T, N>& operator+=(const Vector<T, N>& addend);
		Vector<T, N>& operator-=(const Vector<T, N>& subtrahend);
		Vector<T, N>& operator*=(T multiplicand);
		Vector<T, N>& operator/=(T divisor);

		// Vector Operations
		T Dot(const Vector<T, N>& target) const;
		template <typename U = T>
		std::enable_if_t<N == 3, Vector<T, 3>> Cross(const Vector<T, 3>& other) const;
		T Magnitude() const;
		Vector<T, N> Normalized() const;

	protected:

	private:
		T Data[N];
	};

	// Constructor Definitions
	template <typename T, std::size_t N>
	Vector<T, N>::Vector()
	{
		for (std::size_t i = 0; i < N; i++)
		{
			this->Data[i] = T{};
		}
	}

	template <typename T, std::size_t N>
	Vector<T, N>::Vector(const T& value)
	{
		for (std::size_t i = 0; i < N; i++)
		{
			this->Data[i] = value;
		}
	}

	template <typename T, std::size_t N>
	Vector<T, N>::Vector(std::initializer_list<T> values)
	{
		if (values.size() != N)
		{
			throw std::runtime_error("ARE: Vector Initializer Array Contains Incorrect Number of Elements.");
		}

		std::size_t i = 0;
		for (const auto& value : values)
		{
			this->Data[i] = value;
			i++;
		}
	}

	// Element Access Definitions
	template <typename T, std::size_t N>
	T& Vector<T, N>::operator[](std::size_t index)
	{
		return this->Data[index];
	}

	template <typename T, std::size_t N>
	const T& Vector<T, N>::operator[](std::size_t index) const
	{
		return this->Data[index];
	}

	// Arithmetic Operators
	template <typename T, std::size_t N>
	Vector<T, N> Vector<T, N>::operator+(const Vector<T, N>& target) const
	{
		Vector<T, N> sum;
		for (std::size_t i = 0; i < N; i++)
		{
			sum[i] = this->Data[i] + target[i];
		}
		return sum;
	}

	template <typename T, std::size_t N>
	Vector<T, N> Vector<T, N>::operator-(const Vector<T, N>& target) const
	{
		Vector<T, N> difference;
		for (std::size_t i = 0; i < N; i++)
		{
			difference[i] = this->Data[i] - target[i];
		}
		return difference;
	}

	template <typename T, std::size_t N>
	Vector<T, N> Vector<T, N>::operator*(T scalar) const
	{
		Vector<T, N> product;
		for (std::size_t i = 0; i < N; i++)
		{
			product[i] = this->Data[i] * scalar;
		}
		return product;
	}

	template <typename T, std::size_t N>
	Vector<T, N> Vector<T, N>::operator/(T divisor) const
	{
		Vector<T, N> quotient;
		for (std::size_t i = 0; i < N; i++)
		{
			quotient[i] = this->Data[i] / divisor;
		}
		return quotient;
	}

	// Compound Assignment Operator Definitions
	template <typename T, std::size_t N>
	Vector<T, N>& Vector<T, N>::operator+=(const Vector<T, N>& target)
	{
		for (std::size_t i = 0; i < N; i++)
		{
			this->Data[i] += target[i];
		}
		return *this;
	}

	template <typename T, std::size_t N>
	Vector<T, N>& Vector<T, N>::operator-=(const Vector<T, N>& target)
	{
		for (std::size_t i = 0; i < N; i++)
		{
			this->Data[i] -= target[i];
		}
		return *this;
	}

	template <typename T, std::size_t N>
	Vector<T, N>& Vector<T, N>::operator*=(T scalar)
	{
		for (std::size_t i = 0; i < N; i++)
		{
			this->Data[i] *= scalar;
		}
		return *this;
	}

	template <typename T, std::size_t N>
	Vector<T, N>& Vector<T, N>::operator/=(T divisor)
	{
		for (std::size_t i = 0; i < N; i++)
		{
			this->Data[i] /= divisor;
		}
		return *this;
	}

	// Vector Operations
	template <typename T, std::size_t N>
	T Vector<T, N>::Dot(const Vector<T, N>& target) const
	{
		T sum = T{};
		for (std::size_t i = 0; i < N; i++)
		{
			sum += this->Data[i] * target[i];
		}
		return sum;
	}

	template <typename T, std::size_t N>
	template <typename U>
	std::enable_if_t<N == 3, Vector<T, 3>>
	Vector<T, N>::Cross(const Vector<T, 3>& other) const
	{
		return Vector<T, 3>
		{
			this->Data[1] * other[2] - this->Data[2] * other[1],
			this->Data[2] * other[0] - this->Data[0] * other[2],
			this->Data[0] * other[1] - this->Data[1] * other[0]
		};
	}

	template <typename T, std::size_t N>
	T Vector<T, N>::Magnitude() const
	{
		T sum = T{};
		for (std::size_t i = 0; i < N; i++)
		{
			sum += this->Data[i] * this->Data[i];
		}
		return std::sqrt(sum);
	}

	template <typename T, std::size_t N>
	Vector<T, N> Vector<T, N>::Normalized() const
	{
		T magnitude = this->Magnitude();
		Vector<T, N> norm;
		for (std::size_t i = 0; i < N; i++)
		{
			norm[i] = this->Data[i] / magnitude;
		}
		return *this;
	}
}

#endif