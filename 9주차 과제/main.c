#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define MAX_QUEUE_SIZE 5
typedef struct { // 요소 타입
	int id;
	int arrival_time;
	int service_time;
} element;
typedef struct { // 원형큐 타입
	element  data[MAX_QUEUE_SIZE];
	int front, rear;
} QueueType;



void init_queue(QueueType* q)
{
	q->front = q->rear = 0;
}


int is_empty(QueueType* q)
{
	return (q->front == q->rear);
}


int is_full(QueueType* q)
{
	return ((q->rear + 1) % MAX_QUEUE_SIZE == q->front);
}


void enqueue(QueueType* q, element item)
{
	if (is_full(q))
		printf("대기열이 가득 차 있습니다. 고객이 돌아갑니다.\n");
	q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
	q->data[q->rear] = item;
}

element dequeue(QueueType* q)
{
	if (is_empty(q))
		printf("대기열에 아무도 없습니다.\n");
	q->front = (q->front + 1) % MAX_QUEUE_SIZE;
	return q->data[q->front];
}

int main(void)
{
	int minutes = 50000;
	
	int total_wait = 0;
	int total_customers = 0;
	int service_time = 0;
	int service_customer;

	int vip_wait = 0;			//VIP 대기시간을 저장하는 변수 선언
	int vip_customers = 0;		//VIP 고객 수를 저장하는 변수 선언
	int regular_wait = 0;		//일반 고객 대기시간을 저장하는 변수 선언
	int regular_customers = 0;	//일반 고객 수를 저장하는 변수 선언

	int randValue = 0;			//고객이 생성될 때 rand()값을 여러번 활용해야 하므로 변수 선언

	QueueType regular; // 일반 고객 원형큐 선언
	QueueType Vip;  // VIP 고객 원형큐 선언
	init_queue(&regular);
	init_queue(&Vip);
	srand(time(NULL));

	for (int clock = 0; clock < minutes; clock++) {
		printf("현재시각=%d\n", clock);
		randValue = rand() % 10;  //랜덤함수로 생성한 값을 저장하여 이후 같은 값을 여러번 사용

		if (randValue < 3) { // 전체 고객 발생 확률: 30%
			
			//전체 고객에 대해 공통된 부분을 연산
			element customer;
			customer.id = total_customers++;
			customer.arrival_time = clock;			// 
			customer.service_time = rand() % 3 + 1;	// 고객 업무 소요 시간
			

			if (randValue < 1) { // Vip 고객 발생 확률 : 10%, 나머지 20%는 일반고객 발생
				enqueue(&Vip, customer);
				vip_customers++;  //vip 인원수 증가
				printf("VIP 고객 %d이 %d분에 들어옵니다. 업무처리시간= %d분\n",
					customer.id, customer.arrival_time, customer.service_time);
			}

			else { // 일반 고객 발생 확률 : 20%
				enqueue(&regular, customer);
				regular_customers++;  //일반고객 인원수 증가
				printf("일반 고객 %d이 %d분에 들어옵니다. 업무처리시간= %d분\n",
					customer.id, customer.arrival_time, customer.service_time);
			}
		}

		if (service_time > 0) {
			printf("고객 %d 업무처리중입니다. \n", service_customer);
			service_time--;
		}

		else {
			if (!is_empty(&Vip)) { // Vip 큐에 대기인원이 있다면 먼저 처리
				element customer = dequeue(&Vip);
				service_customer = customer.id;
				service_time = customer.service_time;
				printf("VIP고객 %d이 %d분에 업무 시작, 대기시간은 %d분.\n",
					customer.id, clock, clock - customer.arrival_time);
				total_wait += clock - customer.arrival_time;  //전체 대기시간과 vip 대기시간에 현재 업무처리를 시작한 고객의 대기시간 추가.
				vip_wait += clock - customer.arrival_time;
			}
			else if (!is_empty(&regular)) {// Vip 큐에 대기인원이 없으며 일반고객 큐에 대기인원이 있다면 처리
				element customer = dequeue(&regular);
				service_customer = customer.id;
				service_time = customer.service_time;
				printf("일반고객 %d이 %d분에 업무 시작, 대기시간은 %d분.\n",
					customer.id, clock, clock - customer.arrival_time);
				total_wait += clock - customer.arrival_time;  //전체 대기시간과 일반고객 대기시간에 현재 업무처리를 시작한 고객의 대기시간 추가.
				regular_wait += clock - customer.arrival_time;
			}
		}
	}

	printf("\n");
	printf("VIP 고객 전체 대기 시간 = %d분 \n", vip_wait);
	printf("VIP 고객 방문 수 = %d명 \n",vip_customers);
	printf("VIP 고객 평균 대기 시간 = %.5f분 \n", (float)vip_wait / (float)vip_customers);  //각 평균 대기 시간은 소수로 소수점 5개까지 출력하도록 변경
	printf("\n");
	printf("일반고객 전체 대기 시간 = %d분 \n", regular_wait);
	printf("일반고객 방문 수 = %d명 \n", regular_customers);
	printf("일반고객 평균 대기 시간 = %.5f분 \n", (float)regular_wait / (float)regular_customers);
	printf("\n");
	printf("전체고객 전체 대기 시간 = %d분 \n", total_wait);
	printf("전체고객 방문 수 = %d명 \n", total_customers);
	printf("전체고객 평균 대기 시간 = %.5f분 \n", (float)total_wait / (float)total_customers);
	return 0;
}