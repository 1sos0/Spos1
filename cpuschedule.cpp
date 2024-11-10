#include <iostream>
#include <queue>
#include <vector>
using namespace std;

struct Process {
    int id;
    int arrivalTime;
    int cpuBurst1;
    int ioBurst;
    int cpuBurst2;
    int remainingTime1;
    int remainingTime2;
    int completionTime;
    int waitingTime;
    int turnaroundTime;
    int priority; // Added priority member
    bool finished;
    bool ioCompleted;
};

void inputProcesses(Process processes[], int n) {
    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        cout << "Enter arrival time for process " << (i + 1) << ": ";
        cin >> processes[i].arrivalTime;
        cout << "Enter first CPU burst time for process " << (i + 1) << ": ";
        cin >> processes[i].cpuBurst1;
        cout << "Enter I/O burst time for process " << (i + 1) << ": ";
        cin >> processes[i].ioBurst;
        cout << "Enter second CPU burst time for process " << (i + 1) << ": ";
        cin >> processes[i].cpuBurst2;
        processes[i].remainingTime1 = processes[i].cpuBurst1;
        processes[i].remainingTime2 = processes[i].cpuBurst2;
        processes[i].finished = false;
        processes[i].ioCompleted = false;
    }
}

void inputPriorities(Process processes[], int n) {
    for (int i = 0; i < n; i++) {
        cout << "Enter priority for process " << (i + 1) << ": ";
        cin >> processes[i].priority;
    }
}

void printTable(Process processes[], int n, float avgwt, float avgta) {
    cout << "\npid  arrival  burst  complete turn waiting\n";
    for (int i = 0; i < n; i++) {
        int totalBurst = processes[i].cpuBurst1 + processes[i].ioBurst + processes[i].cpuBurst2;
        cout << processes[i].id << "  \t " << processes[i].arrivalTime << "\t" << totalBurst << "\t" << processes[i].completionTime << "\t" << processes[i].turnaroundTime << "\t" << processes[i].waitingTime << "\n";
    }
    cout << "\nAverage waiting time: " << avgwt << "\n";
    cout << "Average turnaround time: " << avgta << "\n";
}

void FCFS(Process processes[], int n) {
    int ct[n], ta[n], wt[n], temp;
    float avgwt = 0, avgta = 0;

    // Sorting according to arrival times
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - (i + 1); j++) {
            if (processes[j].arrivalTime > processes[j + 1].arrivalTime) {
                swap(processes[j], processes[j + 1]);
            }
        }
    }

    // Finding completion times
    for (int i = 0; i < n; i++) {
        if (i == 0) {
            processes[i].completionTime = processes[i].arrivalTime + processes[i].cpuBurst1 + processes[i].ioBurst + processes[i].cpuBurst2;
        } else {
            if (processes[i].arrivalTime > processes[i - 1].completionTime) {
                processes[i].completionTime = processes[i].arrivalTime + processes[i].cpuBurst1 + processes[i].ioBurst + processes[i].cpuBurst2;
            } else {
                processes[i].completionTime = processes[i - 1].completionTime + processes[i].cpuBurst1 + processes[i].ioBurst + processes[i].cpuBurst2;
            }
        }
        processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime; // Turnaround time = completion time - arrival time
        processes[i].waitingTime = processes[i].turnaroundTime - (processes[i].cpuBurst1 + processes[i].ioBurst + processes[i].cpuBurst2);   // Waiting time = turnaround time - burst time
        avgwt += processes[i].waitingTime;                           // Total waiting time
        avgta += processes[i].turnaroundTime;                           // Total turnaround time
    }

    printTable(processes, n, avgwt / n, avgta / n);
}

void SJF_Preemptive(Process processes[], int n) {
    int f[n] = {0}, k1[n], k2[n], tot = 0, st = 0;
    float avgwt = 0, avgta = 0;

    for (int i = 0; i < n; i++) {
        k1[i] = processes[i].cpuBurst1;
        k2[i] = processes[i].cpuBurst2;
    }

    while (true) {
        int min = 99, c = n;
        if (tot == n)
            break;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= st && f[i] == 0 && processes[i].remainingTime1 < min) {
                min = processes[i].remainingTime1;
                c = i;
            }
        }

        if (c == n) {
            st++;
        } else {
            processes[c].remainingTime1--;
            st++;
            if (processes[c].remainingTime1 == 0) {
                processes[c].completionTime = st + processes[c].ioBurst + processes[c].cpuBurst2;
                f[c] = 1;
                tot++;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
        processes[i].waitingTime = processes[i].turnaroundTime - (k1[i] + processes[i].ioBurst + k2[i]);
        avgwt += processes[i].waitingTime;
        avgta += processes[i].turnaroundTime;
    }

    printTable(processes, n, avgwt / n, avgta / n);
}

