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
	
	const Matrix<M, N, T> operator+() const noexcept;
	const Matrix<M, N, T> operator-() const noexcept;

	const Matrix<M, N, T> operator+(const Matrix<M, N, T> &target) const noexcept;
	const Matrix<M, N, T> operator-(const Matrix<M, N, T> &target) const noexcept;
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

	const Matrix<S, S, T> operator+() const noexcept;
	const Matrix<S, S, T> operator-() const noexcept;

	const Matrix<S, S, T> operator+(const Matrix<S, S, T> &target) const noexcept;
	const Matrix<S, S, T> operator-(const Matrix<S, S, T> &target) const noexcept;
};

template <unsigned int M, unsigned int N, typename T>
const Matrix<M, N, T> Matrix<M, N, T>::operator+() const noexcept {
	return *this;
}

template <unsigned int M, unsigned int N, typename T>
const Matrix<M, N, T> Matrix<M, N, T>::operator-() const noexcept {
	Matrix<M, N, T> result;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			result.mat[i][j] = -mat[i][j];
		}
	}
	return result;
}

template <unsigned int M, unsigned int N, typename T>
const Matrix<M, N, T> Matrix<M, N, T>::operator+(const Matrix<M, N, T> &target) const noexcept {
	Matrix<M, N, T> result;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			result.mat[i][j] = mat[i][j] + target[i][j];
		}
	}
	return result;
}

template <unsigned int M, unsigned int N, typename T>
const Matrix<M, N, T> Matrix<M, N, T>::operator-(const Matrix<M, N, T> &target) const noexcept {
	return *this + (-target);
}

#endif

