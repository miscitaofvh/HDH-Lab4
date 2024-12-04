#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
        //printf("Please input Process ID, Arrival Time, Burst Time of Process %d: ", i + 1);
        //scanf("%d %d %d", &P[i].iPID, &P[i].iArrival, &P[i].iBurst);
        P[i].iPID = i + 1;
        P[i].iArrival = rand() % 21;
        P[i].iBurst = rand() % 11 + 2;
        P[i].iStart = P[i].iFinish = P[i].iWaiting = P[i].iResponse = P[i].iTaT = 0;
        printf("Process ID, Arrival Time, Burst Time of Process %d: %d %d %d\n", i + 1, P[i].iPID, P[i].iArrival, P[i].iBurst);
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
void calculateAWT(int n, PCB P[])
{
    int iAWT = 0;
    for (int i = 0; i < n; ++i)
    {
        iAWT += P[i].iWaiting;
    }
    printf("\nAverage Waiting Time: %.2f\n", (float)iAWT / n);
}
void calculateATaT(int n, PCB P[])
{
    int iTaT = 0;
    for (int i = 0; i < n; ++i)
    {
        iTaT += P[i].iTaT;
    }
    printf("\nAverage Turnaround Time: %.2f\n", (float)iTaT / n);
}

int main()
{
    srand(time(NULL));
    PCB Input[10];
    PCB ReadyQueue[10];
    PCB TerminatedArray[10];
    int iNumberOfProcess;
    printf("Please input number of Process: ");
    scanf("%d", &iNumberOfProcess);
    int iRemain = iNumberOfProcess, iReady = 0, iTerminated = 0;
    inputProcess(iNumberOfProcess, Input);
    quickSort(Input, 0, iNumberOfProcess - 1, SORT_BY_ARRIVAL);
    pushProcess(&iReady, ReadyQueue, Input[0]);
    removeProcess(&iRemain, 0, Input);
    ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
    ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
    ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
    ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
    ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival; 
    printf("\nReady Queue: ");
    printProcess(1, ReadyQueue);
    while (iTerminated < iNumberOfProcess)
    {
        while (iRemain > 0)
        {
            if (Input[0].iArrival <= ReadyQueue[0].iFinish)
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
            pushProcess(&iTerminated, TerminatedArray, ReadyQueue[0]);
            removeProcess(&iReady, 0, ReadyQueue);
            ReadyQueue[0].iStart = TerminatedArray[iTerminated - 1].iFinish;
            ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
            ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
            ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
            ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;
        }
        if (iRemain > 0 && iReady == 0)
        {
            pushProcess(&iReady, ReadyQueue, Input[0]);
            removeProcess(&iRemain, 0, Input);
            ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
            ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
            ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
            ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
            ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;
        }
    }
    printf("\n===== FCFS Scheduling =====\n");
    exportGanttChart(iTerminated, TerminatedArray);
    quickSort(TerminatedArray, 0, iTerminated - 1, SORT_BY_PID);
    calculateAWT(iTerminated, TerminatedArray);
    calculateATaT(iTerminated, TerminatedArray);

    return 0;
}