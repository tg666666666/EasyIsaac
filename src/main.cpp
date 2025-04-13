#include "head.h"
const int Width_i = 1326;//窗口宽度
const int Hight_i = 886;//窗口高度
const int RoomLength_i = (Width_i * 7 / 9) / 13;//房间单元格长度
const int RoomWidth_i = (Hight_i * 4 / 6) / 7;//房间单元格宽度
const int MAXSCORES = 9;//排行榜最大数量

const int health_count = 5;//生命值贴图数量

const int Role_head_count = 4;//头贴图数量
const int Role_shoot_count = 4;//头发射贴图数量

const int Role_bullet_count = 15;//子弹贴图数量

const int Role_walk_front_count = 10;//身体正面移动贴图数量
const int Role_walk_side_count = 10;//身体右向移动贴图数量
const int Role_walk_left_count = 10;//身体左向移动贴图数量

const int Monster_fly_count = 4;//苍蝇飞行贴图数量
const int Monster_fly_dead_count = 11;//苍蝇死亡贴图数量

const int redHp_count = 2;//红心掉落物贴图数量
const int prop_count = 7;

int frameRate_i = 60;//最大帧率

GameState gameState;
RoomState Room;
Character Role;
BulletNode* bulletHead = NULL; // 子弹链表的头指针
EntityNode* entityHead = NULL;//实体链表头指针

IMAGE beginground[2];//开始界面
IMAGE menu;//菜单
IMAGE background[2];//房间背景
IMAGE pauseface_front[2];//暂停界面
IMAGE pauseface_back[2];
IMAGE testament_front,testament_back;//死亡界面
IMAGE health_front[5], health_back[5];;//生命值贴图
IMAGE role_head_front[20], role_head_back[20];//角色头贴图
IMAGE role_walk_front[30], role_walk_back[30];//角色身体贴图
IMAGE role_lift_front, role_lift_back;//角色举起道具贴图
IMAGE role_hurt_front[2], role_hurt_back[3];//角色受伤贴图
IMAGE role_dead_front, role_dead_back;//角色死亡贴图
IMAGE role_fall_front, role_fall_back;//角色倒地贴图
IMAGE bullet_front[15], bullet_back[15];//子弹贴图
IMAGE monster_redfly_front[30], monster_redfly_back[30];//敌人红苍蝇贴图
IMAGE people_front, people_back;//人形怪物贴图
IMAGE stone_front, stone_back;//石头障碍物贴图
IMAGE redHp_front[2], redHp_back[2];//红心掉落物贴图
IMAGE spine_front, spine_back;//尖刺障碍物贴图
IMAGE prop_front[7], prop_back[7];//道具贴图
IMAGE propName_front, propName_back;//道具名称背景

void InitSound()//加载声音
{
    mciSendString("open \"../assets/sounds/menu.mp3\" alias menu", NULL, 0, NULL);
    mciSendString("open \"../assets/sounds/room.mp3\" alias room", NULL, 0, NULL);
    mciSendString("open \"../assets/sounds/fast.mp3\" alias fast", NULL, 0, NULL);
    mciSendString("open \"../assets/sounds/deadPic.mp3\" alias deadPic", NULL, 0, NULL);
    mciSendString("open \"../assets/sounds/hurt0.mp3\" alias hurt", NULL, 0, NULL);//受伤声音
    mciSendString("open \"../assets/sounds/shoot.mp3\" alias shoot", NULL, 0, NULL);//射击声音
    mciSendString("open \"../assets/sounds/shootBreak.mp3\" alias shootBreak", NULL, 0, NULL);//击中(眼泪爆裂)声音
    mciSendString("open \"../assets/sounds/dead.mp3\" alias dead", NULL, 0, NULL);//死亡声音
    mciSendString("open \"../assets/sounds/getProp.mp3\" alias getProp", NULL, 0, NULL);//获得道具声音
}

void InitPIC()//加载图片
{
    int i;
    char filename_front[100] = {};
    char filename_back[100] = {};
    loadimage(&beginground[0], "../assets/images/panel/begin0.jpg");//开始界面
    loadimage(&beginground[1], "../assets/images/panel/begin1.jpg");
    loadimage(&menu, "../assets/images/panel/menu.jpg");//菜单
    loadimage(&testament_front, "../assets/images/panel/testament_front.jpg");//加载死亡界面
    loadimage(&testament_back, "../assets/images/panel/testament_back.jpg");
    for (i = 0; i < 2; i++) {//加载背景
        sprintf(filename_front, "../assets/images/panel/BK%d.jpg", i);
        loadimage(&background[i], filename_front);
    }
    for (i = 0; i < 2; i++) {//加载暂停界面
        sprintf(filename_front, "../assets/images/panel/pause%d_front.jpg", i);
        sprintf(filename_back, "../assets/images/panel/pause%d_back.jpg", i);
        loadimage(&pauseface_front[i], filename_front);
        loadimage(&pauseface_back[i], filename_back);
    }
    for (i = 0; i < Role_bullet_count; i++) {//加载子弹
        sprintf(filename_front, "../assets/images/bullet/bullet%d_front.jpg", i);
        sprintf(filename_back, "../assets/images/bullet/bullet%d_back.jpg", i);
        loadimage(&bullet_front[i], filename_front);
        loadimage(&bullet_back[i], filename_back);
    }
    for (i = 0; i < Role_head_count; i++) {//加载头
        sprintf(filename_front, "../assets/images/character/isaac%d_front.jpg", i);
        sprintf(filename_back, "../assets/images/character/isaac%d_back.jpg", i);
        loadimage(&role_head_front[i], filename_front);
        loadimage(&role_head_back[i], filename_back);
    }
    for (i = 0; i < Role_shoot_count; i++) {//加载射击中的头
        sprintf(filename_front, "../assets/images/character/isaac%d_shoot_front.jpg", i);
        sprintf(filename_back, "../assets/images/character/isaac%d_shoot_back.jpg", i);
        loadimage(&role_head_front[i + 10], filename_front);
        loadimage(&role_head_back[i + 10], filename_back);
    }
    for (i = 0; i < Role_walk_front_count; i++) {//加载正面身体
        sprintf(filename_front, "../assets/images/character/isaac_walk%d_front.jpg", i);
        sprintf(filename_back, "../assets/images/character/isaac_walk%d_back.jpg", i);
        loadimage(&role_walk_front[i], filename_front);
        loadimage(&role_walk_back[i], filename_back);
    }
    for (i = 0; i < Role_walk_side_count; i++) {//加载右面身体
        sprintf(filename_front, "../assets/images/character/isaac_walk%d_side_front.jpg", i);
        sprintf(filename_back, "../assets/images/character/isaac_walk%d_side_back.jpg", i);
        loadimage(&role_walk_front[i + 10], filename_front);
        loadimage(&role_walk_back[i + 10], filename_back);
    }
    for (i = 0; i < Role_walk_left_count; i++) {//加载左面身体
        sprintf(filename_front, "../assets/images/character/isaac_walk%d_left_front.jpg", i);
        sprintf(filename_back, "../assets/images/character/isaac_walk%d_left_back.jpg", i);
        loadimage(&role_walk_front[i + 20], filename_front);
        loadimage(&role_walk_back[i + 20], filename_back);
    }
    for (i = 0; i < Monster_fly_count; i++) {//加载红苍蝇
        sprintf(filename_front, "../assets/images/monster/monster_redfly%d_front.jpg", i);
        sprintf(filename_back, "../assets/images/monster/monster_redfly%d_back.jpg", i);
        loadimage(&monster_redfly_front[i], filename_front);
        loadimage(&monster_redfly_back[i], filename_back);
    }
    for (i = 0; i < Monster_fly_dead_count; i++) {//加载红苍蝇死亡
        sprintf(filename_front, "../assets/images/monster/monster_redfly%d_dead_front.jpg", i);
        sprintf(filename_back, "../assets/images/monster/monster_redfly%d_dead_back.jpg", i);
        loadimage(&monster_redfly_front[i+10], filename_front);
        loadimage(&monster_redfly_back[i+10], filename_back);
    }
    for (i = 0; i < health_count; i++) {//加载生命值
        sprintf(filename_front, "../assets/images/panel/health%d_front.jpg", i);
        sprintf(filename_back, "../assets/images/panel/health%d_back.jpg", i);
        loadimage(&health_front[i], filename_front);
        loadimage(&health_back[i], filename_back);
    }
    for (i = 0; i < redHp_count; i++) {//加载红心掉落物
        sprintf(filename_front, "../assets/images/drops/itemRedHp%d_front.jpg", i);
        sprintf(filename_back, "../assets/images/drops/itemRedHp%d_back.jpg", i);
        loadimage(&redHp_front[i], filename_front);
        loadimage(&redHp_back[i], filename_back);
    }
    for (i = 0; i < prop_count; i++) {//加载道具
        sprintf(filename_front, "../assets/images/prop/prop%d_front.jpg", i);
        sprintf(filename_back, "../assets/images/prop/prop%d_back.jpg", i);
        loadimage(&prop_front[i], filename_front);
        loadimage(&prop_back[i], filename_back);
    }
    for (i = 0; i < 2; i++) {//加载受伤
        sprintf(filename_front, "../assets/images/character/isaac_hurt%d_front.jpg", i);
        sprintf(filename_back, "../assets/images/character/isaac_hurt%d_back.jpg", i);
        loadimage(&role_hurt_front[i], filename_front);
        loadimage(&role_hurt_back[i], filename_back);
    }
    loadimage(&role_lift_front, "../assets/images/character/isaac_lift_front.jpg");//角色举起道具
    loadimage(&role_lift_back, "../assets/images/character/isaac_lift_back.jpg");
    loadimage(&role_dead_front, "../assets/images/character/isaac_dead_front.jpg");
    loadimage(&role_dead_front, "../assets/images/character/isaac_dead_front.jpg");

    loadimage(&role_dead_front,"../assets/images/character/isaac_dead_front.jpg");
    loadimage(&role_dead_back,"../assets/images/character/isaac_dead_back.jpg");
    loadimage(&role_fall_front, "../assets/images/character/isaac_fall_front.jpg");
    loadimage(&role_fall_back, "../assets/images/character/isaac_fall_back.jpg");
    loadimage(&stone_front, "../assets/images/obstacle/stone0_front.jpg");
    loadimage(&stone_back, "../assets/images/obstacle/stone0_back.jpg");
    loadimage(&spine_front, "../assets/images/obstacle/spine0_front.jpg");//尖刺
    loadimage(&spine_back, "../assets/images/obstacle/spine0_back.jpg");
    loadimage(&propName_front, "../assets/images/panel/propName_front.jpg");//道具名称背景
    loadimage(&propName_back, "../assets/images/panel/propName_back.jpg");
    loadimage(&people_front, "../assets/images/monster/people_head_front.jpg");//人形怪物头
    loadimage(&people_back, "../assets/images/monster/people_head_back.jpg");
}

