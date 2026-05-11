//lava的指针操作测试

#define KEY_UP      20
#define KEY_DOWN    21
#define KEY_RIGHT   22
#define KEY_LEFT	23
#define KEY_ENTER   13
#define KEY_SHIFT   26
#define KEY_CAPS    18
#define KEY_ESC	    27
#define KEY_HELP	25
#define	KEY_PAGEUP		19
#define	KEY_PAGEDOWN	14
#define KEY_Y       121
#define KEY_N       110
#define KEY_F1      28      //确定
#define KEY_F2      29      //取消
#define KEY_F4      31      //卸下装备

//片头菜单坐标
char Title_Menu[] =
{
    4, 102, 10, 152, 23, 102, 24, 152, 37, 102, 38, 152, 51, 102, 52, 152, 65
};

char ConvertKey(char key)
{
    if (key == 'w') return KEY_UP;
    if (key == 'a') return KEY_LEFT;
    if (key == 's') return KEY_DOWN;
    if (key == 'd') return KEY_RIGHT;
    if (key == 'j') return KEY_F1;
    if (key == 'k') return KEY_F2;
    return key;
}

char KeyPause(int delaytime)
{
    char key;
    if (delaytime != 0) {
        Delay(delaytime);
    }
    key = CheckKey(128);
    if (key == 0) {
        while (Inkey() != 0);
        key = getchar();
    }
    return ConvertKey(key);
}

void drawTitle(){
    ClearScreen();
    TextOut(20,30,"武侠游戏",0x81);
    Rectangle(18,28,85,47,2);
    Block(22,47,83,48,1);
    TextOut(102, 10, "初入江湖", 1);
    TextOut(102, 24, "再续前缘", 1);
    TextOut(102, 38, "武林豪杰", 1);
    TextOut(102, 52, "江湖再见", 1);
    Refresh();
}
void main() {
  int i, j;
  char p;
  long ptr;
  char key;
  p = 1;
    
    do {
        drawTitle();
        ptr = Title_Menu + 1 + (p - 1) * 4;
        Box(*ptr, *(ptr + 1), *(ptr + 2), * (ptr + 3), 1, 2);
        key = KeyPause(200);
        if (key == 20 || key == 22) {
            if (p > 1) {
                p--;
            }
            else {
                p = Title_Menu[0];
            }
        } else if (key == 21 || key == 21) {
            if (p < Title_Menu[0]) {
                p++;
            }
            else {
                p = 1;
            }
        } else if (key == 27) {
            break;
        }
    } while (1);
    printf("bye!!");
    getchar();

}