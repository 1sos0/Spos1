#include <iostream>
#include <algorithm>
using namespace std;

void bestFit(int blockSize[], int m, int processSize[], int n, int remblockSize[]);
void firstFit(int blockSize[], int m, int processSize[], int n, int remblockSize[]);
void nextFit(int blockSize[], int m, int processSize[], int n, int remblockSize[]);
void worstFit(int blockSize[], int m, int processSize[], int n, int remblockSize[]);

void inputBlocksAndProcesses(int blockSize[], int &m, int processSize[], int &n) {
    cout << "Enter how many number of blocks you want to enter: ";
    cin >> m;
    for (int i = 0; i < m; i++) {
        cout << "Enter Data " << (i + 1) << ": ";
        cin >> blockSize[i];
    }
    cout << "Enter how many number of processes you want to enter: ";
    cin >> n;
    for (int i = 0; i < n; i++) {
        cout << "Enter Data " << (i + 1) << ": ";
        cin >> processSize[i];
    }
}

void copyArray(int source[], int destination[], int size) {
    for (int i = 0; i < size; i++) {
        destination[i] = source[i];
    }
}

int main() {
    int m, n, choice;
    int blockSize[100], processSize[100], remblockSize[100];
    int originalBlockSize[100];

    inputBlocksAndProcesses(blockSize, m, processSize, n);
    copyArray(blockSize, originalBlockSize, m);

    do {
        cout << "\nMemory Management Techniques\n";
        cout << "1. Best Fit\n";
        cout << "2. First Fit\n";
        cout << "3. Next Fit\n";
        cout << "4. Worst Fit\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        copyArray(originalBlockSize, blockSize, m); // Reset block sizes before each method

        switch (choice) {
            case 1:
                bestFit(blockSize, m, processSize, n, remblockSize);
                break;
            case 2:
                firstFit(blockSize, m, processSize, n, remblockSize);
                break;
            case 3:
                nextFit(blockSize, m, processSize, n, remblockSize);
                break;
            case 4:
                worstFit(blockSize, m, processSize, n, remblockSize);
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

void bestFit(int blockSize[], int m, int processSize[], int n, int remblockSize[]) {
    int allocation[n];
    for (int i = 0; i < n; i++) {
        allocation[i] = -1;
    }
    for (int i = 0; i < n; i++) {
        int bestIdx = -1;
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (bestIdx == -1)
                    bestIdx = j;
                else if (blockSize[bestIdx] > blockSize[j])
                    bestIdx = j;
            }
        }
        if (bestIdx != -1) {
            allocation[i] = bestIdx;
            blockSize[bestIdx] -= processSize[i];
            remblockSize[i] = blockSize[bestIdx];
        }
    }

    cout << "\nProcess No.\tProcess Size\tBlock no.\tRemaining Block Size\n";
    for (int i = 0; i < n; i++) {
        cout << " " << (i + 1) << "\t\t" << processSize[i] << "\t\t";
        if (allocation[i] != -1) {
            cout << (allocation[i] + 1) << "\t\t" << remblockSize[i];
        } else {
            cout << "Not Allocated" << "\t" << remblockSize[i];
        }
        cout << endl;
    }
}

void firstFit(int blockSize[], int m, int processSize[], int n, int remblockSize[]) {
    int allocation[n];
    for (int i = 0; i < n; i++) {
        allocation[i] = -1;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                allocation[i] = j;
                blockSize[j] -= processSize[i];
                remblockSize[i] = blockSize[j];
                break;
            }
        }
    }

    cout << "\nProcess No.\tProcess Size\tBlock no.\tRemaining Block Size\n";
    for (int i = 0; i < n; i++) {
        cout << " " << (i + 1) << "\t\t" << processSize[i] << "\t\t";
        if (allocation[i] != -1) {
            cout << (allocation[i] + 1) << "\t\t" << remblockSize[i];
        } else {
            cout << "Not Allocated" << "\t" << remblockSize[i];
        }
        cout << endl;
    }
}

void nextFit(int blockSize[], int m, int processSize[], int n, int remblockSize[]) {
    int allocation[n], j = 0;
    fill_n(allocation, n, -1);
    for (int i = 0; i < n; i++) {
        int count = 0;
        while (count < m) {
            count++;
            if (blockSize[j] >= processSize[i]) {
                allocation[i] = j;
                blockSize[j] -= processSize[i];
                remblockSize[i] = blockSize[j];
                break;
            }
            j = (j + 1) % m;
        }
    }

    cout << "\nProcess No.\tProcess Size\tBlock no.\tRemaining Block Size\n";
    for (int i = 0; i < n; i++) {
        cout << " " << (i + 1) << "\t\t" << processSize[i] << "\t\t";
        if (allocation[i] != -1) {
            cout << (allocation[i] + 1) << "\t\t" << remblockSize[i];
        } else {
            cout << "Not Allocated" << "\t" << remblockSize[i];
        }
        cout << endl;
    }
}

void worstFit(int blockSize[], int m, int processSize[], int n, int remblockSize[]) {
    int allocation[n];
    for (int i = 0; i < n; i++) {
        allocation[i] = -1;
    }
    for (int i = 0; i < n; i++) {
        int wstIdx = -1;
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (wstIdx == -1)
                    wstIdx = j;
                else if (blockSize[wstIdx] < blockSize[j])
                    wstIdx = j;
            }
        }
        if (wstIdx != -1) {
            allocation[i] = wstIdx;
            blockSize[wstIdx] -= processSize[i];
            remblockSize[i] = blockSize[wstIdx];
        }
    }

    cout << "\nProcess No.\tProcess Size\tBlock no.\tRemaining Block Size\n";
    for (int i = 0; i < n; i++) {
        cout << " " << (i + 1) << "\t\t" << processSize[i] << "\t\t";
        if (allocation[i] != -1) {
            cout << (allocation[i] + 1) << "\t\t" << remblockSize[i];
        } else {
            cout << "Not Allocated" << "\t" << remblockSize[i];
        }
        cout << endl;
    }
}