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

typedef struct {//游戏状态
    int level;//当前所处关卡
    int enemyNum;//当前房间敌人数量
    float score;//当局分数
    int ifSave;//当局是否存档
    int mode;//当局模式
    int Roompause;//每关清理后停顿秒数
    int backgroundNum_i; // 当前界面编号
    int menuNum;//当前菜单选项
    int pauseNum_i; // 当前暂停界面编号
    int rankNum;//当前排行榜编号
    int restart_i; // 是否重新开始
    int pause_i; // 游戏是否暂停
    int showCollision_i; // 是否显示碰撞箱和方格
    int nowFrame_i; //当前帧数
    int gameFrame_i;//本局帧数
    int roomFrame_i;//关卡完成后停留帧数
} GameState;

typedef struct {//房间状态
    int room[ROW][COL];
}RoomState;

typedef struct {//结构体小球
    float x;//当前位置坐标
    float y;
    float a;//和加速度
    float ax;//分加速度
    float ay;
    float dx;//分速度
    float dy;
    int r;//碰撞半径
    int hp;//生命值
    float speed;//最大速度
}Ball;

typedef struct {//角色属性
    float friction_f;//摩擦力
    float tears_f; // 发射速度（射速）
    int shootSpeed_i; // 子弹速度（弹速）
    int range_i; // 子弹射程（射程）
    int tearsHeight_i; // 眼泪显示高度（眼泪高度)
    float initDamage_f; // 基础伤害
    int maxHp_i;//红心上限
    int redHp_i;//红心数量
    int blueHp_i;//蓝心数量
    int light_i;//轻度，越大越轻

} RoleConfig;

typedef struct {//角色状态
    int ghost;//是否子弹幽灵效果
    int pene;//是否子弹穿透效果
    int fly;//是否飞行
    int lift;//是否举起物品
    int moveUp_i;//向上走
    int moveDown_i;//向下走
    int moveLeft_i;//向左走
    int moveRight_i;//向右走
    int roleHeadNum_i; // 头的方向
    int roleWalkNum_i; // 行走方向
    int shootUp_i;//是否有向上发射按键
    int shootDown_i;//是否有向下发射按键
    int shootLeft_i;//是否有向左发射按键
    int shootRight_i;// 是否有向右发射按键
    int ifShoot_i;//是否在试图发射子弹
    int beginShoot_i;//是否开始射出子弹
    float backx_f;//击退速度
    float backy_f;//击退速度
    int shootFrame_i; // 射击后帧数
    int hurtFrame_i;//受伤后帧数
    int deadFrame_i;//死亡后帧数
} PlayerState;

typedef struct {//玩家角色结构体
    Ball player;//角色小球
    RoleConfig property;//角色属性
    PlayerState state;//角色状态
}Character;

typedef struct BulletNode {//链表子弹
    int camp;//子弹阵营
    int type;//子弹类型
    Ball bullet;
    float oldx;
    float oldy;
    int bullet_height;
    int appear_frame;//碰撞后存在帧数
    BulletNode* next;
} BulletNode;

typedef struct EntityNode {//链表实体
    int aniNum;//动画编号
    float nowSpeed;//当前速度
    int camp;//实体阵营
    int type;//实体种类
    int light;//轻度，越大越轻
    float backx;
    float backy;
    float ca;
    int appear_frame;
    int dead_frame;
    Ball entity;
    EntityNode* next;
}EntityNode;

typedef struct//存档
{
    int score;//分数
    int time;//使用时间
    int mode;//模式
}Score;

typedef struct Point {
    int x, y;
    struct Point* parent; // 添加父节点指针
    struct Point* next;
} Point;

typedef struct Queue {//队列
    Point* front;
    Point* rear;
} Queue;

extern GameState gameState;//游戏状态
extern const int Width_i;//窗口宽度
extern const int Hight_i;//窗口高度
extern const int RoomLength_i;//房间单元格长度
extern const int RoomWidth_i;//房间单元格宽度
extern Character Role;//角色
extern int frameRate_i;//最大帧率

//工具
int Max(int num1, int num2);//返回大的值
int Min(int num1, int num2);//返回小的值
int ten(int num);//去掉个位
int Timer(double duration_d, int id_i);//定时器
int Framer(int frameDuration_i, int frameId_i);//帧定时器
int DetectKey(int key);//检测按键并在松开前只执行一次
void DockArrayToCoordinate(int* arrayRow, int* arrayCol, float* coordinateX, float* coordinateY, int forward);//转换数组索引和坐标
Point* BFS(int(*room)[COL], int sX, int sY, int eX, int eY, int findPath); // 寻路(索引)
int AINextPath(float beginX, float beginY, float endX, float endY, float* nextX, float* nextY, int(*room)[COL]);//下一个点寻路(坐标)
void CalculateScore();//计算总分
void GenerateMap(int(*room)[COL], int level);//生成地图

//初始化
void Init();//初始化
void InitPIC();//加载图片
void InitSound();//加载音乐
void ItsetGameState();//初始化游戏属性
void ItsetRole();//初始化角色
void InitEntity();//初始化实体和子弹

//游戏状态
void GameStateDeal();//游戏状态处理
void Update();//更新游戏
void NextLevel();//进入下一个关卡

void BGM();//音乐输出
void Sound();//音效输出

//渲染
void show();//输出游戏中画面
void RanderHealth();//渲染生命值
void UpdateRolePic();//更新玩家贴图
void RanderRole();//渲染角色
void RanderObstacle();//渲染障碍物
void RanderBeginground();//显示开始界面
void RanderMenu();//显示菜单界面
void RanderRank(int mode);//显示排行榜
void RanderBacground();//显示背景
void RanderPause();//显示暂停界面
void RanderDeadInterface();//显示死亡界面
void DrowRoomLine();//画出房间方格    
void ShowEntity();//渲染实体
void ShowBullets();//渲染子弹
void ShowData();//显示游戏数据

//实体管理
void MoveRole();//角色移动
void GetHurt();//角色受伤
void AddBullet(int dx, int dy);//添加子弹
void AddEntity(int x, int y, int type, int camp);//添加实体
void UpdateEntity();//更新实体
void UpdateBullets(); // 更新子弹
void Attack();//处理角色子弹攻击
void SummonEntity();//生成实体处理

//碰撞处理
void limit();//防止角色溢出屏幕
int CheckCollision(Ball ball1, Ball ball2);//两小球是否碰撞
int ObstacleCollision(int oX_i, int oY_i, Ball ball, int* leftX, int* leftY, int* rightX, int* rightY);//检测小球和方块碰撞
void RoleObstacleCollision();//障碍物与角色碰撞处理
void BulletObstacleCollision();//子弹障碍物碰撞处理
void EntityObstacleCollision();//实体障碍物碰撞处理
void EntityCollision();//实体碰撞和实体与角色碰撞处理
void Knockback(EntityNode* entity, BulletNode* bullet);//子弹击退

//按键
void Key();//游戏中按键系统
void Key_Move();//角色移动按键
void Key_Shoot();//发射子弹按键
void BeginkKey();//游戏开始界面按键
void MenuKey();//菜单界面按钮
void RankKey();//排行榜界面按钮
void PauseKey();//暂停时游戏按键
void DeadKey();//死亡时游戏按键