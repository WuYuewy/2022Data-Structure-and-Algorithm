#include <stdio.h>
#include <map>
#include <stack>
#include <string.h>
using namespace std;

typedef struct {
    char data;
    int weight; // Ҷ��Ȩֵ������
    int parent, lchild, rchild; // ���澲̬����
} HTNode, * HuffmanTree;

const unsigned int MAX_WEIGHT = 100000;
map<char, int> cmap;

// ͳ���ַ�
void StatisChar(char* FILENAME) {
    int count = 0;
    char ch;
    FILE* fp = fopen(FILENAME, "r");
    if (fp == NULL)
    {
        printf("Error: can't open %s", FILENAME);
        return;
    }

    // �����ļ�
    while ((ch = fgetc(fp)) != EOF)
    {
        if (cmap.find(ch) == cmap.end())
            cmap[ch] = 1;
        else
            cmap[ch]++;
        count++;
    }
    fclose(fp);

    //������
    // for (map<char, int>::iterator it = cmap.begin();it != cmap.end();it++) {
    //     printf("�ַ�%c���ִ���: %d\n", it->first, it->second);
    // }
}

// �������������Ǵ�HT[0..s]���ҵ�Ȩֵ��С���������
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

// ������������
void CreateHuffmanTree(HuffmanTree& HT, int n) {
    int s1, s2;
    int m = 2 * n - 1; // ���ս��õ�2n-1�����
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

// ����ջ
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

// ���뺯��
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

// ����������
void HUffmanCoding(HuffmanTree& HT, char**& HC, int n) {
    stack<char> S;
    HC = (char**)malloc(sizeof(char*) * (n + 1));
    Coding(HT, HC, 2 * n - 2, S);
}

// ��ӡ����
void PrintCode(HuffmanTree& HT, char** HC, int n) {
    for (int i = 0;i < n;i++) {
        printf("%c�ַ�����%d������������Ϊ: %s\n", HT[i].data, HT[i].weight, HC[i]);
    }
}

// ��ӡ������
void PrintTree(HuffmanTree& HT, int n, int type, int level)
{
	int i;

	if (-1 == n)
        return;

    // �ݹ�
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

// �����ļ�
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

    // �����ļ�
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

// �����ļ�
void HuffmanDecode(HuffmanTree& HT, char** HC, int n, char* FILENAME) {
    char ch;
    char temp[20];
    int flag = 0, flag2 = 0;    // �ж�ƥ��ɹ�

    FILE* fp = fopen(FILENAME, "r");
    FILE* fp2 = fopen("decode.txt", "w");
    FILE* fp3 = fp;
    if (fp == NULL)
    {
        printf("Error: can't open %s", FILENAME);
        return;
    }

    // �����ļ�
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

    printf("�ļ�������ɣ�����decode.txt\n");
    fclose(fp);
    fclose(fp2);
}

int main() {
    StatisChar("stdio.h");
    int Bytes1 = 18024;
    HuffmanTree HT = NULL;
    char** HC;
    int n = cmap.size();

    // �����ӡ��������
    CreateHuffmanTree(HT, n);
    HUffmanCoding(HT, HC, n);
    PrintCode(HT, HC, n);
    printf("\n############\n");
    printf("����Ĺ���������\n");
    PrintTree(HT, 2 * n - 2, 0, 0);

    // �����ļ�
    int count2 = HuffmanFile(HT, HC, n, "stdio.txt");
    int Bytes2 = count2 / 8;
    double ratio = 100.0 * Bytes2 / Bytes1;
    printf("\n############\n");
    printf("ԭ�ļ��ֽ���Ϊ: 18024\n������ֽ���Ϊ: %d\nѹ����Ϊ: %.2lf%%.\n", Bytes2, ratio);

    // �����ļ�
    HuffmanDecode(HT, HC, n, "stdio.txt");

    return 0;
}
