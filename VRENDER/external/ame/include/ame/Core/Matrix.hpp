#ifndef MATH_CORE_MATRIX_HPP
#define MATH_CORE_MATRIX_HPP

#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <initializer_list>
#include <iostream>

#include "Vector.hpp"

namespace ame
{
	// N x M where N is rows and M is columns
	template <typename T, std::size_t N, std::size_t M>
	class alignas(16) Matrix
	{
	public:
		// Constructors
		Matrix();
		explicit Matrix(const T& value);
		Matrix(std::initializer_list<T> values);

		Matrix(const Matrix<T, N, M>& other);
		Matrix<T, N, M>& operator=(const Matrix<T, N, M>& other);

		// Component Access
		T& operator()(std::size_t n, std::size_t m);
		const T& operator()(std::size_t n, std::size_t m) const;
		const T* data() const;
		const Matrix<T, M, N> as_column_major() const;

		// Arithmetic Operators
		Matrix<T, N, M> operator+(const Matrix<T, N, M>& addend) const;
		Matrix<T, N, M> operator-(const Matrix<T, N, M>& subtrahend) const;
		template <std::size_t P>
		Matrix<T, N, P> operator*(const Matrix<T, M, P>& multiplicand) const;
		Vector<T, N> operator*(const Vector<T, M>& multiplicand) const;
		Matrix<T, N, M> operator*(const T& multiplicand) const;
		Matrix<T, N, M> operator/(T& divisor) const;

		// Compound Assignment Operators
		Matrix<T, N, M>& operator+=(const Matrix<T, N, M>& addend);
		Matrix<T, N, M>& operator-=(const Matrix<T, N, M>& subtrahend);
		Matrix<T, N, M>& operator*=(T& multiplicand);
		Matrix<T, N, M>& operator/=(T& divisor);

		// Matrix Operations
		static Matrix<T, N, M> Identity() requires(N == M);

		Matrix<T, M, N> Transpose() const;
		Matrix<T, N, M> Inverse() const requires(N == M);
		T Determinant() const requires(N == M);

		template <std::size_t P>
		Matrix<T, N, M + P> Augment(const Matrix<T, N, P>& target) const;
	protected:
		T& operator[](const std::size_t i) { return this->Data[i]; }
		const T& operator[](const std::size_t i) const { return this->Data[i]; }

	private:
		T Data[N * M];

		constexpr std::size_t Index(std::size_t n, std::size_t m) const
		{
			return n * M + m;
		}
	};

	// Constructor Definitions
	template <typename T, std::size_t N, std::size_t M>
	Matrix<T, N, M>::Matrix()
	{
		for (std::size_t i = 0; i < N * M; i++)
		{
			this->Data[i] = T{};
		}
	}

	template <typename T, std::size_t N, std::size_t M>
	Matrix<T, N, M>::Matrix(const T& value)
	{
		for (std::size_t i = 0; i < N * M; i++)
		{
			this->Data[i] = value;
		}
	}

	template <typename T, std::size_t N, std::size_t M>
	Matrix<T, N, M>::Matrix(std::initializer_list<T> values)
	{
		if (values.size() != N * M)
		{
			throw std::runtime_error("ARE: Matrix Initializer Array Contains Incorrect Number of Elements.");
		}

		std::size_t i = 0;
		for (const auto& value : values)
		{
			this->Data[i] = value;
			i++;
		}
	}

	template <typename T, std::size_t N, std::size_t M>
	Matrix<T, N, M>::Matrix<T, N, M>(const Matrix<T, N, M>& other)
	{
		if (this == &other)
		{
			return;
		}

		for (std::size_t i = 0; i < N; i++)
		{
			for (std::size_t j = 0; j < M; j++)
			{
				this->Data[this->Index(i, j)] = other(i, j);
			}
		}
	}

	template <typename T, std::size_t N, std::size_t M>
	Matrix<T, N, M>& Matrix<T, N, M>::operator=(const Matrix<T, N, M>& other)
	{
		if (this == &other)
		{
			return *this;
		}

		for (std::size_t i = 0; i < N; i++)
		{
			for (std::size_t j = 0; j < M; j++)
			{
				this->Data[this->Index(i, j)] = other(i, j);
			}
		}

		return *this;
	}

