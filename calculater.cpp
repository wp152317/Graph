#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <ctype.h>
#include "calculater.h"

double stack[10000];
int stackpointer=-1;

void push(double a){
    stack[++stackpointer]=a;
}
double pop(){
    return stack[stackpointer--];
}

typedef struct node{        ///parent,left child,right child, data, whats calculater, position
    struct node* l;
    struct node* r;
    double data;
    int calc;
    int index;
}nd;

nd *BST=(nd*)malloc(sizeof(nd)),*headBST;
void insert(double data,int calc,int index);


void back(nd *p){
    if(p==NULL)return;
    if(p->calc!=2)
        back(p->l);
    back(p->r);
    if(p->calc)printf("%c\n",(char)p->data);
    else printf("%.2lf\n",p->data);
}

void mid(nd *p){
    if(p==NULL)return;
    if(p->calc!=2)
        mid(p->l);
    if(p->calc)printf("%c\n",(char)p->data);
    else printf("%.2lf\n",p->data);
    mid(p->r);
}
long long factorial(long long num){
    long long result=1;
    while(num>1){
        result*=num--;
    }
    return result;
}

void backturn(nd *p,double x=0,double y=0){
    if(p==NULL)return;
    if(p->calc!=2)
        backturn(p->l,x,y);
    backturn(p->r,x,y);


    if(p->calc==1){
        double b=pop();
        double a=pop();
        if(p->data=='+'){
            push(a+b);
        }
        else if(p->data=='-'){
            push(a-b);
        }
        else if(p->data=='*'){
            push(a*b);
        }
        else if(p->data=='/'){
            push(a/b);
        }
        else if(p->data=='^'){
            push(pow(a,b));
        }
        else if(p->data=='r'){
            push(pow(b,1/a));
        }
        else if(p->data=='l'){
            push(log10(b)/log10(a));
        }
        else if(p->data=='%'){
            double c=b;
            int m=0;
            if(a<0){a=-a;m=1;}
            while(a>=b){
                while(a>c*2)c*=2;
                a-=c;
                c=b;
            }
            if(m)
                push(b-a);
            else
                push(a);
        }
    }
    else if(p->calc==2){
        double a=pop();
        if(p->data=='s'){
            push(sin(a));
        }
        else if(p->data=='c'){
            push(cos(a));
        }
        else if(p->data=='t'){
            push(tan(a));
        }
        else if(p->data=='!'){
            double b;
            long long num;
            if(modf(a,&b)==0){
                num=b;
                push(factorial(num));
            }
            else push(a);
        }
        else if(p->data=='a'){
            push(fabs(a));
        }
        else if(p->data=='m'){
            push(-a);
        }
        else if(p->data=='a'+'s'){
            push(asin(a));
        }
        else if(p->data=='a'+'c'){
            push(acos(a));
        }
        else if(p->data=='a'+'t'){
            push(atan(a));
        }
        else if(p->data=='S'){
            push(sinh(a));
        }
        else if(p->data=='C'){
            push(cosh(a));
        }
        else if(p->data=='T'){
            push(tanh(a));
        }
        else if(p->data==1){
            push(1/cos(a));
        }
        else if(p->data==2){
            push(acos(1/a));
        }
        else if(p->data==3){
            push(1/cosh(a));
        }
        else if(p->data==4){
            push(1/sin(a));
        }
        else if(p->data==5){
            push(asin(1/a));
        }
        else if(p->data==6){
            push(1/sinh(a));
        }
        else if(p->data==7){
            push(1/tan(a));
        }
        else if(p->data==8){
            push(1/atan(a));
        }
        else if(p->data==9){
            push(1/tanh(a));
        }
        else if(p->data==10){
            push(log(a+sqrt(1+pow(a,2))));
        }
        else if(p->data==11){
            push(log(a+sqrt(pow(a,2)-1)));
        }
        else if(p->data==12){
            push(log((1+a)/(1-a))/2);
        }
        else if(p->data==13){
            push(log(1/a+sqrt(1-pow(a,2))/fabs(a)));
        }
        else if(p->data==14){
            push(log(1/a+sqrt(1+pow(a,2))/fabs(a)));
        }
        else if(p->data==15){
            push(log((a+1)/(a-1))/2);
        }
    }
    else if(p->calc==3){
        double a=pop();
        if(p->data=='!'){
            double b;
            long long num;
            if(modf(a,&b)==0){
                num=b;
                push(factorial(num));
            }
            else push(a);
        }
    }
    else {
        if(p->data=='x'){
            push(x);
            //printf("%.2lf\n",x);
        }
        else if(p->data=='y'){
            push(y);
            //printf("%.2lf\n",y);
        }
        else
            push(p->data);
    }
}