void ItsetGameState()//初始化游戏属性
{
    gameState.level = 0;
    gameState.Roompause = 3;
    gameState.enemyNum = 0;
    gameState.score = 0;
    gameState.ifSave = 0;
    gameState.mode = 0;
    gameState.backgroundNum_i = 0;
    gameState.menuNum = 0;
    gameState.nowFrame_i = 0;
    gameState.gameFrame_i = 0;
    gameState.roomFrame_i = 0;
    gameState.pauseNum_i = 0;
    gameState.pause_i = 0;
    gameState.restart_i = 0;
    gameState.showCollision_i = 0;
}

void ItsetRole()//初始化角色
{
    Role.player.x = Width_i / 2;//初始坐标
    Role.player.y = Hight_i*5 / 6;
    Role.player.speed = 8;//行走速度（像素/帧）
    Role.player.dx = 0;//分速度
    Role.player.dy = 0;
    Role.player.ax = 0;//分加速度
    Role.player.ay = 0;
    Role.player.a = 2;//和加速度
    Role.player.r = 20;//角色碰撞半径（像素）
    Role.player.hp = 1;//角色生命

    Role.property.tears_f = 0.4;//发射速度（秒/次）
    Role.property.shootSpeed_i = 10;//子弹速度（像素/帧）
    Role.property.range_i = 450;//子弹射程（像素）
    Role.property.tearsHeight_i = 40;//眼泪显示高度（像素）
    Role.property.friction_f = 0.4;//摩擦力（像素）
    Role.property.maxHp_i = 6;//最大红心数量
    Role.property.redHp_i = 6;//红心数
    Role.property.blueHp_i = 0;//蓝心数
    Role.property.initDamage_f = 3.5;//伤害
    Role.property.light_i = 4;//轻度

    Role.state.roleHeadNum_i = 0;//头的方向
    Role.state.roleWalkNum_i = 0;//行走方向
    Role.state.shootLeft_i = 0;//是否有向左射击按键
    Role.state.shootRight_i = 0;//向右
    Role.state.shootUp_i = 0;//向上
    Role.state.shootDown_i = 0;//向下
    Role.state.moveDown_i = 0;//角色是否有向下行走按键
    Role.state.moveLeft_i = 0;//向左
    Role.state.moveRight_i = 0;//向右
    Role.state.moveUp_i = 0;//向上
    Role.state.backx_f = 0;//击退分速度
    Role.state.backy_f = 0;
    Role.state.ifShoot_i = 0;//是否试图发射子弹
    Role.state.beginShoot_i = 0;//是否开始射出子弹
    Role.state.shootFrame_i=0; // 射击后帧数
    Role.state.hurtFrame_i = 30;//受伤后帧数
    Role.state.deadFrame_i = 0;//死亡后帧数
    Role.state.ghost = 0;//是否幽灵眼泪
    Role.state.pene=0;//是否子弹穿透效果
    Role.state.fly=0;//是否飞行
    Role.state.lift = 0;//是否举起物品
}

void InitEntity()//初始化实体和子弹
{
    gameState.enemyNum = 0;
    EntityNode* current1 = entityHead;
    while (current1 != NULL)
    {
        EntityNode* temp=current1;
        current1=current1->next;
        free(temp);
    }
    entityHead = NULL;
    BulletNode* current = bulletHead;
    while (current != NULL)
    {
        BulletNode* temp = current;
        current = current->next;
        free(temp);
    }
    bulletHead = NULL;
}

void RanderHealth()//渲染生命值
{
    if (Role.property.redHp_i>Role.property.maxHp_i) Role.property.redHp_i = Role.property.maxHp_i;
    if (Role.property.blueHp_i + Role.property.maxHp_i > 24)Role.property.blueHp_i = 24 - Role.property.maxHp_i;
    int healthX_i= Width_i / 9, healthY_i= Hight_i / 20;//生命值位置
    int nowX_i = healthX_i, nowY_i = healthY_i;
    int fullRedHp_i = Role.property.redHp_i / 2,//整红心
        halfRedHp_i = Role.property.redHp_i % 2,//半红心
        EmptyRedHp_i=Role.property.maxHp_i / 2 - fullRedHp_i - halfRedHp_i,//空红心
        fullBlueHp_i = Role.property.blueHp_i / 2, //满蓝心
        halfBlueHp_i = Role.property.blueHp_i % 2;//半蓝心
    int k;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 6; j++) {
            if (fullRedHp_i > 0) { k = 0; fullRedHp_i--; }
            else if (halfRedHp_i >0) { k = 1; halfRedHp_i--; }
            else if (EmptyRedHp_i > 0) { k = 2; EmptyRedHp_i--; }
            else if (fullBlueHp_i >0) { k = 3; fullBlueHp_i--; }
            else if (halfBlueHp_i >0) { k = 4; halfBlueHp_i--; }
            else break;
            putimage(nowX_i + j  * 40, nowY_i + i * 40, health_front+k, SRCAND);
            putimage(nowX_i + j * 40, nowY_i + i * 40, health_back+k, SRCPAINT);
        }
    }
}

void GetHurt(int damage_i)//角色受伤
{
    if (!Role.player.hp)return;
    while (damage_i > 0) 
    {
        if (Role.property.blueHp_i)Role.property.blueHp_i--;
        else if (Role.property.redHp_i)Role.property.redHp_i--;
        damage_i--;
    }
    if (!(Role.property.redHp_i + Role.property.blueHp_i))Role.player.hp--;
    if (!Role.player.hp)Role.state.deadFrame_i= 70;
}

