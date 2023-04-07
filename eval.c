#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

float numbers[50]; 
int tn=-1, to=-1;
char op[50];

void push_num(float n){
    numbers[++tn]=n;
}
void push_op(char ch){
    op[++to]=ch;
}
float pop_num(){
    return numbers[tn--];
}
char pop_op(){
    return op[to--];
}

float infix_evalution(float numbers[50], char op[50]){
    
    char p;
    float x = pop_num();
    float y = pop_num();
    p = pop_op();
   switch(p){
    case '+':
       return x+y;
    case '-':
       return y-x;
    case '*':
       return x*y;
    case '/':
       if(x==0){
        printf("divide by zero");
        exit(0);
       }
       else{
        return y/x;
       }
   }
}

int is_op(char c){
    return (c=='+'||c=='-'||c=='*'||c=='/');
}

int precedence(char c){
    switch(c){
        case '+':
        return 1;
        case '-':
        return 1;
        case '*':
        return 2;
        case '/':
        return 2;

    }
    return -1;
}
int isDIGIT(char c){
    return ((c>='0'&&c<='9')||c=='.');
}
float valuation(char exp[100]){
float num,dec ,v, output;
char c;
    for(int i=0;exp[i]!='\0';i++){
    c = exp[i];
        if(exp[i]==' '){
            continue;
        }
        if(isDIGIT(c)){
             num=0;
            dec =0;
             while(isDIGIT(c)){
                if(c=='.'){
                    dec = 10.0;
                    
                }else{
                    if(dec>0){
                        num += (c-'0')/dec;
                        dec = dec*10;
                    }else{
                        num *=10;
                        num+=c-'0';
                    }
                }
                i++;
                if(i<strlen(exp)){
                    c=exp[i];
                }
                else{
                    break;
                }
             }
             i--;
             push_num(num);
        }
        else if(c=='('){
            push_op(c);
        }
        else if(c==')'){
            while(op[to]!='('){
                v = infix_evalution(numbers, op);
                push_num(v);
            } 
            pop_op();
        }
        else if(is_op(c)){
            while(to!=-1 && precedence(c)<=precedence(op[to])){
                output = infix_evalution(numbers, op);
                push_num(output);
            }
            push_op(c);
        }
    }
    while(to!=-1){
        output = infix_evalution(numbers, op);
        push_num(output);
    }
    return pop_num();
}
int main(){

    char buf[100];
    strcpy(buf, "9+9.9*9");
    printf("%f", valuation(buf));
}