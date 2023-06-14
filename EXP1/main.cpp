#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>

typedef struct {
  float coef; // ϵ��
  int expn;    // ָ��
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
    return;                               //�ձ�
  }
  else {
    if (head->next->data.expn > 0) {
      printf("%.6fx^%d", head->next->data.coef, head->next->data.expn);
    }
    else {                               //��һ��Ԫ�����⴦��
      printf("%.6f", head->next->data.coef);
    }
    head = head->next->next;
  }
  while (head != NULL) {                 //�������Ԫ��
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
    if (pa->data.expn < pb->data.expn) {             //С��ָ������
      rc->next = pa; rc = pa; pa = pa->next;
    }
    else if (pa->data.expn > pb->data.expn) {      
      rc->next = pb; rc = pb; pb = pb->next;
    }
    else {
      double sum = pa->data.coef + pb->data.coef;   //ָͬ���ϲ�
      if (fabs(sum) < 1e-6) {
        LNode* qa = pa; pa = pa->next; delete qa;
      }
      else {                                        //�ϲ������
        pa->data.coef = sum;
        rc->next = pa; rc = pa; pa = pa->next;
      }
      LNode* qb = pb; pb = pb->next; delete qb;
    }
  }
  if (!pb) rc->next = pa;                           //�������Ԫ��
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
  result = (Polynomial)malloc(sizeof(LNode));       //����ͷ���
  result->next = NULL;                              //����������
  LNode* pa = La->next, * pb = Lb->next, * pc;
  while (pa != NULL) {
    Lc = (Polynomial)malloc(sizeof(LNode));         //����ͷ���
    Lc->next = NULL;
    pc = Lc;                                        //����ʽ������
    pb = Lb->next;
    while (pb != NULL) {
      p = (Polynomial)malloc(sizeof(LNode));        //����������
      p->next = NULL;
      p->data.coef = pa->data.coef * pb->data.coef;
      p->data.expn = pa->data.expn + pb->data.expn;
      pc->next = p;
      pc = p;                                       //�������pc
      pb = pb->next;
    }
    pc->next = NULL;
    AddPolyn(result, Lc);                           //pc�ۼ�
    pa = pa->next;
  }
  return result;                                    //������
}
Polynomial CreatePolyn() {
  Polynomial head, p, tail;
  float x;                      //�洢����ϵ��
  int   e;                      //�洢����ָ��

  head = (Polynomial)malloc(sizeof(LNode));
  head->next = NULL;            //����ͷ���
  tail = head;
  printf("please input the parameter like coef,expn (the parameter expn should be increasing)\n");
  printf("(input coef=0 to stop):\n");
  scanf("%f,%d", &x, &e);
  while (x != 0)                //��x=0ʱ������ʽ�������
  {
    p = (Polynomial)malloc(sizeof(LNode));
    p->data.coef = x;
    p->data.expn = e;
    tail->next = p;             //��β����
    tail = p;
    scanf("%f,%d", &x, &e);	    //��������
  }
  tail->next = NULL;            //���βΪNULL

  return head;
}

void DestroyPolyn(Polynomial& head) {
  Polynomial p, q;
  p = head->next;
  while (p != NULL) {
    q = p->next;                    //����ɾ��
    delete p;
    p = q;
  }
  head->next = NULL;
}

int Expn(Polynomial La) {           //�����ߴ�ָ��
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
double Coef(Polynomial La) {        //�����ߴ�ϵ��
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
    printf("error: can't mod 0!\n");        //��0����
    return;
  }
  int expa, expb;
  LNode* p, * q;
  Quo = (Polynomial)malloc(sizeof(LNode));
  Quo->next = NULL;                         //������ͷ���
  rem = (Polynomial)malloc(sizeof(LNode));
  rem->next = NULL;                     //��������ͷ���

  while (Expn(La) >= Expn(Lb)&&fabs(Coef(La))>1e-6) {
    p = (Polynomial)malloc(sizeof(LNode));
    p->next = NULL;                         //�������㵥��ʽͷ���
    q = (Polynomial)malloc(sizeof(LNode));
    q->next = NULL;                         //���γ˷�Ԫ
    q->data.expn = Expn(La) - Expn(Lb);
    q->data.coef = Coef(La) / Coef(Lb);
    p->next = q;
    Polynomial temp = MutiplyPolyn(Lb, p);  //�̳���
    MinusPolyn(La, temp);
    AddPolyn(Quo, p);                       //���ۼ�
  }
  AddPolyn(rem, La);
}

void CopyPolyn(Polynomial& La, Polynomial& Lb) {  //���ƶ���ʽ
  Polynomial p, q;
  p = (Polynomial)malloc(sizeof(LNode));    //ͷ���
  p->next = NULL;
  Polynomial head = La->next, tail = p;
  while (head != NULL) {
    q = (Polynomial)malloc(sizeof(LNode));
    q->next = NULL;                         //��һ������
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
  CopyPolyn(Lb, Bcpy);                    //����AB

  while (Expn(Lb) != 0||fabs(Coef(Lb))>1e-6) {
    ModPolyn(La, Lb, Quo, rem);;
    La = Lb;                              //շת�����
    Lb = rem;
  }
  gcd=La;
  p = (Polynomial)malloc(sizeof(LNode));
  p->next = NULL;                         //������һ��ͷ���
  q = (Polynomial)malloc(sizeof(LNode));
  q->next = NULL;                         //��һ������
  q->data.expn = 0;
  q->data.coef = 1 / Coef(gcd);
  p->next = q;
  Gcd = MutiplyPolyn(gcd, p);             //��һ��
  
  q->data.coef = 1 / (Coef(Acpy) * Coef(Bcpy));
  lcm = MutiplyPolyn(Acpy, Bcpy);
  lcm = MutiplyPolyn(lcm, p);             //��һ��
  ModPolyn(lcm, Gcd, Quo, rem);           //��С����ʽ
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