void UpdateRolePic()//更新玩家贴图
{
    if(fabs(Role.player.dx- Role.player.dy)<=0.1&& fabs(Role.player.dx)<=0.1) Role.state.roleWalkNum_i = 0;
    else if (fabs(Role.player.dx) - fabs(Role.player.dy) > 0.1)//身体动画
    {
        if (Role.player.dx > 0) {
            if (ten(Role.state.roleWalkNum_i) != 10)
                Role.state.roleWalkNum_i = 10;
            if (Framer(frameRate_i / (Role.player.speed * 1.7), 1))
                if (Role.state.roleWalkNum_i >= 19)
                    Role.state.roleWalkNum_i = 10;
                else Role.state.roleWalkNum_i++;
        }//向右
        else {
            if (ten(Role.state.roleWalkNum_i) != 20)
                Role.state.roleWalkNum_i = 20;
            if (Framer(frameRate_i / (Role.player.speed * 1.7), 1))
                if (Role.state.roleWalkNum_i >= 29)
                    Role.state.roleWalkNum_i = 20;
                else Role.state.roleWalkNum_i++;
        }//向左
    }
    else//向上和向下
    {
        if (ten(Role.state.roleWalkNum_i) != 0)
            Role.state.roleWalkNum_i = 0;
        if (Framer(frameRate_i / (Role.player.speed * 1.7), 1))
            if (Role.state.roleWalkNum_i == 9)
                Role.state.roleWalkNum_i = 0;
            else Role.state.roleWalkNum_i++;
    }

    if (!Role.state.ifShoot_i) //头部动画
    {
        if (Role.state.moveUp_i)Role.state.roleHeadNum_i = ten(Role.state.roleHeadNum_i)+2;
        else if (Role.state.moveDown_i)Role.state.roleHeadNum_i = ten(Role.state.roleHeadNum_i) + 0;
        else if (Role.state.moveRight_i)Role.state.roleHeadNum_i = ten(Role.state.roleHeadNum_i) + 1;
        else if (Role.state.moveLeft_i)Role.state.roleHeadNum_i = ten(Role.state.roleHeadNum_i) + 3;
        else if ((Role.player.dx - Role.player.dy <= 0.1) && fabs(Role.player.dx) <= 0.1)Role.state.roleHeadNum_i = ten(Role.state.roleHeadNum_i) + 0;
    }
    else
    {
        if (Role.state.beginShoot_i) {
            Role.state.roleHeadNum_i = Role.state.roleHeadNum_i % 10 + 10;
            mciSendString("play shoot from 0", NULL, 0, NULL);
            Role.state.shootFrame_i = 0;
        }
    }
    Role.state.shootFrame_i++;
    if (Role.state.shootFrame_i > (frameRate_i/60)*60 * Role.property.tears_f * 0.1+6){
        Role.state.shootFrame_i = 0;
        Role.state.roleHeadNum_i = Role.state.roleHeadNum_i % 10;
    }
}

void RanderRole()//渲染角色
{
    int i = 0;//是否向右受伤
    float realX_role = Role.player.x - (float)role_walk_front[Role.state .roleWalkNum_i].getwidth() / 2 ;//角色贴图位置
    float realY_role = Role.player.y - (float)role_walk_front[Role.state .roleWalkNum_i].getheight() / 2;
    if (Role.player.hp) 
    {
        if (Role.state.hurtFrame_i % 8 < 6) {
            if (Role.state.hurtFrame_i > 30) {
                if (Role.state.backx_f > 0)i = 0;
                else i = 1;
                int realX_hurt = Role.player.x - role_hurt_front[i].getwidth() / 2;
                int realY_hurt = Role.player.y - role_hurt_front[i].getheight() / 2;
                putimage(realX_hurt, realY_hurt - 65, &role_hurt_front[i], SRCAND);
                putimage(realX_hurt, realY_hurt - 65, &role_hurt_back[i], SRCPAINT);
            }
            else if (Role.state.lift) {//如果举起物品
                int realX_lift = Role.player.x - role_lift_front.getwidth() / 2;
                int realY_lift = Role.player.y - role_lift_back.getheight() / 2;
                putimage(realX_lift, realY_lift - 65, &role_lift_front, SRCAND);
                putimage(realX_lift, realY_lift - 65, &role_lift_back, SRCPAINT);
            }
            else{
                if (!Role.state.fly) {
                    putimage(realX_role, realY_role - 20, &role_walk_front[Role.state.roleWalkNum_i], SRCAND);
                    putimage(realX_role, realY_role - 20, &role_walk_back[Role.state.roleWalkNum_i], SRCPAINT);
                }
                putimage(realX_role, realY_role - 49, &role_head_front[Role.state.roleHeadNum_i], SRCAND);
                putimage(realX_role, realY_role - 49, &role_head_back[Role.state.roleHeadNum_i], SRCPAINT);
            }
        }
    }
    else {
        if (Role.state.deadFrame_i > 50)
        {
            int realX_dead = Role.player.x - role_dead_front.getwidth() / 2 ;
            int realY_dead = Role.player.y - role_dead_front.getheight() / 2 ;
            putimage(realX_dead, realY_dead - 60, &role_dead_front, SRCAND);
            putimage(realX_dead, realY_dead - 60, &role_dead_back, SRCPAINT);
        }
        else {
            int realX_fall = Role.player.x - role_fall_front.getwidth() / 2 ;
            int realY_fall = Role.player.y - role_fall_front.getheight() / 2 ;
            putimage(realX_fall, realY_fall - 60, &role_fall_front, SRCAND);
            putimage(realX_fall, realY_fall - 60, &role_fall_back, SRCPAINT);
        }
        Role.state.deadFrame_i--;
    }
    if (Role.state.hurtFrame_i > 0)Role.state.hurtFrame_i--;
    if (gameState.showCollision_i )
        circle(Role.player.x , Role.player.y , Role.player.r);//角色碰撞箱
}

void RanderObstacle()//渲染障碍物
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (Room.room[i][j] == 1)//石头
            {
                float x=0, y=0;
                DockArrayToCoordinate(&i, &j, &x, &y, 1);
                putimage(x - stone_front.getwidth() / 2, y - stone_front.getwidth() / 2,&stone_front, SRCAND);
                putimage(x - stone_front.getwidth() / 2, y - stone_front.getwidth() / 2,&stone_back, SRCPAINT);
            }
            if (Room.room[i][j] == 2)//尖刺
            {
                float x = 0, y = 0;
                DockArrayToCoordinate(&i, &j, &x, &y, 1);
                putimage(x - spine_front.getwidth() / 2, y - spine_front.getwidth() / 2, &spine_front, SRCAND);
                putimage(x - spine_front.getwidth() / 2, y - spine_front.getwidth() / 2, &spine_back, SRCPAINT);
            }
        }
    }
}

void RanderBeginground()//显示开始界面
{
        static int i = 0;
        if (i < 8)
            putimage(0, 0, &beginground[0]);
        else
            putimage(0, 0, &beginground[1]);
        i = (i + 1) % 16;
}

void RanderMenu()//显示菜单界面
{
    putimage(0, 0, &menu);
    settextstyle(130, 0, "方正流行体_GBK");
    int shift = 0;//文字偏移
    if (gameState.menuNum == 0) { settextcolor(RED); shift = 2; }
    else {settextcolor(RGB(91,83,80)); shift = 0;}
    outtextxy(Width_i*2.7 / 8+ shift, Hight_i / 6+ shift, "经典模式");
    if (gameState.menuNum == 1) { settextcolor(RED); shift = 2; }
    else { settextcolor(RGB(91,83,80)); shift = 0; }
    outtextxy(Width_i*2.6 / 8+ shift, Hight_i*2 / 6+ shift, "快马加鞭");
    if (gameState.menuNum == 2) { settextcolor(RED); shift = 2; }
    else { settextcolor(RGB(91,83,80)); shift = 0; }
    outtextxy(Width_i*2.5 / 8+ shift, Hight_i*3 / 6+ shift, "排行榜");
    if (gameState.menuNum == 3) { settextcolor(RED); shift = 2; }
    else { settextcolor(RGB(91,83,80)); shift = 0; }
    outtextxy(Width_i*2.4 / 8+ shift, Hight_i*4 / 6+ shift, "返回");
}