int check_part(char* str,int* i,int j){
    static int occnt=0;
    if(str[*i]==')')occnt++;
    else if(str[*i]=='(')occnt--;
    while(j!=occnt&&*i>0){
        (*i)--;
        if(str[*i]==')')occnt++;
        else if(str[*i]=='(')occnt--;
    }
    if(*i<0)return 1;
    return 0;
}

int check_wrong(node *nd){
    if(nd==NULL)return 0;
    if(check_wrong(nd->l)==1)return 1;
    if(nd->calc==0&&!(nd->l==NULL&&nd->r==NULL))return 1;
    else if(nd->calc==1&&!(nd->l!=NULL&&nd->r!=NULL))return 1;
    else if(nd->calc==2&&!(nd->l==NULL&&nd->r!=NULL))return 1;
    else if(nd->calc==3&&!(nd->l!=NULL&&nd->r==NULL))return 1;
    if(check_wrong(nd->r)==1)return 1;
    return 0;
}

double calculater(double arcs,char *str){
    headBST=BST;
    int i,j,len;double num;             ///연산자 검사 루프, 괄호 단계 구분 루프 변수,입력받은 문자열의 길이,상수버퍼
    int ncnt=0,ccnt=0;                  ///상수의 수, 다항연산자의 수
    int occnt=0,maxopen=0;              ///괄호수, 한번에 열리는 괄호의 수의 최댓값
    len=strlen(str);
    *BST={NULL,NULL,0,-1,-1};
    for(i=len-1;i>=0;i--)                   ///단항연산 조건 설립
        if(str[i]==' '||str[i]=='\t')
            str[i]=0;

    for(i=len-1;i>=0;i--){                  ///연산자 갯수와 숫자의 수 매치 확인
        //str[i]=tolower(str[i]);
        if(str[i]=='+'||str[i]=='-'||str[i]=='*'||str[i]=='/'||str[i]=='^'||(memcmp(&str[i],"root",4)==0&&i<len-3)||(memcmp(&str[i],"log",3)==0&&i<len-2)||memcmp(&str[i],"mod",3)==0)
            ccnt++;
        if(!isdigit(str[i])&&str[i]!='.'&&isdigit(str[i+1])||str[i]=='e'||memcmp(&str[i],"pi",2)==0||memcmp(&str[i],"arcs",4)==0)
            ncnt++;
    }
    for(i=len-1;i>=0;i--){                  ///상수 e로 인식된 sec의 e의 갯수 제거
        if(memcmp(&str[i],"sec",3)==0)ncnt--;
    }
    for(i=len-1;i>=0;i--){                  ///괄호 수 확인
        if(str[i]==')'){occnt++;if(maxopen<occnt)maxopen=occnt;}
        else if(str[i]=='('){occnt--;if(occnt<0)break;}
    }
    if(isdigit(str[0]))ncnt++;
    //printf("숫자 수 : %d\n다항 연산자 수 : %d\n\n한번에 열리는 괄호의 수 : %d\n",ncnt,ccnt,maxopen);
    if(ncnt-1!=ccnt){return arcs;}
    if(occnt){return arcs;}

    for(j=0;j<=maxopen;j++){
        //occnt=0;
        for(i=len-1;i>=0;i--){                  ///우선순위 최하위
            if(check_part(str,&i,j))break;
            if(memcmp(&str[i],"mod",3)==0&&i<len-2){
                insert('%',1,i);
                memcpy(&str[i],"\0\0\0",3);
            }
        }
        for(i=len-1;i>=0;i--){                  ///우선순위 하위
            if(check_part(str,&i,j))break;
            if(str[i]=='+'||str[i]=='-'){
                insert(str[i],1,i);
                str[i]=0;
            }
        }
        for(i=len-1;i>=0;i--){                  ///우선순위 중하위
            if(check_part(str,&i,j))break;
            if(str[i]=='*'||str[i]=='/'){
                insert(str[i],1,i);
                str[i]=0;
            }
        }
        for(i=len-1;i>=0;i--){                  ///우선순위 중위
            if(check_part(str,&i,j))break;
            else if(str[i]=='m'&&str[i+1]!='o'){               ///minus 부호 대체
                insert(str[i],2,i);
                str[i]=0;
            }
        }
        for(i=len-1;i>=0;i--){                  ///우선순위 상위
            if(check_part(str,&i,j))break;
            if(memcmp(&str[i],"log",3)==0&&i<len-2){
                insert('l',1,i);
                memcpy(&str[i],"\0\0\0",3);
            }
            else if(memcmp(&str[i],"sin",3)==0&&i<len-2){   ///sin,asin,sinh
                if(str[i-1]=='a'){
                    if(str[i+3]=='h'){
                        insert(10,2,i-1);
                        memcpy(&str[i-1],"\0\0\0\0\0",5);
                    }
                    else{
                        insert('a'+'s',2,i-1);
                        memcpy(&str[i-1],"\0\0\0\0",4);
                    }
                }
                else if(str[i+3]=='h'){
                    insert('S',2,i-1);
                    memcpy(&str[i],"\0\0\0\0",4);
                }
                else {
                    insert('s',2,i);
                    memcpy(&str[i],"\0\0\0",3);
                }
            }
            else if(memcmp(&str[i],"cos",3)==0&&i<len-2){   ///cos,acos,cosh
                if(str[i-1]=='a'){
                    if(str[i+3]=='h'){
                        insert(11,2,i-1);
                        memcpy(&str[i-1],"\0\0\0\0\0",5);
                    }
                    else{
                        insert('a'+'c',2,i-1);
                        memcpy(&str[i-1],"\0\0\0\0",4);
                    }
                }
                else if(str[i+3]=='h'){
                    insert('C',2,i-1);
                    memcpy(&str[i],"\0\0\0\0",4);
                }
                else{
                    insert('c',2,i);
                    memcpy(&str[i],"\0\0\0",3);
                }
            }
            else if(memcmp(&str[i],"tan",3)==0&&i<len-2){   ///tan,atan,tanh
                if(str[i-1]=='a'){
                    if(str[i+3]=='h'){
                        insert(12,2,i-1);
                        memcpy(&str[i-1],"\0\0\0\0\0",5);
                    }
                    else{
                        insert('a'+'t',2,i-1);
                        memcpy(&str[i-1],"\0\0\0\0",4);
                    }
                }
                else if(str[i+3]=='h'){
                    insert('T',2,i-1);
                    memcpy(&str[i],"\0\0\0\0",4);
                }
                else{
                    insert('t',2,i);
                    memcpy(&str[i],"\0\0\0",3);
                }
            }
            else if(memcmp(&str[i],"sec",3)==0&&i<len-2){   ///sec,asec,sech
                if(str[i-1]=='a'){
                    if(str[i+3]=='h'){
                        insert(13,2,i-1);
                        memcpy(&str[i-1],"\0\0\0\0\0",5);
                    }
                    else{
                        insert(2,2,i-1);
                        memcpy(&str[i-1],"\0\0\0\0",4);
                    }
                }
                else if(str[i+3]=='h'){
                    insert(3,2,i-1);
                    memcpy(&str[i],"\0\0\0\0",4);
                }
                else {
                    insert(1,2,i);
                    memcpy(&str[i],"\0\0\0",3);
                }
            }
            else if(memcmp(&str[i],"csc",3)==0&&i<len-2){   ///csc,acsc,csch
                if(str[i-1]=='a'){
                    if(str[i+3]=='h'){
                        insert(14,2,i-1);
                        memcpy(&str[i-1],"\0\0\0\0\0",5);
                    }
                    else{
                        insert(5,2,i-1);
                        memcpy(&str[i-1],"\0\0\0\0",4);
                    }
                }
                else if(str[i+3]=='h'){
                    insert(6,2,i-1);
                    memcpy(&str[i],"\0\0\0\0",4);
                }
                else {
                    insert(4,2,i);
                    memcpy(&str[i],"\0\0\0",3);
                }
            }
            else if(memcmp(&str[i],"cot",3)==0&&i<len-2){   ///cot,acot,coth
                if(str[i-1]=='a'){
                    if(str[i+3]=='h'){
                        insert(15,2,i-1);
                        memcpy(&str[i-1],"\0\0\0\0\0",5);
                    }
                    else{
                        insert(8,2,i-1);
                        memcpy(&str[i-1],"\0\0\0\0",4);
                    }
                }
                else if(str[i+3]=='h'){
                    insert(9,2,i-1);
                    memcpy(&str[i],"\0\0\0\0",4);
                }
                else {
                    insert(7,2,i);
                    memcpy(&str[i],"\0\0\0",3);
                }
            }
            else if(memcmp(&str[i],"fac",3)==0&&i<len-2){
                insert('!',2,i);
                memcpy(&str[i],"\0\0\0",3);
            }
            else if(memcmp(&str[i],"abs",3)==0&&i<len-2){
                insert('a',2,i);
                memcpy(&str[i],"\0\0\0",3);
            }
        }
        for(i=len-1;i>=0;i--){                  ///우선순위 최상위
            if(check_part(str,&i,j))break;
            if(str[i]=='^'){
                insert(str[i],1,i);
                str[i]=0;
            }
            else if(str[i]=='!'){
                insert(str[i],3,i);
                str[i]=0;
            }
            else if(memcmp(&str[i],"root",4)==0&&i<len-3){
                insert('r',1,i);
                memcpy(&str[i],"\0\0\0\0",4);
            }
        }
    }
    for(i=len-1;i>=0;i--){
        if(str[i]=='('||str[i]==')')str[i]=0;
    }
    char clearbuff[100000]={};
    for(i=len-1;i>=0;i--){                  ///수 대입
            if(str[i]=='e'){                ///자연상수
                insert(M_E,0,i);
                str[i]=0;
            }
            else if(memcmp(&str[i],"pi",2)==0){         ///원주율
                insert(M_PI,0,i);
                memcpy(&str[i],"\0\0",2);
            }
            else if(memcmp(&str[i],"arcs",2)==0){           ///최근 연산된 값(default==0)
                insert(arcs,0,i);
                memcpy(&str[i],"\0\0\0\0",4);
            }
        if((str[i]==0)&&(str[i+1]!=0)){ ///상수
            num=atof(&str[i+1]);
            insert(num,0,i+1);
            memcpy(&str[i+1],clearbuff,strlen(&str[i+1]));
        }
    }
    if(isdigit(str[0])){
        num=atof(str);
        insert(num,0,i);
        memcpy(str,clearbuff,strlen(str));
    }
    for(i=0;i<len;i++){
        if(str[i]){
            return arcs;
        }
    }
    //mid(headBST);
    //printf("\n\n");
    //back(headBST);
    if(check_wrong(headBST)){
        return arcs;
    }
    backturn(headBST);
    double result=pop();
    return result;
}
void insert(double data,int calc,int index){
    if(BST->calc==-1){
        (*BST)={NULL,NULL,data,calc,index};
        return;
    }
    while(1){
        if(index<BST->index){
            if(BST->l==NULL){
                BST->l=(nd*)malloc(sizeof(nd));
                BST=BST->l;
                BST->l=NULL;
                BST->r=NULL;
                BST->data=data;
                BST->calc=calc;
                BST->index=index;
                break;
            }
            else BST=BST->l;
        }
        else {
            if(BST->r==NULL){
                BST->r=(nd*)malloc(sizeof(nd));
                BST=BST->r;
                BST->l=NULL;
                BST->r=NULL;
                BST->data=data;
                BST->calc=calc;
                BST->index=index;
                break;
            }
            else BST=BST->r;
        }
    }
    BST=headBST;
}


