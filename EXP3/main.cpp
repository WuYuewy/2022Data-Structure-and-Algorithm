#include <stdio.h>
#include <map>
#include <stack>
#include <string.h>
using namespace std;

typedef struct {
    char data;
    int weight; // 叶子权值的类型
    int parent, lchild, rchild; // 三叉静态链表
} HTNode, * HuffmanTree;

const unsigned int MAX_WEIGHT = 100000;
map<char, int> cmap;

// 统计字符
void StatisChar(char* FILENAME) {
    int count = 0;
    char ch;
    FILE* fp = fopen(FILENAME, "r");
    if (fp == NULL)
    {
        printf("Error: can't open %s", FILENAME);
        return;
    }

    // 遍历文件
    while ((ch = fgetc(fp)) != EOF)
    {
        if (cmap.find(ch) == cmap.end())
            cmap[ch] = 1;
        else
            cmap[ch]++;
        count++;
    }
    fclose(fp);

    //输出结果
    // for (map<char, int>::iterator it = cmap.begin();it != cmap.end();it++) {
    //     printf("字符%c出现次数: %d\n", it->first, it->second);
    // }
}

// 本函数的作用是从HT[0..s]中找到权值最小的两个结点
void Select(HuffmanTree HT, int s, int& l, int& r) {
    int WL = MAX_WEIGHT, WR = MAX_WEIGHT;
    for (int i = 0; i <= s; ++i) {
        if (HT[i].parent == -1) {
            if (HT[i].weight < WL) {
                WR = WL; WL = HT[i].weight; r = l; l = i;
            }
            else if (HT[i].weight < WR) {
                WR = HT[i].weight; r = i;
            }
        }
    }
}

// 创建哈夫曼树
void CreateHuffmanTree(HuffmanTree& HT, int n) {
    int s1, s2;
    int m = 2 * n - 1; // 最终将得到2n-1个结点
    HT = new HTNode[m];
    int i = 0;
    for (map<char, int>::iterator it = cmap.begin();it != cmap.end();it++, i++) {
        HT[i].data = it->first;
        HT[i].weight = it->second;
        HT[i].lchild = HT[i].rchild = HT[i].parent = -1;
    }
    for (int i = n; i < m; ++i) {
        HT[i].data = '#';
        Select(HT, i - 1, s1, s2); ; HT[s1].parent = HT[s2].parent = i;
        HT[i].weight = HT[s1].weight + HT[s2].weight;
        HT[i].lchild = s1; HT[i].rchild = s2; HT[i].parent = -1;
    }
}

// 遍历栈
void StackTraverse(char* code, stack<char> S) {
    int n = S.size();
    stack<char> S2;
    while (!S.empty()) {
        char ch = S.top();
        S.pop();
        S2.push(ch);
    }


    for (int i = 0;i < n;i++) {
        char ch = S2.top();
        code[i] = ch;
        S2.pop();
        S.push(ch);
    }
}

// 编码函数
void Coding(HuffmanTree& HT, char** HC, int root, stack<char> S) {
    if (root != -1) {
        if (HT[root].lchild == -1) {
            S.push('\0');
            HC[root] = (char*)malloc(S.size());
            StackTraverse(HC[root], S);
            S.pop();
        }

        S.push('0');
        Coding(HT, HC, HT[root].lchild, S);
        S.pop();
        S.push('1');
        Coding(HT, HC, HT[root].rchild, S);
        S.pop();
    }
}

// 哈弗曼编码
void HUffmanCoding(HuffmanTree& HT, char**& HC, int n) {
    stack<char> S;
    HC = (char**)malloc(sizeof(char*) * (n + 1));
    Coding(HT, HC, 2 * n - 2, S);
}

// 打印编码
void PrintCode(HuffmanTree& HT, char** HC, int n) {
    for (int i = 0;i < n;i++) {
        printf("%c字符个数%d，哈夫曼编码为: %s\n", HT[i].data, HT[i].weight, HC[i]);
    }
}

// 打印二叉树
void PrintTree(HuffmanTree& HT, int n, int type, int level)
{
	int i;

	if (-1 == n)
        return;

    // 递归
    PrintTree(HT, HT[n].rchild, 2, level + 1);
	switch (type)
	{
	case 0:
		printf("|%2c|\n", HT[n].data);
		break;
	case 1:
		for (i = 0; i < level; i++)
			printf("\t");
		printf("\\ |%2c|\n", HT[n].data);
		break;
	case 2:
		for (i = 0; i < level; i++)
			printf("\t");
		printf("/ |%2c|\n", HT[n].data);
		break;
	}
	PrintTree(HT,HT[n].lchild, 1,  level+1);
}

// 编码文件
int HuffmanFile(HuffmanTree& HT, char** HC, int n, char* FILENAME) {
    char ch;
    FILE* fp1 = fopen("stdio.h", "r");
    FILE* fp2 = fopen(FILENAME, "w");
    int count = 0;
    if (fp2 == NULL)
    {
        printf("Error: can't create %s", FILENAME);
        return 0;
    }

    // 遍历文件
    while ((ch = fgetc(fp1)) != EOF)
    {
        for (int i = 0;i < n;i++) {
            if (ch == HT[i].data) {
                fputs(HC[i], fp2);
                count+=strlen(HC[i]);
            }
        }
    }
    fclose(fp1);
    fclose(fp2);
    return count;
}

// 解码文件
void HuffmanDecode(HuffmanTree& HT, char** HC, int n, char* FILENAME) {
    char ch;
    char temp[20];
    int flag = 0, flag2 = 0;    // 判断匹配成功

    FILE* fp = fopen(FILENAME, "r");
    FILE* fp2 = fopen("decode.txt", "w");
    FILE* fp3 = fp;
    if (fp == NULL)
    {
        printf("Error: can't open %s", FILENAME);
        return;
    }

    // 遍历文件
    do {
        fp3 = fp;
        for (int i = 1;i < 20;i++) {
            fgets(temp, i, fp3);
            if (strlen(temp) < i - 1) {
                flag2 = 1;
                break;
            }
            for (int j = 0;j < n;j++) {
                if (strcmp(HC[j], temp) == 0) {
                    fputc(HT[j].data, fp2);
                    flag = 1;
                    break;
                }
            }
            if (flag == 1) {
                flag = 0;
                break;
            }
            fseek(fp3, -i + 1, SEEK_CUR);
        }
        fseek(fp, -1, SEEK_CUR);
        if (flag2 == 1)
            break;
    } while ((ch = fgetc(fp)) != EOF);

    printf("文件解码完成，生成decode.txt\n");
    fclose(fp);
    fclose(fp2);
}

int main() {
    StatisChar("stdio.h");
    int Bytes1 = 18024;
    HuffmanTree HT = NULL;
    char** HC;
    int n = cmap.size();

    // 构造打印哈夫曼树
    CreateHuffmanTree(HT, n);
    HUffmanCoding(HT, HC, n);
    PrintCode(HT, HC, n);
    printf("\n############\n");
    printf("构造的哈夫曼树：\n");
    PrintTree(HT, 2 * n - 2, 0, 0);

    // 编码文件
    int count2 = HuffmanFile(HT, HC, n, "stdio.txt");
    int Bytes2 = count2 / 8;
    double ratio = 100.0 * Bytes2 / Bytes1;
    printf("\n############\n");
    printf("原文件字节数为: 18024\n编码后字节数为: %d\n压缩比为: %.2lf%%.\n", Bytes2, ratio);

    // 解码文件
    HuffmanDecode(HT, HC, n, "stdio.txt");

    return 0;
}
