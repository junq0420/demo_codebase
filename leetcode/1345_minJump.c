#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 方法1: bucket + dfs
 * 利用 bucket 将所有元素值相同的下标放进同一 bucket 里,
 * 然后在用 dfs 搜索，每个元素节点的跳跃可分为3 种情况:
 * 向前跳1; 向后跳1; 同值跳跃
 * 缺点: 当 arr[i] 值域过大时，会爆内存。更适合 10e5 这种小数据
 */

int minJumps1(int *arr, int arrSize)
{
    if (arrSize <= 1)
        return 0;

    const int OFFSET = 100000;
    const int RANGE = 200001;

    int *cnt = calloc(RANGE, sizeof(int));
    for (int i = 0; i < arrSize; i++) {
        cnt[arr[i] + OFFSET]++;
    }

    // bucket[v] 对应元素值，bucket[v][i] 对应元素下标
    int **bucket = malloc(RANGE * sizeof(int *));
    int *bsize = calloc(RANGE, sizeof(int)); // 每个 bucket 的大小
    for (int i = 0; i < RANGE; i++) {
        if (cnt[i] > 0)
            bucket[i] = malloc(cnt[i] * sizeof(int));
    }

    // 填充 bucket
    for (int i = 0; i < arrSize; i++) {
        int v = arr[i] + OFFSET;
        bucket[v][bsize[v]++] = i;
    }

    // bfs
    int *q = malloc(arrSize * sizeof(int));
    bool *vis = calloc(arrSize, sizeof(bool)); // 元素是否被访问过
    bool *used = calloc(RANGE, sizeof(bool));  // 元素值是否已被作为跳板

    int hh = 0, tt = 0;
    q[tt++] = 0; // 下标0入队
    vis[0] = true;
    int step = 0;

    while (hh < tt) { // 当前元素可跳跃的方式入队
        int levelSize = tt - hh;
        for (int i = 0; i < levelSize; i++) {
            int cur = q[hh++];

            // 已到终点，返回 step
            if (cur == arrSize - 1)
                return step;

            // 向后跳
            if (cur + 1 < arrSize && !vis[cur + 1]) {
                vis[cur + 1] = true;
                q[tt++] = cur + 1;
            }

            // 向前跳
            if (cur - 1 >= 0 && !vis[cur - 1]) {
                vis[cur - 1] = true;
                q[tt++] = cur - 1;
            }

            // 同值跳跃
            int v = arr[cur] + OFFSET;
            if (!used[v]) {
                used[v] = true;
                for (int j = 0; j < bsize[v]; j++) {
                    int nxt = bucket[v][j];
                    if (!vis[nxt]) {
                        vis[nxt] = true;
                        q[tt++] = nxt;
                    }
                }
            }
        }
        step++;
    }
    return -1;
}

/* ***************** */

/* 方法2: uniq + linkedlist + dfs
 * 用 uniq + linkedlist 实现上面的 bucket，即一个值对应多个下标
 * 特点: 链表通过 array 来实现，不用定义相应 Node 结构体和 API
 *       但是理解起来不太容易
 * 缺点: 由于使用 qsort 排序，会拉高时间复杂度
 */

int cmp_int(const void *a, const void *b)
{
    int pa = *(const int *)a;
    int pb = *(const int *)b;
    return (pa > pb) - (pa < pb); // 避免溢出，等价 pa-pb
}

// 二分查找，在有序去重的 uniq[0..n-1] 中找到 x 的位置
int get_id(int *uniq, int n, int x)
{
    int l = 0, r = n - 1;
    while (l < r) {
        int mid = l + (r - l) / 2;
        if (uniq[mid] < x)
            l = mid + 1;
        else
            r = mid;
    }
    return l;
}

int minJumps2(int *arr, int arrSize)
{
    if (arrSize <= 1)
        return 0;

    int n = arrSize;

    // 1. 排序去重
    int *sorted = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        sorted[i] = arr[i];
    qsort(sorted, n, sizeof(int), cmp_int);

    int *uniq = malloc(n * sizeof(int));
    int m = 0;
    for (int i = 0; i < n; i++) {
        // 首项或相邻不等的元素
        if (i == 0 || sorted[i] != sorted[i - 1])
            uniq[m++] = sorted[i];
    }
    printf("soted and uniq\n");

    // 2. 建邻接表：相同值的原始下标串成链表
    // 使用数组 head, to, nxt 表示链表节点的 'idx', 'val', 'next'
    int *head = malloc(m * sizeof(int));
    for (int i = 0; i < m; i++)
        head[i] = -1;

    int *to = malloc(n * sizeof(int));  // 链表节点存储原始下标
    int *nxt = malloc(n * sizeof(int)); // 节点 next 指针
    int ecnt = 0;

    for (int i = 0; i < n; i++) {
        int id = get_id(uniq, m, arr[i]);
        to[ecnt] = i;         // 原始下标 i
        nxt[ecnt] = head[id]; // head[id] = -1
        head[id] = ecnt++;
    }
    printf("created linked list\n");

    // 3. bfs
    int *q = malloc(n * sizeof(int));
    bool *vis = calloc(n, sizeof(bool));
    bool *used = calloc(m, sizeof(bool));

    int hh = 0, tt = 0;
    q[tt++] = 0;
    vis[0] = true;
    int step = 0;

    while (hh < tt) {
        int levelSize = tt - hh;
        for (int i = 0; i < levelSize; i++) {
            int cur = q[hh++];
            printf("cur = %d, v: %d\n", cur, arr[cur]);

            if (cur == n - 1) // 到达终点，返回 step
                return step;

            if (cur + 1 < n && !vis[cur + 1]) { // 向后跳
                vis[cur + 1] = true;
                q[tt++] = cur + 1;
            }

            if (cur - 1 >= 0 && !vis[cur - 1]) { // 向前跳
                vis[cur - 1] = true;
                q[tt++] = cur - 1;
            }

            // 同值跳
            int id = get_id(uniq, m, arr[cur]);
            if (!used[id]) {
                used[id] = true;

                for (int e = head[id]; e != -1; e = nxt[e]) {
                    int v = to[e];
                    if (!vis[v]) {
                        vis[v] = true;
                        q[tt++] = v;
                    }
                }
            }
        }
        step++;
    }
    return -1;
}

