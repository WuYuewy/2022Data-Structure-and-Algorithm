#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>

typedef struct {
  float coef; // 系数
  int expn;    // 指数
} ElemType;
typedef struct LNode {
  ElemType data;
  LNode* next;
} *Polynomial;

void PrintMenu()
{
  printf("************************************************\n");
  printf("\tpolynomial operator function menu\t\n");
  printf("************************************************\n");
  printf("0.exit\n");
  printf("1.Create a polynomial\n");
  printf("2.Print a polynomial\n");
  printf("3.Destroy a polynomial\n");
  printf("4.add two polynomial\n");
  printf("5.minus two polynomial\n");
  printf("6.multiply two polynomial\n");
  printf("7.mod two polynomial\n");
  printf("8.gcd and lcm of two polynomial\n");
  printf("9.Indefinite integral of a polynomial\n");
  printf("10.Definite integral of a polynomial on [a, b]\n");
  printf("************************************************\n");
}
void PrintPolyn(Polynomial head) {
  if (head->next == NULL || fabs(head->next->data.coef) < 1e-6) {
    printf("0\n");
    return;                               //空表
  }
  else {
    if (head->next->data.expn > 0) {
      printf("%.6fx^%d", head->next->data.coef, head->next->data.expn);
    }
    else {                               //第一个元素特殊处理
      printf("%.6f", head->next->data.coef);
    }
    head = head->next->next;
  }
  while (head != NULL) {                 //输出后续元素
    if (fabs(head->data.coef) > 1e-6) {
      if ((head->data.coef) > 0)
        printf("+");
      printf("%.6fx^%d", head->data.coef, head->data.expn);
    }
    head = head->next;
  }
  printf("\n");
}
void AddPolyn(Polynomial& La, Polynomial& Lb) {
  LNode* pa = La->next, * pb = Lb->next, * rc = La;
  while (pa && pb) {
    if (pa->data.expn < pb->data.expn) {             //小的指数插入
      rc->next = pa; rc = pa; pa = pa->next;
    }
    else if (pa->data.expn > pb->data.expn) {      
      rc->next = pb; rc = pb; pb = pb->next;
    }
    else {
      double sum = pa->data.coef + pb->data.coef;   //同指数合并
      if (fabs(sum) < 1e-6) {
        LNode* qa = pa; pa = pa->next; delete qa;
      }
      else {                                        //合并后插入
        pa->data.coef = sum;
        rc->next = pa; rc = pa; pa = pa->next;
      }
      LNode* qb = pb; pb = pb->next; delete qb;
    }
  }
  if (!pb) rc->next = pa;                           //插入后续元素
  else rc->next = pb;
  delete Lb;
}

void MinusPolyn(Polynomial& La, Polynomial& Lb) {
  Polynomial p = Lb->next;
  while (p != NULL) {
    p->data.coef = -p->data.coef;
    p = p->next;
  }
  AddPolyn(La, Lb);
}

Polynomial MutiplyPolyn(Polynomial& La, Polynomial& Lb) {
  Polynomial result, p, Lc;
  result = (Polynomial)malloc(sizeof(LNode));       //生成头结点
  result->next = NULL;                              //储存运算结果
  LNode* pa = La->next, * pb = Lb->next, * pc;
  while (pa != NULL) {
    Lc = (Polynomial)malloc(sizeof(LNode));         //生成头结点
    Lc->next = NULL;
    pc = Lc;                                        //单项式运算结果
    pb = Lb->next;
    while (pb != NULL) {
      p = (Polynomial)malloc(sizeof(LNode));        //单次运算结果
      p->next = NULL;
      p->data.coef = pa->data.coef * pb->data.coef;
      p->data.expn = pa->data.expn + pb->data.expn;
      pc->next = p;
      pc = p;                                       //结果插入pc
      pb = pb->next;
    }
    pc->next = NULL;
    AddPolyn(result, Lc);                           //pc累加
    pa = pa->next;
  }
  return result;                                    //输出结果
}
Polynomial CreatePolyn() {
  Polynomial head, p, tail;
  float x;                      //存储输入系数
  int   e;                      //存储输入指数

  head = (Polynomial)malloc(sizeof(LNode));
  head->next = NULL;            //生成头结点
  tail = head;
  printf("please input the parameter like coef,expn (the parameter expn should be increasing)\n");
  printf("(input coef=0 to stop):\n");
  scanf("%f,%d", &x, &e);
  while (x != 0)                //当x=0时，多项式输入结束
  {
    p = (Polynomial)malloc(sizeof(LNode));
    p->data.coef = x;
    p->data.expn = e;
    tail->next = p;             //表尾插入
    tail = p;
    scanf("%f,%d", &x, &e);	    //继续输入
  }
  tail->next = NULL;            //表结尾为NULL

  return head;
}

