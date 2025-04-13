#pragma once
#pragma warning(disable:4996)
#pragma comment(lib, "winmm.lib")
#include <stdio.h>
#include <stdlib.h>
#include <easyx.h>
#include <time.h>
#include <math.h>
#include <shlobj.h> 
#define ROW 7
#define COL 13

typedef struct {//��Ϸ״̬
    int level;//��ǰ�����ؿ�
    int enemyNum;//��ǰ�����������
    float score;//���ַ���
    int ifSave;//�����Ƿ�浵
    int mode;//����ģʽ
    int Roompause;//ÿ�������ͣ������
    int backgroundNum_i; // ��ǰ������
    int menuNum;//��ǰ�˵�ѡ��
    int pauseNum_i; // ��ǰ��ͣ������
    int rankNum;//��ǰ���а���
    int restart_i; // �Ƿ����¿�ʼ
    int pause_i; // ��Ϸ�Ƿ���ͣ
    int showCollision_i; // �Ƿ���ʾ��ײ��ͷ���
    int nowFrame_i; //��ǰ֡��
    int gameFrame_i;//����֡��
    int roomFrame_i;//�ؿ���ɺ�ͣ��֡��
} GameState;

typedef struct {//����״̬
    int room[ROW][COL];
}RoomState;

typedef struct {//�ṹ��С��
    float x;//��ǰλ������
    float y;
    float a;//�ͼ��ٶ�
    float ax;//�ּ��ٶ�
    float ay;
    float dx;//���ٶ�
    float dy;
    int r;//��ײ�뾶
    int hp;//����ֵ
    float speed;//����ٶ�
}Ball;

typedef struct {//��ɫ����
    float friction_f;//Ħ����
    float tears_f; // �����ٶȣ����٣�
    int shootSpeed_i; // �ӵ��ٶȣ����٣�
    int range_i; // �ӵ���̣���̣�
    int tearsHeight_i; // ������ʾ�߶ȣ�����߶�)
    float initDamage_f; // �����˺�
    int maxHp_i;//��������
    int redHp_i;//��������
    int blueHp_i;//��������
    int light_i;//��ȣ�Խ��Խ��

} RoleConfig;

typedef struct {//��ɫ״̬
    int ghost;//�Ƿ��ӵ�����Ч��
    int pene;//�Ƿ��ӵ���͸Ч��
    int fly;//�Ƿ����
    int lift;//�Ƿ������Ʒ
    int moveUp_i;//������
    int moveDown_i;//������
    int moveLeft_i;//������
    int moveRight_i;//������
    int roleHeadNum_i; // ͷ�ķ���
    int roleWalkNum_i; // ���߷���
    int shootUp_i;//�Ƿ������Ϸ��䰴��
    int shootDown_i;//�Ƿ������·��䰴��
    int shootLeft_i;//�Ƿ��������䰴��
    int shootRight_i;// �Ƿ������ҷ��䰴��
    int ifShoot_i;//�Ƿ�����ͼ�����ӵ�
    int beginShoot_i;//�Ƿ�ʼ����ӵ�
    float backx_f;//�����ٶ�
    float backy_f;//�����ٶ�
    int shootFrame_i; // �����֡��
    int hurtFrame_i;//���˺�֡��
    int deadFrame_i;//������֡��
} PlayerState;

typedef struct {//��ҽ�ɫ�ṹ��
    Ball player;//��ɫС��
    RoleConfig property;//��ɫ����
    PlayerState state;//��ɫ״̬
}Character;

typedef struct BulletNode {//�����ӵ�
    int camp;//�ӵ���Ӫ
    int type;//�ӵ�����
    Ball bullet;
    float oldx;
    float oldy;
    int bullet_height;
    int appear_frame;//��ײ�����֡��
    BulletNode* next;
} BulletNode;

typedef struct EntityNode {//����ʵ��
    int aniNum;//�������
    float nowSpeed;//��ǰ�ٶ�
    int camp;//ʵ����Ӫ
    int type;//ʵ������
    int light;//��ȣ�Խ��Խ��
    float backx;
    float backy;
    float ca;
    int appear_frame;
    int dead_frame;
    Ball entity;
    EntityNode* next;
}EntityNode;

typedef struct//�浵
{
    int score;//����
    int time;//ʹ��ʱ��
    int mode;//ģʽ
}Score;