void RanderRank(int mode)//排行榜
{
    char path[MAX_PATH];
    SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path);// 获取用户文档目录的路径
    strcat(path, "\\save");// 在文档目录路径后追加文件夹名
    CreateDirectory(path, NULL);//创建文件夹
    putimage(0, 0, &menu);
    settextstyle(100, 0, "方正流行体_GBK");
    settextcolor(RGB(91,83,80));
    char fileName[MAX_PATH] = { 0 };
    if (mode == 0)
    {
        outtextxy(Width_i /4, Hight_i / 8, " RANK: 经典模式");
        sprintf(fileName, "%s/score0.dat",path);
    }
    else if (mode == 1)
    {
        outtextxy(Width_i /4, Hight_i / 8, " RANK: 快马加鞭");
        sprintf(fileName, "%s/score1.dat",path);
    }
    else return;
    FILE* file = fopen(fileName, "rb");
    if (!file)
    {
        settextstyle(80, 0, "方正流行体_GBK");
        outtextxy(Width_i * 3 / 8, Hight_i * 4/11, "暂无分数");
        return;
    }
    Score scores[MAXSCORES];
    int count = 0;
    int shift = 0;//换行偏移
    while (fread(&scores[count], sizeof(Score), 1, file) == 1 && count < MAXSCORES)
    {
        char score[30] = { 0 };
        char time[30] = { 0 };
        sprintf(score, "Num%d: %d", count + 1, scores[count].score);
        sprintf(time, "Time: %ds", scores[count].time);
        settextstyle(80, 0, "方正流行体_GBK");
        outtextxy(Width_i /4, Hight_i * (2 + 0.6 * shift) / 8, score);
        outtextxy(Width_i * 8/ 15, Hight_i * (2 + 0.6 * shift) / 8, time);
        shift++;
        count++;
    }
    fclose(file);
}

void RanderBacground()//显示背景
{
    if (gameState.backgroundNum_i >= 1) {//房间界面
        putimage(0, 0, &background[gameState.backgroundNum_i-1]);
    }
}

void RanderPause()//显示暂停界面
{
    if (!gameState.pause_i)return;
    int realx = Width_i / 2 - pauseface_front[gameState.pauseNum_i].getwidth() / 2;
    int realy = Hight_i / 2 - pauseface_front[gameState.pauseNum_i].getheight() / 2;
    putimage(realx, realy, &pauseface_front[gameState.pauseNum_i], SRCAND);
    putimage(realx, realy, &pauseface_back[gameState.pauseNum_i], SRCPAINT);
}

void RanderDeadInterface()//显示死亡界面
{
    int realx = Width_i/2-testament_front.getwidth()/2;
    int realy = Hight_i/2-testament_front.getheight()/2;
    putimage(realx, realy, &testament_front,SRCAND);
    putimage(realx, realy, &testament_back,SRCPAINT);
}

void ShowData()//显示游戏数据
{
    char time[20] = { 0 };
    settextstyle(50, 0, "方正流行体_GBK"); // 设置文本大小和字体
    setbkmode(TRANSPARENT); // 设置文本背景为透明
    settextcolor(WHITE); // 设置文本颜色
    sprintf(time,"GameTime: %ds", gameState.gameFrame_i / frameRate_i);
    outtextxy(Width_i * 4 / 9, Hight_i / 8, time);

    char score[20] = { 0 };
    sprintf(score, "Score: %.1f", gameState.score);
    outtextxy(Width_i * 9 / 20, Hight_i / 15, score);

    char level[10] = {0};
    sprintf(level, "level %d", gameState.level);
    outtextxy(Width_i*9/20, Hight_i*14/15, level);

    settextstyle(30, 0, "方正流行体_GBK"); 
    char property[20] = { 0 };

    sprintf(property, "%.f", Role.player.speed);
    outtextxy(0, Hight_i * 6 / 30, "SPEED");
    outtextxy(Width_i / 70, Hight_i * 7 / 30, property);

    sprintf(property, "%.2f", Role.property.tears_f);
    outtextxy(0, Hight_i * 9 / 30, "TEARS");
    outtextxy(Width_i / 70, Hight_i * 10 / 30, property);

    sprintf(property, "%2.2f", Role.property.initDamage_f);
    outtextxy(0, Hight_i * 12 / 30, "DAMAGE");
    outtextxy(Width_i / 70, Hight_i * 13 / 30, property);

    sprintf(property, "%2d", Role.property.range_i);
    outtextxy(0, Hight_i * 15 / 30, "RANGE");
    outtextxy(Width_i / 70, Hight_i * 16 / 30, property);

    sprintf(property, "%2d", Role.property.shootSpeed_i);
    outtextxy(0, Hight_i * 18 / 30, "SHOOTSPEED");
    outtextxy(Width_i / 70, Hight_i * 19 / 30, property);

    if (gameState.roomFrame_i > 0)
    {
        setfillcolor(WHITE);
        float one = (float)Width_i * 7 / 9 / gameState.Roompause;//单位长度
        solidrectangle(Width_i/9+one * gameState.roomFrame_i / 60, Hight_i * 6 / 7, Width_i * 8 / 9, Hight_i * 5 / 6);
    }
}

void show()//输出游戏中画面
{
    RanderBacground();//背景
    RanderObstacle();//障碍物
    ShowEntity();//实体
    RanderRole();//角色
    ShowBullets();//子弹
    RanderHealth();//生命值
    ShowData();//显示游戏数据
    DrowRoomLine();//画出方格
    RanderPause();//暂停页面
    if (Role.state.deadFrame_i<5&&Role.player.hp<=0)
        RanderDeadInterface();//死亡页面
}

void MoveRole()//角色移动
{
    if (Role.player.hp == 0)return;
    float magnitude = 0;
    float speedLimit = 0;
    Role.player.ax = (Role.state.moveRight_i - Role.state.moveLeft_i);
    Role.player.ay = (Role.state.moveDown_i - Role.state.moveUp_i);
    magnitude = sqrt(Role.player.ax * Role.player.ax + Role.player.ay * Role.player.ay);
    if (magnitude > 1e-9|| magnitude<-(1e-9)) {
        Role.player.ax /= magnitude;
        Role.player.ay /= magnitude;
        Role.player.dx += Role.player.ax * Role.player.a;
        Role.player.dy += Role.player.ay * Role.player.a;
    }
        speedLimit = sqrt(pow(Role.player.dx, 2) + pow(Role.player.dy, 2));
        if (speedLimit > Role.player.speed) {
            Role.player.dx = (Role.player.dx / speedLimit) * Role.player.speed;
            Role.player.dy = (Role.player.dy / speedLimit) * Role.player.speed;
        }
        Role.player.dx += Role.state.backx_f;
        Role.player.dy += Role.state.backy_f;
        if (fabs(Role.player.dx) < 1) {
            Role.player.dx = 0;
        }
        if (fabs(Role.player.dy) < 1) {
            Role.player.dy = 0;
        }
        Role.player.x += Role.player.dx;
        Role.player.y += Role.player.dy;
        Role.state.backx_f *= 0.3;
        Role.state.backy_f *= 0.3;
        if(Role.player.dx>0.9)Role.player.dx -= Role.property.friction_f;
        if(Role.player.dy>0.9)Role.player.dy -= Role.property.friction_f;
        if (Role.player.dx < -0.9)Role.player.dx += Role.property.friction_f;
        if (Role.player.dy < -0.9)Role.player.dy += Role.property.friction_f;
}

void AddBullet(int dx, int dy) {//添加子弹
    BulletNode* newNode = (BulletNode*)malloc(sizeof(BulletNode));
    if (newNode == NULL) return;
    newNode->bullet.x = Role.player.x;
    newNode->bullet.y = Role.player.y;
    newNode->oldx = Role.player.x;
    newNode->oldy = Role.player.y;
    newNode->bullet.dx = dx;
    newNode->bullet.dy = dy;
    newNode->bullet.r = 15;
    newNode->bullet.hp = 1;
    newNode->bullet_height = Role.property.tearsHeight_i;
    newNode->appear_frame = 0;
    newNode->next = bulletHead;
    bulletHead = newNode;
}

