#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void optimalPageReplacement(int pages[], int noofpages, int capacity);
void leastRecentlyUsed(int pages[], int noofpages, int capacity);
void fifo(int pages[], int noofpages, int capacity);

void inputPagesAndCapacity(int pages[], int &noofpages, int &capacity) {
    cout << "Enter the number of pages you want to enter: ";
    cin >> noofpages;
    for (int i = 0; i < noofpages; i++) {
        cout << "Enter page " << (i + 1) << ": ";
        cin >> pages[i];
    }
    cout << "Enter the capacity of frame: ";
    cin >> capacity;
}

int main() {
    int noofpages, capacity, choice;
    int pages[100];

    inputPagesAndCapacity(pages, noofpages, capacity);

    do {
        cout << "\nPage Replacement Algorithms\n";
        cout << "1. Optimal Page Replacement\n";
        cout << "2. Least Recently Used (LRU)\n";
        cout << "3. FIFO\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                optimalPageReplacement(pages, noofpages, capacity);
                break;
            case 2:
                leastRecentlyUsed(pages, noofpages, capacity);
                break;
            case 3:
                fifo(pages, noofpages, capacity);
                break;
            case 4:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 4);

    return 0;
}

void optimalPageReplacement(int pages[], int noofpages, int capacity) {
    int frame[capacity];
    int table[noofpages][capacity];
    int ptr = 0, hit = 0, fault = 0;
    bool isFull = false;
    double hitRatio, faultRatio;

    fill_n(frame, capacity, -1);

    cout << "----------------------------------------------------------------------\n";
    for (int i = 0; i < noofpages; i++) {
        int search = -1;
        for (int j = 0; j < capacity; j++) {
            if (frame[j] == pages[i]) {
                search = j;
                hit++;
                cout << "  H";
                break;
            }
        }
        if (search == -1) {
            if (isFull) {
                int index[capacity];
                bool index_flag[capacity] = {false};
                fill_n(index, capacity, 0);
                for (int j = i + 1; j < noofpages; j++) {
                    for (int k = 0; k < capacity; k++) {
                        if ((pages[j] == frame[k]) && (!index_flag[k])) {
                            index[k] = j;
                            index_flag[k] = true;
                            break;
                        }
                    }
                }
                int max = index[0];
                ptr = 0;
                if (max == 0)
                    max = 200;
                for (int j = 0; j < capacity; j++) {
                    if (index[j] == 0)
                        index[j] = 200;
                    if (index[j] > max) {
                        max = index[j];
                        ptr = j;
                    }
                }
            }
            frame[ptr] = pages[i];
            fault++;
            cout << "  F";
            if (!isFull) {
                ptr++;
                if (ptr == capacity) {
                    ptr = 0;
                    isFull = true;
                }
            }
        }
        copy(frame, frame + capacity, table[i]);
    }
    cout << "\n----------------------------------------------------------------------\n";
    for (int i = 0; i < capacity; i++) {
        for (int j = 0; j < noofpages; j++)
            cout << table[j][i] << " ";
        cout << endl;
    }
    cout << "----------------------------------------------------------------------\n";
    hitRatio = ((double) hit / noofpages) * 100;
    faultRatio = ((double) fault / noofpages) * 100;
    cout << "Page Fault: " << fault << "\nPage Hit: " << hit << endl;
    cout << "Hit Ratio: " << hitRatio << " \nFault Ratio: " << faultRatio << endl;
}

void leastRecentlyUsed(int pages[], int noofpages, int capacity) {
    vector<int> arr;
    int frame[capacity];
    int table[noofpages][capacity];
    int hit = 0, fault = 0, index = 0;
    bool isFull = false;
    double hitRatio, faultRatio;

    fill_n(frame, capacity, -1);

    cout << "----------------------------------------------------------------------\n";
    for (int i = 0; i < noofpages; i++) {
        auto it = find(arr.begin(), arr.end(), pages[i]);
        if (it != arr.end()) {
            arr.erase(it);
        }
        arr.push_back(pages[i]);
        int search = -1;
        for (int j = 0; j < capacity; j++) {
            if (frame[j] == pages[i]) {
                search = j;
                hit++;
                cout << "  H";
                break;
            }
        }
        if (search == -1) {
            if (isFull) {
                int min_loc = noofpages;
                for (int j = 0; j < capacity; j++) {
                    auto it = find(arr.begin(), arr.end(), frame[j]);
                    if (it != arr.end()) {
                        int temp = distance(arr.begin(), it);
                        if (temp < min_loc) {
                            min_loc = temp;
                            index = j;
                        }
                    }
                }
            }
            frame[index] = pages[i];
            fault++;
            cout << "  F";
            index++;
            if (index == capacity) {
                index = 0;
                isFull = true;
            }
        }
        copy(frame, frame + capacity, table[i]);
    }
    cout << "\n----------------------------------------------------------------------\n";
    for (int i = 0; i < capacity; i++) {
        for (int j = 0; j < noofpages; j++)
            cout << table[j][i] << " ";
        cout << endl;
    }
    cout << "----------------------------------------------------------------------\n";
    hitRatio = ((double) hit / noofpages) * 100;
    faultRatio = ((double) fault / noofpages) * 100;
    cout << "Page Fault: " << fault << "\nPage Hit: " << hit << endl;
    cout << "Hit Ratio: " << hitRatio << " \nFault Ratio: " << faultRatio << endl;
}

void fifo(int pages[], int noofpages, int capacity) {
    int frame[capacity];
    int table[noofpages][capacity];
    int index = 0;
    int hit = 0, fault = 0;
    double faultRatio, hitRatio;

    fill_n(frame, capacity, -1);

    cout << "----------------------------------------------------------------------\n";
    for (int i = 0; i < noofpages; i++) {
        int search = -1;
        for (int j = 0; j < capacity; j++) {
            if (frame[j] == pages[i]) {
                search = j;
                hit++;
                cout << "  H";
                break;
            }
        }
        if (search == -1) {
            frame[index] = pages[i];
            fault++;
            cout << "  F";
            index++;
            if (index == capacity) {
                index = 0;
            }
        }
        copy(frame, frame + capacity, table[i]);
    }
    cout << "\n----------------------------------------------------------------------\n";
    for (int i = 0; i < capacity; i++) {
        for (int j = 0; j < noofpages; j++)
            cout << table[j][i] << " ";
        cout << endl;
    }
    cout << "----------------------------------------------------------------------\n";
    faultRatio = ((double) fault / noofpages) * 100;
    hitRatio = ((double) hit / noofpages) * 100;
    cout << "Page Fault: " << fault << "\nPage Hit: " << hit << endl;
    cout << "Hit Ratio: " << hitRatio << " \nFault Ratio: " << faultRatio << endl;
}