void DestroyPolyn(Polynomial& head) {
  Polynomial p, q;
  p = head->next;
  while (p != NULL) {
    q = p->next;                    //遍历删除
    delete p;
    p = q;
  }
  head->next = NULL;
}

int Expn(Polynomial La) {           //获得最高次指数
  Polynomial p = La->next;
  int expn = 0;
  while (p != NULL) {
    if (fabs(p->data.coef) > 1e-6) {
      expn = p->data.expn;
    }
    p = p->next;
  }
  return expn;
}
double Coef(Polynomial La) {        //获得最高次系数
  Polynomial p = La->next;
  double coef = 0;
  while (p != NULL) {
    if (fabs(p->data.coef) > 1e-6) {
      coef = p->data.coef;
    }
    p = p->next;
  }
  return coef;
}
void ModPolyn(Polynomial& La, Polynomial& Lb, Polynomial& Quo, Polynomial& rem) {
  if (Lb->next == NULL) {
    printf("error: can't mod 0!\n");        //除0错误
    return;
  }
  int expa, expb;
  LNode* p, * q;
  Quo = (Polynomial)malloc(sizeof(LNode));
  Quo->next = NULL;                         //生成商头结点
  rem = (Polynomial)malloc(sizeof(LNode));
  rem->next = NULL;                     //生成余数头结点

  while (Expn(La) >= Expn(Lb)&&fabs(Coef(La))>1e-6) {
    p = (Polynomial)malloc(sizeof(LNode));
    p->next = NULL;                         //单次运算单项式头结点
    q = (Polynomial)malloc(sizeof(LNode));
    q->next = NULL;                         //单次乘法元
    q->data.expn = Expn(La) - Expn(Lb);
    q->data.coef = Coef(La) / Coef(Lb);
    p->next = q;
    Polynomial temp = MutiplyPolyn(Lb, p);  //短除法
    MinusPolyn(La, temp);
    AddPolyn(Quo, p);                       //商累加
  }
  AddPolyn(rem, La);
}

void CopyPolyn(Polynomial& La, Polynomial& Lb) {  //复制多项式
  Polynomial p, q;
  p = (Polynomial)malloc(sizeof(LNode));    //头结点
  p->next = NULL;
  Polynomial head = La->next, tail = p;
  while (head != NULL) {
    q = (Polynomial)malloc(sizeof(LNode));
    q->next = NULL;                         //归一化因子
    q->data.coef = head->data.coef;
    q->data.expn = head->data.expn;
    tail->next = q;
    tail = q;
    head = head->next;
  }
  Lb = p;
}
void GcdLcmPolyn(Polynomial& La, Polynomial& Lb, Polynomial& Gcd, Polynomial& Lcm) {
  Polynomial gcd , lcm;
  Polynomial Quo, rem, p, q, Acpy, Bcpy;
  CopyPolyn(La, Acpy);
  CopyPolyn(Lb, Bcpy);                    //备份AB

  while (Expn(Lb) != 0||fabs(Coef(Lb))>1e-6) {
    ModPolyn(La, Lb, Quo, rem);;
    La = Lb;                              //辗转相除法
    Lb = rem;
  }
  gcd=La;
  p = (Polynomial)malloc(sizeof(LNode));
  p->next = NULL;                         //创建归一化头结点
  q = (Polynomial)malloc(sizeof(LNode));
  q->next = NULL;                         //归一化因子
  q->data.expn = 0;
  q->data.coef = 1 / Coef(gcd);
  p->next = q;
  Gcd = MutiplyPolyn(gcd, p);             //归一化
  
  q->data.coef = 1 / (Coef(Acpy) * Coef(Bcpy));
  lcm = MutiplyPolyn(Acpy, Bcpy);
  lcm = MutiplyPolyn(lcm, p);             //归一化
  ModPolyn(lcm, Gcd, Quo, rem);           //最小公倍式
  Lcm = Quo;
}

