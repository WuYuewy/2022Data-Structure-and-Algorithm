#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

typedef string VertexType;
typedef int ArcType;

/* �ڽӾ����ʾ�� */

const int MAX_VERTEX_NUM = 25;
const int INFINITY = 0x7fffff;

// ͼ�����ͣ�����ͼ��������������ͼ��������
typedef enum { DG, DN, UDG, UDN } GraphKind;

typedef struct {
    // VertexType��ʾ��������ͣ�����char��int����
    VertexType vexs[MAX_VERTEX_NUM];
    // ArcType��ʾ�ߺ�Ȩֵ������
    // ��Ϊ��Ȩͼ����ȡbool��
    // ��Ϊ������ȡint�͡�float�͡���
    ArcType arcs[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
    int vexnum, arcnum;
    GraphKind kind;
} MGraph;

void CreateUDN(MGraph& G); // ����������
int LocateVex(MGraph G, VertexType v); // ���Ҷ���
void showGraph(MGraph G); // ��ʾͼ
void Dijkstra(MGraph G, int v0, int D[MAX_VERTEX_NUM], int P[][MAX_VERTEX_NUM]); // Dijkstra�����·��
void ShortestWay(MGraph G, int v0, int v1, int D[MAX_VERTEX_NUM], int P[][MAX_VERTEX_NUM]); // �����·��
void DeleteCity(MGraph& G, VertexType v); // ɾ������

/* �ڽӾ����ʾ�� */
void CreateUDN(MGraph& G) {
    G.kind = UDN; // UDN��ʾ������
    G.vexnum = 0;
    G.arcnum = 0;
    int i = 0;
    int vi, vj;

    // ��ȡ����city
    ifstream fin;
    fin.open("city.txt", ios::in);
    if (!fin.is_open())
    {
        cout << "�޷��ҵ�����ļ���" << endl;
        return;
    }
    string buff;
    while (getline(fin, buff))          // ����getline�ָ��ո�
    {
        istringstream str(buff);
        string out;
        while (str >> out) {
            if (i % 2 == 1) {
                G.vexs[G.vexnum] = out;
            }
            i++;
        }
        G.vexnum++;
    }
    fin.close();

    // ����ͨ����Ϊ����
    for (vi = 0; vi < G.vexnum; ++vi)
        for (vj = 0; vj < G.vexnum; ++vj)
            G.arcs[vi][vj] = INFINITY; // ���磬INFINITY=INT_MAX

    // ��ȡ��dist
    ifstream fin2;
    fin2.open("dist.txt", ios::in);
    if (!fin2.is_open())
    {
        cout << "�޷��ҵ�����ļ���" << endl;
        return;
    }
    while (getline(fin2, buff))         // ����getline�ָ��ո�
    {
        istringstream str(buff);
        string out;
        while (str >> out) {
            if (i % 3 == 0) {
                vi = stoi(out);
            }
            else if (i % 3 == 2) {
                vj = stoi(out);
            }
            else {
                G.arcs[vi][vj] = G.arcs[vj][vi] = stoi(out);
            }
            i++;
        }
        G.arcnum++;
    }
    fin2.close();
}

int LocateVex(MGraph G, VertexType v) {
    for (int i = 0; i < G.vexnum; ++i)
        if (G.vexs[i] == v) return i;
    return -1;
}

void showGraph(MGraph G) {
    /* ��ӡ�к� */
    cout << "\t|\t";
    for (int i = 0; i < G.vexnum; i++) cout << G.vexs[i] << "\t";
    cout << endl;
    /* ��ӡ�ָ��� */
    cout << "-------\t|------\t";
    for (int i = 0; i < G.vexnum; i++) cout << "-------" << "\t";
    cout << endl;
    /* �ֱ��ӡÿһ�� */
    for (int i = 0; i < G.vexnum; i++) {
        cout << G.vexs[i] << "\t|\t";
        for (int j = 0; j < G.vexnum; j++) {
            if (G.arcs[i][j] >= INFINITY / 2) cout << "INF\t";
            else cout << G.arcs[i][j] << "\t";
        }
        cout << endl;
    }
}

// Dijkstra�����·��
void Dijkstra(MGraph G, int v0, int D[MAX_VERTEX_NUM], int P[][MAX_VERTEX_NUM]) {
    int S[MAX_VERTEX_NUM] = { 0 };
    for (int i = 0;i < G.vexnum;i++) {          //��ʼ��
        D[i] = G.arcs[v0][i];
        if (D[i] != INFINITY) {
            P[i][0] = v0;P[i][1] = i;P[i][2] = -1;  // -1Ϊ��β
        }
    }
    S[v0] = 1;
    D[v0] = 0;
    int k, w;
    for (int i = 1;i < G.vexnum;i++) {
        int min = INFINITY;
        for (int j = 0;j < G.vexnum;j++) {      // �����·��
            if (!S[j] && D[j] < min) {
                min = D[i];
                k = j;
            }
        }
        S[k] = 1;
        for (int j = 0;j < G.vexnum;j++) {      // �����������·��
            if (!S[j] && D[k] + G.arcs[k][j] < D[j]) {
                D[j] = D[k] + G.arcs[k][j];
                for (w = 0;P[k][w] != -1;w++)
                    P[j][w] = P[k][w];
                P[j][w] = j;
                P[j][w + 1] = -1;
            }
        }
    }
}

// �����·��
void ShortestWay(MGraph G, int v0, int v1, int D[MAX_VERTEX_NUM], int P[][MAX_VERTEX_NUM]) {
    cout << G.vexs[v0] << "��" << G.vexs[v1] << "�����·��Ϊ��" << endl;
    cout << G.vexs[v0];
    for (int i = 1;P[v1][i] != -1;i++) {
        cout << "->" << G.vexs[P[v1][i]];
    }
    cout << endl << "����Ϊ" << D[v1];
}

// ɾ������
void DeleteCity(MGraph& G, VertexType v) {
    int v0 = LocateVex(G, v);
    for (int i = 0;i < G.vexnum;i++) {      // ȫ����ΪINF ��Ϊ�Ͽ�
        G.arcs[v0][i] = G.arcs[i][v0] = INFINITY;
    }
}

int main() {
    MGraph G;
    int D[MAX_VERTEX_NUM], P[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
    CreateUDN(G);
    DeleteCity(G,"Changchun");
    // showGraph(G);
    // int v0 = LocateVex(G, "Shenyang");
    // int v1 = LocateVex(G, "Xi'an");
    // int v0 = LocateVex(G, "Huhehaote");
    // int v1 = LocateVex(G, "Chengdu");
    int v0 = LocateVex(G, "Shenyang");
    int v1 = LocateVex(G, "Haerbin");
    Dijkstra(G, v0, D, P);
    ShortestWay(G, v0, v1, D, P);
    return 0;
}

/*
���н����

1. δɾ��ʱ��

Shenyang��Xi'an�����·��Ϊ��
Shenyang->Tianjin->Beijing->Zhengzhou->Xi'an
����Ϊ2047

Huhehaote��Chengdu�����·��Ϊ��
Huhehaote->Lanzhou->Xi'an->Chengdu
����Ϊ2663

Shanghai��Wulumuqi�����·��Ϊ��
Shanghai->Xuzhou->Zhengzhou->Xi'an->Lanzhou->Wulumuqi
����Ϊ4079

2. ɾ��֣�ݺ�

Shenyang��Xi'an�����·��Ϊ��
Shenyang->Tianjin->Beijing->Huhehaote->Lanzhou->Xi'an
����Ϊ3330

Huhehaote��Chengdu�����·��Ϊ��
Huhehaote->Lanzhou->Xi'an->Chengdu
����Ϊ2663

Shanghai��Wulumuqi�����·��Ϊ��
Shanghai->Xuzhou->Tianjin->Beijing->Huhehaote->Lanzhou->Wulumuqi
����Ϊ5167

*/