void Priority_NonPreemptive(Process processes[], int n) {
    int temp;
    float avgwt = 0, avgta = 0;

    inputPriorities(processes, n); // Ask for priorities here

    // Sorting according to arrival times and priority
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (processes[i].arrivalTime > processes[j].arrivalTime || (processes[i].arrivalTime == processes[j].arrivalTime && processes[i].priority > processes[j].priority)) {
                swap(processes[i], processes[j]);
            }
        }
    }

    int totalTime = 0, tLap = 0, minIndex = 0, currentIndex = 0;
    for (int i = 0; i < n; i++) {
        totalTime += processes[i].cpuBurst1 + processes[i].ioBurst + processes[i].cpuBurst2;
    }

    for (int i = 0; i < n; i++) {
        if (processes[i].arrivalTime < processes[minIndex].arrivalTime) {
            minIndex = i;
            currentIndex = i;
        }
    }

    totalTime = processes[minIndex].arrivalTime + totalTime;
    tLap = processes[minIndex].arrivalTime;

    while (tLap < totalTime) {
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= tLap && processes[i].priority < processes[minIndex].priority) {
                minIndex = i;
                currentIndex = i;
            }
        }
        tLap += processes[currentIndex].cpuBurst1 + processes[currentIndex].ioBurst + processes[currentIndex].cpuBurst2;
        processes[currentIndex].completionTime = tLap;
        processes[currentIndex].priority = 9999;
    }

    for (int i = 0; i < n; i++) {
        processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
        processes[i].waitingTime = processes[i].turnaroundTime - (processes[i].cpuBurst1 + processes[i].ioBurst + processes[i].cpuBurst2);
        avgwt += processes[i].waitingTime;
        avgta += processes[i].turnaroundTime;
    }

    printTable(processes, n, avgwt / n, avgta / n);
}

void RoundRobin(Process processes[], int n, int quantum) {
    int sumBurst = 0, time = 0;
    float avgwt = 0, avgta = 0;
    queue<int> q;
    vector<bool> inQueue(n, false);

    for (int i = 0; i < n; i++) {
        sumBurst += processes[i].cpuBurst1 + processes[i].ioBurst + processes[i].cpuBurst2;
    }

    q.push(0);
    inQueue[0] = true;

    for (time = processes[0].arrivalTime; time < sumBurst;) {
        int i = q.front();
        q.pop();
        inQueue[i] = false;

        if (processes[i].remainingTime1 > 0) {
            if (processes[i].remainingTime1 <= quantum) {
                time += processes[i].remainingTime1;
                processes[i].remainingTime1 = 0;
                processes[i].ioCompleted = true;
                processes[i].completionTime = time + processes[i].ioBurst + processes[i].cpuBurst2;
            } else {
                time += quantum;
                processes[i].remainingTime1 -= quantum;
            }
        } else if (processes[i].ioCompleted && processes[i].remainingTime2 > 0) {
            if (processes[i].remainingTime2 <= quantum) {
                time += processes[i].remainingTime2;
                processes[i].remainingTime2 = 0;
                processes[i].finished = true;
                processes[i].completionTime = time;
            } else {
                time += quantum;
                processes[i].remainingTime2 -= quantum;
            }
        }

        for (int j = 0; j < n; j++) {
            if (processes[j].arrivalTime <= time && !processes[j].finished && !inQueue[j]) {
                q.push(j);
                inQueue[j] = true;
            }
        }

        if (!processes[i].finished && !inQueue[i]) {
            q.push(i);
            inQueue[i] = true;
        }
    }

    for (int i = 0; i < n; i++) {
        processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
        processes[i].waitingTime = processes[i].turnaroundTime - (processes[i].cpuBurst1 + processes[i].ioBurst + processes[i].cpuBurst2);
        avgwt += processes[i].waitingTime;
        avgta += processes[i].turnaroundTime;
    }

    printTable(processes, n, avgwt / n, avgta / n);
}

int main() {
    int n, choice, quantum;
    cout << "Enter number of processes: ";
    cin >> n;
    Process processes[n];
    inputProcesses(processes, n);

    do {
        cout << "\nCPU Scheduling Algorithms\n";
        cout << "1. FCFS\n";
        cout << "2. SJF (Preemptive)\n";
        cout << "3. Priority (Non-Preemptive)\n";
        cout << "4. Round Robin (Preemptive)\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                FCFS(processes, n);
                break;
            case 2:
                SJF_Preemptive(processes, n);
                break;
            case 3:
                Priority_NonPreemptive(processes, n);
                break;
            case 4:
                cout << "Enter time quantum: ";
                cin >> quantum;
                RoundRobin(processes, n, quantum);
                break;
            case 5:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}