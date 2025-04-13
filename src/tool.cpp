#include "head.h"

typedef struct {//按键是否按下结构体
    int key;
    int down;
}ifKey;
ifKey ifKeyDown[] = {
    {' ',0},
    {VK_ESCAPE,0},
    {VK_UP,0},
    {VK_DOWN,0},
    {VK_RIGHT,0},
    {VK_LEFT,0},
    {VK_RETURN,0},
    {'U',0}
};

int DetectKey(int key)//检测按键并在松开前只执行一次
{
    int i;
    for (i = 0; i < 8; i++) {
        if (ifKeyDown[i].key == key) {
            if (GetAsyncKeyState(ifKeyDown[i].key) & 0x8000) {
                if (!ifKeyDown[i].down) {
                    ifKeyDown[i].down = 1;
                    return 1;
                }
            }
            else if (ifKeyDown[i].down)ifKeyDown[i].down = 0;
        }
    }
    return 0;
}

int Min(int num1, int num2)//返回小的值
{
    if (num1 > num2) return num2;
    return num1;
}

int Max(int num1, int num2)//返回大的值
{
    if (num1 < num2) return num2;
    return num1;
}

int ten(int num)//去掉个位
{
    return num - num % 10;
}

int Timer(double duration_d, int id_i)//定时器
{
    static double starttime_ad[10];
    double endtime_d = clock();
    if (endtime_d - starttime_ad[id_i] > duration_d)
    {
        starttime_ad[id_i] = endtime_d;
        return 1;
    }
    return 0;
}

int Framer(int frameDuration_i, int frameId_i)//帧定时器
{
    static int startFrame_ai[10];
    int endframe_i = gameState.nowFrame_i;
    if (endframe_i - startFrame_ai[frameId_i] > frameDuration_i)
    {
        startFrame_ai[frameId_i] = endframe_i;
        return 1;
    }
    return 0;
}


void DockArrayToCoordinate(int* arrayRow, int* arrayCol, float* coordinateX, float* coordinateY, int forward)//数组转化坐标
{
    int edgeX_i = Width_i / 9 + RoomLength_i / 2;
    int edgeY_i = Hight_i / 6 + RoomWidth_i / 2;

    if (forward)
    {
        *coordinateX = *arrayCol * RoomLength_i + edgeX_i;
        *coordinateY = *arrayRow * RoomWidth_i + edgeY_i;
    }
    else
    {
        *arrayRow = (*coordinateY- Hight_i / 6) / RoomLength_i;
        *arrayCol = (*coordinateX- Width_i / 9) / RoomWidth_i;
        *arrayRow = Max(0, Min(*arrayRow, 6));
        *arrayCol = Max(0, Min(*arrayCol, 12));
    }
}

void Key()//游戏中按键系统
{
    if (Role.player.hp) {
        Key_Move();
        Key_Shoot();
    }
    if (DetectKey(VK_ESCAPE)) { gameState.pause_i = 1; gameState.pauseNum_i = 0; }//暂停
    if (DetectKey('U')) {//显示碰撞箱及方格
        if (gameState.showCollision_i)gameState.showCollision_i = 0;
        else gameState.showCollision_i = 1;
    }
}


void Key_Move()//角色移动按键
{
    if (GetAsyncKeyState('W') & 0x8000) Role.state.moveUp_i = 1;
    else Role.state.moveUp_i = 0;
    if (GetAsyncKeyState('A') & 0x8000) Role.state.moveLeft_i = 1;
    else Role.state.moveLeft_i = 0;
    if (GetAsyncKeyState('S') & 0x8000) Role.state.moveDown_i = 1;
    else Role.state.moveDown_i = 0;
    if (GetAsyncKeyState('D') & 0x8000) Role.state.moveRight_i = 1;
    else Role.state.moveRight_i = 0;
}

