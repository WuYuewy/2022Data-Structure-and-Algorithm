// ��һ������ͷ���ĵ�������д�С����Ĳ�������

// ������������ݽṹ�Ķ��壬�Լ�һЩ���������Ͳ��Ժ������������Ҫ�����Ӧ�ĺ���
// ��д��ɺ󣬿���ʹ��main��������

#include <stdio.h>
#include <stdlib.h>

typedef int ElemType;
typedef struct LNode {
    ElemType data;
    LNode* next;
} *LinkList;

// ��Ҫ��ɵĺ���
void InsertSort(LinkList &head) {
}

// ������һ�����������������ǲ���һ������ΪN�ĵ��������е������������
LinkList RandomNumberLinkList(int N) {
    // �ȴ���һ��ͷ��㣬ͷ��㲻������
    LNode* h = new LNode;
    LNode* p = h;
    for (int i = 0; i < N; ++i) {
        // ���δ���N�����ݽ��
        LNode* t = new LNode;
        t->data = rand() % 1000;
        p->next = t; p = t;
    }
    p->next = NULL;
    return h;
}

// ������һ�����������������Ǵ�ӡһ��������
void PrintLinkList(LinkList head) {
    // ͷ��㲻�����ݣ�����
    LNode* p = head->next;
    while (p) {
        // ���δ�ӡÿ������
        printf("%d", p->data);
        if (p->next) {
            printf("->");
        }
        p = p->next;
    }
}

void InsSort(LinkList head) {
    int i,j;
    for (i = 1;i <= L.len;i++) {
        if (L.r[i] > L.r[i - 1])continue;
        L.r[0] = L.r[i];
        for (j = i - 1;L.r[0] < L.r[j];j--)
            L.r[j + 1] = L.r[j];
        L.r[j + 1] = L.r[0];
    }
}

// �����������������ڲ���
int main() {
    int N = 10;
    LinkList head = RandomNumberLinkList(N);
    InsertSort(head);
    PrintLinkList(head);
    return EXIT_SUCCESS;
}