void DrowRoomLine()//画出房间方格
{
    if (!gameState.showCollision_i)return;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 14; j++)
        {
            line(j * RoomLength_i + Width_i / 9, Hight_i / 6, j * RoomLength_i + Width_i / 9, Hight_i*5 / 6);
            line(Width_i / 9, i * RoomWidth_i + Hight_i/6, Width_i*8 / 9, i * RoomWidth_i + Hight_i / 6);
        }
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            float x = (float)0.0, y = (float)0.0;
            DockArrayToCoordinate(&i, &j, &x, &y, 1);
            settextstyle(20, 0, "楷体");
            setbkmode(TRANSPARENT);
            char text[50] = { 0 };
            sprintf(text, "%d", Room.room[i][j]);
            outtextxy((int)x, (int)y, text);
        }
    }
}

void AddEntity(int x, int y,int type,int camp)//添加实体
{
    EntityNode* newNode = (EntityNode*)malloc(sizeof(EntityNode));
    if (newNode == NULL)return;
    if (type == 1) //红苍蝇
    {
        newNode->type = type;
        newNode->camp = camp;
        newNode->entity.x = x;
        newNode->entity.y = y;
        newNode->entity.dx = 0;
        newNode->entity.dy = 0;
        newNode->nowSpeed = 0;
        newNode->backx = 0;
        newNode->backy = 0;
        newNode->entity.r = 30;
        newNode->entity.hp = 8;
        newNode->entity.a = 0.5;
        newNode->ca = 0.5;
        newNode->appear_frame = 0;
        newNode->dead_frame = 0;
        newNode->entity.speed =5;
        newNode->light = 10;
    }
    else if (type == 2)//人形怪物
    {
        newNode->type = type;
        newNode->camp = camp;
        newNode->entity.x = x;
        newNode->entity.y = y;
        newNode->entity.dx = 0;
        newNode->entity.dy = 0;
        newNode->nowSpeed = 0;
        newNode->backx = 0;
        newNode->backy = 0;
        newNode->entity.r = 20;
        newNode->entity.hp = 20;
        newNode->entity.a = 0.4;
        newNode->aniNum = 0;
        newNode->appear_frame = 0;
        newNode->dead_frame = 0;
        newNode->entity.speed = 5;
        newNode->light = 17;
    }
    else if (type > 10 && type < 20)//掉落物
    {
        if (type % 10 <= redHp_count)
        {
            newNode->type = type;
            newNode->camp = camp;
            newNode->entity.x = x;
            newNode->entity.y = y;
            newNode->entity.r = 20;
            newNode->entity.hp = 1;
        }
    }
    else if(type>20)//道具
    {
        newNode->type = type;
        newNode->camp = camp;
        newNode->entity.x = x;
        newNode->entity.y = y;
        newNode->entity.r = 30;
        newNode->entity.hp = 1;
        newNode->dead_frame = 0;
    }
    else return;
    newNode->next = entityHead;
    entityHead = newNode;
}

void ShowEntity()//渲染实体
{
    EntityNode* current = entityHead;
    int realX_entity=0,realY_entity=0,ani=0;
    while (current!=NULL)
    {
        if (current->type == 1) {//红苍蝇
            if (current->entity.hp>0)
            {
                ani = (current->appear_frame) / 3;
                realX_entity = current->entity.x - monster_redfly_front[ani].getwidth() / 2 ;
                realY_entity = current->entity.y - monster_redfly_front[ani].getheight() / 2 ;
            }
            else if (current->dead_frame < 10*2)
            {
                ani = current->dead_frame/2+10;
                realX_entity = current->entity.x - monster_redfly_front[ani].getwidth() / 2 ;
                realY_entity = current->entity.y - monster_redfly_front[ani].getheight() / 2 ;
            }
            if (gameState.showCollision_i )
            circle(current->entity.x , current->entity.y, current->entity.r);
            putimage(realX_entity, realY_entity - 40, monster_redfly_front + ani, SRCAND);
            putimage(realX_entity, realY_entity - 40, monster_redfly_back + ani, SRCPAINT);
        }
        else if (current->type == 2) {//人形怪物
            if (current->entity.hp > 0)
            {
                if (fabs(current->entity.dx - current->entity.dy) <= 0.1 && fabs(current->entity.dx) <= 0.1) Role.state.roleWalkNum_i = 0;
                else if (fabs(current->entity.dx) - fabs(current->entity.dy) > 0.1)//动画
                {
                    if (current->entity.dx > 0.1) {
                        if (ten(current->aniNum)!= 10)
                            current->aniNum = 10;
                        if (current->appear_frame % 4 == 1)
                            if (current->aniNum >= 19)
                                current->aniNum = 10;
                            else current->aniNum++;
              
                    }//向右
                    else{
                        if (ten(current->aniNum) !=20)
                            current->aniNum = 20;
                        if (current->appear_frame % 4 == 1)
                            if (current->aniNum >= 29)
                                current->aniNum = 20;
                            else current->aniNum++;
                    }//向左
                }
                else//向上和向下
                {
                    if (ten(current->aniNum) != 0)
                        current->aniNum = 0;
                    if (current->appear_frame % 4 == 1)
                        if (current->aniNum == 9)
                            current->aniNum = 0;
                        else current->aniNum++;
                }
                realX_entity = current->entity.x - role_walk_front[0].getwidth() / 2;
                realY_entity = current->entity.y - role_walk_front[0].getheight() / 2;
                putimage(realX_entity, realY_entity - 20, role_walk_front + current->aniNum, SRCAND);
                putimage(realX_entity, realY_entity - 20, role_walk_back + current->aniNum, SRCPAINT);
                putimage(realX_entity + 3, realY_entity - 49, &people_front, SRCAND);
                putimage(realX_entity + 3, realY_entity - 49, &people_back, SRCPAINT);
            }
            else if (current->dead_frame >0)
            {
                realX_entity = current->entity.x - role_fall_front.getwidth() / 2;
                realY_entity = current->entity.y - role_fall_back.getheight() / 2;
                putimage(realX_entity, realY_entity - 60, &role_fall_front, SRCAND);
                putimage(realX_entity, realY_entity - 60, &role_fall_back, SRCPAINT);
            }
            if (gameState.showCollision_i)
                circle(current->entity.x, current->entity.y, current->entity.r);
        }
        else if (current->type > 10 && current->type < 20)//掉落物
        {
            if (current->type % 10 <= redHp_count)
            {
                realX_entity = current->entity.x - redHp_front[0].getwidth() / 2;
                realY_entity = current->entity.y - redHp_front[0].getheight() / 2;
                if (gameState.showCollision_i)
                    circle(current->entity.x, current->entity.y, current->entity.r);
                putimage(realX_entity, realY_entity, &redHp_front[current->type % 10 - 1], SRCAND);
                putimage(realX_entity, realY_entity, &redHp_back[current->type % 10 - 1], SRCPAINT);
            }
        }
        else if (current->type > 20)
        {
            realX_entity = current->entity.x - prop_front[0].getwidth() / 2;
            realY_entity = current->entity.y - prop_back[0].getheight() / 2;
            if (gameState.showCollision_i)
                circle(current->entity.x, current->entity.y, current->entity.r);
            putimage(realX_entity, realY_entity, &prop_front[current->type % 10 - 1], SRCAND);
            putimage(realX_entity, realY_entity, &prop_back[current->type % 10 - 1], SRCPAINT);
        }
        current = current->next;
    }
}

