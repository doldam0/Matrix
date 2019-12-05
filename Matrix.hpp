#ifndef __MATRIX__HPP__
#define __MATRIX__HPP__

#include <initializer_list>
#include <stdexcept>
#include <string>

using size_m = unsigned int;

template <size_m M, size_m N, typename Type = float>
class Matrix {
	Type mat[M][N];

public:
	constexpr Matrix() noexcept : mat{ 0 } { }
	
	template <size_m U, size_m V>
	constexpr Matrix(const Type (&arr)[M][N]) noexcept {
		static_assert(U <= M && V <= N, "Initializer list size must be smaller than matrix size");
		for (int i = 0; i < U; i++) {
			for (int j = 0; j < V; j++) {
				(*this)(i, j) = arr[i][j];
			}
		}
	}

	constexpr Matrix(const std::initializer_list<Type> &list) {
		if (list.size() > M * N)
			throw std::out_of_range("Initializer list size must be smaller than matrix size");

		int i = 0, j = 0;
		for (const auto &element : list) {
			(*this)(i, j++) = element;
			if (j >= N) {
				j = 0;
				i++;
			}
		}
	}

	constexpr Matrix(const std::initializer_list< std::initializer_list<Type> > &list) {
		if (list.size() > M)
			throw std::out_of_range("Initializer list size must be smaller than matrix size");

		int i = 0, j = 0;
		for (const auto &inner_list : list) {
			if (inner_list.size() > N)
				throw std::out_of_range("Initializer list size must be smaller tham matrix size");

			for (const auto &element : inner_list) {
				(*this)(i, j++) = element;
			}
			i++;
		}
	}

	const Type operator()(const size_m i, const size_m j) const noexcept;
	Type& operator()(const size_m i, const size_m j) noexcept;

	const Matrix<N, M, Type> T() const noexcept;
};

template <size_m S, typename Type>
class Matrix<S, S, Type> {
	Type mat[S][S];

public:
	constexpr Matrix() noexcept : mat{ 0 } { }
	
	template <size_m U, size_m V>
	constexpr Matrix(const Type (&arr)[U][V]) noexcept {
		static_assert(U <= S && V <= S, "Initializer list size must be smaller than matrix size");
		for (int i = 0; i < U; i++) {
			for (int j = 0; j < V; j++) {
				(*this)(i, j) = arr[i][j];
			}
		}
	}

	constexpr Matrix(const std::initializer_list<Type> &list) {
		if (list.size() > S * S)
			throw std::out_of_range("Initializer list size must be smaller than matrix size");

		int i = 0, j = 0;
		for (const auto &element : list) {
			(*this)(i, j++) = element;
			if (j >= S) {
				j = 0;
				i++;
			}
		}
	}

	constexpr Matrix(const std::initializer_list< std::initializer_list<Type> > &list) {
		if (list.size() > S)
			throw std::out_of_range("Initializer list size must be smaller than matrix size");

		int i = 0, j = 0;
		for (const auto &inner_list : list) {
			if (inner_list.size() > S)
				throw std::out_of_range("Initializer list size must be smaller tham matrix size");

			for (const auto &element : inner_list) {
				(*this)(i, j++) = element;
			}
			i++;
		}
	}

	const Type operator()(const size_m i, const size_m j) const noexcept;
	Type& operator()(const size_m i, const size_m j) noexcept;

	const Matrix<S, S, Type> T() const noexcept;

	static const Type tr(const Matrix<S, S, Type> &target) noexcept;
	const Type tr() const noexcept;
};

template <size_m M, size_m N, typename Type>
const Type Matrix<M, N, Type>::operator()(const size_m i, const size_m j) const noexcept {
	static_assert(i < M && j < N, "access error");
	return mat[i][j];
}

template <size_m M, size_m N, typename Type>
Type& Matrix<M, N, Type>::operator()(const size_m i, const size_m j) noexcept {
	static_assert(i < M && j < N, "access error");
	return mat[i][j];
}

template <size_m M, size_m N, typename Type>
const Matrix<M, N, Type> operator+(const Matrix<M, N, Type> &target) noexcept {
	return target;
}

template <size_m M, size_m N, typename Type>
const Matrix<M, N, Type> operator-(const Matrix<M, N, Type> &target) noexcept {
	Matrix<M, N, Type> result;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			result(i, j) = -target(i, j);
		}
	}
	return result;
}

template <size_m M, size_m N, typename Type>
const Matrix<M, N, Type> operator+(const Matrix<M, N, Type> &m1, const Matrix<M, N, Type> &m2) noexcept {
	Matrix<M, N, Type> result;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			result(i, j) = m1(i, j) + m2(i, j);
		}
	}
	return result;
}

template <size_m M, size_m N, typename Type>
const Matrix<M, N, Type> operator-(const Matrix<M, N, Type> &m1, const Matrix<M, N, Type> &m2) noexcept {
	return m1 + (-m2);
}

template <size_m M, size_m N, typename Type>
const Matrix<M, N, Type> operator*(const Matrix<M, N, Type> &target, const int mul) noexcept {
	Matrix<M, N, Type> result;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			result(i, j) = target(i, j) * target;
		}
	}
	return result;
}

template <size_m M, size_m N, typename Type>
const Matrix<M, N, Type> operator*(const int mul, const Matrix<M, N, Type> &target) noexcept {
	return target * mul;
}

template <size_m M, size_m N, size_m P, typename Type>
const Matrix<M, P, Type> operator*(const Matrix<M, N, Type> &m1, const Matrix<N, P, Type> &m2) noexcept {
	Matrix<M, P, Type> result;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < P; j++) {
			for (int k = 0; k < N; k++) {
				result(i, j) += m1(i, k) * m2(k, j);
			}
		}
	}
	return result;
}

template <size_m M, size_m N, typename Type>
const Matrix<N, M, Type> Matrix<M, N, Type>::T() const noexcept {
	Matrix<N, M, Type> result;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			result(j, i) = (*this)(i, j);
		}
	}
	return result;
}

template <size_m S, typename Type>
const Type Matrix<S, S, Type>::tr(const Matrix<S, S, Type> &target) noexcept {
	Type result = 0;
	for (int i = 0; i < S; i++) {
		result += target(i, i);
	}
	return result;
}

template <size_m S, typename Type>
const Type Matrix<S, S, Type>::tr() const noexcept {
	return tr(*this);
}

#endif

