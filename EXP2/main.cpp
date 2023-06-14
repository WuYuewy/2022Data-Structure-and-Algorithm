#include <iostream>
#include <stack>
#include <string.h>

using namespace std;


bool IsOperator(char ch) {             //判断操作符
    return (ch == '+') || (ch == '-') || (ch == '/') || (ch == '*') || (ch == '#') || (ch == '(') || (ch == ')');
}
int Priority(char ch) {                //判断优先级
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
bool IsPrior(char c, char ch) {         //判断优先级
    return Priority(c) >= Priority(ch);
}
float Operate(float a, char ch, float b) {    //运算数字
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

bool IsValid(char exp[]) {              //判断是否有效表达式
    char* p = exp;
    int temp = 0;
    while (*p != '\0') {
        if (!IsOperator(*p) && (*p < '0' || *p>'9' || *p == '#')) {
            printf("Error1: 存在无效字符");
            return false;
        }
        if (*p == '(') {
            temp++;
            if (IsOperator(*(p + 1)) && *(p + 1) != '(') {
                printf("Error2: \"(\"与\"%c\"缺少表达式", *(p + 1));
                return false;
            }
        }
        if (*p == ')') {
            temp--;
            if (IsOperator(*(p + 1))) {
                if (*(p + 1) == '(') {
                    printf("Error3: \")\"与\"(\"缺少运算符");
                    return false;
                }
            }
            if (*(p + 1) == '\0')
                break;
        }

        if (IsOperator(*p)) {
            if (IsOperator(*(p + 1) && *(p + 1) != '(')) {
                printf("Error2: \"%c\"与\"%c\"缺少运算数", *p, *(p + 1));
                return false;
            }
            if (*(p + 1) == '\0') {
                printf("Error4: 不能以\"%c\"结尾", *p);
                return false;
            }
            if (*p=='/'&&*(p + 1) == '0') {
                printf("Error5: 不能以0作除数", *p);
                return false;
            }
        }
        p++;
    }
    if (temp != 0) {
        printf("Error6: 括号不闭合");
        return false;
    }
    return true;
}

float Calculate(char exp[]) {
    stack<char> Operator;
    stack<float> Value;
    Operator.push('#');         //压入#
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
            while ((c = Operator.top()) != '(') {       //遇到( 全部弹出运算
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
            while ((c = Operator.top()) && IsPrior(c, ch)) {        //优先级高，直接运算
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
    printf("请输入表达式:");
    scanf("%s", exp);
    if (IsValid(exp)) {
        strcat(exp, "#");       //末尾加#
        printf("结果为: %.2f", Calculate(exp));
    }
    return 0;
}
