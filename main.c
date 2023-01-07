#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>
#include <limits.h>
#define MAX_INT 10000;	

int* allocateMemory(int m);
int** allocateMemory2d(int m, int n);


int main() {

	char* ptr;			//문자열을 넣기위한 포인터
	int* data;
	char buffer[100];		//fgets함수를 위한 버퍼입니다. 
	int size = 0;
	int p = 0;
	int j = 1;
	int k = 2;
	int num = 1;		//몇 번째 그래프인지를 나타내는 변수

	int** cost = NULL; //집합 C 정점간 가중치
	int* distance = NULL; //집합 D 최소거리
	int* selection = NULL; //집합 S 최소거리 구해진 정점들
	int** path = NULL; //각 정점까지 최소 경로 - 각 행의 인덱스를 정점 번호로 하고 행에는 1번 정점으로부터 각 정점까지의 경로를 넣으려고 합니다. 예) 3번 인덱스 - 1 4 3

	FILE* fpr = fopen("input2.txt", "r");
	if (fpr == NULL) {
		printf("%s\n", "ERROR:파일 명을 확인해주세요.");
		return 0;
	}

	while (!feof(fpr)) {		//여러 개의 그래프를 받아야 하기 때문에 파일 끝까지 while문을 돌립니다.
		fscanf(fpr, "%d", &size); //맨 앞 숫자를 읽어 배열의 사이즈 측정
		fgetc(fpr); //텍스트 파일에 있는 엔터 지우기

		data = allocateMemory(20);				
		cost = allocateMemory2d(size + 1, size + 1);  
		distance = allocateMemory(size + 1);		
		selection = allocateMemory(size + 1);
		path = allocateMemory2d(size + 1, size + 1);

		for (int i = 0; i < size + 1; i++) {				//cost 배열 초기화
			for (int j = 0; j < size + 1; j++) {
				cost[i][j] = MAX_INT;
			}
		}

		for (int i = 0; i < size; i++) {				//우선 한개의 그래프 정보를 받기위해 size만큼 for문을 돌립니다.
			p = 0;
			j = 1;
			k = 2;
			for (int l = 0; l < 20; l++) {
				data[l] = 0;						//data배열 초기화
			}
			fgets(buffer, 100, fpr);			//파일에서 한 줄을 buffer에 입력 받습니다.
			ptr = strtok(buffer, " ");			//띄어쓰기를 기준으로 buffer에서 문자열 하나를 ptr에 받습니다.
			while (ptr != NULL) {
				data[p] = atoi(ptr);			//문자열 하나를 정수로 바꾸고 data배열에 저장합니다.
				p++;
				ptr = strtok(NULL, " ");		//다음 문자열 받기
			}
			while (data[j] != 0) {
				cost[data[0]][data[j]] = data[k];	//data의 첫번째 숫자는 cost배열의 행, data배열의 짝수번째 숫자는 열, data배열의 3이상의 홀수번째 숫자는 가중치
				j += 2;
				k += 2;								//이렇게 저장하면 cost배열의 0행이랑 0열은 사용하지 않습니다.

			}
		}



		//최단경로 구하기

		selection[1] = 1;
		distance[0] = MAX_INT;

		for (int i = 1; i <= size; i++) {			//정점 1로부터의 거리로 distance 배열 초기화
			distance[i] = cost[1][i];			
		}

		for (int i = 2; i <= size; i++) {			//최소 경로 초기화 - 초기 값은 1 2, 1 3, 1 4, 1 5, 와 같이 1번과 자기자신으로 했고, 나머지 값들은 전부 0으로 하였습니다.
			path[i][0] = 1;
			path[i][1] = i;
			for (int j = 2; j <= size; j++) {
				path[i][j] = 0;
			}
		}

		for (int i = 2; i <= size - 1; i++) {			//selection 배열에 정점을 하나씩 추가하는 반복문, selection[i]는 현재 루프에서 새롭게 선택할 정점을 의미

			//selection 배열에 포함되지 않으면서 최소 거리를 갖는 인덱스를 selection에 추가
			int minidx = 0;				
			for (int j = 2;j <= size; j++) {					
				int in_S = 0;			//selection 배열에 포함된 정점인지 여부를 0또는 1로 저장하는 변수
				for (int k = 1; k < i; k++) {
					if (j == selection[k])
						in_S = 1;
				}
				if (!in_S && distance[j] < distance[minidx])
					minidx = j;
			}
			selection[i] = minidx;

			//distance 배열 값을 갱신
			for (int j = 2; j <= size; j++) {			
				int in_S = 0;			//selection 배열에 포함된 정점인지 여부를 0또는 1로 저장하는 변수
				for (int k = 1; k <= i; k++) {
					if (j == selection[k])
						in_S = 1;
				}
				if (!in_S) {

					if (distance[selection[i]] + cost[selection[i]][j] < distance[j]) {
						distance[j] = distance[selection[i]] + cost[selection[i]][j];
						
						// 정점 j까지의 거리가 다른 값으로 바뀌었을 경우 경로도 바꾸어 줌
						int l = 0;
						while (path[selection[i]][l] != 0) {
							path[j][l] = path[selection[i]][l];
							l++;
						}
						path[j][l] = j;
					}
				}
			}
		}

		//결과 출력
		printf("그래프 [%d]\n------------------------------\n시작점: 1\n", num);
		for (int i = 2; i <= size; i++) {
			printf("정점 [%d]:", i);
			int j=0;
			while (path[i][j] != 0) {
				printf(" %d -", path[i][j]);
				j++;
			}
			printf("\b, 길이: %d\n", distance[i]);
		}
		printf("==============================\n\n\n");

		num++;		//그래프 번호를 증가시킴
	}
	
	for (int i = 0; i < size + 1; i++) {
		free(cost[i]);
		free(path[i]);
	}
	free(distance);
	free(selection);
	
}


int* allocateMemory(int m) {  //메모리 동적할당
	int* A;
	A = (int*)malloc(sizeof(int*) * m);

	return A;
}

int** allocateMemory2d(int m, int n) {
	int** A;
	A = (int**)malloc(sizeof(int*) * m);
	for (int i = 0; i < m; i++) {
		A[i] = (int*)malloc(sizeof(int) * n);
	}
	return A;
}