	// Element Access Definitions
	template <typename T, std::size_t N, std::size_t M>
	T& Matrix<T, N, M>::operator()(std::size_t n, std::size_t m)
	{
		return this->Data[this->Index(n, m)];
	}

	template <typename T, std::size_t N, std::size_t M>
	const T& Matrix<T, N, M>::operator()(std::size_t n, std::size_t m) const
	{
		return this->Data[this->Index(n, m)];
	}

	template <typename T, std::size_t N, std::size_t M>
	const T* Matrix<T, N, M>::data() const
	{
		return &Data[0];
	}

	template <typename T, std::size_t N, std::size_t M>
	const Matrix<T, M, N> Matrix<T, N, M>::as_column_major() const
	{
		Matrix<M, N, T> column_major;
		for (std::size_t i = 0; i < M; i++)
		{
			for (std::size_t j = 0; j < N; j++)
			{
				column_major(i, j) = this->Data[this->Index(j, i)];
			}
		}
		return column_major;
	}

	// Arithmetic Operator Definitions
	// These should probably be redefined as free functions to properly do left hand multiplication
	template <typename T, std::size_t N, std::size_t M>
	Matrix<T, N, M> Matrix<T, N, M>::operator+(const Matrix<T, N, M>& addend) const
	{
		Matrix<T, N, M> sum;
		for (std::size_t i = 0; i < N * M; i++)
		{
			sum[i] = this->Data[i] + addend[i];
		}
		return sum;
	}

	template <typename T, std::size_t N, std::size_t M>
	Matrix<T, N, M> Matrix<T, N, M>::operator-(const Matrix<T, N, M>& subtrahend) const
	{
		Matrix<T, N, M> difference;
		for (std::size_t i = 0; i < N * M; i++)
		{
			difference[i] = this->Data[i] - subtrahend[i];
		}
		return difference;
	}

	template <typename T, std::size_t N, std::size_t M>
	template <std::size_t P>
	Matrix<T, N, P> Matrix<T, N, M>::operator*(const Matrix<T, M, P>& multiplicand) const
	{
		Matrix<T, N, P> product;
		for (std::size_t i = 0; i < N; i++)
		{
			for (std::size_t j = 0; j < P; j++)
			{
				T sum = T{};
				for (std::size_t k = 0; k < M; k++)
				{
					sum += this->Data[this->Index(i, k)] * multiplicand(k, j);
				}
				product(i, j) = sum;
			}
		}
		return product;
	}

	template <typename T, std::size_t N, std::size_t M>
	Vector<T, N> Matrix<T, N, M>::operator*(const Vector<T, M>& multiplicand) const
	{
		Vector<T, N> product;
		for (std::size_t i = 0; i < N; i++)
		{
			T sum = T{};
			for (std::size_t j = 0; j < M; j++)
			{
				sum += this->Data[this->Index(i, j)] * multiplicand(j);
			}
			product[i] = sum;
		}
		return product;
	}

	template <typename T, std::size_t N, std::size_t M>
	Matrix<T, N, M> Matrix<T, N, M>::operator*(const T& multiplicand) const
	{
		Matrix<T, N, M> product;
		for (std::size_t i = 0; i < N * M; i++)
		{
			product[i] = this->Data[i] * multiplicand;
		}
		return product;
	}

	// Compound Assignment Operators
	template <typename T, std::size_t N, std::size_t M>
	Matrix<T, N, M>& Matrix<T, N, M>::operator+=(const Matrix<T, N, M>& addend)
	{
		for (int i = 0; i < N * M; i++)
		{
			this->Data[i] += addend[i];
		}
		return *this;
	}

	template <typename T, std::size_t N, std::size_t M>
	Matrix<T, N, M>& Matrix<T, N, M>::operator-=(const Matrix<T, N, M>& subtrahend)
	{
		for (int i = 0; i < N * M; i++)
		{
			this->Data[i] -= subtrahend[i];
		}
		return *this;
	}

	template <typename T, std::size_t N, std::size_t M>
	Matrix<T, N, M>& Matrix<T, N, M>::operator*=(T& multiplicand)
	{
		for (int i = 0; i < N * M; i++)
		{
			this->Data[i] += multiplicand;
		}
		return *this;
	}

