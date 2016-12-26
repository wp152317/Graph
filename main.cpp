#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include "calculater.h"

typedef struct equation{
    int index;
    char* equate;
    char leftPart[100000];
    char rightPart[100000];
    struct equation *next;
}equat;

typedef struct inputEquation{
    int index;
    equat eq;
    int color;
    struct inputEquation *next;
}inEquat;

inEquat *equationList=(inEquat*)malloc(sizeof(inEquat));
inEquat *headList=equationList;
int graph[500][500];


/*  Declare Windows procedure  */
LRESULT CALLBACK windporc (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

typedef struct button{
    char calcul[8];
    int downed;
}bt;

bt but[7][6]={{{" BACK",0},{"  C  ",0},{"  x  ",0},{"  y  ",0},{"  ,  ",0},{" abs ",0}},
              {{"     ",0},{" arc ",0},{" sin ",0},{" cos ",0},{" tan ",0},{" mod ",0}},
              {{" h y ",0},{"archy",0},{" sec ",0},{" csc ",0},{" cot ",0},{" log ",0}},


              {{" +/- ",0},{"  (  ",0},{"  )  ",0},{"  .  ",0},{"  !  ",0},{" root",0}},
              {{"  +  ",0},{"  -  ",0},{"  *  ",0},{"  /  ",0},{"  =  ",0},{"  ^  ",0}},
              {{"  6  ",0},{"  7  ",0},{"  8  ",0},{"  9  ",0},{"  0  ",0},{" Pi  ",0}},
              {{"  1  ",0},{"  2  ",0},{"  3  ",0},{"  4  ",0},{"  5  ",0},{"  e  ",0}}};

void searchEquate(equat* peq,char *str){
    int i;
    for(i=0;str[i];i++){
        if(str[i]=='='){peq->equate="=";str[i]=0;break;}
        else if(str[i]=='<'){peq->equate="<";str[i]=0;break;}
        else if(str[i]=='>'){peq->equate=">";str[i]=0;break;}
    }
}

void cutEquate(char *str){
    int i;
    for(i=0;str[i];i++){
        if(str[i]==','){str[i]=0;}
    }
}

/** \brief 식 추가
 *  parameter를 안넘겨줄시 식에 값을 넣지 않음
 * \param pIE inEquat*
 * \param str=NULL char*
 * \return int
 *
 */
int createList(inEquat* pIE,char* str=NULL){
    int cnt=2;
    while(pIE->next!=NULL){
        pIE=pIE->next;
        cnt++;
    }
    pIE->next=(inEquat*)malloc(sizeof(inEquat));
    pIE=pIE->next;
    *pIE={cnt,{1,NULL,NULL,NULL,NULL},0xFF0000,NULL};
    //int i;
    //searchEquate(peq,str);
    //strcpy(pIE->eq.leftPart,str);
    //strcpy(pIE->eq.rightPart,&str[strlen(str)+1]);
    pIE=headList;
    return 1;
}
/** \brief 조건식 추가
 *
 * \param pIE inEquat*
 * \param index int
 * \param str char*
 * \return void
 *
 */
void addEquation(equat* peq,char *str){
    //equat* peq=&pIE->eq;
    int cnt=2;
    while(peq->next!=NULL){
        peq=peq->next;
        cnt++;
    }
    peq->next=(equat*)malloc(sizeof(equat));
    peq=peq->next;
    *peq={cnt,str,NULL,NULL};
    searchEquate(peq,str);
    strcpy(peq->leftPart,str);
    strcpy(peq->rightPart,&str[strlen(str)+1]);
    //printf("Add Completed");
    return;
}

/** \brief 리스트 갯수 리턴
 *
 * \param pIE inEquat*
 * \return int
 *
 */
int checkList(inEquat* pIE){
    if(pIE==NULL){
        //printf("0\n");
        return 0;
    }
    int cnt=1;
    while(pIE->next!=NULL){
        pIE=pIE->next;
        cnt++;
    }
    //printf("%d\n",cnt);
    pIE=headList;
    return cnt;
}

/** \brief 방정식 단위 리스트 하나 지우기
 *
 * \param pIE inEquat*
 * \param index int
 * \return void
 *
 */
void deleteinEqList(inEquat *pIE,int index){
    if(pIE->index==index){
        if(pIE->next==NULL)return;
        inEquat* deleteIE;
        deleteIE=pIE;
        pIE=pIE->next;
        free(deleteIE);
        headList=pIE;
    }
    else{
        while(pIE->next->index!=index){
            pIE=pIE->next;
        }
        inEquat* deleteIE;
        deleteIE=pIE->next;
        pIE->next=pIE->next->next;
        pIE=pIE->next;
        free(deleteIE);
    }
    while(pIE!=NULL){
        pIE->index-=1;
        pIE=pIE->next;
    }

    return;
}

void deleteineEquat(inEquat* pIE,int listIndex,int eqIndex){
    while(pIE->index!=listIndex){
        pIE=pIE->next;
    }
    equat *deleteEq,*peq=&pIE->eq;

    if(pIE->eq.index==eqIndex){
        deleteEq=peq;
        peq=peq->next;
        pIE->eq=*peq;
    }
    else{

        while(peq->next!=NULL&&peq->next->index!=eqIndex){
            peq=peq->next;
        }
        //printf("Hello\n");
        deleteEq=peq->next;
        peq->next=peq->next->next;
        peq=peq->next;
        free(deleteEq);
    }
    while(peq!=NULL){
        peq->index=1;
        peq=peq->next;
        //printf("HELLO\n");
    }
}

/** \brief 리스트 안의 내용 바꾸기
 *
 * \param pIE inEquat*
 * \param index int
 * \param str char*
 * \return void
 *
 */
void changeList(inEquat* pIE,int index,char *str,int color=-1){
    while(pIE->index!=index){
        pIE=pIE->next;
    }

    if(color!=-1){
        pIE->color=color;
    }
    equat* peq=&pIE->eq;
    while(peq->next!=NULL){
        deleteineEquat(pIE,index,1);
    }
    cutEquate(str);
    pIE->eq={1,NULL,NULL,NULL,NULL};
    searchEquate(peq,str);
    strcpy(pIE->eq.leftPart,str);
    str+=strlen(str)+1;
    strcpy(pIE->eq.rightPart,str);
    str+=strlen(str)+1;
    //printf("%s%s%s\n",pIE->eq.leftPart,pIE->eq.equate,pIE->eq.rightPart);
    peq=&pIE->eq;
    while(*str!=NULL){
        int size=strlen(str);
        addEquation(peq,str);
        str+=size+1;
    }
}

void toggleTriangleFunction(char fir,char sec){
    for(int k=1;k<=2;k++){
        for(int l=2;l<=4;l++){
            but[k][l].calcul[0]=fir;
            but[k][l].calcul[4]=sec;
        }
    }
}

void printEquatList(inEquat* pIE,int index,HDC hdc){
    HFONT hfont=CreateFont(18,0,0,0,5,0,0,0,ANSI_CHARSET,0,0,0,0,TEXT("Consolas"));
    SelectObject(hdc,hfont);
    while(pIE->index!=index){
        pIE=pIE->next;
    }
    equat* peq=&pIE->eq;
    char dump[100000]={};
    while(peq!=NULL){
        if(peq->equate==NULL)break;
        strcat(dump,peq->leftPart);
        strcat(dump,peq->equate);
        strcat(dump,peq->rightPart);
        if(peq->next!=NULL)
            strcat(dump,",");
        peq=peq->next;
    }
    TextOut(hdc,0,605,dump,strlen(dump));
    DeleteObject(hfont);
}

/** \brief 그래프 그리기
 *
 * \param pIE inEquat* 리스트
 * \param hdc=NULL HDC 진행상황 그리기
 * \return void
 *
 */
void drawgraph(inEquat* pIE,HDC hdc=NULL){
    HPEN hpen=CreatePen(PS_SOLID,1,RGB(255,0,0));
    HPEN hWhitePen=CreatePen(PS_SOLID,1,RGB(255,255,255));
    HPEN hBlackPen=CreatePen(PS_SOLID,1,RGB(0,0,0));
    for(int i=0;i<500;i++){
        for(int j=0;j<500;j++){
            graph[i][j]=0;
        }
    }
    while(pIE!=NULL){
        if(pIE->eq.equate==NULL){pIE=pIE->next;continue;}
        equat *peq=&pIE->eq;
        int **tmpgraph=(int**)malloc(sizeof(int*)*500);
        for(int i=0;i<500;i++){
            tmpgraph[i]=(int*)malloc(sizeof(int)*500);
            for(int j=0;j<500;j++){
                tmpgraph[i][j]=pIE->color;
            }
        }
        while(peq!=NULL){
            char buff[210000]={};
            strcat(buff,"(");
            strcat(buff,peq->leftPart);
            strcat(buff,")-(");
            strcat(buff,peq->rightPart);
            strcat(buff,")");
            if(buildTree(buff)==0){break;}
            //printf("%.3lf\n",solveEquation((double)(-250)/50,(double)(250)/50));
            for(int i=0;i<500;i++){
                if(hdc!=NULL){
                    SelectObject(hdc,hpen);
                    MoveToEx(hdc,i,501,NULL);
                    LineTo(hdc,i,550);
                }
                for(int j=0;j<500;j++){
                    MSG msg;
                    while(::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)){
                        ::SendMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
                    }
                    if(tmpgraph[i][j]==0)continue;
                    if(peq->equate[0]=='='){
                        double data[4]={solveEquation((double)(i-250)/50,(double)(250-j)/50),
                                        solveEquation((double)((i+1)-250)/50,(double)(250-j)/50),
                                        solveEquation((double)(i-250)/50,(double)(250-(1+j))/50),
                                        solveEquation((double)((i+1)-250)/50,(double)(250-(1+j))/50)};
                        if((data[0]>=0&&data[3]<=0)||(data[0]<=0&&data[3]>=0)||(data[1]>=0&&data[2]<=0)||(data[1]<=0&&data[2]>=0)){}
                        else tmpgraph[i][j]=0;
                    }
                    else if(peq->equate[0]=='<'){
                        double data=solveEquation((double)(i-250)/50,(double)(250-j)/50);
                        if(data<0){}
                        else tmpgraph[i][j]=0;
                    }
                    else if(peq->equate[0]=='>'){
                        double data=solveEquation((double)(i-250)/50,(double)(250-j)/50);
                        if(data>0){}
                        else tmpgraph[i][j]=0;
                    }
                }
                if(hdc!=NULL){
                    SelectObject(hdc,hWhitePen);
                    MoveToEx(hdc,i,501,NULL);
                    LineTo(hdc,i,550);
                }
            }
            peq=peq->next;
        }
        for(int i=0;i<500;i++){
            for(int j=0;j<500;j++){
                if(tmpgraph[i][j])
                    graph[j][i]=tmpgraph[i][j];
            }
            free(tmpgraph[i]);
        }
        free(tmpgraph);
        pIE=pIE->next;
    }
    DeleteObject(hpen);
    DeleteObject(hWhitePen);
    DeleteObject(hBlackPen);
    pIE=headList;
}

