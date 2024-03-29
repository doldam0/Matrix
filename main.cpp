#include <iostream>
#include "Matrix.hpp"

using namespace std;

int main() {
	Matrix<3, 4> mat;
	int arr[3][3] = {
		{ 2, 3, 5 },
		{ 1, 2, 6 },
		{ 7, 7, 4 }
	};
	Matrix<4, 4, int> mat1(arr);
	Matrix<2, 3, int> mat2 = { 1, 2, 3, 4 };
	Matrix<2, 2, int> mat3 = {
		{ 5, 2 },
		{ 3, 3 }
	};
	
	cout << mat3 << endl;
	cout << mat3.T() << endl;
	cout << mat3.tr() << endl;

	mat2 = static_cast< Matrix<2, 3, int> >(mat1);
	cout << mat2 << endl;
	return 0;
}