void UpdateEntity()//更新实体
{
    EntityNode* current = entityHead;
    EntityNode* pre = NULL;
    while (current !=NULL)
    {
        if (current->type == 1) {//红苍蝇
            if (current->entity.hp > 0) {
                current->entity.ax = Role.player.x - current->entity.x;
                current->entity.ay = Role.player.y - current->entity.y;
                float disRole= sqrt(pow(current->entity.ax, 2) + pow(current->entity.ay, 2));//与角色距离
                current->entity.ax = ((current->entity.ax) / disRole) * current->entity.a;
                current->entity.ay = ((current->entity.ay) / disRole) * current->entity.a;
                current->entity.dx += current->entity.ax;
                current->entity.dy += current->entity.ay;
                if (pow(current->entity.dx, 2) + pow(current->entity.dy, 2) > pow(current->entity.speed, 2))
                {
                    current->ca = current->entity.speed * pow((disRole / 200), 4);
                    if (current->ca > current->entity.speed)current->ca = current->entity.speed;
                    current->entity.dx = (current->entity.dx) / sqrt(pow(current->entity.dx, 2) + 
                        pow(current->entity.dy, 2)) * current->ca + current->backx;
                    current->entity.dy = (current->entity.dy) / sqrt(pow(current->entity.dx, 2) + 
                        pow(current->entity.dy, 2)) * current->ca + current->backy;
                    current->backx=current->backx*0.3;
                    current->backy=current->backy*0.3;
                }
                current->entity.dx+= current->backx;
                current->entity.dy += current->backy;
                current->entity.x += current->entity.dx;
                current->entity.y += current->entity.dy;
                current->appear_frame = (current->appear_frame + 1) % 12;

                current->entity.x = Min(current->entity.x, Width_i * 8 / 9 - Role.player.r);
                current->entity.y = Min(current->entity.y, Hight_i * 5 / 6 - Role.player.r);
                current->entity.x = Max(current->entity.x, Width_i / 9 + Role.player.r);
                current->entity.y = Max(current->entity.y, Hight_i / 6 + Role.player.r);
            }
            else {
                current->entity.hp = 0;
                current->dead_frame++;
                if (current->dead_frame > 9*2) {
                    gameState.enemyNum--;//敌人减一
                    EntityNode* temp = current;
                    if (pre != NULL) pre->next = current->next;
                    else entityHead = current->next;
                    current = current->next;
                    free(temp);
                    temp = NULL;
                    continue;
                }
            }
        }
        else if (current->type == 2) {//人形怪物
            if (current->entity.hp > 0) {
                float nextX = 0, nextY = 0;
                if (!AINextPath(current->entity.x, current->entity.y, Role.player.x, Role.player.y, &nextX, &nextY, Room.room))
                {
                    nextX = Role.player.x;
                    nextY = Role.player.y;
                }
                float dirX = nextX - current->entity.x;
                float dirY = nextY - current->entity.y;
                float distance = sqrt(dirX * dirX + dirY * dirY);
                if (distance > 0) {
                    dirX /= distance;
                    dirY /= distance;
                    current->entity.dx += dirX * current->entity.a;
                    current->entity.dy += dirY * current->entity.a;
                    // 限制速度不超过最大速度
                    float speed = sqrt(current->entity.dx * current->entity.dx + current->entity.dy * current->entity.dy);
                    if (speed > current->entity.speed) {
                        current->entity.dx = current->entity.dx / speed * current->entity.speed;
                        current->entity.dy = current->entity.dy / speed * current->entity.speed;
                    }
                }
                current->entity.dx += current->backx;
                current->entity.dy += current->backy;
                current->backx *= 0.3;
                current->backy *= 0.3;
                current->entity.x += current->entity.dx;
                current->entity.y += current->entity.dy;
                current->appear_frame++;

                current->entity.x = Min(current->entity.x, Width_i * 8 / 9 - Role.player.r);
                current->entity.y = Min(current->entity.y, Hight_i * 5 / 6 - Role.player.r);
                current->entity.x = Max(current->entity.x, Width_i / 9 + Role.player.r);
                current->entity.y = Max(current->entity.y, Hight_i / 6 + Role.player.r);
            }
            else {
                current->dead_frame++;
                if (current->dead_frame > 9 * 2) {
                    gameState.enemyNum--;//敌人减一
                    EntityNode* temp = current;
                    if (pre != NULL) pre->next = current->next;
                    else entityHead = current->next;
                    current = current->next;
                    free(temp);
                    temp = NULL;
                    continue;
                }
            }
        }
        else if (current->type > 10 && current->type < 20)//掉落物
        {
            if (current->type % 10 <= redHp_count)//红心
            {
                if (Role.property.redHp_i < Role.property.maxHp_i && CheckCollision(current->entity, Role.player))
                {
                    if (current->type % 10 == 1)Role.property.redHp_i += 2;
                    else if (current->type % 10 == 2)Role.property.redHp_i += 1;
                    current->entity.hp = 0;
                    if (current->entity.hp <= 0) {
                        EntityNode* temp = current;
                        if (pre != NULL) pre->next = current->next;
                        else entityHead = current->next;
                        current = current->next;
                        free(temp);
                        temp = NULL;
                        continue;
                    }
                }
            }
        }
        else if (current->type >20)//道具
        {
            if (CheckCollision(current->entity, Role.player) && current->entity.hp > 0)
            {
                Role.state.lift = 1;//举起物品
                current->entity.hp = 0;
                mciSendString("play getProp from 0", NULL, 0, NULL);
                switch (current->type % 10) {
                case 1:
                    Role.property.maxHp_i += 2;
                    Role.property.redHp_i += 4;
                    Role.property.maxHp_i = Min(24, Role.property.maxHp_i);
                    break;
                case 2:
                    Role.state.pene = 1;
                    break;
                case 3:
                    Role.state.ghost = 1;
                    break;
                case 4:
                    Role.state.fly = 1;
                    break;
                case 5:
                    Role.property.initDamage_f += 1;
                    break;
                case 6:
                    Role.property.tears_f -= 0.05;
                    if (Role.property.tears_f < 0.1)Role.property.tears_f = 0.1;
                    break;
                case 7:
                    Role.property.range_i += 60;
                    break;
                }
            }
            if (current->entity.hp <= 0)
            {
                settextstyle(70, 0, "方正流行体_GBK");
                settextcolor(WHITE);
                putimage(Width_i / 20, Hight_i / 15, &propName_front, SRCAND);
                putimage(Width_i / 20, Hight_i / 15, &propName_back, SRCPAINT);
                switch (current->type % 10) {
                case 1:outtextxy(Width_i*4 / 10, Hight_i / 8, "     午餐");break;
                case 2:outtextxy(Width_i*4 / 10, Hight_i / 8 ,"丘比特之箭"); break;
                case 3:outtextxy(Width_i*4 / 10, Hight_i / 8, "   通灵板"); break;
                case 4:outtextxy(Width_i*4 / 10, Hight_i / 8, " 超凡升天"); break;
                case 5:outtextxy(Width_i* 4/ 10, Hight_i / 8, "   史蒂文"); break;
                case 6:outtextxy(Width_i* 4/ 10, Hight_i / 8, " 悲伤洋葱"); break;
                case 7:outtextxy(Width_i*4 / 10, Hight_i / 8, "   长条虫"); break;
                }
                current->entity.x = Role.player.x- (float)Width_i/250;
                current->entity.y = Role.player.y-120;
                current->dead_frame++;
                if (current->dead_frame >= 1.4*60*60/frameRate_i) {
                    Role.state.lift = 0;//放下物品
                    EntityNode* temp = current;
                    if (pre != NULL) pre->next = current->next;
                    else entityHead = current->next;
                    current = current->next;
                    free(temp);
                    temp = NULL;
                    continue;
                }
            }
        }
        pre = current;
        current = current->next;
    }
}

void ShowBullets()//渲染子弹
{
    BulletNode* current = bulletHead;
    while (current != NULL) {
        if ( current->appear_frame <= 14) {
            int realX_bullet = current->bullet.x - bullet_front[current->appear_frame].getwidth() / 2 ;
            int realY_bullet = current->bullet.y - bullet_front[current->appear_frame].getheight() / 2 - current->bullet_height;
            putimage(realX_bullet, realY_bullet, bullet_front + current->appear_frame % Role_bullet_count, SRCAND);
            putimage(realX_bullet, realY_bullet, bullet_back + current->appear_frame % Role_bullet_count, SRCPAINT);
            if(gameState.showCollision_i )
            circle(current->bullet.x , current->bullet.y , current->bullet.r);
        }
        current = current->next;
    }
}

void UpdateBullets() // 更新子弹
{
    BulletNode* current = bulletHead;
    BulletNode* pre = NULL;
    while (current != NULL) {
        // 如果子弹的hp大于0，则更新位置
        if (current->bullet.hp > 0) {
            current->bullet.x += current->bullet.dx;
            current->bullet.y += current->bullet.dy;

            // 检查子弹是否超出射程
            if (pow(fabs(current->oldx - current->bullet.x), 2) + pow(fabs(current->oldy - current->bullet.y), 2) > pow(Role.property.range_i, 2))
            {
                current->bullet_height-=5;
                if(current->bullet_height<=0)
                current->bullet.hp = 0; 
            }
            // 检查子弹是否碰到墙
            if (current->bullet.x < Width_i/9 -30 || current->bullet.x > Width_i * 8 / 9 + 30 || current->bullet.y < Hight_i/6 || current->bullet.y > Hight_i * 5 / 6+60)
            {
                current->bullet.hp = 0;
            }
        }
        else {
            if(current->appear_frame==0)mciSendString("play shootBreak from 0", NULL, 0, NULL);
            current->appear_frame++;
            if (current->appear_frame > 14)
            {
                if (pre != NULL) {
                    pre->next = current->next;
                }
                else {
                    bulletHead = current->next;
                }
                BulletNode* temp = current;
                current = current->next;
                free(temp);
                temp = NULL;
                continue;

            }
        }
        pre = current;
        current = current->next;
    }
}