void Key_Shoot()//发射子弹按键
{
    if (Role.state.lift)return;
    Role.state.shootUp_i = Role.state.shootDown_i = Role.state.shootLeft_i = Role.state.shootRight_i = 0;
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { Role.state.shootRight_i = 1; Role.state.roleHeadNum_i = 1 + ten(Role.state.roleHeadNum_i); }
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) { Role.state.shootLeft_i = 1; Role.state.roleHeadNum_i = 3 + ten(Role.state.roleHeadNum_i); }
    if (GetAsyncKeyState(VK_DOWN) & 0x8000) { Role.state.shootDown_i = 1; Role.state.roleHeadNum_i = 0 + ten(Role.state.roleHeadNum_i); }
    if (GetAsyncKeyState(VK_UP) & 0x8000) { Role.state.shootUp_i = 1; Role.state.roleHeadNum_i = 2 + ten(Role.state.roleHeadNum_i); }
    if (Role.state.shootUp_i || Role.state.shootDown_i || Role.state.shootLeft_i || Role.state.shootRight_i)
    {
        Role.state.ifShoot_i = 1;
        if (Framer(Role.property.tears_f * 60, 0))
        {
            Role.state.beginShoot_i = 1;
            if (Role.state.shootUp_i) { AddBullet(Role.player.dx, -Role.property.shootSpeed_i + Role.player.dy / 3); }
            else if (Role.state.shootDown_i)  AddBullet(Role.player.dx, Role.property.shootSpeed_i + Role.player.dy / 3);
            else if (Role.state.shootLeft_i)  AddBullet(-Role.property.shootSpeed_i + Role.player.dx / 3, Role.player.dy);
            else if (Role.state.shootRight_i)  AddBullet(Role.property.shootSpeed_i + Role.player.dx / 3, Role.player.dy);
        }
        else Role.state.beginShoot_i = 0;
    }
    else Role.state.ifShoot_i = 0;
}


void BeginkKey()//游戏开始界面按键
{
    if (DetectKey(' ')||DetectKey(VK_RETURN)) { gameState.backgroundNum_i = -1; }
    if (DetectKey(VK_ESCAPE)) { exit(0); }
}

void MenuKey()//菜单界面按键
{
    if (DetectKey(VK_DOWN))gameState.menuNum = (gameState.menuNum + 1) % 4;
    if (DetectKey(VK_UP))gameState.menuNum--;
    if (gameState.menuNum < 0)gameState.menuNum = 3;
    switch (gameState.menuNum) {
    case 0:
        if (DetectKey(' ')||DetectKey(VK_RETURN))
        {
            gameState.backgroundNum_i = 1;
            gameState.restart_i = 1;
            gameState.pause_i = 0;
            gameState.mode = 0;
            frameRate_i = 60;
        }
        break;
    case 1:
        if (DetectKey(' ')||DetectKey(VK_RETURN))
        {
            gameState.backgroundNum_i = 2;
            gameState.restart_i = 1;
            gameState.pause_i = 0;
            gameState.mode = 1;
            frameRate_i = 120;
        }
    case 2:
        if (DetectKey(' ')||DetectKey(VK_RETURN))
        {
            gameState.backgroundNum_i = -2;
        }
    case 3:
        if (DetectKey(' ')||DetectKey(VK_RETURN))gameState.backgroundNum_i = 0;
    }
    if (DetectKey(VK_ESCAPE))gameState.backgroundNum_i = 0;
}

void RankKey()//排行榜界面按键
{
    if (DetectKey(VK_RIGHT))gameState.rankNum++;
    else if (DetectKey(VK_LEFT)) gameState.rankNum--;
    if (gameState.rankNum == 2)gameState.rankNum = 0;
    else if (gameState.rankNum == -1)gameState.rankNum = 1;
    if (DetectKey(VK_ESCAPE))gameState.backgroundNum_i = -1;
}

void PauseKey()//暂停时游戏按键
{
    if (DetectKey(VK_ESCAPE))gameState.pause_i = 0;
    if (DetectKey(' ')||DetectKey(VK_RETURN)) {
        if (gameState.pauseNum_i == 0) gameState.pause_i = 0;
        if (gameState.pauseNum_i == 1) gameState.backgroundNum_i = -1;
    }
    if (GetAsyncKeyState(VK_UP) & 0x8000 && Framer(20, 4))
        if (gameState.pauseNum_i > 0)gameState.pauseNum_i--;
        else gameState.pauseNum_i = 1;
    if (GetAsyncKeyState(VK_DOWN) & 0x8000 && Framer(20, 4))gameState.pauseNum_i = (gameState.pauseNum_i + 1) % 2;
}

void DeadKey()//死亡时游戏按键
{
    if (Role.state.deadFrame_i < 0)
    {
        if (DetectKey(' ')||DetectKey(VK_RETURN))gameState.restart_i = 1;
        if (GetAsyncKeyState('R') & 0x8000)gameState.restart_i = 1;
        if (DetectKey(VK_ESCAPE))gameState.backgroundNum_i = -1;
    }
}
