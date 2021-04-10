#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

int main(void) {
	clock_t startTime = clock();
	int intMemSize = 1024000000; // 1기가. 약 20초 내외의 테스트하기 적당한 시간이 나온다.

	for(int i = 0; i < 10; i++) {
		char* cpStr = (char*)malloc(intMemSize);
		for(int i = 0; i < intMemSize; i++){
			cpStr[i] = 'A';
		}
		free(cpStr);
		std::cout << "test : " << (i+1) << "회 종료" << std::endl;
	}
	clock_t endTime = clock();
	printf("Time: %lf\n", (double)(endTime - startTime)/CLOCKS_PER_SEC);
	return 0;
}