void changeListColor(inEquat *pIE,int index,int color){
    while(pIE->index!=index)
        pIE=pIE->next;
    if(color==0)
        pIE->color=-1;
    else
        pIE->color=color;
}
int getListColor(inEquat *pIE,int index){
    while(pIE->index!=index)
        pIE=pIE->next;
    if(pIE->color==-1)
        return 0;
    return pIE->color;
}

int WINAPI Graph    (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow);

int WINAPI WinMain(HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow){
    Graph(hThisInstance,hPrevInstance,lpszArgument,nCmdShow);
    return 0;
}

void insertStr(HWND hwnd,char* str,char* src){
    strcat(str,src);
    RECT rect={0,570,500,600};
    InvalidateRect(hwnd,&rect,false);
    return;
}

void printUserListInterface(HWND hwnd,inEquat* pIE,int selected,int userPage){
    HDC hdc=GetDC(hwnd);
    HFONT hfont=CreateFont(18,0,0,0,5,0,0,0,ANSI_CHARSET,0,0,0,0,TEXT("Consolas"));
    HBRUSH hBlackBrush=CreateSolidBrush(RGB(0,0,0));
    HBRUSH hWhiteBrush=CreateSolidBrush(RGB(255,255,255));
    SelectObject(hdc,hfont);
    SetBkMode(hdc,TRANSPARENT);
    while(pIE!=NULL&&userPage-(pIE->index-1)/48>0){
        pIE=pIE->next;
    }
    while(pIE!=NULL&&userPage-(pIE->index-1)/48==0){
        if(selected==pIE->index){
            SetTextColor(hdc,RGB(255,255,255));
            SelectObject(hdc,hBlackBrush);
        }
        else{
            SetTextColor(hdc,RGB(0,0,0));
            SelectObject(hdc,hWhiteBrush);
        }
            Rectangle(hdc,(pIE->index-1-userPage*48)%4*50+500,(pIE->index-1-userPage*48)/4*30+20,
                      ((pIE->index-1-userPage*48)%4+1)*50+500,((pIE->index-1-userPage*48)/4)*30+50);
        char dump[10];
        sprintf(dump,"%2d",pIE->index);
        TextOut(hdc,(pIE->index-1-userPage*48)%4*50+525,((pIE->index-1-userPage*48)/4)*30+27,dump,strlen(dump));
        pIE=pIE->next;
    }
    DeleteObject(hBlackBrush);
    DeleteObject(hWhiteBrush);
    DeleteObject(hfont);
    ReleaseDC(hwnd,hdc);
}

