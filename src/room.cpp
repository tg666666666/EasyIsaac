#include "head.h"

Point* CreateNode(int x, int y, Point* parent) // 创建新节点
{
	Point* newNode = (Point*)malloc(sizeof(Point));
	if (!newNode)
	{
		fprintf(stderr, "申请新节点内存失败");
		return NULL;
	}
	newNode->x = x;
	newNode->y = y;
	newNode->parent = parent; // 初始化父节点
	newNode->next = NULL;
	return newNode;
}

void JoinQueue(Queue* queue, int x, int y, Point* parent) // 入队
{
	if (!queue)
	{
		fprintf(stderr, "入队失败：空队列");
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

Queue* InitQueue() // 创建新队列
{
	Queue* newQue = (Queue*)malloc(sizeof(Queue));
	if (!newQue)
	{
		fprintf(stderr, "创建队列失败：内存申请失败");
		return NULL;
	}
	newQue->front = newQue->rear = NULL;
	return newQue;
}

Point* DeleteQueue(Queue* nowqueue, int* dex, int* dey) // 出队
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

void FreeQueue(Queue* queue) // 释放队列
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

int isValid(int row, int col) // 判断没有越界
{
	return (row >= 0) && (row < ROW) && (col >= 0) && (col < COL);
}

Point* BFS(int(*room)[COL], int sX, int sY, int eX, int eY,int findPath) // 寻路
{
	int visited[ROW][COL] = { 0 };
	int rowNum[4] = { 1, -1, 0, 0 }; // 方向
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

int AINextPath(float beginX,float beginY,float endX,float endY,float* nextX,float* nextY,int (*room)[COL])//下一个点寻路(坐标)
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

void GenerateMap(int (*room)[COL],int level)//生成地图
{
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 13; j++) {
			room[i][j] = 0;
		}
	}
	int randomNumber = rand() % 100;
	int stone = Min(level / 5 * 3+rand()%5,20);//石头障碍物数量
	int pine = Min(level / 5 * 2 + rand() % 4, 10);//尖刺数量
	int redSky = Min(1+level/4+rand()%2,8);//红苍蝇数量
	int people = Min( level / 6 + rand() % 3, 6);//人形敌人数量
	int itemRedHp = Max(0, rand() % 10- 7);//红心掉落物数量
	int itemHalfRedHp =Max(0, rand() % 12 - 9);//半红心掉落物数量
	int prop = Max(0, rand() % 2);//道具数量
	int i=0;
	while (i < stone+pine+redSky+ people + itemRedHp+ itemHalfRedHp+prop)
	{
		int type = 0;//障碍物,敌人,掉落物种类(0-10为障碍物,10以上为敌人20,以上为掉落物)
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
		int row = rand() % 7;//行数0-6
		int col = rand() % 13;//列数0-12
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