#include <iostream>
#include <stack>
#include <string.h>

using namespace std;


bool IsOperator(char ch) {             //�жϲ�����
    return (ch == '+') || (ch == '-') || (ch == '/') || (ch == '*') || (ch == '#') || (ch == '(') || (ch == ')');
}
int Priority(char ch) {                //�ж����ȼ�
    switch (ch) {
    case '+':
        return 1;
    case '-':
        return 1;
    case '*':
        return 2;
    case '/':
        return 2;
    case '(':
        return 0;
    case '#':
        return -1;
    }
}
bool IsPrior(char c, char ch) {         //�ж����ȼ�
    return Priority(c) >= Priority(ch);
}
float Operate(float a, char ch, float b) {    //��������
    switch (ch) {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        return a / b;
    }
}

bool IsValid(char exp[]) {              //�ж��Ƿ���Ч���ʽ
    char* p = exp;
    int temp = 0;
    while (*p != '\0') {
        if (!IsOperator(*p) && (*p < '0' || *p>'9' || *p == '#')) {
            printf("Error1: ������Ч�ַ�");
            return false;
        }
        if (*p == '(') {
            temp++;
            if (IsOperator(*(p + 1)) && *(p + 1) != '(') {
                printf("Error2: \"(\"��\"%c\"ȱ�ٱ��ʽ", *(p + 1));
                return false;
            }
        }
        if (*p == ')') {
            temp--;
            if (IsOperator(*(p + 1))) {
                if (*(p + 1) == '(') {
                    printf("Error3: \")\"��\"(\"ȱ�������");
                    return false;
                }
            }
            if (*(p + 1) == '\0')
                break;
        }

        if (IsOperator(*p)) {
            if (IsOperator(*(p + 1) && *(p + 1) != '(')) {
                printf("Error2: \"%c\"��\"%c\"ȱ��������", *p, *(p + 1));
                return false;
            }
            if (*(p + 1) == '\0') {
                printf("Error4: ������\"%c\"��β", *p);
                return false;
            }
            if (*p=='/'&&*(p + 1) == '0') {
                printf("Error5: ������0������", *p);
                return false;
            }
        }
        p++;
    }
    if (temp != 0) {
        printf("Error6: ���Ų��պ�");
        return false;
    }
    return true;
}

float Calculate(char exp[]) {
    stack<char> Operator;
    stack<float> Value;
    Operator.push('#');         //ѹ��#
    char* p = exp;
    char ch, c;
    float num=0;
    int degree = 0;
    int flag;
    while (!Operator.empty()) {
        
        ch = *p++;
        if (!IsOperator(ch)) {
            num = num*10+(ch - '0');
            degree++;
            continue;
        }
        if (degree > 0) {
            Value.push(num);
            num = 0;
            degree = 0;
        }
            
        switch (ch) {
        case '(':
            Operator.push(ch);
            break;
        case ')':
            while ((c = Operator.top()) != '(') {       //����( ȫ����������
                Operator.pop();
                float a = Value.top();
                Value.pop();
                float b = Value.top();
                Value.pop();
                Value.push(Operate(b, c, a));
            }
            Operator.pop();
            break;
        default:
            while ((c = Operator.top()) && IsPrior(c, ch)) {        //���ȼ��ߣ�ֱ������
                Operator.pop();
                if (c == '#')
                    break;
                float a = Value.top();
                Value.pop();
                float b = Value.top();
                Value.pop();
                Value.push(Operate(b, c, a));
            }
            if (ch != '#') {
                Operator.push(ch);
            }
            break;
        }
    }
    return Value.top();
}

int main() {
    char exp[100];
    printf("��������ʽ:");
    scanf("%s", exp);
    if (IsValid(exp)) {
        strcat(exp, "#");       //ĩβ��#
        printf("���Ϊ: %.2f", Calculate(exp));
    }
    return 0;
}
