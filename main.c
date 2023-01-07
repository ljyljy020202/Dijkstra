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

	char* ptr;			//���ڿ��� �ֱ����� ������
	int* data;
	char buffer[100];		//fgets�Լ��� ���� �����Դϴ�. 
	int size = 0;
	int p = 0;
	int j = 1;
	int k = 2;
	int num = 1;		//�� ��° �׷��������� ��Ÿ���� ����

	int** cost = NULL; //���� C ������ ����ġ
	int* distance = NULL; //���� D �ּҰŸ�
	int* selection = NULL; //���� S �ּҰŸ� ������ ������
	int** path = NULL; //�� �������� �ּ� ��� - �� ���� �ε����� ���� ��ȣ�� �ϰ� �࿡�� 1�� �������κ��� �� ���������� ��θ� �������� �մϴ�. ��) 3�� �ε��� - 1 4 3

	FILE* fpr = fopen("input2.txt", "r");
	if (fpr == NULL) {
		printf("%s\n", "ERROR:���� ���� Ȯ�����ּ���.");
		return 0;
	}

	while (!feof(fpr)) {		//���� ���� �׷����� �޾ƾ� �ϱ� ������ ���� ������ while���� �����ϴ�.
		fscanf(fpr, "%d", &size); //�� �� ���ڸ� �о� �迭�� ������ ����
		fgetc(fpr); //�ؽ�Ʈ ���Ͽ� �ִ� ���� �����

		data = allocateMemory(20);				
		cost = allocateMemory2d(size + 1, size + 1);  
		distance = allocateMemory(size + 1);		
		selection = allocateMemory(size + 1);
		path = allocateMemory2d(size + 1, size + 1);

		for (int i = 0; i < size + 1; i++) {				//cost �迭 �ʱ�ȭ
			for (int j = 0; j < size + 1; j++) {
				cost[i][j] = MAX_INT;
			}
		}

		for (int i = 0; i < size; i++) {				//�켱 �Ѱ��� �׷��� ������ �ޱ����� size��ŭ for���� �����ϴ�.
			p = 0;
			j = 1;
			k = 2;
			for (int l = 0; l < 20; l++) {
				data[l] = 0;						//data�迭 �ʱ�ȭ
			}
			fgets(buffer, 100, fpr);			//���Ͽ��� �� ���� buffer�� �Է� �޽��ϴ�.
			ptr = strtok(buffer, " ");			//���⸦ �������� buffer���� ���ڿ� �ϳ��� ptr�� �޽��ϴ�.
			while (ptr != NULL) {
				data[p] = atoi(ptr);			//���ڿ� �ϳ��� ������ �ٲٰ� data�迭�� �����մϴ�.
				p++;
				ptr = strtok(NULL, " ");		//���� ���ڿ� �ޱ�
			}
			while (data[j] != 0) {
				cost[data[0]][data[j]] = data[k];	//data�� ù��° ���ڴ� cost�迭�� ��, data�迭�� ¦����° ���ڴ� ��, data�迭�� 3�̻��� Ȧ����° ���ڴ� ����ġ
				j += 2;
				k += 2;								//�̷��� �����ϸ� cost�迭�� 0���̶� 0���� ������� �ʽ��ϴ�.

			}
		}



		//�ִܰ�� ���ϱ�

		selection[1] = 1;
		distance[0] = MAX_INT;

		for (int i = 1; i <= size; i++) {			//���� 1�κ����� �Ÿ��� distance �迭 �ʱ�ȭ
			distance[i] = cost[1][i];			
		}

		for (int i = 2; i <= size; i++) {			//�ּ� ��� �ʱ�ȭ - �ʱ� ���� 1 2, 1 3, 1 4, 1 5, �� ���� 1���� �ڱ��ڽ����� �߰�, ������ ������ ���� 0���� �Ͽ����ϴ�.
			path[i][0] = 1;
			path[i][1] = i;
			for (int j = 2; j <= size; j++) {
				path[i][j] = 0;
			}
		}

		for (int i = 2; i <= size - 1; i++) {			//selection �迭�� ������ �ϳ��� �߰��ϴ� �ݺ���, selection[i]�� ���� �������� ���Ӱ� ������ ������ �ǹ�

			//selection �迭�� ���Ե��� �����鼭 �ּ� �Ÿ��� ���� �ε����� selection�� �߰�
			int minidx = 0;				
			for (int j = 2;j <= size; j++) {					
				int in_S = 0;			//selection �迭�� ���Ե� �������� ���θ� 0�Ǵ� 1�� �����ϴ� ����
				for (int k = 1; k < i; k++) {
					if (j == selection[k])
						in_S = 1;
				}
				if (!in_S && distance[j] < distance[minidx])
					minidx = j;
			}
			selection[i] = minidx;

			//distance �迭 ���� ����
			for (int j = 2; j <= size; j++) {			
				int in_S = 0;			//selection �迭�� ���Ե� �������� ���θ� 0�Ǵ� 1�� �����ϴ� ����
				for (int k = 1; k <= i; k++) {
					if (j == selection[k])
						in_S = 1;
				}
				if (!in_S) {

					if (distance[selection[i]] + cost[selection[i]][j] < distance[j]) {
						distance[j] = distance[selection[i]] + cost[selection[i]][j];
						
						// ���� j������ �Ÿ��� �ٸ� ������ �ٲ���� ��� ��ε� �ٲپ� ��
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

		//��� ���
		printf("�׷��� [%d]\n------------------------------\n������: 1\n", num);
		for (int i = 2; i <= size; i++) {
			printf("���� [%d]:", i);
			int j=0;
			while (path[i][j] != 0) {
				printf(" %d -", path[i][j]);
				j++;
			}
			printf("\b, ����: %d\n", distance[i]);
		}
		printf("==============================\n\n\n");

		num++;		//�׷��� ��ȣ�� ������Ŵ
	}
	
	for (int i = 0; i < size + 1; i++) {
		free(cost[i]);
		free(path[i]);
	}
	free(distance);
	free(selection);
	
}


int* allocateMemory(int m) {  //�޸� �����Ҵ�
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
