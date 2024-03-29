#ifndef __MATRIX__HPP__
#define __MATRIX__HPP__

#include <initializer_list>
#include <stdexcept>
#include <string>
#include <algorithm>

using size_m = unsigned int;

template <size_m M, size_m N, typename Type = float>
class Matrix {
	Type (*mat)[M][N];

public:
	constexpr Matrix() noexcept;
	template <size_m U, size_m V> constexpr Matrix(const Type (&arr)[U][V]) noexcept;
	constexpr Matrix(const std::initializer_list<Type> &list);
	constexpr Matrix(const std::initializer_list< std::initializer_list<Type> > &list);

	constexpr Matrix(const Matrix<M, N, Type> &target) noexcept;
	constexpr Matrix(Matrix<M, N, Type> &&target) noexcept;

	Matrix& operator=(const Matrix<M, N, Type> &target) noexcept;
	Matrix& operator=(Matrix<M, N, Type> &&target) noexcept;

	template <size_m U, size_m V> explicit operator Matrix<U, V, Type>() noexcept;
	template <size_m U> explicit operator Matrix<U, N, Type>() noexcept;

	const Type operator()(const size_m i, const size_m j) const;
	Type& operator()(const size_m i, const size_m j);

	const Matrix<N, M, Type> T() const noexcept;
};

template <size_m S, typename Type>
class Matrix<S, S, Type> {
	Type (*mat)[S][S];

public:
	constexpr Matrix() noexcept;
	template <size_m U, size_m V> constexpr Matrix(const Type (&arr)[U][V]) noexcept;
	constexpr Matrix(const std::initializer_list<Type> &list);
	constexpr Matrix(const std::initializer_list< std::initializer_list<Type> > &list);
	
	constexpr Matrix(const Matrix<S, S, Type> &target) noexcept;
	constexpr Matrix(Matrix<S, S, Type> &&target) noexcept;

	Matrix<S, S, Type>& operator=(const Matrix<S, S, Type> &target) noexcept;
	Matrix<S, S, Type>& operator=(Matrix<S, S, Type> &&target) noexcept;

	template <size_m U, size_m V> explicit operator Matrix<U, V, Type>() noexcept;
	template <size_m U> explicit operator Matrix<U, S, Type>() noexcept;

	const Type operator()(const size_m i, const size_m j) const;
	Type& operator()(const size_m i, const size_m j);

	const Matrix<S, S, Type> T() const noexcept;

	static const Type tr(const Matrix<S, S, Type> &target) noexcept;
	const Type tr() const noexcept;
};

template <size_m M, size_m N, typename Type>
const Type Matrix<M, N, Type>::operator()(const size_m i, const size_m j) const {
	if (i >= M || j >= N)
		throw std::out_of_range("access error: M is " + std::to_string(M) + 
			" and N is " + std::to_string(N) + 
			" but i is " + std::to_string(i) +
			", j is " + std::to_string(j));
	return (*mat)[i][j];
}

template <size_m M, size_m N, typename Type>
Type& Matrix<M, N, Type>::operator()(const size_m i, const size_m j) {
	if (i >= M || j >= N)
		throw std::out_of_range("access error: M is " + std::to_string(M) + 
			" and N is " + std::to_string(N) + 
			" but i is " + std::to_string(i) +
			", j is " + std::to_string(j));
	return (*mat)[i][j];
}

template <size_m S, typename Type>
const Type Matrix<S, S, Type>::operator()(const size_m i, const size_m j) const {
	if (i >= S || j >= S)
		throw std::out_of_range("access error: S is " + std::to_string(S) + 
			" but i is " + std::to_string(i) +
			", j is " + std::to_string(j));
	return (*mat)[i][j];
}

template <size_m S, typename Type>
Type& Matrix<S, S, Type>::operator()(const size_m i, const size_m j) {
	if (i >= S || j >= S)
		throw std::out_of_range("access error: S is " + std::to_string(S) + 
			" but i is " + std::to_string(i) +
			", j is " + std::to_string(j));
	return (*mat)[i][j];
}

template <size_m M, size_m N, typename Type>
constexpr Matrix<M, N, Type>::Matrix() noexcept : mat(reinterpret_cast<Type(*)[M][N]>(new Type[M * N])) { }

template <size_m M, size_m N, typename Type>
template <size_m U, size_m V>
Matrix<M, N, Type>::operator Matrix<U, V, Type>() noexcept {
	Matrix<U, V, Type> result;
	const size_m min_M = std::min(M, U);
	const size_m min_N = std::min(N, V);

	for (int i = 0; i < min_M; i++) {
		for (int j = 0; j < min_N; j++) {
			result(i, j) = (*this)(i, j);
		}
	}
	return result;
}

template <size_m M, size_m N, typename Type>
template <size_m U>
Matrix<M, N, Type>::operator Matrix<U, N, Type>() noexcept {
	Matrix<U, N, Type> result;
	const size_m min_M = std::min(U, M);

	std::copy(&(*this)(0, 0), &(*this)(0, 0) + min_M * N, &result(0, 0));
	return result;
}

template <size_m M, size_m N, typename Type>
constexpr Matrix<M, N, Type>::Matrix(const Matrix<M, N, Type> &target) noexcept : Matrix<M, N, Type>() {
	std::copy(&target(0, 0), &target(0, 0) + M * N, &(*this)(0, 0));
}

template <size_m M, size_m N, typename Type>
constexpr Matrix<M, N, Type>::Matrix(Matrix<M, N, Type> &&target) noexcept {
	mat = target.mat;
	target.mat = nullptr;
}

