#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct TrieNode {
    struct TrieNode *children[10]; // 对应数字 0~9
} TrieNode;

TrieNode *create_node(void)
{
    TrieNode *node = malloc(sizeof(struct TrieNode));
    for (int i = 0; i < 10; i++) {
        node->children[i] = NULL;
    }
    return node;
}

// 十叉树
void insert_node(TrieNode *root, char *s)
{
    int idx = 0;
    TrieNode *cur = root;

    for (int i = 0; s[i]; i++) {
        idx = s[i] - '0';
        if (cur->children[idx] == NULL) {
            cur->children[idx] = create_node();
        }
        cur = cur->children[idx];
    }
}

int find_node(TrieNode *root, char *s)
{
    int idx = 0;
    int len = 0;
    TrieNode *cur = root;

    for (int i = 0; s[i]; i++) {
        idx = s[i] - '0';
        if (cur->children[idx]) {
            len++;
            cur = cur->children[idx];
        } else {
            return len;
        }
    }

    return len;
}

int longestCommonPrefix(int *arr1, int arr1Size, int *arr2, int arr2Size)
{
    char s[20];
    int max = 0;
    TrieNode *root = NULL;

    root = create_node();

    // 将 arr1 的元素插入到 Trie 中
    for (int i = 0; i < arr1Size; i++) {
        sprintf(s, "%d", arr1[i]);
        insert_node(root, s);
    }

    // 拿 arr2 的元素从 Trie 中查找并获取到所匹配的长度
    for (int i = 0; i < arr2Size; i++) {
        int temp = 0;
        sprintf(s, "%d", arr2[i]);
        temp = find_node(root, s);
        max = max > temp ? max : temp;
    }

    return max;
}

int main(void)
{
    int arr1[] = {10};
    int arr2[] = {17, 11};

    int arr1Size = sizeof(arr1) / sizeof(arr1[0]);
    int arr2Size = sizeof(arr2) / sizeof(arr2[0]);

    int res = longestCommonPrefix(arr1, arr1Size, arr2, arr2Size);
    printf("res: %d\n", res);

    return 0;
}