int WINAPI Graph    (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    *equationList={1,{1,NULL,NULL,NULL,NULL},0xFF0000,NULL};

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = windporc;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           TEXT("그래프"),       /* Title Text */
           WS_POPUP, /* default window */
           50,       /* Windows decides the position */
           50,       /* where the window ends up on the screen */
           900,                 /* The programs width */
           700,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to windporc */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK windporc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int mousemove=0;
    static int doi=0,doj=0;
    static char str[100000]={};
    static int nowListp=1;
    static char colorRGB[7]="FF0000";
    static int isItCal=0;
    static int userPage=0;
    switch (message)                  /* handle the messages */
    {
    /** \brief 그리기
     *
     */
    case WM_PAINT:{
        PAINTSTRUCT ps;
        HDC hdc=BeginPaint(hwnd,&ps);
        HPEN hpen=CreatePen(PS_NULL,0,RGB(0,0,0));
        HBRUSH hWhiteBrush=CreateSolidBrush(RGB(255,255,255));
        HBRUSH hBlackBrush=CreateSolidBrush(RGB(0,0,0));
        HFONT hfont=CreateFont(18,0,0,0,5,0,0,0,ANSI_CHARSET,0,0,0,0,TEXT("Consolas"));
        SelectObject(hdc,hpen);
        SelectObject(hdc,hfont);
        Rectangle(hdc,0,0,901,901);
        SetBkMode(hdc,TRANSPARENT);
        for(int i=0;i<7;i++){
            for(int j=0;j<7;j++){
                if(but[i][j].downed==1){
                        SelectObject(hdc,hBlackBrush);
                        SetTextColor(hdc,RGB(255,255,255));
                }
                else {
                        SelectObject(hdc,hWhiteBrush);
                        SetTextColor(hdc,RGB(0,0,0));
                }
                Rectangle(hdc,j*67+500,i*43+400,(j+1)*67+500,(i+1)*43+400);
                TextOut(hdc,j*67+510,i*43+410,but[i][j].calcul,strlen(but[i][j].calcul));
            }
        }
        DeleteObject(hpen);
        hpen=CreatePen(PS_SOLID,1,RGB(0,0,0));
        SelectObject(hdc,hpen);
        MoveToEx(hdc,500,0,NULL);
        LineTo(hdc,500,900);
        MoveToEx(hdc,0,500,NULL);
        LineTo(hdc,500,500);
        MoveToEx(hdc,700,0,NULL);
        LineTo(hdc,700,400);
        MoveToEx(hdc,500,400,NULL);
        LineTo(hdc,900,400);
        MoveToEx(hdc,0,550,NULL);
        LineTo(hdc,500,550);
        MoveToEx(hdc,0,570,NULL);
        LineTo(hdc,500,570);
        MoveToEx(hdc,0,600,NULL);
        LineTo(hdc,500,600);
        MoveToEx(hdc,0,630,NULL);
        LineTo(hdc,500,630);
        TextOut(hdc,0,575,"입력식 : ",strlen("입력식 : "));
        TextOut(hdc,0,551,TEXT("현재 여기의 X축 그래프를 찾고 있어요."),strlen("현재 여기의 X축 그래프를 찾고 있어요."));
        TextOut(hdc,470,575,"GO!",3);
        {
            int size=strlen(str);
            if(size<50)
                TextOut(hdc,60,575,str,size);
            else{
                char dumy[51]="...";
                strcat(dumy,&str[size-47]);
                TextOut(hdc,60,575,dumy,strlen(dumy));
            }
        }
        TextOut(hdc,550,2,TEXT("입력 식 리스트"),strlen("입력 식 리스트"));
        TextOut(hdc,750,2,TEXT("정의 식 리스트"),strlen("정의 식 리스트"));
        printEquatList(equationList,nowListp,hdc);
        Rectangle(hdc,450,640,500,670);
        TextOut(hdc,455,645,"DRAW!",5);
        Rectangle(hdc,450,670,500,700);
        TextOut(hdc,455,675,"=/</>",5);
        SetBkMode(hdc,OPAQUE);
        SetBkColor(hdc,RGB(255,255,255));
        SetTextColor(hdc,RGB(0,0,0));
        for(int i=0;i<6;i++){
            char dumpString[2]={colorRGB[i],0};
            TextOut(hdc,i*9,641,dumpString,1);
        }
        DeleteObject(hpen);
        SelectObject(hdc,hpen);
        for(int i=0;i<500;i++){
            for(int j=0;j<500;j++){
                if(graph[i][j]){
                    if(graph[i][j]==-1){
                        hpen=CreatePen(PS_SOLID,1,RGB(0,0,0));
                    }
                    else
                        hpen=CreatePen(PS_SOLID,1,RGB(graph[i][j]/256/256,graph[i][j]/256%256,graph[i][j]%256));
                    SelectObject(hdc,hpen);
                    MoveToEx(hdc,j,i,NULL);
                    LineTo(hdc,j+1,i);
                    DeleteObject(hpen);
                }
            }
        }
        DeleteObject(hpen);
        hpen=CreatePen(PS_SOLID,1,RGB(0,0,0));
        SelectObject(hdc,hpen);
        MoveToEx(hdc,250,0,NULL);
        LineTo(hdc,250,500);
        MoveToEx(hdc,0,250,NULL);
        LineTo(hdc,500,250);
        MoveToEx(hdc,495,245,NULL);
        LineTo(hdc,500,250);
        LineTo(hdc,494,256);
        MoveToEx(hdc,245,5,NULL);
        LineTo(hdc,250,0);
        LineTo(hdc,256,6);
        for(int i=1;i<10;i++){
            MoveToEx(hdc,i*50,245,NULL);
            LineTo(hdc,i*50,256);
            MoveToEx(hdc,245,i*50,NULL);
            LineTo(hdc,256,i*50);
        }
        for(int i=1;i<50;i++){
            MoveToEx(hdc,i*10,248,NULL);
            LineTo(hdc,i*10,253);
            MoveToEx(hdc,248,i*10,NULL);
            LineTo(hdc,253,i*10);
        }
        DeleteObject(hfont);
        DeleteObject(hBlackBrush);
        DeleteObject(hWhiteBrush);
        DeleteObject(hpen);
        printUserListInterface(hwnd,equationList,nowListp,userPage);
        EndPaint(hwnd,&ps);
        ReleaseDC(hwnd,hdc);
        break;
    }
    /** \brief 왼쪽 마우스 누를 경우(테스트 케이스)
     *
     */
    case WM_LBUTTONDOWN:{
        int x=LOWORD(lParam),y=HIWORD(lParam);
        if(isItCal)break;
        else if(x<=500&&y<=500){
                /*
                if(strlen(str)<=0)break;
            HDC hdc=GetDC(hwnd);
            char dump[100000]={};
            strcpy(dump,str);
            char deletebuff[100000]={};
            memcpy(str,deletebuff,strlen(str));
            changeList(equationList,nowListp,dump);
            drawgraph(equationList,hdc);
            RECT rect={0,0,500,500};
            ReleaseDC(hwnd,hdc);
            InvalidateRect(hwnd,&rect,false);
            for(int i=0;i<128;i++)
                GetAsyncKeyState(i);
                */
        }
        else if(x>=500&&y>=400){
            int i=(x-500)/67,j=(y-400)/43;
            but[j][i].downed=1;
            //printf("%d %d\n",i,j);
            RECT rect={i*67+500,j*43+400,(i+1)*67+500,(j+1)*67+500};
            mousemove=1;
            InvalidateRect(hwnd,&rect,false);
        }
        else if(x>=450&&y>=670){
            if(but[4][4].calcul[2]=='=')
                but[4][4].calcul[2]='<';
            else if(but[4][4].calcul[2]=='<')
                but[4][4].calcul[2]='>';
            else if (but[4][4].calcul[2]=='>')
                but[4][4].calcul[2]='=';
            RECT rect={768,572,871,615};
            InvalidateRect(hwnd,&rect,false);
        }
        else if(x<=700&&y<20){
            if(userPage==0)break;
            nowListp=48*userPage--;
            RECT rect={500,20,700,400};
            InvalidateRect(hwnd,&rect,false);
            rect={0,640,54,660};
            InvalidateRect(hwnd,&rect,false);
        }
        else if(x<=700&&y>380&&y<400){
            if(checkList(equationList)-userPage*48<=48)break;
            nowListp=++userPage*48+1;
            RECT rect={500,20,700,400};
            InvalidateRect(hwnd,&rect,false);
            rect={0,640,54,660};
            InvalidateRect(hwnd,&rect,false);
        }
        else if(x<=700&&y>=20&&y<=380){
            int i=(x-500)/50,j=(y-20)/30;
            if(j*4+i+1>checkList(equationList))break;
            nowListp=userPage*48+j*4+i+1;
            RECT rect={500,20,700,400};
            InvalidateRect(hwnd,&rect,false);
            rect={0,600,500,630};
            InvalidateRect(hwnd,&rect,false);
            rect={0,640,54,660};
            InvalidateRect(hwnd,&rect,false);
        }
        else if(x>=450&&y<=600&&y>=570){
            int size=strlen(str);
            char dump[100000]={};
            strcpy(dump,str);
            if(size<=0)break;
            changeList(equationList,nowListp,str);
            char deletebuff[100000]={};
            memcpy(str,deletebuff,size);

            changeList(equationList,nowListp,dump);
            RECT rect={0,570,500,630};
            InvalidateRect(hwnd,&rect,false);
        }
        else if(x>=450&&y>=640&&y<=670){
            isItCal=1;
            HDC hdc=GetDC(hwnd);
            drawgraph(equationList,hdc);
            isItCal=0;
            RECT rect={0,0,500,500};
            ReleaseDC(hwnd,hdc);
            InvalidateRect(hwnd,&rect,false);
            for(int i=0;i<128;i++)
                GetAsyncKeyState(i);
            ReleaseDC(hwnd,hdc);
        }
        else if(x<=42&&y>=640&&y<=660){
            HDC hdc=GetDC(hwnd);
            HFONT hfont=CreateFont(18,0,0,0,5,0,0,0,ANSI_CHARSET,0,0,0,0,TEXT("Consolas"));
            SelectObject(hdc,hfont);
            SetBkColor(hdc,RGB(0,0,0));
            SetTextColor(hdc,RGB(255,255,255));
            char dumpString[2]={colorRGB[0]};
            int rgb=0;
            for(int j=0;j<10;j++){
                GetAsyncKeyState(j+0x30);
                GetAsyncKeyState(j+0x60);
                GetAsyncKeyState(j+65);
            }
            TextOut(hdc,0,641,dumpString,1);
            isItCal=1;
            for(int i=0;i<6;){
                for(int j=0;j<10;j++){
                    if(GetAsyncKeyState(j+0x30)&0x0001||GetAsyncKeyState(j+0x60)&0x0001){
                        rgb*=16;
                        rgb+=j;
                        colorRGB[i]=j+48;
                        dumpString[0]=colorRGB[i];
                        SetBkColor(hdc,RGB(255,255,255));
                        SetTextColor(hdc,RGB(0,0,0));
                        TextOut(hdc,i*9,641,dumpString,1);
                        i++;
                        if(i>5)break;
                        dumpString[0]=colorRGB[i];
                        SetBkColor(hdc,RGB(0,0,0));
                        SetTextColor(hdc,RGB(255,255,255));
                        TextOut(hdc,i*9,641,dumpString,1);
                        break;
                    }
                }
                for(int j=0;j<10;j++){
                    if(GetAsyncKeyState(j+65)&0x0001){
                        rgb*=16;
                        rgb+=j+10;
                        colorRGB[i]=j+65;
                        dumpString[0]=colorRGB[i];
                        SetBkColor(hdc,RGB(255,255,255));
                        SetTextColor(hdc,RGB(0,0,0));
                        TextOut(hdc,i*9,641,dumpString,1);
                        i++;
                        if(i>5)break;
                        dumpString[0]=colorRGB[i];
                        SetBkColor(hdc,RGB(0,0,0));
                        SetTextColor(hdc,RGB(255,255,255));
                        TextOut(hdc,i*9,641,dumpString,1);
                        break;
                    }
                }
                MSG msg;
                while(::PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE)){
                    ::SendMessage(msg.hwnd,msg.message,msg.wParam,msg.lParam);
                }
            }
            isItCal=0;
            DeleteObject(hfont);
            changeListColor(equationList,nowListp,rgb);
            RECT rect={0,640,54,660};
            InvalidateRect(hwnd,&rect,false);
            ReleaseDC(hwnd,hdc);
        }
        break;
    }
    case WM_MOUSEMOVE:{
        if(isItCal)break;
        if(mousemove==0)break;
        int x=LOWORD(lParam),y=HIWORD(lParam);
        if(x>=500&&y>=400){
            int i=(x-500)/67,j=(y-400)/43;
            if(doi==i&&doj==j)break;
            but[j][i].downed=1;
            but[doj][doi].downed=0;

            //printf("%d %d\n",i,j);
            RECT rect={i*67+500,j*43+400,(i+1)*67+500,(j+1)*67+500};
            InvalidateRect(hwnd,&rect,false);
            rect={doi*67+500,doj*43+400,(doi+1)*67+500,(doj+1)*67+500};
            InvalidateRect(hwnd,&rect,false);
            doj=j;
            doi=i;
        }
        break;
    }
    case WM_LBUTTONUP:{
        mousemove=0;
        if(isItCal)break;
        int x=LOWORD(lParam),y=HIWORD(lParam);
        if(x>=500&&y>=400){
            int i=(x-500)/67,j=(y-400)/43;
            but[j][i].downed=0;
            //printf("%d %d\n",i,j);
            RECT rect={i*67+500,j*43+400,(i+1)*67+500,(j+1)*67+500};
            InvalidateRect(hwnd,&rect,false);
            switch(j*10+i){
            case 10:{
                toggleTriangleFunction(' ',' ');
                RECT rect={634,443,835,529};
                InvalidateRect(hwnd,&rect,false);
                break;
            }
            case 11:{
                toggleTriangleFunction('a',' ');
                RECT rect={634,443,835,529};
                InvalidateRect(hwnd,&rect,false);
                break;
            }
            case 20:{
                toggleTriangleFunction('a',' ');
                RECT rect={634,443,835,529};
                InvalidateRect(hwnd,&rect,false);
                break;
            }
            case 21:{
                toggleTriangleFunction('a',' ');
                RECT rect={634,443,835,529};
                InvalidateRect(hwnd,&rect,false);
                break;
            }
            case 2:
            case 3:
            case 4:
            case 31:
            case 32:
            case 33:
            case 34:
            case 40:
            case 41:
            case 42:
            case 43:
            case 44:
            case 45:
            case 50:
            case 51:
            case 52:
            case 53:
            case 54:
            case 55:
            case 60:
            case 61:
            case 62:
            case 63:
            case 64:
            case 65:{
                char dump[7];
                sscanf(but[j][i].calcul,"%s",dump);
                strcat(str,dump);
                int size=strlen(str);
                rect={0,570,500,600};
                InvalidateRect(hwnd,&rect,false);
                break;
            }
            case 0:{
                int size=strlen(str);
                if(size<=0)break;
                str[--size]=0;
                rect={0,570,500,600};
                InvalidateRect(hwnd,&rect,false);
                break;
            }
            case 1:{
                int size=strlen(str);
                for(int k=0;k<size;k++)
                    str[k]=0;
                rect={0,570,500,600};
                InvalidateRect(hwnd,&rect,false);
                break;
            }
            case 30:{
                strcat(str,"m(");
                int size=strlen(str);
                rect={0,570,500,600};
                InvalidateRect(hwnd,&rect,false);
                break;
            }
            default:{
                char dump[7];
                sscanf(but[j][i].calcul,"%s",dump);
                strcat(dump,"(");
                strcat(str,dump);
                int size=strlen(str);
                rect={0,570,500,600};
                InvalidateRect(hwnd,&rect,false);
                break;
            }
            }
        }
        break;
    }
    /** \brief 키를 누른 경우
     *
     */
    case WM_KEYDOWN:{
        if(isItCal)break;
        if(GetAsyncKeyState(VK_CONTROL)<0){
            if(GetAsyncKeyState('N')&0x0001){
                createList(equationList);
                nowListp=checkList(equationList);
                userPage=(nowListp-1)/48;
                RECT rect={500,20,700,400};
                InvalidateRect(hwnd,&rect,false);
                rect={0,600,500,630};
                InvalidateRect(hwnd,&rect,false);
                strcpy(colorRGB,"FF0000");
                rect={0,640,54,660};
                InvalidateRect(hwnd,&rect,false);
            }
        }
        else if(GetAsyncKeyState(VK_SHIFT)<0){
            if(GetAsyncKeyState(0x30)&0x0001){
                insertStr(hwnd,str,")");
                break;
            }if(GetAsyncKeyState(0x31)&0x0001){
                insertStr(hwnd,str,"!");
                break;
            }if(GetAsyncKeyState(0x36)&0x0001){
                insertStr(hwnd,str,"^");
                break;
            }if(GetAsyncKeyState(0x38)&0x0001){
                insertStr(hwnd,str,"*");
                break;
            }if(GetAsyncKeyState(0x39)&0x0001){
                insertStr(hwnd,str,"(");
                break;
            }if(GetAsyncKeyState(0xBB)&0x0001){
                insertStr(hwnd,str,"+");
                break;
            }
            for(int i=0;i<10;i++)GetAsyncKeyState(0x30+i);
        }
        else if(GetAsyncKeyState('C')&0x0001){
            int size=strlen(str);
            while(size--){
                str[size]=0;
            }
            RECT rect={0,570,500,600};
            InvalidateRect(hwnd,&rect,false);
        }
        else if(GetAsyncKeyState(VK_BACK)&0x0001){
            int size=strlen(str);
            str[--size]=0;
            RECT rect={0,570,500,600};
            InvalidateRect(hwnd,&rect,false);
        }
        else if(GetAsyncKeyState('X')&0x0001){
            int size=strlen(str);
            str[size++]='x';
            RECT rect={0,570,500,600};
            InvalidateRect(hwnd,&rect,false);
        }
        else if(GetAsyncKeyState('Y')&0x0001){
            int size=strlen(str);
            str[size++]='y';
            RECT rect={0,570,500,600};
            InvalidateRect(hwnd,&rect,false);
        }
        else if(GetAsyncKeyState(VK_DELETE)&0x0001){
            deleteinEqList(equationList,nowListp);
            int temp=checkList(equationList);
            if(temp<nowListp)nowListp=temp;
            userPage=(nowListp-1)/48;
            RECT rect={500,20,700,400};
            InvalidateRect(hwnd,&rect,false);
            rect={0,600,500,630};
            InvalidateRect(hwnd,&rect,false);
            rect={0,640,54,660};
            InvalidateRect(hwnd,&rect,false);
        }
        else if(GetAsyncKeyState(VK_ESCAPE)<0){/**< esc >*/
            PostQuitMessage (0);
            return 0;
        }
        else if(GetAsyncKeyState(VK_MULTIPLY)&0x0001){
            insertStr(hwnd,str,"*");
        }
        else if(GetAsyncKeyState(VK_ADD)&0x0001){
            insertStr(hwnd,str,"+");
        }
        else if(GetAsyncKeyState(VK_DIVIDE)&0x0001||GetAsyncKeyState(0xBF)&0x0001){
            insertStr(hwnd,str,"/");;
        }
        else if(GetAsyncKeyState(VK_SUBTRACT)&0x0001||GetAsyncKeyState(0xBD)&0x0001){
            insertStr(hwnd,str,"-");
        }
        else if(GetAsyncKeyState(VK_DECIMAL)&0x0001||GetAsyncKeyState(0xBE)&0x0001){
            insertStr(hwnd,str,".");
        }
        else if(GetAsyncKeyState(0xBB)&0x0001){
            insertStr(hwnd,str,"=");
        }
        for(int i=0;i<10;i++){
            if(GetAsyncKeyState(i+0x30)&0x0001||GetAsyncKeyState(i+0x60)&0x0001){
                int size=strlen(str);
                str[size++]=i+0x30;
                RECT rect={0,570,500,600};
                InvalidateRect(hwnd,&rect,false);
                break;
            }
        }
        break;
    }
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
