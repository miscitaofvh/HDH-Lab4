#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_BURST 2
#define SORT_BY_START 3

typedef struct {
    int iPID;
    int iArrival, iBurst;
    int iStart, iFinish, iWaiting, iResponse, iTaT;
} PCB;

void inputProcess(int n, PCB P[])
{
    for (int i = 0; i < n; ++i)
    {
        printf("Please input Process ID, Arrival Time, Burst Time of Process %d: ", i + 1);
        scanf("%d %d %d", &P[i].iPID, &P[i].iArrival, &P[i].iBurst);
        P[i].iStart = P[i].iFinish  = P[i].iResponse = P[i].iTaT = 0;
        P[i].iWaiting  = -P[i].iBurst; // P[i].iWaiting = P[i].iFinish - P[i].iArrival - P[i].iBurst
    }
}

void printProcess(int n, PCB P[])
{
    for (int i = 0; i < n; ++i)
    {
        printf("\nPID: %d\nArrival Time: %d\nBurst Time: %d\nStart Time: %d\nFinish Time: %d\nWaiting Time: %d\nResponse Time: %d\nTurnaround Time: %d\n", P[i].iPID, P[i].iArrival, P[i].iBurst, P[i].iStart, P[i].iFinish, P[i].iWaiting, P[i].iResponse, P[i].iTaT);
        printf("--------------------------------------\n");
    }
}
void exportGanttChart (int n, PCB P[])
{
    printf("\nGantt Chart\n");
    for (int i = 0; i < n; ++i)
    {
        if (i > 0 && P[i].iStart > 9) 
            printf("|  P%d ", P[i].iPID);
        else
            printf("| P%d ", P[i].iPID);
    }
    printf("|\n");
    for (int i = 0; i < n; ++i)
    {
        printf("%d    ", P[i].iStart);
    }
    printf("%d\n", P[n - 1].iFinish);
}
void pushProcess(int *n, PCB P[], PCB Q)
{
    P[*n] = Q;
    ++(*n);
}
void removeProcess(int *n, int index, PCB P[])
{
    for (int i = index; i < *n - 1; ++i)
    {
        P[i] = P[i + 1];
    }
    --(*n);
}
void swapProcess(PCB *P, PCB *Q)
{
    PCB temp = *P;
    *P = *Q;
    *Q = temp;
}

int partition (PCB P[], int low, int high, int iCriteria)
{
    if (iCriteria == 0)
    {
        int pivot = P[high].iArrival;
        int i = low - 1;
        for (int j = low; j <= high - 1; ++j)
        {
            if (P[j].iArrival < pivot)
            {
                ++i;
                swapProcess(&P[i], &P[j]);
            }
        }
        swapProcess(&P[i + 1], &P[high]);
        return i + 1;   
    }
    if (iCriteria == 1)
    {
        int pivot = P[high].iPID;
        int i = low - 1;
        for (int j = low; j <= high - 1; ++j)
        {
            if (P[j].iPID < pivot)
            {
                ++i;
                swapProcess(&P[i], &P[j]);
            }
        }
        swapProcess(&P[i + 1], &P[high]);
        return i + 1;
    }
    if (iCriteria == 2)
    {
        int pivot = P[high].iBurst;
        int i = low - 1;
        for (int j = low; j <= high - 1; ++j)
        {
            if (P[j].iBurst < pivot)
            {
                ++i;
                swapProcess(&P[i], &P[j]);
            }
        }
        swapProcess(&P[i + 1], &P[high]);
        return i + 1;
    }
    if (iCriteria == 3)
    {
        int pivot = P[high].iStart;
        int i = low - 1;
        for (int j = low; j <= high - 1; ++j)
        {
            if (P[j].iStart < pivot)
            {
                ++i;
                swapProcess(&P[i], &P[j]);
            }
        }
        swapProcess(&P[i + 1], &P[high]);
        return i + 1;
    }
    return -1;
}