typedef struct Point {
    int x, y;
    struct Point* parent; // ��Ӹ��ڵ�ָ��
    struct Point* next;
} Point;

typedef struct Queue {//����
    Point* front;
    Point* rear;
} Queue;

extern GameState gameState;//��Ϸ״̬
extern const int Width_i;//���ڿ��
extern const int Hight_i;//���ڸ߶�
extern const int RoomLength_i;//���䵥Ԫ�񳤶�
extern const int RoomWidth_i;//���䵥Ԫ����
extern Character Role;//��ɫ
extern int frameRate_i;//���֡��

//����
int Max(int num1, int num2);//���ش��ֵ
int Min(int num1, int num2);//����С��ֵ
int ten(int num);//ȥ����λ
int Timer(double duration_d, int id_i);//��ʱ��
int Framer(int frameDuration_i, int frameId_i);//֡��ʱ��
int DetectKey(int key);//��ⰴ�������ɿ�ǰִֻ��һ��
void DockArrayToCoordinate(int* arrayRow, int* arrayCol, float* coordinateX, float* coordinateY, int forward);//ת����������������
Point* BFS(int(*room)[COL], int sX, int sY, int eX, int eY, int findPath); // Ѱ·(����)
int AINextPath(float beginX, float beginY, float endX, float endY, float* nextX, float* nextY, int(*room)[COL]);//��һ����Ѱ·(����)
void CalculateScore();//�����ܷ�
void GenerateMap(int(*room)[COL], int level);//���ɵ�ͼ

//��ʼ��
void Init();//��ʼ��
void InitPIC();//����ͼƬ
void InitSound();//��������
void ItsetGameState();//��ʼ����Ϸ����
void ItsetRole();//��ʼ����ɫ
void InitEntity();//��ʼ��ʵ����ӵ�

//��Ϸ״̬
void GameStateDeal();//��Ϸ״̬����
void Update();//������Ϸ
void NextLevel();//������һ���ؿ�

void BGM();//�������
void Sound();//��Ч���

//��Ⱦ
void show();//�����Ϸ�л���
void RanderHealth();//��Ⱦ����ֵ
void UpdateRolePic();//���������ͼ
void RanderRole();//��Ⱦ��ɫ
void RanderObstacle();//��Ⱦ�ϰ���
void RanderBeginground();//��ʾ��ʼ����
void RanderMenu();//��ʾ�˵�����
void RanderRank(int mode);//��ʾ���а�
void RanderBacground();//��ʾ����
void RanderPause();//��ʾ��ͣ����
void RanderDeadInterface();//��ʾ��������
void DrowRoomLine();//�������䷽��    
void ShowEntity();//��Ⱦʵ��
void ShowBullets();//��Ⱦ�ӵ�
void ShowData();//��ʾ��Ϸ����

//ʵ�����
void MoveRole();//��ɫ�ƶ�
void GetHurt();//��ɫ����
void AddBullet(int dx, int dy);//����ӵ�
void AddEntity(int x, int y, int type, int camp);//���ʵ��
void UpdateEntity();//����ʵ��
void UpdateBullets(); // �����ӵ�
void Attack();//�����ɫ�ӵ�����
void SummonEntity();//����ʵ�崦��

//��ײ����
void limit();//��ֹ��ɫ�����Ļ
int CheckCollision(Ball ball1, Ball ball2);//��С���Ƿ���ײ
int ObstacleCollision(int oX_i, int oY_i, Ball ball, int* leftX, int* leftY, int* rightX, int* rightY);//���С��ͷ�����ײ
void RoleObstacleCollision();//�ϰ������ɫ��ײ����
void BulletObstacleCollision();//�ӵ��ϰ�����ײ����
void EntityObstacleCollision();//ʵ���ϰ�����ײ����
void EntityCollision();//ʵ����ײ��ʵ�����ɫ��ײ����
void Knockback(EntityNode* entity, BulletNode* bullet);//�ӵ�����

//����
void Key();//��Ϸ�а���ϵͳ
void Key_Move();//��ɫ�ƶ�����
void Key_Shoot();//�����ӵ�����
void BeginkKey();//��Ϸ��ʼ���水��
void MenuKey();//�˵����水ť
void RankKey();//���а���水ť
void PauseKey();//��ͣʱ��Ϸ����
void DeadKey();//����ʱ��Ϸ����