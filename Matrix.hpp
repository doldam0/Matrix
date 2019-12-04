#ifndef __MATRIX__HPP__
#define __MATRIX__HPP__

#include <initializer_list>
#include <stdexcept>
#include <string>

template <unsigned int M, unsigned int N, typename T = float>
class Matrix {
	T mat[M][N];

public:
	constexpr Matrix() noexcept : mat{ 0 } { }
	
	template <unsigned int U, unsigned int V>
	constexpr Matrix(const T (&arr)[M][N]) noexcept {
		static_assert(U <= M && V <= N, "Initializer list size must be smaller than matrix size");
		for (int i = 0; i < U; i++) {
			for (int j = 0; j < V; j++) {
				mat[i][j] = arr[i][j];
			}
		}
	}

	constexpr Matrix(const std::initializer_list<T> &list) {
		if (list.size() > M * N)
			throw std::out_of_range("Initializer list size must be smaller than matrix size");

		int i = 0, j = 0;
		for (const auto &element : list) {
			mat[i][j++] = element;
			if (j >= N) {
				j = 0;
				i++;
			}
		}
	}

	constexpr Matrix(const std::initializer_list< std::initializer_list<T> > &list) {
		if (list.size() > M)
			throw std::out_of_range("Initializer list size must be smaller than matrix size");

		int i = 0, j = 0;
		for (const auto &inner_list : list) {
			if (inner_list.size() > N)
				throw std::out_of_range("Initializer list size must be smaller tham matrix size");

			for (const auto &element : inner_list) {
				mat[i][j++] = element;
			}
			i++;
		}
	}

	const T operator()(const int i, const int j) const noexcept;
	const T& operator()(const int i, const int j) noexcept;
};

template <unsigned int S, typename T>
class Matrix<S, S, T> {
	T mat[S][S];

public:
	constexpr Matrix() noexcept : mat{ 0 } { }
	
	template <unsigned int U, unsigned int V>
	constexpr Matrix(const T (&arr)[U][V]) noexcept {
		static_assert(U <= S && V <= S, "Initializer list size must be smaller than matrix size");
		for (int i = 0; i < U; i++) {
			for (int j = 0; j < V; j++) {
				mat[i][j] = arr[i][j];
			}
		}
	}

	constexpr Matrix(const std::initializer_list<T> &list) {
		if (list.size() > S * S)
			throw std::out_of_range("Initializer list size must be smaller than matrix size");

		int i = 0, j = 0;
		for (const auto &element : list) {
			mat[i][j++] = element;
			if (j >= S) {
				j = 0;
				i++;
			}
		}
	}

	constexpr Matrix(const std::initializer_list< std::initializer_list<T> > &list) {
		if (list.size() > S)
			throw std::out_of_range("Initializer list size must be smaller than matrix size");

		int i = 0, j = 0;
		for (const auto &inner_list : list) {
			if (inner_list.size() > S)
				throw std::out_of_range("Initializer list size must be smaller tham matrix size");

			for (const auto &element : inner_list) {
				mat[i][j++] = element;
			}
			i++;
		}
	}

	const T operator()(const int i, const int j) const noexcept;
	const T& operator()(const int i, const int j) noexcept;
};

template <unsigned int M, unsigned int N, typename T>
const T Matrix<M, N, T>::operator()(const int i, const int j) const noexcept {
	return mat[i][j];
}

template <unsigned int M, unsigned int N, typename T>
const T& Matrix<M, N, T>::operator()(const int i, const int j) noexcept {
	return mat[i][j];
}

template <unsigned int M, unsigned int N, typename T>
const Matrix<M, N, T> operator+(const Matrix<M, N, T> &target) noexcept {
	return target;
}

template <unsigned int M, unsigned int N, typename T>
const Matrix<M, N, T> operator-(const Matrix<M, N, T> &target) noexcept {
	Matrix<M, N, T> result;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			result(i, j) = -target(i, j);
		}
	}
	return result;
}

template <unsigned int M, unsigned int N, typename T>
const Matrix<M, N, T> operator+(const Matrix<M, N, T> &m1, const Matrix<M, N, T> &m2) noexcept {
	Matrix<M, N, T> result;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			result(i, j) = m1(i, j) + m2(i, j);
		}
	}
	return result;
}

template <unsigned int M, unsigned int N, typename T>
const Matrix<M, N, T> operator-(const Matrix<M, N, T> &m1, const Matrix<M, N, T> &m2) noexcept {
	return m1 + (-m2);
}

template <unsigned int M, unsigned int N, typename T>
const Matrix<M, N, T> operator*(const Matrix<M, N, T> &target, const int mul) noexcept {
	Matrix<M, N, T> result;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			result(i, j) = target(i, j) * target;
		}
	}
	return result;
}

template <unsigned int M, unsigned int N, typename T>
const Matrix<M, N, T> operator*(const int mul, const Matrix<M, N, T> &target) noexcept {
	return target * mul;
}

template <unsigned int M, unsigned int N, unsigned int P, typename T>
const Matrix<M, P, T> operator*(const Matrix<M, N, T> &m1, const Matrix<N, P, T> &m2) noexcept {
	Matrix<M, P, T> result;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < P; j++) {
			for (int k = 0; k < N; k++) {
				result(i, j) += m1(i, k) * m2(k, j);
			}
		}
	}
	return result;
}

#endif