int buildTree(char *str){
    headBST=BST;
    int i,j,len;double num;             ///연산자 검사 루프, 괄호 단계 구분 루프 변수,입력받은 문자열의 길이,상수버퍼
    int ncnt=0,ccnt=0;                  ///상수의 수, 다항연산자의 수
    int occnt=0,maxopen=0;              ///괄호수, 한번에 열리는 괄호의 수의 최댓값
    len=strlen(str);
    *BST={NULL,NULL,0,-1,-1};
    for(i=len-1;i>=0;i--)                   ///단항연산 조건 설립
        if(str[i]==' '||str[i]=='\t')
            str[i]=0;
    for(i=len-1;i>=0;i--){                  ///연산자 갯수와 숫자의 수 매치 확인
        str[i]=tolower(str[i]);
        if(str[i]=='+'||str[i]=='-'||str[i]=='*'||str[i]=='/'||str[i]=='^'||(memcmp(&str[i],"root",4)==0&&i<len-3)||(memcmp(&str[i],"log",3)==0&&i<len-2)||memcmp(&str[i],"mod",3)==0)
            ccnt++;
        if(!isdigit(str[i])&&str[i]!='.'&&isdigit(str[i+1])||str[i]=='e'||str[i]=='x'||str[i]=='y'||memcmp(&str[i],"pi",2)==0||memcmp(&str[i],"arcs",4)==0)
            ncnt++;
    }
    for(i=len-1;i>=0;i--){                  ///상수 e로 인식된 sec의 e의 갯수 제거
        if(memcmp(&str[i],"sec",3)==0)ncnt--;
    }
    for(i=len-1;i>=0;i--){                  ///괄호 수 확인
        if(str[i]==')'){occnt++;if(maxopen<occnt)maxopen=occnt;}
        else if(str[i]=='('){occnt--;if(occnt<0)break;}
    }
    if(isdigit(str[0]))ncnt++;
    //printf("숫자 수 : %d\n다항 연산자 수 : %d\n\n한번에 열리는 괄호의 수 : %d\n",ncnt,ccnt,maxopen);
    if(ncnt-1!=ccnt){return 0;}
    if(occnt){return 0;}
    for(j=0;j<=maxopen;j++){
        //occnt=0;
        for(i=len-1;i>=0;i--){                  ///우선순위 최하위
            if(check_part(str,&i,j))break;
            if(memcmp(&str[i],"mod",3)==0&&i<len-2){
                insert('%',1,i);
                memcpy(&str[i],"\0\0\0",3);
            }
        }
        for(i=len-1;i>=0;i--){                  ///우선순위 하위
            if(check_part(str,&i,j))break;
            if(str[i]=='+'||str[i]=='-'){
                insert(str[i],1,i);
                str[i]=0;
            }
        }
        for(i=len-1;i>=0;i--){                  ///우선순위 중하위
            if(check_part(str,&i,j))break;
            if(str[i]=='*'||str[i]=='/'){
                insert(str[i],1,i);
                str[i]=0;
            }
        }
        for(i=len-1;i>=0;i--){                  ///우선순위 중위
            if(check_part(str,&i,j))break;
            else if(str[i]=='m'&&str[i+1]!='o'){               ///minus 부호 대체
                insert(str[i],2,i);
                str[i]=0;
            }
        }
        for(i=len-1;i>=0;i--){                  ///우선순위 상위
            if(check_part(str,&i,j))break;
            if(memcmp(&str[i],"log",3)==0&&i<len-2){
                insert('l',1,i);
                memcpy(&str[i],"\0\0\0",3);
            }
            else if(memcmp(&str[i],"sin",3)==0&&i<len-2){   ///sin,asin,sinh
                if(str[i-1]=='a'){
                    if(str[i+3]=='h'){
                        insert(10,2,i-1);
                        memcpy(&str[i-1],"\0\0\0\0\0",5);
                    }
                    else{
                        insert('a'+'s',2,i-1);
                        memcpy(&str[i-1],"\0\0\0\0",4);
                    }
                }
                else if(str[i+3]=='h'){
                    insert('S',2,i-1);
                    memcpy(&str[i],"\0\0\0\0",4);
                }
                else {
                    insert('s',2,i);
                    memcpy(&str[i],"\0\0\0",3);
                }
            }
            else if(memcmp(&str[i],"cos",3)==0&&i<len-2){   ///cos,acos,cosh
                if(str[i-1]=='a'){
                    if(str[i+3]=='h'){
                        insert(11,2,i-1);
                        memcpy(&str[i-1],"\0\0\0\0\0",5);
                    }
                    else{
                        insert('a'+'c',2,i-1);
                        memcpy(&str[i-1],"\0\0\0\0",4);
                    }
                }
                else if(str[i+3]=='h'){
                    insert('C',2,i-1);
                    memcpy(&str[i],"\0\0\0\0",4);
                }
                else{
                    insert('c',2,i);
                    memcpy(&str[i],"\0\0\0",3);
                }
            }
            else if(memcmp(&str[i],"tan",3)==0&&i<len-2){   ///tan,atan,tanh
                if(str[i-1]=='a'){
                    if(str[i+3]=='h'){
                        insert(12,2,i-1);
                        memcpy(&str[i-1],"\0\0\0\0\0",5);
                    }
                    else{
                        insert('a'+'t',2,i-1);
                        memcpy(&str[i-1],"\0\0\0\0",4);
                    }
                }
                else if(str[i+3]=='h'){
                    insert('T',2,i-1);
                    memcpy(&str[i],"\0\0\0\0",4);
                }
                else{
                    insert('t',2,i);
                    memcpy(&str[i],"\0\0\0",3);
                }
            }
            else if(memcmp(&str[i],"sec",3)==0&&i<len-2){   ///sec,asec,sech
                if(str[i-1]=='a'){
                    if(str[i+3]=='h'){
                        insert(13,2,i-1);
                        memcpy(&str[i-1],"\0\0\0\0\0",5);
                    }
                    else{
                        insert(2,2,i-1);
                        memcpy(&str[i-1],"\0\0\0\0",4);
                    }
                }
                else if(str[i+3]=='h'){
                    insert(3,2,i-1);
                    memcpy(&str[i],"\0\0\0\0",4);
                }
                else {
                    insert(1,2,i);
                    memcpy(&str[i],"\0\0\0",3);
                }
            }
            else if(memcmp(&str[i],"csc",3)==0&&i<len-2){   ///csc,acsc,csch
                if(str[i-1]=='a'){
                    if(str[i+3]=='h'){
                        insert(14,2,i-1);
                        memcpy(&str[i-1],"\0\0\0\0\0",5);
                    }
                    else{
                        insert(5,2,i-1);
                        memcpy(&str[i-1],"\0\0\0\0",4);
                    }
                }
                else if(str[i+3]=='h'){
                    insert(6,2,i-1);
                    memcpy(&str[i],"\0\0\0\0",4);
                }
                else {
                    insert(4,2,i);
                    memcpy(&str[i],"\0\0\0",3);
                }
            }
            else if(memcmp(&str[i],"cot",3)==0&&i<len-2){   ///cot,acot,coth
                if(str[i-1]=='a'){
                    if(str[i+3]=='h'){
                        insert(15,2,i-1);
                        memcpy(&str[i-1],"\0\0\0\0\0",5);
                    }
                    else{
                        insert(8,2,i-1);
                        memcpy(&str[i-1],"\0\0\0\0",4);
                    }
                }
                else if(str[i+3]=='h'){
                    insert(9,2,i-1);
                    memcpy(&str[i],"\0\0\0\0",4);
                }
                else {
                    insert(7,2,i);
                    memcpy(&str[i],"\0\0\0",3);
                }
            }
            else if(memcmp(&str[i],"fac",3)==0&&i<len-2){
                insert('!',2,i);
                memcpy(&str[i],"\0\0\0",3);
            }
            else if(memcmp(&str[i],"abs",3)==0&&i<len-2){
                insert('a',2,i);
                memcpy(&str[i],"\0\0\0",3);
            }
        }
        for(i=len-1;i>=0;i--){                  ///우선순위 최상위
            if(check_part(str,&i,j))break;
            if(str[i]=='^'){
                insert(str[i],1,i);
                str[i]=0;
            }
            else if(str[i]=='!'){
                insert(str[i],3,i);
                str[i]=0;
            }
            else if(memcmp(&str[i],"root",4)==0&&i<len-3){
                insert('r',1,i);
                memcpy(&str[i],"\0\0\0\0",4);
            }
        }
    }
    for(i=len-1;i>=0;i--){
        if(str[i]=='('||str[i]==')')str[i]=0;
    }
    char clearbuff[100000]={};
    for(i=len-1;i>=0;i--){                  ///수 대입
            if(str[i]=='e'){                ///자연상수
                insert(M_E,0,i);
                str[i]=0;
            }

            else if(str[i]=='x'||str[i]=='y'){
                insert(str[i],0,i);
                str[i]=0;
            }

            else if(memcmp(&str[i],"pi",2)==0){         ///원주율
                insert(M_PI,0,i);
                memcpy(&str[i],"\0\0",2);
            }

        if((str[i]==0)&&(str[i+1]!=0)){ ///상수
            num=atof(&str[i+1]);
            insert(num,0,i+1);
            memcpy(&str[i+1],clearbuff,strlen(&str[i+1]));
        }
    }
    if(isdigit(str[0])){
        num=atof(str);
        insert(num,0,i);
        memcpy(str,clearbuff,strlen(str));
    }
    //mid(headBST);
    //printf("\n\n");
    //back(headBST);
    if(check_wrong(headBST)){

        return 0;
    }
    return 1;
}

double solveEquation(double x,double y){
    //printf("%.2lf %.2lf\n",x,y);
    backturn(BST,x,y);
    double result=pop();
    return result;
}