	template <typename T, std::size_t N, std::size_t M>
	Matrix<T, N, M>& Matrix<T, N, M>::operator/=(T& divisor)
	{
		for (int i = 0; i < N * M; i++)
		{
			this->Data[i] /= divisor;
		}
		return *this;
	}

	// Matrix Operations
	template <typename T, std::size_t N, std::size_t M>
	Matrix<T, N, M> Matrix<T, N, M>::Identity() requires(N == M)
	{
		Matrix<T, N, N> identity;
		for (std::size_t i = 0; i < N; i++)
		{
			identity(i, i) = T{ 1 };
		}
		return identity;
	}

	template <typename T, std::size_t N, std::size_t M>
	Matrix<T, M, N> Matrix<T, N, M>::Transpose() const
	{
		Matrix<T, M, N> transpose;
		for (std::size_t i = 0; i < N; i++)
		{
			for (std::size_t j = 0; j < M; j++)
			{
				transpose(j, i) = this->Data[this->Index(i, j)];
			}
		}
		return transpose;
	}

	template <typename T, std::size_t N, std::size_t M>
	Matrix<T, N, M> Matrix<T, N, M>::Inverse() const requires(N == M)
	{
		//static_assert(this->Determinant() != 0);

		// Implements Gauss - Jordan Elimination
		Matrix<T, N, N> A = *this;							// Copy current matrix for easier manipulation
		Matrix<T, N, N> I = Matrix<T, N, N>::Identity();	// Identity Matrix

		for (std::size_t col = 0; col < N; col++)
		{
			// Find Pivot Row
			std::size_t pivot = col;
			for (std::size_t row = col + 1; row < N; row++)
			{
				if (std::abs(A(row, col)) > std::abs(A(pivot, row)))
				{
					pivot = row;
				}
			}

			// Check for Singular Matrix
			if (A(pivot, col) == T{ 0 })
			{
				throw std::runtime_error("AME: Singular Matrices Cannot Be Inverted.");
			}

			// Swap Current Row with Pivot Row in Both A and I
			if (pivot != col)
			{
				for (std::size_t j = 0; j < N; j++)
				{
					std::swap(A(col, j), A(pivot, j));
					std::swap(I(col, j), I(pivot, j));
				}
			}

			// Normalize Pivot Row
			T pivot_val = A(col, col);
			for (std::size_t j = 0; j < N; j++)
			{
				A(col, j) /= pivot_val;
				I(col, j) /= pivot_val;
			}

			// Eliminate All Other Entries in This Column
			for (std::size_t row = 0; row < N; row++)
			{
				if (row == col) continue;

				T factor = A(row, col);
				for (std::size_t j = 0; j < N; j++)
				{
					A(row, j) -= factor * A(col, j);
					I(row, j) -= factor * I(col, j);
				}
			}
		}

		return I;
	}

	template <typename T, std::size_t N, std::size_t M>
	T Matrix<T, N, M>::Determinant() const requires(N == M)
	{

	}

	template <typename T, std::size_t N, std::size_t M>
	template <std::size_t P>
	Matrix<T, N, M + P> Matrix<T, N, M>::Augment(const Matrix<T, N, P>& target) const
	{
		Matrix<T, N, M + P> augment;
		for (std::size_t i = 0; i < N; i++)
		{
			// Copy data from this
			for (std::size_t j = 0; j < M; j++)
			{
				augment(i, j) = this->Data[this->Index(i, j)];
			}

			// Copy data from target
			for (std::size_t j = 0; j < P; j++)
			{
				augment(i, j + M) = target(i, j);
			}
		}
		return augment;
	}
}

// Free Functions
template <typename T, std::size_t N, std::size_t M>
std::ostream& operator<<(std::ostream& os, const ame::Matrix<T, N, M> mat)
{
	os << N << "x" << M << "\:" << std::endl;
	for (std::size_t i = 0; i < N; i++)
	{
		for (std::size_t j = 0; j < M; j++)
		{
			os << mat(i, j) << " ";
		}
		os << std::endl;
	}

	return os;
}

#endif