void quickSort(PCB P[], int low, int high, int iCriteria)
{
    if (low < high)
    {
        int pi = partition(P, low, high, iCriteria);
        quickSort(P, low, pi - 1, iCriteria);
        quickSort(P, pi + 1, high, iCriteria);
    }
}
void calculateAWT(int n, PCB P[], int iNumberOfProcess)
{
    int iWT = 0;
    for (int i = 0; i < n; ++i)
    {
        bool ok = true;
        for (int j = i + 1; j < n; ++j)
        {
            if (P[i].iPID == P[j].iPID)
                ok = false;
        }
        if (ok)
        {
            P[i].iWaiting += P[i].iFinish - P[i].iArrival;
            //printf("Waiting Time of Process %d: %d\n", P[i].iPID, P[i].iWaiting);
            iWT += P[i].iWaiting; 
        }
    }

    //printf("\nTotal Waiting Time: %d", iWT);
    printf("\nAverage Waiting Time: %.2f\n", (float)iWT / iNumberOfProcess);
}
void calculateATaT(int n, PCB P[], int iNumberOfProcess)
{
    int iTaT = 0;
    for (int i = 0; i < n; ++i)
    {
        bool ok = true;
        for (int j = i + 1; j < n; ++j)
        {
            if (P[i].iPID == P[j].iPID)
                ok = false;
        }
        if (ok)
        {
            P[i].iTaT = P[i].iFinish - P[i].iArrival;
            //printf("Turnaround Time of Process %d: %d\n", P[i].iPID, P[i].iTaT);
            iTaT += P[i].iTaT; 
        }
    }
    printf("\nAverage Turnaround Time: %.2f\n", (float)iTaT / iNumberOfProcess);
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

int main()
{
    PCB Input[10];
    PCB ReadyQueue[10];
    PCB TerminatedArray[10];
    int iNumberOfProcess;
    int iQuantumTime;
    PCB vGanttChart[1411];
    int iGanttIndex = 0;
    int iTime = 0;
    printf("Please input Quantum Time: ");
    scanf("%d", &iQuantumTime);
    printf("Please input number of Process: ");
    scanf("%d", &iNumberOfProcess);
    int iRemain = iNumberOfProcess, iReady = 0, iTerminated = 0;
    inputProcess(iNumberOfProcess, Input);
    quickSort(Input, 0, iNumberOfProcess - 1, SORT_BY_ARRIVAL);
    pushProcess(&iReady, ReadyQueue, Input[0]);
    removeProcess(&iRemain, 0, Input);
    ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
    ReadyQueue[0].iFinish = ReadyQueue[0].iStart + min(ReadyQueue[0].iBurst, iQuantumTime);
    ReadyQueue[0].iBurst -= min(ReadyQueue[0].iBurst, iQuantumTime);
    pushProcess(&iGanttIndex, vGanttChart, ReadyQueue[0]);
    iTime = ReadyQueue[0].iFinish;
    printf("\nReady Queue: ");
    //printProcess(1, ReadyQueue);

    while (iTerminated < iNumberOfProcess)
    {
        while (iRemain > 0)
        {
            if (Input[0].iArrival <= iTime)
            {
                pushProcess(&iReady, ReadyQueue, Input[0]);
                removeProcess(&iRemain, 0, Input);
                continue;
            }
            else
                break;
        }
        if (iReady > 0)
        {   
            if (ReadyQueue[0].iBurst == 0)
            {
                pushProcess(&iTerminated, TerminatedArray, ReadyQueue[0]);
                removeProcess(&iReady, 0, ReadyQueue);
                
                if (iReady == 0)
                {
                    if (iRemain > 0)
                    {
                        pushProcess(&iReady, ReadyQueue, Input[0]);
                        removeProcess(&iRemain, 0, Input);
                        ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
                        ReadyQueue[0].iFinish = ReadyQueue[0].iStart + min(ReadyQueue[0].iBurst, iQuantumTime);
                        ReadyQueue[0].iBurst -= min(ReadyQueue[0].iBurst, iQuantumTime);
                        iTime = ReadyQueue[0].iFinish;
                        pushProcess(&iGanttIndex, vGanttChart, ReadyQueue[0]);
                    }
                }
                else
                {
                    ReadyQueue[0].iStart = iTime;
                    ReadyQueue[0].iFinish = ReadyQueue[0].iStart + min(ReadyQueue[0].iBurst, iQuantumTime);
                    ReadyQueue[0].iBurst -= min(ReadyQueue[0].iBurst, iQuantumTime);
                    iTime = ReadyQueue[0].iFinish;
                    pushProcess(&iGanttIndex, vGanttChart, ReadyQueue[0]);
                }
            }
            else
            {
                pushProcess(&iReady, ReadyQueue, ReadyQueue[0]);
                removeProcess(&iReady, 0, ReadyQueue);
                ReadyQueue[0].iStart = iTime;
                ReadyQueue[0].iFinish = ReadyQueue[0].iStart + min(ReadyQueue[0].iBurst, iQuantumTime);
                ReadyQueue[0].iBurst -= min(ReadyQueue[0].iBurst, iQuantumTime);
                iTime = ReadyQueue[0].iFinish;
                pushProcess(&iGanttIndex, vGanttChart, ReadyQueue[0]);
            }
        }
    }
    printf("\n===== RR Scheduling =====\n");
    exportGanttChart(iGanttIndex, vGanttChart);
    quickSort(TerminatedArray, 0, iTerminated - 1, SORT_BY_PID);
    calculateAWT(iGanttIndex, vGanttChart, iNumberOfProcess);
    calculateATaT(iGanttIndex, vGanttChart, iNumberOfProcess);

    return 0;
}