void limit()//防止溢出屏幕
{
    int i;
    Role.player.x = Min(Role.player.x, Width_i * 8 / 9 - Role.player.r);
    Role.player.y = Min(Role.player.y, Hight_i * 5 / 6 - Role.player.r);
    Role.player.x = Max(Role.player.x, Width_i/9+ Role.player.r);
    Role.player.y = Max(Role.player.y, Hight_i /6+Role.player.r);
}

int CheckCollision(Ball ball1, Ball ball2) {//两小球是否碰撞
    float distX = ball1.x - ball2.x;
    float distY = ball1.y - ball2.y;
    float distance = sqrt(distX * distX + distY * distY);
    if (distance < ball1.r + ball2.r) {
        return 1;
    }
    return 0;
}

int ObstacleCollision(int oX_i, int oY_i, Ball ball, int* leftX, int* leftY, int* rightX, int* rightY)//检测小球和方块碰撞
{
    float x = 0, y = 0;
    DockArrayToCoordinate(&oX_i, &oY_i, &x, &y, 1);
    int leftX_i = x - (float)RoomLength_i / 2, leftY_i = y - (float)RoomWidth_i / 2;//障碍物左上角坐标
    int rightX_i = x + (float)RoomLength_i / 2, rightY_i = y + (float)RoomWidth_i / 2;//障碍物右下角坐标
    if (leftX != NULL && leftY != NULL && rightX != NULL && rightY != NULL)
    {
        *leftX = leftX_i;
        *leftY = leftY_i;
        *rightX = rightX_i;
        *rightY = rightY_i;
    }
    if (ball.x + ball.r > leftX_i && ball.x< rightX_i + ball.r
        && ball.y + ball.r>leftY_i && ball.y - ball.r < rightY_i)//是否碰撞
    {
        return 1;
    }
    else return 0;
}

void RoleObstacleCollision()//障碍物与角色碰撞处理
{
    if (Role.state.fly)return;
    int leftX_i = 0, leftY_i = 0;//障碍物左上角坐标
    int rightX_i = 0, rightY_i = 0;//障碍物右下角坐标
    for (int i = 0; i < ROW; i++) {
        for (int j = 0;j < COL; j++) {
            if (Room.room[i][j] == 1) //石头障碍物
            {
                if (ObstacleCollision(i,j,Role.player,&leftX_i,&leftY_i,&rightX_i,&rightY_i))
                {
                    int x = (leftX_i + rightX_i) / 2, y = (leftY_i + rightY_i) / 2;//障碍物坐标
                    if (fabs(Role.player.x - x) > fabs(Role.player.y - y) )
                    {
                        Role.player.x < x ? Role.player.x = leftX_i - Role.player.r : Role.player.x = rightX_i + Role.player.r;
                    }
                    else Role.player.y < y ? Role.player.y = leftY_i - Role.player.r : Role.player.y = rightY_i + Role.player.r;
                }
            }
            if (Room.room[i][j] == 2)//尖刺
            {
                if (ObstacleCollision(i, j, Role.player, &leftX_i, &leftY_i, &rightX_i, &rightY_i))
                {
                    float x = (leftX_i + rightX_i) / 2, y = (leftY_i + rightY_i) / 2;//障碍物坐标
                    float Dx = Role.player.x-x, Dy =  Role.player.y-y;
                    float distance = sqrt(Dx * Dx + Dy * Dy);
                    if (distance == 0)distance = 1;
                    Dx /= distance;
                    Dy /= distance;
                    if (Role.state.hurtFrame_i <= 0)
                    {
                        GetHurt(1);
                        Role.state.hurtFrame_i += 60;
                        Role.state.backx_f += Dx * 8;
                        Role.state.backy_f += Dy * 8;
                    }
                }
            }
        }
    }
}

void BulletObstacleCollision()//子弹障碍物碰撞处理
{
    BulletNode* current = bulletHead;
    while (current != NULL)
    {
        for (int i = 0; i < ROW; i++) {
            for (int j = 0; j < COL; j++) {
                if (Room.room[i][j] == 1) {
                    if (ObstacleCollision(i, j, current->bullet, NULL, NULL, NULL, NULL))
                    {
                        if(!Role.state.ghost)current->bullet.hp = 0;
                    }
                }
            }
        }
        current = current->next;
    }
}

void EntityObstacleCollision()//实体障碍物碰撞处理
{
    EntityNode* current = entityHead;
    while (current != NULL)
    {
        if (current->type == 2)//人形怪物
        {
            for (int i = 0; i < ROW; i++) {
                for (int j = 0; j < COL; j++) {
                    if (Room.room[i][j] == 1 ||Room.room[i][j]==2) {
                        int leftX_i = 0, rightX_i = 0, leftY_i = 0, rightY_i = 0;
                        if (ObstacleCollision(i, j, current->entity, &leftX_i, &leftY_i, &rightX_i, &rightY_i))
                        {
                            int x = (leftX_i + rightX_i) / 2, y = (leftY_i + rightY_i) / 2;//障碍物坐标
                            if (fabs(current->entity.x - x) > fabs(current->entity.y - y))
                            {
                                current->entity.x < x ? current->entity.x = leftX_i - current->entity.r : current->entity.x = rightX_i + current->entity.r;
                            }
                            else current->entity.y < y ? current->entity.y = leftY_i - current->entity.r : current->entity.y = rightY_i + current->entity.r;
                        }
                    }
                }
            }
        }
        current = current->next;
    }
}

void EntityCollision()//实体碰撞和实体与角色碰撞处理
{
    EntityNode* current1=entityHead;
    while (current1 != NULL) {
        EntityNode* current2 = current1->next;
        while (current2 != NULL&&current1->type < 10&& current2->type<10)
        {
            if (CheckCollision(current1->entity, current2->entity))
            {
                float Dx = current2->entity.x - current1->entity.x;
                float Dy = current2->entity.y - current1->entity.y;
                float length = sqrt(pow(Dx, 2) + pow(Dy, 2));
                if (length == 0) length = 1;
                Dx /= length;
                Dy /= length;
                
                current1->backx -= Dx*0.5;
                current1->backy -= Dy*0.5;
                current2->backx += Dx*0.5;
                current2->backy += Dy*0.5;
            }
            current2 = current2->next;
        }
        if (CheckCollision(current1->entity, Role.player)&&Role.player.hp) {
            float Dx = Role.player.x - current1->entity.x;
            float Dy = Role.player.y - current1->entity.y;
            float length = sqrt(pow(Dx, 2) + pow(Dy, 2));
            if (length == 0) length = 1;
            Dx /= length;
            Dy /= length;
            if (current1->camp == 1&&Role.state.hurtFrame_i<=0)
            {
                GetHurt(1);
                Role.state.hurtFrame_i += 60;
                Role.state.backx_f += Dx*8 ;
                Role.state.backy_f += Dy*8 ;
            }
            current1->backx -= Dx *0.05*current1->light;
            current1->backy -= Dy *0.05 * current1->light;
            Role.state.backx_f += Dx * 0.3;
            Role.state.backy_f += Dy * 0.3;
        }
        current1 = current1->next;
    }
}

void Knockback(EntityNode* entity, BulletNode* bullet)//子弹击退
{
    float budirecx = bullet->bullet.dx;
    float budirecy = bullet->bullet.dy;
    float length = sqrt(pow(budirecx, 2) + pow(budirecy, 2));
    budirecx /= length;
    budirecy /= length;
    entity->backx += budirecx * entity->light/3;
    entity->backy += budirecy * entity->light/3;
}

