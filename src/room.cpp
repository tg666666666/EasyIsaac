#include "head.h"

Point* CreateNode(int x, int y, Point* parent) // �����½ڵ�
{
	Point* newNode = (Point*)malloc(sizeof(Point));
	if (!newNode)
	{
		fprintf(stderr, "�����½ڵ��ڴ�ʧ��");
		return NULL;
	}
	newNode->x = x;
	newNode->y = y;
	newNode->parent = parent; // ��ʼ�����ڵ�
	newNode->next = NULL;
	return newNode;
}

void JoinQueue(Queue* queue, int x, int y, Point* parent) // ���
{
	if (!queue)
	{
		fprintf(stderr, "���ʧ�ܣ��ն���");
		return;
	}
	Point* newNode = CreateNode(x, y, parent);
	if (queue->rear == NULL)
	{
		queue->front = queue->rear = newNode;
		return;
	}
	queue->rear->next = newNode;
	queue->rear = newNode;
}

Queue* InitQueue() // �����¶���
{
	Queue* newQue = (Queue*)malloc(sizeof(Queue));
	if (!newQue)
	{
		fprintf(stderr, "��������ʧ�ܣ��ڴ�����ʧ��");
		return NULL;
	}
	newQue->front = newQue->rear = NULL;
	return newQue;
}

Point* DeleteQueue(Queue* nowqueue, int* dex, int* dey) // ����
{
	if (!nowqueue) return NULL;
	if (nowqueue->front == NULL) return NULL;
	*dex = nowqueue->front->x;
	*dey = nowqueue->front->y;
	Point* temp = nowqueue->front;
	nowqueue->front = nowqueue->front->next;
	if (nowqueue->front == NULL) nowqueue->rear = NULL;
	return temp;
}

void FreeQueue(Queue* queue) // �ͷŶ���
{
	if (queue == NULL) return;
	Point* current = queue->front;
	while (current != NULL)
	{
		Point* temp = current;
		current = current->next;
		free(temp);
	}
	free(queue);
}

int isValid(int row, int col) // �ж�û��Խ��
{
	return (row >= 0) && (row < ROW) && (col >= 0) && (col < COL);
}

Point* BFS(int(*room)[COL], int sX, int sY, int eX, int eY,int findPath) // Ѱ·
{
	int visited[ROW][COL] = { 0 };
	int rowNum[4] = { 1, -1, 0, 0 }; // ����
	int colNum[4] = { 0, 0, 1, -1 };
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			visited[i][j] = 0;
		}
	}
	if (!isValid(sX, sY) || !isValid(eX, eY)) return NULL;
	if (findPath)
	{
		if (room[sX][sY] >0 && room[sX][sY]<10 || room[eX][eY] >0 && room[eX][eY]<10) return NULL;
	}
	else
	{
		if (room[sX][sY] || room[eX][eY]) return NULL;
	}

	Queue* q = InitQueue();
	JoinQueue(q, sX, sY, NULL);
	visited[sX][sY] = 1;
	int curX = 0;
	int curY = 0;
	while (q->front != NULL)
	{
		Point* parent = DeleteQueue(q, &curX, &curY);
		if (curX == eX && curY == eY)
		{
			Point* path = parent;
			FreeQueue(q);
			return path;
		}
		for (int i = 0; i < 4; i++)
		{
			int row = curX + rowNum[i];
			int col = curY + colNum[i];
			if (isValid(row, col) && visited[row][col] == 0)
			{
				if (findPath && (room[row][col] > 10 || room[row][col] == 0))
				{
					JoinQueue(q, row, col, parent);
					visited[row][col] = 1;
				}
				else if (room[row][col] == 0)
				{
					JoinQueue(q, row, col, parent);
					visited[row][col] = 1;
				}
			}
		}
	}
	FreeQueue(q);
	return NULL;
}

int AINextPath(float beginX,float beginY,float endX,float endY,float* nextX,float* nextY,int (*room)[COL])//��һ����Ѱ·(����)
{
	int bRow = 0, bCol = 0;
	DockArrayToCoordinate(&bRow, &bCol, &beginX, &beginY, 0);
	int eRow=0, eCol=0;
	DockArrayToCoordinate(&eRow, &eCol, &endX, &endY, 0);
	Point* current = BFS(room, bRow, bCol, eRow, eCol,1);
	if (current==NULL||current->parent == NULL)return 0;
	while (current->parent->parent != NULL)
	{
		current = current->parent;
	}
	float nX = 0, nY = 0;
	DockArrayToCoordinate(&current->x, &current->y, &nX, &nY, 1);
	*nextX = nX;
	*nextY = nY;
	return 1;
}

void GenerateMap(int (*room)[COL],int level)//���ɵ�ͼ
{
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 13; j++) {
			room[i][j] = 0;
		}
	}
	int randomNumber = rand() % 100;
	int stone = Min(level / 5 * 3+rand()%5,20);//ʯͷ�ϰ�������
	int pine = Min(level / 5 * 2 + rand() % 4, 10);//�������
	int redSky = Min(1+level/4+rand()%2,8);//���Ӭ����
	int people = Min( level / 6 + rand() % 3, 6);//���ε�������
	int itemRedHp = Max(0, rand() % 10- 7);//���ĵ���������
	int itemHalfRedHp =Max(0, rand() % 12 - 9);//����ĵ���������
	int prop = Max(0, rand() % 2);//��������
	int i=0;
	while (i < stone+pine+redSky+ people + itemRedHp+ itemHalfRedHp+prop)
	{
		int type = 0;//�ϰ���,����,����������(0-10Ϊ�ϰ���,10����Ϊ����20,����Ϊ������)
		if (i < stone)type = 1;
		else if (i < stone + pine)type = 2;
		else if (i < stone + pine + redSky)type = 11;
		else if (i < stone + pine + redSky+people)type = 12;
		else if (i < stone + pine + redSky + people + itemRedHp)type = 21;
		else if (i < stone + pine + redSky + people + itemRedHp + itemHalfRedHp)type = 22;
		else if (i < stone + pine + redSky + people + itemRedHp + itemHalfRedHp + prop)
		{
			int kind = rand() % 7+1;
			switch (kind) {
			case 1:type = 31; break;
			case 2:type = 32; break;
			case 3:type = 33; break;
			case 4:type = 34; break;
			case 5:type = 35; break;
			case 6:type = 36; break;
			case 7:type = 37; break;
			}
		}
		int row = rand() % 7;//����0-6
		int col = rand() % 13;//����0-12
		if (room[row][col])continue;
		room[row][col] = type;
		if (BFS(room, 4 - 1, 1 - 1, 1 - 1, 7 - 1,0)&& 
			BFS(room, 4 - 1, 1 - 1, 4 - 1, 13 - 1,0)&& 
		    BFS(room, 4 - 1, 1 - 1, 7 - 1, 7 - 1,0))
			i++;
		else room[row][col] = 0;
	}
	return;
}