/* ***************** */

/* 方法3: uniq + linkedlist + dfs
 * 用 uniq + linkedlist 实现上面的 bucket，即一个值对应多个下标
 * 特点: 链表通过 array 来实现，不用定义相应 Node 结构体和 API
 *       但是理解起来不太容易
 * 缺点: 由于使用 qsort 排序，会拉高时间复杂度
 */

int SIZE;

struct HASH {
    int num; // 元素值
    int loc; // 元素下标
    struct HASH *next;
};

void init_hash(struct HASH *hash)
{
    for (int i = 0; i < SIZE; i++)
        hash[i].next = NULL;
}
int hashed(int num) { return abs(num) % SIZE; }
void create_hash(struct HASH *hash, int num, int loc)
{
    int temp = hashed(num);
    struct HASH *p = malloc(sizeof(struct HASH));
    p->num = num;
    p->loc = loc;
    p->next = hash[temp].next;
    hash[temp].next = p;
}

struct NUMS {
    int num;
    struct NUMS *next;
};

void init_nums(struct NUMS *nums)
{
    for (int i = 0; i < SIZE; i++)
        nums[i].next = NULL;
}
void create_nums(struct NUMS *nums, int num)
{
    int temp = hashed(num);
    struct NUMS *p = malloc(sizeof(struct NUMS));
    p->num = num;
    p->next = nums[temp].next;
    nums[temp].next = p;
}
// 是否存在相同的 num
bool judge(struct NUMS *nums, int num)
{
    int temp = hashed(num);
    struct NUMS *p = nums[temp].next;
    while (p) {
        if (p->num == num)
            return true;
        p = p->next;
    }
    return false;
}

int minJumps3(int *arr, int arrSize)
{
    SIZE = arrSize;
    int vis[arrSize];
    memset(vis, 0, sizeof(vis));

    struct HASH hash[arrSize];
    init_hash(hash);

    // 用于同值跳跃的一次性维护
    struct NUMS nums[arrSize];
    init_nums(nums);

    for (int i = 0; i < arrSize; i++) {
        create_hash(hash, arr[i], i);
    }

    struct Queue {
        int num;
        int loc;
        int step;
    } queue[arrSize + 1];
    int front = 0;
    int rear = 1;

    queue[front].num = arr[0];
    queue[front].loc = 0;
    queue[front].step = 0;
    vis[0] = 1;

    while (front < rear) {
        if (queue[front].loc == arrSize - 1)
            break;

        struct Queue ele = queue[front++];

        if (ele.loc + 1 < arrSize && !vis[ele.loc + 1]) {
            vis[ele.loc + 1] = 1;
            queue[rear].loc = ele.loc + 1;
            queue[rear].num = arr[ele.loc + 1];
            queue[rear].step = ele.step + 1;
            rear++;
        }

        if (ele.loc - 1 >= 0 && !vis[ele.loc - 1]) {
            vis[ele.loc - 1] = 1;
            queue[rear].loc = ele.loc - 1;
            queue[rear].num = arr[ele.loc - 1];
            queue[rear].step = ele.step + 1;
            rear++;
        }

        // 若 ele.num 已经存在说明之前同值已经遍历过，后续直接跳过
        if (judge(nums, ele.num))
            continue;

        // 无同值出现，添加到 nums 中，首次遍历同值情况
        create_nums(nums, ele.num);
        struct HASH *p = hash[hashed(ele.num)].next;
        while (p) {
            if (p->num == ele.num && !vis[p->loc]) {
                vis[p->loc] = 1;
                queue[rear].loc = p->loc;
                queue[rear].num = p->num;
                queue[rear].step = ele.step + 1;
                rear++;
            }
            p = p->next;
        }
    }
    return queue[front].step;
}