template <size_m M, size_m N, typename Type> 
template <size_m U, size_m V>
constexpr Matrix<M, N, Type>::Matrix(const Type (&arr)[U][V]) noexcept : Matrix<M, N, Type>() {
	static_assert(U <= M && V <= N, "Initializer list size must be smaller than matrix size");

	for (int i = 0; i < U; i++) {
		for (int j = 0; j < V; j++) {
			(*this)(i, j) = arr[i][j];
		}
	}
}

template <size_m M, size_m N, typename Type>
constexpr Matrix<M, N, Type>::Matrix(const std::initializer_list<Type> &list) : Matrix<M, N, Type>() {
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

template <size_m M, size_m N, typename Type>
constexpr Matrix<M, N, Type>::Matrix(const std::initializer_list< std::initializer_list<Type> > &list) : Matrix<M, N, Type>() {
	if (list.size() > M)
		throw std::out_of_range("Initializer list size must be smaller than matrix size");

	int i = 0, j = 0;
	for (const auto &inner_list : list) {
		if (inner_list.size() > N)
			throw std::out_of_range("Initializer list size must be smaller tham matrix size");

		for (const auto &element : inner_list) {
			(*this)(i, j++) = element;
		}
		j = 0; i++;
	}
}

template <size_m S, typename Type>
constexpr Matrix<S, S, Type>::Matrix() noexcept : mat(reinterpret_cast<Type(*)[S][S]>(new Type[S * S])) { }

template <size_m S, typename Type>
template <size_m U, size_m V>
Matrix<S, S, Type>::operator Matrix<U, V, Type>() noexcept {
	Matrix<U, V, Type> result;
	const size_m min_M = std::min(S, U);
	const size_m min_N = std::min(S, V);

	for (size_m i = 0; i < min_M; i++) {
		for (size_m j = 0; j < min_N; j++) {
			result(i, j) = (*this)(i, j);
		}
	}
	return result;
}

template <size_m S, typename Type>
template <size_m U>
Matrix<S, S, Type>::operator Matrix<U, S, Type>() noexcept {
	Matrix<U, S, Type> result;
	const size_m min_M = std::min(U, S);

	std::copy((*this)(0, 0), (*this)(0, 0) + min_M * S, &result(0, 0));
	return result;
}

template <size_m S, typename Type>
constexpr Matrix<S, S, Type>::Matrix(const Matrix<S, S, Type> &target) noexcept : Matrix<S, S, Type>() {
	std::copy(&target(0, 0), &target(0, 0) + S * S, &(*this)(0, 0));
}

template <size_m S, typename Type>
constexpr Matrix<S, S, Type>::Matrix(Matrix<S, S, Type> &&target) noexcept {
	mat = target.mat;
	target.mat = nullptr;
}
	
template <size_m S, typename Type> 
template <size_m U, size_m V>
constexpr Matrix<S, S, Type>::Matrix(const Type (&arr)[U][V]) noexcept : Matrix<S, S, Type>() {
	static_assert(U <= S && V <= S, "Initializer list size must be smaller than matrix size");
	for (int i = 0; i < U; i++) {
		for (int j = 0; j < V; j++) {
			(*this)(i, j) = arr[i][j];
		}
	}
}

template <size_m S, typename Type>
constexpr Matrix<S, S, Type>::Matrix(const std::initializer_list<Type> &list) : Matrix<S, S, Type>() {
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

template <size_m S, typename Type>
constexpr Matrix<S, S, Type>::Matrix(const std::initializer_list< std::initializer_list<Type> > &list) : Matrix<S, S, Type>() {
	if (list.size() > S)
		throw std::out_of_range("Initializer list size must be smaller than matrix size");

	int i = 0, j = 0;
	for (const auto &inner_list : list) {
		if (inner_list.size() > S)
			throw std::out_of_range("Initializer list size must be smaller tham matrix size");

		for (const auto &element : inner_list) {
			(*this)(i, j++) = element;
		}
		j = 0; i++;
	}
}

template <size_m M, size_m N, typename Type>
Matrix<M, N, Type>& Matrix<M, N, Type>::operator=(const Matrix<M, N, Type> &target) noexcept {
	std::copy(&target(0, 0), &target(0, 0) + M * N, &(*this)(0, 0));
	return *this;
}

template <size_m M, size_m N, typename Type>
Matrix<M, N, Type>& Matrix<M, N, Type>::operator=(Matrix<M, N, Type> &&target) noexcept {
	delete []mat;
	mat = target.mat;
	target.mat = nullptr;
	return *this;
}

template <size_m S, typename Type>
Matrix<S, S, Type>& Matrix<S, S, Type>::operator=(const Matrix<S, S, Type> &target) noexcept {
	std::copy(&target(0, 0), &target(0, 0) + S * S, &(*this)(0, 0));
	return *this;
}

template <size_m S, typename Type>
Matrix<S, S, Type>& Matrix<S, S, Type>::operator=(Matrix<S, S, Type> &&target) noexcept {
	delete []mat;
	mat = target.mat;
	target.mat = nullptr;
	return *this;
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
const Matrix<S, S, Type> Matrix<S, S, Type>::T() const noexcept {
	Matrix<S, S, Type> result;
	for (int i = 0; i < S; i++) {
		for (int j = 0; j < S; j++) {
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

template <size_m M, size_m N, typename Type>
std::ostream& operator<<(std::ostream &stream, const Matrix<M, N, Type> &target) {
	stream << '[' << '\n';
	for (int i = 0; i < M; i++) {
		stream << '\t';
		for (int j = 0; j < N; j++) {
			stream << target(i, j) << '\t';
		} 	
		stream << '\n';
	}
	stream << ']';
	return stream;	
}

#endif

