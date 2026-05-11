/*
 * LavaX 指针功能演示程序
 *
 * 演示 LavaX 语言中的指针用法：
 * - addr 类型作为指针
 * - long 类型作为指针（addr 是 long 的别名）
 * - (int *) / (char *) / (long *) 解引用语法
 * - * 作为 (char *) 的简写
 * - 指针写回：(int *)p = val
 * - 指针跨函数传递
 * - 指针指向数组元素
 */

/* 通过指针设置 int 值 */
void set_int(addr p, int val) {
    (int *)p = val;
}

/* 通过指针读取 int 值 */
int get_int(addr p) {
    return (int *)p;
}

/* 交换两个整数（通过指针） */
void swap(addr a, addr b) {
    int tmp;
    tmp = (int *)a;
    (int *)a = (int *)b;
    (int *)b = tmp;
}

/* 求数组最大值，返回指向最大元素的指针 */
addr arr_max(addr base, int n) {
    addr best;
    int i;
    best = base;
    i = 1;
    while (i < n) {
        if ((int *)(base + i * 2) > (int *)best) {
            best = base + i * 2;
        }
        i = i + 1;
    }
    return best;
}

void main() {
    int a;
    int b;
    addr pa;
    addr pb;
    char c;
    long L;
    addr pL;
    int arr[5];
    addr pmax;
    int i;

    SetScreen(0);

    /* === 基本读写 === */
    printf("=== 基本指针读写 ===\n");

    a = 42;
    pa = &a;
    printf("a=%d, (int *)pa=%d\n", a, (int *)pa);

    /* 通过指针修改值 */
    (int *)pa = 100;
    printf("after (int*)pa=100: a=%d\n", a);

    /* char 指针 */
    c = 65;  /* 'A' */
    pb = &c;
    printf("c=%d, (char *)pb=%d, *pb=%d\n", c, (char *)pb, *pb);

    /* * 写回 */
    *pb = 66;  /* 'B' */
    printf("after *pb=66: c=%d\n", c);

    /* long/addr 指针 */
    L = 999999;
    pL = &L;
    printf("L=%ld, (long *)pL=%ld\n", L, (long *)pL);
    (long *)pL = 123456;
    printf("after (long*)pL=123456: L=%ld\n", L);

    /* === 跨函数传递指针 === */
    printf("\n=== 跨函数传递指针 ===\n");

    a = 0;
    set_int(&a, 77);
    printf("set_int(&a,77): a=%d\n", a);

    a = 55;
    printf("get_int(&a)=%d\n", get_int(&a));

    /* 交换 */
    a = 10;
    b = 20;
    printf("before swap: a=%d b=%d\n", a, b);
    swap(&a, &b);
    printf("after swap:  a=%d b=%d\n", a, b);

    /* === long 作为指针 === */
    printf("\n=== long 作为指针 ===\n");
    a = 88;
    L = &a;
    printf("a=%d, (int *)L=%d\n", a, (int *)L);
    (int *)L = 99;
    printf("after (int*)L=99: a=%d\n", a);

    /* === 指向数组元素 === */
    printf("\n=== 指向数组元素 ===\n");
    arr[0] = 5;
    arr[1] = 3;
    arr[2] = 8;
    arr[3] = 1;
    arr[4] = 6;

    /* 指向第2个元素 */
    pa = &arr[2];
    printf("arr[2]=%d, (int *)pa=%d\n", arr[2], (int *)pa);

    /* 通过指针修改数组元素 */
    (int *)pa = 99;
    printf("after (int*)pa=99: arr[2]=%d\n", arr[2]);

    /* 求最大值指针 */
    arr[2] = 8;  /* restore */
    pmax = arr_max(&arr[0], 5);
    printf("max element = %d\n", (int *)pmax);

    /* === 指针指向指针（addr 存 addr） === */
    printf("\n=== 二级指针（addr of addr） ===\n");
    a = 7;
    pa = &a;
    pb = &pa;
    /* pb 指向 pa（addr 类型，4字节），(long *)pb 取 pa 的值，再用 (int *) 解引用 */
    printf("a=%d via double ptr=%d\n", a, (int *)(long *)pb);

    printf("\nDone.\n");
}