void IndefPolyn(Polynomial& La) {
  Polynomial head = La->next;
  while (head != NULL) {
    head->data.expn++;
    head->data.coef /= head->data.expn;
    head = head->next;
  }
}
double Defpolyn(Polynomial& La,double a,double b) {
  IndefPolyn(La);
  double asum = 0, bsum = 0;
  Polynomial head = La->next;
  while (head != NULL) {
    asum += head->data.coef * pow(a, head->data.expn);
    bsum += head->data.coef * pow(b, head->data.expn);
    head = head->next;
  }
  return bsum - asum;
}
int main(int argc, char** argv) {
  int ctrl;
  float a, b;
  Polynomial head, La, Lb, quo, rem, gcd, lcm;
  while (1) {
    La = NULL;
    Lb = NULL;
    PrintMenu();
    printf("Please select a menu number:");
    scanf("%d", &ctrl);

    if (ctrl == 0)
      break;
    switch (ctrl)
    {
    case 1:
      head = CreatePolyn();
      break;
    case 2:
      PrintPolyn(head);
      break;
    case 3:
      DestroyPolyn(head);
      break;
    case 4:
      printf("input the first Polynomial:\n");
      La = CreatePolyn();
      printf("input the second Polynomial:\n");
      Lb = CreatePolyn();
      printf("Result:\n");
      PrintPolyn(La);
      printf("x\n");
      PrintPolyn(Lb);
      printf("is:");
      AddPolyn(La, Lb);
      PrintPolyn(La);
      break;
    case 5:
      printf("input the first Polynomial:\n");
      La = CreatePolyn();
      printf("input the second Polynomial:\n");
      Lb = CreatePolyn();
      printf("Result:\n");
      PrintPolyn(La);
      printf("-\n");
      PrintPolyn(Lb);
      printf("is:");
      MinusPolyn(La, Lb);
      PrintPolyn(La);
      break;
    case 6:
      printf("input the first Polynomial:\n");
      La = CreatePolyn();
      printf("input the second Polynomial:\n");
      Lb = CreatePolyn();
      printf("Result:\n");
      PrintPolyn(La);
      printf("x\n");
      PrintPolyn(Lb);
      printf("is:");
      PrintPolyn(MutiplyPolyn(La, Lb));
      break;
    case 7:
      printf("input the first Polynomial:\n");
      La = CreatePolyn();
      printf("input the second Polynomial:\n");
      Lb = CreatePolyn();
      printf("Result:\n");
      PrintPolyn(La);
      printf("mod\n");
      PrintPolyn(Lb);
      ModPolyn(La, Lb, quo, rem);
      printf("The Quotient is:\n");
      PrintPolyn(quo);
      printf("The Remainder is:\n");
      PrintPolyn(rem);
      break;
    case 8:
      printf("input the first Polynomial:\n");
      La = CreatePolyn();
      printf("input the second Polynomial:\n");
      Lb = CreatePolyn();
      PrintPolyn(La);
      PrintPolyn(Lb);
      printf("Result:\n");
      GcdLcmPolyn(La, Lb, gcd, lcm);
      printf("The Gcd is:\n");
      PrintPolyn(gcd);
      printf("The Lcm is:\n");
      PrintPolyn(lcm);
      break;
    case 9:
      printf("input the Polynomial:\n");
      La = CreatePolyn();
      printf("Result:\n");
      PrintPolyn(La);
      printf("Indefinite integral is:\n");
      IndefPolyn(La);
      PrintPolyn(La);
      break;
    case 10:
      printf("input the Polynomial:\n");
      La = CreatePolyn();
      printf("please input the interval like (a,b):\n");
      scanf("%f,%f", &a, &b);
      printf("Result:\n");
      PrintPolyn(La);
      printf("Definite integral between [a,b] is:\n");
      printf("%.6f", Defpolyn(La, a, b));
      break;
    default:
      break;
    }
    system("pause");
    system("cls");
  }
}
