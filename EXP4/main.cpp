#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

typedef string VertexType;
typedef int ArcType;

/* 邻接矩阵表示法 */

const int MAX_VERTEX_NUM = 25;
const int INFINITY = 0x7fffff;

// 图的类型：有向图、有向网、无向图、无向网
typedef enum { DG, DN, UDG, UDN } GraphKind;

typedef struct {
    // VertexType表示顶点的类型，例如char、int……
    VertexType vexs[MAX_VERTEX_NUM];
    // ArcType表示边和权值的类型
    // 若为无权图，可取bool型
    // 若为网，可取int型、float型……
    ArcType arcs[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
    int vexnum, arcnum;
    GraphKind kind;
} MGraph;

void CreateUDN(MGraph& G); // 构造无向网
int LocateVex(MGraph G, VertexType v); // 查找顶点
void showGraph(MGraph G); // 显示图
void Dijkstra(MGraph G, int v0, int D[MAX_VERTEX_NUM], int P[][MAX_VERTEX_NUM]); // Dijkstra求最短路径
void ShortestWay(MGraph G, int v0, int v1, int D[MAX_VERTEX_NUM], int P[][MAX_VERTEX_NUM]); // 找最短路径
void DeleteCity(MGraph& G, VertexType v); // 删除城市

/* 邻接矩阵表示法 */
void CreateUDN(MGraph& G) {
    G.kind = UDN; // UDN表示无向网
    G.vexnum = 0;
    G.arcnum = 0;
    int i = 0;
    int vi, vj;

    // 读取顶点city
    ifstream fin;
    fin.open("city.txt", ios::in);
    if (!fin.is_open())
    {
        cout << "无法找到这个文件！" << endl;
        return;
    }
    string buff;
    while (getline(fin, buff))          // 利用getline分隔空格
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

    // 不连通边设为无穷
    for (vi = 0; vi < G.vexnum; ++vi)
        for (vj = 0; vj < G.vexnum; ++vj)
            G.arcs[vi][vj] = INFINITY; // 例如，INFINITY=INT_MAX

    // 读取边dist
    ifstream fin2;
    fin2.open("dist.txt", ios::in);
    if (!fin2.is_open())
    {
        cout << "无法找到这个文件！" << endl;
        return;
    }
    while (getline(fin2, buff))         // 利用getline分隔空格
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
    /* 打印行号 */
    cout << "\t|\t";
    for (int i = 0; i < G.vexnum; i++) cout << G.vexs[i] << "\t";
    cout << endl;
    /* 打印分隔行 */
    cout << "-------\t|------\t";
    for (int i = 0; i < G.vexnum; i++) cout << "-------" << "\t";
    cout << endl;
    /* 分别打印每一行 */
    for (int i = 0; i < G.vexnum; i++) {
        cout << G.vexs[i] << "\t|\t";
        for (int j = 0; j < G.vexnum; j++) {
            if (G.arcs[i][j] >= INFINITY / 2) cout << "INF\t";
            else cout << G.arcs[i][j] << "\t";
        }
        cout << endl;
    }
}

// Dijkstra求最短路径
void Dijkstra(MGraph G, int v0, int D[MAX_VERTEX_NUM], int P[][MAX_VERTEX_NUM]) {
    int S[MAX_VERTEX_NUM] = { 0 };
    for (int i = 0;i < G.vexnum;i++) {          //初始化
        D[i] = G.arcs[v0][i];
        if (D[i] != INFINITY) {
            P[i][0] = v0;P[i][1] = i;P[i][2] = -1;  // -1为结尾
        }
    }
    S[v0] = 1;
    D[v0] = 0;
    int k, w;
    for (int i = 1;i < G.vexnum;i++) {
        int min = INFINITY;
        for (int j = 0;j < G.vexnum;j++) {      // 找最短路径
            if (!S[j] && D[j] < min) {
                min = D[i];
                k = j;
            }
        }
        S[k] = 1;
        for (int j = 0;j < G.vexnum;j++) {      // 修正其他最短路径
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

// 找最短路径
void ShortestWay(MGraph G, int v0, int v1, int D[MAX_VERTEX_NUM], int P[][MAX_VERTEX_NUM]) {
    cout << G.vexs[v0] << "到" << G.vexs[v1] << "的最短路径为：" << endl;
    cout << G.vexs[v0];
    for (int i = 1;P[v1][i] != -1;i++) {
        cout << "->" << G.vexs[P[v1][i]];
    }
    cout << endl << "长度为" << D[v1];
}

// 删除城市
void DeleteCity(MGraph& G, VertexType v) {
    int v0 = LocateVex(G, v);
    for (int i = 0;i < G.vexnum;i++) {      // 全部设为INF 则为断开
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
运行结果：

1. 未删除时：

Shenyang到Xi'an的最短路径为：
Shenyang->Tianjin->Beijing->Zhengzhou->Xi'an
长度为2047

Huhehaote到Chengdu的最短路径为：
Huhehaote->Lanzhou->Xi'an->Chengdu
长度为2663

Shanghai到Wulumuqi的最短路径为：
Shanghai->Xuzhou->Zhengzhou->Xi'an->Lanzhou->Wulumuqi
长度为4079

2. 删除郑州后：

Shenyang到Xi'an的最短路径为：
Shenyang->Tianjin->Beijing->Huhehaote->Lanzhou->Xi'an
长度为3330

Huhehaote到Chengdu的最短路径为：
Huhehaote->Lanzhou->Xi'an->Chengdu
长度为2663

Shanghai到Wulumuqi的最短路径为：
Shanghai->Xuzhou->Tianjin->Beijing->Huhehaote->Lanzhou->Wulumuqi
长度为5167

*/
