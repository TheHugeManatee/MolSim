#include <omp.h>
#include <iostream>
#include <stdio.h>

#include "src/utils/Matrix.h"

using namespace utils;
using namespace std;

int main(void) {
/*
	Matrix i, s, sh, tr, rot;
	double v1[3]={1,2,3}, v2[3]={1,2,3}, v3[3]={1,2,3};

	cout << "Identity Matrix:" << endl<< i.toString() << endl;


	s = Matrix::scale(0.1, 0.2, 0.3);
	cout << "Scale Matrix:" << endl << s.toString() << endl;

	sh = Matrix::shear(1, 2, 3, 4, 5, 6);
	cout << "shear Matrix:" << endl << sh.toString() << endl;

	tr = Matrix::translate(10, 20, 30);
	cout << "translation Matrix:" << endl << tr.toString() << endl;

	rot = Matrix::rotatex0(180);
	cout << "rotate Matrix:" << endl<< rot.toString() << endl;

	cout << "translate, then rotate Matrix:" << endl << (rot*tr).toString() << endl;

	s.transform(v2);
	cout << "scaled vector:" << v2[0] << " " << v2[1] << " " << v2[2] << endl;

	tr.transform(v2);
	cout << "translated vector:" << v2[0] << " " << v2[1] << " " << v2[2] << endl;

	(s*tr).transform(v3);
	cout << "translated, then rotated vector:" << v3[0] << " " << v3[1] << " " << v3[2] << endl;
*/
	/*#pragma omp parallel
	{
		int tid = omp_get_thread_num();
		printf("Thread %i\n", tid);
	}
*/
	printf("all done!");
}