int minJumps4(int *arr, int arrSize)
{
    if (arrSize == 1)
        return 0;

    int i, j, k;
    SIZE = arrSize;

    int vis[arrSize];
    memset(vis, 0, sizeof(vis));

    int opvis[arrSize];
    memset(opvis, 0, sizeof(opvis));

    struct HASH hash[SIZE];
    init_hash(hash);

    struct NUMS nums[SIZE];
    struct NUMS opnums[SIZE];
    init_nums(nums);
    init_nums(opnums);
    for (int i = 0; i < arrSize; i++)
        create_hash(hash, arr[i], i);

    struct Queue {
        int num;
        int loc;
        int step;
    };
    struct Queue queue[arrSize];
    struct Queue opqueue[arrSize];

    int front = 0;
    int rear = 1;
    queue[front].num = arr[0];
    queue[front].loc = 0;
    queue[front].step = 0;
    vis[0] = 1;

    int opfront = 0;
    int oprear = 1;
    opqueue[opfront].num = arr[arrSize - 1];
    opqueue[opfront].loc = arrSize - 1;
    opqueue[opfront].step = 0;
    opvis[arrSize - 1] = 1;

    int temp;
    int optemp;
    while (1) {
        struct Queue ele = queue[front];
        front++;
        if (ele.loc + 1 < arrSize && !vis[ele.loc + 1]) {
            if (opvis[ele.loc + 1]) {
                for (k = rear - 1; k >= 0; k--)
                    if (opqueue[k].loc == ele.loc + 1)
                        return opqueue[k].step + 1 + ele.step;
            }
            vis[ele.loc + 1] = 1;
            queue[rear].loc = ele.loc + 1;
            queue[rear].num = arr[ele.loc + 1];
            queue[rear].step = ele.step + 1;
            rear++;
        }

        if (ele.loc - 1 >= 0 && !vis[ele.loc - 1]) {
            if (opvis[ele.loc - 1]) {
                for (k = rear - 1; k >= 0; k--)
                    if (opqueue[k].loc == ele.loc - 1)
                        return opqueue[k].step + 1 + ele.step;
            }
            vis[ele.loc - 1] = 1;
            queue[rear].loc = ele.loc - 1;
            queue[rear].num = arr[ele.loc - 1];
            queue[rear].step = ele.step + 1;
            rear++;
        }

        if (judge(nums, ele.num) == true)
            continue;

        create_nums(nums, ele.num);
        struct HASH *p = hash[hashed(ele.num)].next;
        while (p) {
            if (p->num == ele.num && !vis[p->loc]) {
                if (opvis[p->loc]) {
                    for (k = rear - 1; k >= 0; k--)
                        if (opqueue[k].loc == p->loc)
                            return opqueue[k].step + 1 + ele.step;
                }
                vis[p->loc] = 1;
                queue[rear].loc = p->loc;
                queue[rear].num = p->num;
                queue[rear].step = ele.step + 1;
                rear++;
            }
            p = p->next;
        }
        struct Queue opele = opqueue[opfront];
        opfront++;
        if (opele.loc + 1 < arrSize && !opvis[opele.loc + 1]) {
            if (vis[opele.loc + 1]) {
                for (k = rear - 1; k >= 0; k--)
                    if (queue[k].loc == opele.loc + 1)
                        return queue[k].step + 1 + opele.step;
            }
            opvis[opele.loc + 1] = 1;
            opqueue[oprear].loc = opele.loc + 1;
            opqueue[oprear].num = arr[opele.loc + 1];
            opqueue[oprear].step = opele.step + 1;
            oprear++;
        }
        if (opele.loc - 1 >= 0 && !opvis[opele.loc - 1]) {
            if (vis[opele.loc - 1]) {
                for (k = rear - 1; k >= 0; k--)
                    if (queue[k].loc == opele.loc - 1)
                        return queue[k].step + 1 + opele.step;
            }
            opvis[opele.loc - 1] = 1;
            opqueue[oprear].loc = opele.loc - 1;
            opqueue[oprear].num = arr[opele.loc - 1];
            opqueue[oprear].step = opele.step + 1;
            oprear++;
        }
        if (judge(opnums, opele.num) == true)
            continue;
        create_nums(opnums, opele.num);
        struct HASH *q = hash[hashed(opele.num)].next;
        while (q) {
            if (q->num == opele.num && !opvis[q->loc]) {
                if (vis[q->loc]) {
                    for (k = rear - 1; k >= 0; k--)
                        if (queue[k].loc == q->loc)
                            return queue[k].step + 1 + opele.step;
                }
                opvis[q->loc] = 1;
                opqueue[oprear].loc = q->loc;
                opqueue[oprear].num = q->num;
                opqueue[oprear].step = opele.step + 1;
                oprear++;
            }
            q = q->next;
        }
    }
    return 0;
}

int main(void)
{
    int arr[] = {100, -23, -23, 404, 100, 23, 23, 23, 3, 404};
    int arrSize = sizeof(arr) / sizeof(arr[0]);

    printf("res: %d\n", minJumps1(arr, arrSize));
    printf("res: %d\n", minJumps2(arr, arrSize));
    printf("res: %d\n", minJumps3(arr, arrSize));
    printf("res: %d\n", minJumps4(arr, arrSize));

    return 0;
}
