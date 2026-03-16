// 2026_1_OOAD.cpp : 애플리케이션의 진입점을 정의합니다.
//

#include "2026_1_OOAD.h"

using namespace std;

int SampleAdd(int a, int b) {
    return a + b;
}

int main()
{
	cout << "Hello CMake." << endl;
    cout << "3 + 5 = " << SampleAdd(3, 5) << endl;
	return 0;
}
