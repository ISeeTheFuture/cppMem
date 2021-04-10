#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
	pid_t pid, ppid;
	int intTmp, intTmp2;
	
	// 생성할 자식 프로세스의 개수
	int intChildsCt = 10;

	//자식프로세스의 PID저장할 목록(불필요)
	int intChilds[10];
	
	// 프로세스 당 메모리를 할당 받을 횟수
	int intMemAllocCt = 10;

	// 위의 횟수만큼 할당받은 메모리의 포인터를 저장할 포인터배열
	char *cpStr[100];

	// 한번 메모리 할당 받을 때의 메모리 크기(Byte)
	int intMemSize=10240000;
	
	// 한번 할당받은 메모리를 4K Byte 단위로 나누었을 때의 조각의 개수
	int intPageCt = intMemSize/4096;

	// 부모의 PID 획득
	ppid = getpid();
	
	pid = 1;
	
	clock_t startTime = clock();

	// 자식 프로세스를 intChildsCT에서 지정한 만큼 반복하여 생성
	for (intTmp=0; intTmp < intChildsCt; intTmp++) {
		if (pid != 0) { // 부모만 fork()가 실행되도록
			// 자식을 생성(fork())하고 부모프로세스는 pid에 자식 pid를 받음. 자식에게는 0이 리턴됨.
			if ((pid=fork()) < 0) {
				printf("%d'th child process fork error\n", intTmp);
				return 2;
			} else if (pid != 0) { // 부모는 pid를 배열에 저장.
				intChilds[intTmp] = pid;
			}
		}
	}

	// for() 후 부모프로세스가 실행할 루틴.
	if ( pid != 0 ) {
		printf ("==== ChildProcess List====\n");
		for (intTmp=0; intTmp < intChildsCt; intTmp++) {
			printf ("%d'st Child process pid : %d\n", intTmp, intChilds[intTmp]);
		}
	}
	
	// fork() 후 자식 프로세스가 실행할 루틴.
	if ( pid == 0 ) {
		printf ("Child Routine...\n");
		// 생성된 자식은 intMemAllocCt에서 지장한 횟수만큼 반복하여 메모리 할당
		
		// 총 할당받는 메모리는 intChildsCt(자식의 개수) * intMemAlloct(자식 당 할당받을 횟수)  * intMemSize(1회에 할당받을 메모리) 만큼
		for (intTmp2=0; intTmp2<intMemAllocCt; intTmp2++) {
			// 메모리를 할당하고 첫 주소를 cpStr[]에 저장. (포인터를 이해하려면 아래 라인을 완전하게 이해해야 함.)
			cpStr[intTmp2] = (char *)malloc(intMemSize);
			
			// 할당받은 메모리의 모든 바이트에 알파벳 K를 쓴다. (이 라인을 활성화하면 CPU 100%)
			// for (intTmp=0; intTmp<intMemSize; ++intTmp) *((char *)cpStr+intTmp)='K';

			printf("Child %d , Memory Allocate : %d\n", getpid(), intTmp2);
			sleep(1);
		}
	}
	printf("ppid = %d, getpid = %d\n", ppid, getpid());
	//printf("%s\n", (char*)cpStr);
	
	// 메모리 할당 할 시간 대기. 메모리 할당량 당 1초씩은 기다려주자.
	sleep (intMemAllocCt);

	// 각 자식 프로세스는 자신이 할당받은 메모리 횟수 만큼에 대해
	for (intTmp2=0; intTmp2<intMemAllocCt; intTmp2++) {
		// 할당받은 메모리를 4096바이트(page (4K))로 나눈 몫의 횟수만큼 (할당받은 페이지의 수 만큼) 반복
		for (intTmp=1; intTmp<intPageCt; intTmp++) {
			//각 페이지의 첫번째 바이트에 'K'를 쓴다. 할당받은 모든 페이지를 실제로 접근 발생시켜 실제 사용하는 페이지로 만든다.
			*((char *)cpStr[intTmp2]+(4096*intTmp)) = 'K'; // 여기서 자꾸 segfault. 원인 알아보자.
		}
	}
	// 할당받은 메모리의 모든 페이지를 한번씩 액세스하고 2초간 대기
	sleep (2);

	// 할당받은 메모리를 반환한다.
	for (intTmp2=0; intTmp2<intMemAllocCt; intTmp2++) {
		free(cpStr[intTmp2]);
		printf("pid=%d, %d is free...\n", getpid(), intTmp2);
	}

	clock_t endTime = clock();
	printf("Time: %lf\n", (double)(endTime - startTime)/CLOCKS_PER_SEC);

	//프로그램 종료
	return 0;
}