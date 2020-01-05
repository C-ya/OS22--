#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

#define maxapJ 10
#define proNum 3 //进程数

const int zyNum = 10; //资源数
int option; //0-采用死锁避免  1-不用
int Ava;    //定义可用资源向量Available
bool ADVANCE;//t-必然死锁

struct COP
{
    int id;
    int claim[maxapJ]; //动态申请资源数组
    int maxClaim;      //最大资源需求量
    int len = 0;       //申请释放次数
    int allo;          //进程已占资源
    int vps;    //进程完成请求标志，为 0 时表示已完成各次请求
    int vcount; //进程请求次数计数器。其值表示该进程的第几次请求
    int lock;//死锁位 1-产生死锁
} COP[proNum + 1];

int Safe(int in, int Ava)
{int k=0;
    int a = Ava, i = in;
    int p[proNum + 1];
    int v[proNum + 1];
    for (int j = 1; j <= proNum; j++)
    {
        p[j] = COP[j].vps;
        v[j] = COP[j].vcount + 1;
    }
    while (i <= proNum && p[i] == 1)
    {

        if (a >= COP[i].claim[v[i]])
        {
            a -= COP[i].claim[v[i]];
           // cout << endl << i << "    " << a << "     " << v[i] << endl;
            v[i]++;
        }
        else
        {
            return -1; //不安全
        }
        if (v[i] == COP[i].len)
            p[i] = 0;
        i++;
        if(i > proNum)
            i -= proNum;
    }
    return 1; //安全
}
void Bank()
{
    int i = 1, j = 1, c = 1, num = 0, lo = 0;
    Ava = zyNum;
    while (i <= proNum && COP[i].vps == 1 && COP[i].lock == 0)
    {
        if (Ava < COP[i].claim[COP[i].vcount + 1])
        {
            cout << "\nTHE REMAINDER IS LESS THAN PROCESS " << i << " CLAIMS";
            i++;
        }
        else
        {
            cout << "\n(" << c++ << ")";
            //假定分配资源
            int v = COP[i].vcount + 1;
            int a = COP[i].allo + COP[i].claim[v];
            int r = Ava - COP[i].claim[v];
            cout << setw(10) << i << setw(15) << COP[i].claim[v]
                 << setw(15) << a << setw(15) << r;
            if (v == COP[i].len)
            {
                COP[i].vps = 0; //进程完成所有请求
                num++;
                cout << "\nPROCESS " << i << " HAS FINISHED, RETURN ITS RESOURSE";
                if (num == proNum)
                {
                    cout << "\nTHE WHOLE WORK IS COMPLETED";
                    break;
                }
            }
            if (Safe(i, Ava) == 1)
            {//安全则分配资源
                cout << "\nRESOURCE IS ALLOCATED TO PROCESS " << i;
                COP[i].vcount = v;
                COP[i].allo = a;
                Ava = r;
                i++;
                for(int x=1; x<=proNum; x++)
                    COP[x].lock = 0;//安全则解除各进程死锁位
            }
            else if (Safe(i, Ava) == -1)
            {
                COP[i].lock=1;
                lo++;
                if(lo == proNum)
                {
                    cout << "\nDEADLOCK MUST OCCUR";
                    break;
                }
                cout << "\nIF ALLOCATED,DEADLOCK MAY OCCUR";
                for(int x=1; x<=proNum; x++){//从头寻找未完成且不会发生死锁的进程
                    if(COP[x].vps != 0 && COP[x].lock != 1){
                        i = x;
                        break;
                    }
                }
            }
        }
        if (i > proNum)
            i -= proNum;
    }
}

void Deal()
{
    int i = 1, j = 1, c = 1, num = 0;
    Ava = zyNum;
    while (i <= proNum && COP[i].vps == 1)
    {
        if (Ava < COP[i].claim[COP[i].vcount + 1])
        {
            cout << "\nTHE REMAINDER IS LESS THAN PROCESS " << i << " CLAIMS";
            break;
        }
        else
        {
            cout << "\n(" << c++ << ")";
            int v = COP[i].vcount + 1;
            int a = COP[i].allo + COP[i].claim[v];
            int r = Ava - COP[i].claim[v];
            cout << setw(10) << i << setw(15) << COP[i].claim[v]
                 << setw(15) << a << setw(15) << r;
            cout << "\nRESOURCE IS ALLOCATED TO PROCESS " << i;
            COP[i].vcount = v;
            COP[i].allo = a;
            Ava = r;
            if (COP[i].vcount == COP[i].len)
            {
                COP[i].vps = 0; //进程完成所有请求
                num++;
                cout << "\nPROCESS " << i << " HAS FINISHED, RETURN ITS RESOURSE";
                if (num == proNum)
                {
                    cout << "\nTHE WHOLE WORK IS COMPLETED";
                    break;
                }
            }
            i++;
        }

        if (i > proNum)
            i -= proNum;
    }
}

void init()
{
    for (int i = 1; i <= proNum; i++)
    {
        cout << "CLAIM OF PROCESS " << i << " IS: ";
        int maxc = 0;
        COP[i].id = i;
        COP[i].allo = 0;
        COP[i].vps = 1;
        COP[i].vcount = 0;
        COP[i].lock = 0;
        for (int j = 1; j <= maxapJ; j++)
        {
            cin >> COP[i].claim[j];
            COP[i].len++;
            if (COP[i].claim[j] > 0)
                maxc += COP[i].claim[j];
            if (COP[i].claim[j] == 0)
                break;
        }
        COP[i].maxClaim = maxc;
    }
    Ava = zyNum;
}

void printInit()
{
    bool flag = true;
    for (int i = 1; i <= proNum; i++)
    {
        if (COP[i].maxClaim > zyNum)
        {
            flag = false;
            break;
        }
    }
    if (flag == true)
    {
        for (int i = 1; i <= proNum; i++)
            cout << "MAXCLAIM OF PROCESS " << i << " IS: " << COP[i].maxClaim << endl;
        cout << "THE SYSTEM ALLOCTION PROCESS IS AS FOLLOWS:\n";
        cout << "           PROCESS       CLAIM       ALLOCATION        REMAINDER\n";
    }
    else
    {
        cout << "ERROR!\n";
    }
}

int main()
{
    cout << "INPUT(Ava= " << zyNum << " ):\nOPTION= ";
    cin >> option;
    while (option != -1)
    {
        init();
        printInit();
        if (option == 0)
            Bank();
        else
            Deal();
        cout << "\n*******************************************";
        system("pause");
        cout << "\nINPUT:\nOPTION= ";
        cin >> option;
    }
    return 0;
}