void Attack() {//处理角色子弹攻击
    BulletNode* bullet = bulletHead;
    EntityNode* entity = entityHead;

    while (bullet != NULL) {
        while (entity != NULL) {
            if (bullet->bullet.hp > 0 && entity->entity.hp > 0&&entity->type<10) {
                if (CheckCollision(bullet->bullet, entity->entity) && Framer((13*60 / frameRate_i), 2)) {
                    entity->entity.hp -= Role.property.initDamage_f;
                    Knockback(entity, bullet);
                    if (!Role.state.pene) bullet->bullet.hp = 0;
                }
            }
            entity = entity->next;
        }
        bullet = bullet->next;
        entity = entityHead; 
    }
}

void SummonEntity()//生成实体处理
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (Room.room[i][j] == 11)//红苍蝇
            {
                float x=0, y=0;
                DockArrayToCoordinate(&i,&j,&x,&y,1);
                AddEntity(x, y, 1, 1);
                gameState.enemyNum++;
            }
            else if (Room.room[i][j] == 12)//人形敌人
            {
                float x = 0, y = 0;
                DockArrayToCoordinate(&i, &j, &x, &y, 1);
                AddEntity(x, y, 2, 1);
                gameState.enemyNum++;
            }
            else if (Room.room[i][j] > 20 && Room.room[i][j] < 30)//掉落物
            {
                if ((Room.room[i][j] - 10) % 10 <= redHp_count)//红心掉落物
                {
                    float x = 0, y = 0;
                    DockArrayToCoordinate(&i, &j, &x, &y, 1);
                    AddEntity(x, y, 10 + Room.room[i][j] % 10, 0);
                }
            }
            else if (Room.room[i][j] > 30)//道具
            {
                float x = 0, y = 0;
                DockArrayToCoordinate(&i, &j, &x, &y, 1);
                AddEntity(x, y, 20 + Room.room[i][j] % 10, 0);
            }
        }
    }
}

void CalculateScore()//计算总分
{
    gameState.score = gameState.level * 10-((float)gameState.gameFrame_i/frameRate_i)*0.5;
    if (gameState.score <= 0)gameState.score = 0;
}

void Save(int score,int time,int mode)//储存分数
{
    char path[MAX_PATH];
    SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path);    // 获取用户文档目录的路径
    strcat(path, "\\save");// 在文档目录路径后追加文件夹名
    CreateDirectory(path, NULL);//创建文件夹
    char fileName[MAX_PATH] = { 0 };
    if (mode == 0)sprintf(fileName, "%s/score0.dat", path);
    else if (mode == 1)sprintf(fileName, "%s/score1.dat", path);
    FILE* file = fopen(fileName, "rb+");
    if (!file)
    {
        file = fopen(fileName, "wb");
        if (!file)return;
    }
    Score scores[MAXSCORES+1];
    int count = 0;
    while (fread(&scores[count], sizeof(Score), 1, file) == 1 && count <= MAXSCORES)
    {
        count++;
    }
    scores[count].score = score;
    scores[count].time = time;
    scores[count].mode = mode;
    count++;
    for (int i = 0; i < count-1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (scores[j].score > scores[i].score)
            {
                Score temp = scores[i];
                scores[i] = scores[j];
                scores[j] = temp;
            }
        }
    }
    if (count > MAXSCORES)count = MAXSCORES;
    if (freopen(fileName, "wb", file) == NULL)return;
    fwrite(scores, sizeof(Score), count, file);
    fclose(file);
}

void Sound()//输出音效
{
    static int preHp = 0;
    if (gameState.restart_i)
    {
        preHp = 6;
        return;
    }
    int nowHp = Min(Role.property.maxHp_i, Role.property.redHp_i + Role.property.blueHp_i);
    if (preHp > nowHp)
    {
        mciSendString("play hurt from 0", NULL, 0, NULL);
    }
    preHp = nowHp;
}

void BGM()//输出背景音乐
{
    static int pre= 100;
    int dead = 0;
    static int preDead = 0;
    if (Role.player.hp <= 0)dead = 0;
    else dead = 1;
    if (!(pre != gameState.backgroundNum_i && !(pre <= 0 && gameState.backgroundNum_i <= 0) || preDead != dead))return;
    preDead = dead;
    pre = gameState.backgroundNum_i;
    mciSendString("stop menu", NULL, 0, NULL);
    mciSendString("stop dead", NULL, 0, NULL);
    mciSendString("stop deadPic", NULL, 0, NULL);
    mciSendString("stop room", NULL, 0, NULL);
    mciSendString("stop fast", NULL, 0, NULL);
    if (gameState.backgroundNum_i <= 0) mciSendString("play menu from 0 repeat", NULL, 0, NULL);
    else if (Role.player.hp <= 0)
    {
        mciSendString("play dead from 0", NULL, 0, NULL);
        mciSendString("play deadPic from 0 repeat", NULL, 0, NULL);
    }
    else if (gameState.backgroundNum_i == 1) mciSendString("play room from 0 repeat", NULL, 0, NULL);
    else if (gameState.backgroundNum_i == 2) mciSendString("play fast from 0 repeat", NULL, 0, NULL);
}

void Init()//初始化
{
    InitEntity();
    ItsetRole();
    GenerateMap(Room.room, 1);
    SummonEntity();
    gameState.level = 1;
}

void Update()//更新游戏
{
    gameState.gameFrame_i++;//帧计数
    Key();//游戏时按键
    CalculateScore();//计算总分
    UpdateBullets();
    MoveRole();
    limit();
    EntityCollision();
    RoleObstacleCollision();
    BulletObstacleCollision();
    EntityObstacleCollision();
    UpdateEntity();
    Attack();
    UpdateRolePic();
}

void NextLevel()//进入下一个关卡
{

    gameState.level++;
    Role.player.x = Width_i / 2;
    Role.player.y = Hight_i * 5 / 6;
    Role.state.hurtFrame_i = 30;
    InitEntity();
    Role.state.lift = 0;
    GenerateMap(Room.room, gameState.level);
    SummonEntity();
}

void GameStateDeal()//游戏状态处理
{
    show();//更新游戏中画面
    if (gameState.backgroundNum_i <= 0)frameRate_i = 60;//充值帧数
    if (gameState.backgroundNum_i == 0) //处于开始界面
    {
        RanderBeginground();//开始界面
        BeginkKey();
    }
    else if (gameState.backgroundNum_i == -1)//处于菜单
    {
        RanderMenu();
        MenuKey();
    }
    else if (gameState.backgroundNum_i == -2)//处于排行榜
    {
        RanderRank(gameState.rankNum);
        RankKey();
    }
    else {//进入游戏
        if (GetAsyncKeyState('R') & 0x8000)//游戏中的重新开始
        {
            if (Framer(30, 3))gameState.restart_i = 1;
        }
        Sound();
        if (gameState.restart_i) {
            Init();
            gameState.level = 1;
            gameState.gameFrame_i= 0;
            gameState.restart_i = 0;
            gameState.score = 0;
            gameState.roomFrame_i = 0;
            gameState.ifSave = 0;
        }
        if (Role.state.deadFrame_i < 0)//死亡
        {
            if (!gameState.ifSave)
            {
                Save(gameState.score, gameState.gameFrame_i / frameRate_i, gameState.mode);
                gameState.ifSave = 1;
            }
            DeadKey();//死亡按键
        }
        else if (gameState.pause_i)PauseKey();//暂停按键
        else
        {
            if (gameState.enemyNum <= 0)//判断没有敌人
            {
                gameState.roomFrame_i++;
                if (gameState.roomFrame_i > 60 * gameState.Roompause)
                {
                    gameState.roomFrame_i = 0; 
                    NextLevel();//进入下一关
                }
            }
            Update();//更新游戏
        }
    }
}

int main()
{
    AddFontResourceEx("../assets/fonts/方正流行体_GBK.TTF", FR_PRIVATE, 0);
    srand(time(NULL));
    initgraph(Width_i, Hight_i);
    InitPIC();
    InitSound();
    ItsetGameState();
    Init();
    BeginBatchDraw();
    while (1) {
        BGM();
        if (Timer(CLOCKS_PER_SEC / frameRate_i, 0)) {
            gameState.nowFrame_i++;//帧率计时
            cleardevice();
            GameStateDeal();
            FlushBatchDraw();
        }
    }
    EndBatchDraw();
    closegraph();
    RemoveFontResourceEx("../assets/fonts/方正流行体_GBK.TTF", FR_PRIVATE, 0);
    return 0;
}