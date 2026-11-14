#include <iostream>
#include <string>
using namespace std;

class Train {
public:
    int id;
    string name;
    string source;
    string destination;
    int departure;   // HHMM format, e.g., 1830 means 18:30
    int fare;
    int seats;

    void display() {
        int h = departure / 100;
        int m = departure % 100;
        cout << id << " | " << name << " | " << source << " to " << destination << " | ";
        if (h < 10) cout << "0";
        cout << h << ":";
        if (m < 10) cout << "0";
        cout << m << " | Rs " << fare << " | seats: " << seats << endl;
    }
};

void showAll(Train t[], int n) {
    for (int i = 0; i < n; i++) {
        t[i].display();
    }
}

// bubble sort, earliest departure first
// used bubble sort because number of trains is small, and it is easy to implement
void sortByDeparture(Train t[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (t[j].departure > t[j + 1].departure) {
                Train temp = t[j];
                t[j] = t[j + 1];
                t[j + 1] = temp;
            }
        }
    }
}

// bubble sort, cheapest fare first
void sortByFare(Train t[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (t[j].fare > t[j + 1].fare) {
                Train temp = t[j];
                t[j] = t[j + 1];
                t[j + 1] = temp;
            }
        }
    }
}

// linear search: check every train one by one, returns index or -1
int linearSearch(Train t[], int n, int searchId) {
    for (int i = 0; i < n; i++) {
        if (t[i].id == searchId) {
            return i;
        }
    }
    return -1;
}

// binary search: the array MUST be sorted by id, returns index or -1
int binarySearch(Train t[], int n, int searchId) {
    int low = 0;
    int high = n - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        cout << "  low=" << low << " high=" << high << " mid=" << mid
             << " (id " << t[mid].id << ")" << endl;
        if (t[mid].id == searchId) {
            return mid;
        } else if (t[mid].id < searchId) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

// show all trains that go from one station to another
void searchByRoute(Train t[], int n, string from, string to) {
    bool found = false;
    for (int i = 0; i < n; i++) {
        if (t[i].source == from && t[i].destination == to) {
            t[i].display();
            found = true;
        }
    }
    if (found == false) {
        cout << "No train found for this route" << endl;
    }
}

int main() {
    const int MAX = 10;
    Train trains[MAX];
    int count = 0;

    trains[count++] = {1, "Green Line", "Karachi", "Rawalpindi", 600, 3600, 3};
    trains[count++] = {2, "Tezgam", "Karachi", "Peshawar", 1830, 4500, 2};
    trains[count++] = {3, "Awam Express", "Lahore", "Peshawar", 915, 1800, 1};
    trains[count++] = {4, "Shalimar", "Multan", "Lahore", 745, 1000, 4};

    // searching first, while the trains are still in id order (1, 2, 3, 4)
    cout << "Linear search for id 3:" << endl;
    int pos = linearSearch(trains, count, 3);
    if (pos != -1) {
        trains[pos].display();
    }

    cout << endl << "Binary search for id 4:" << endl;
    pos = binarySearch(trains, count, 4);
    if (pos != -1) {
        cout << "Found at index " << pos << ": ";
        trains[pos].display();
    }

    cout << endl << "Binary search for id 7:" << endl;
    pos = binarySearch(trains, count, 7);
    if (pos == -1) {
        cout << "Train not found" << endl;
    }

    cout << endl << "Trains from Karachi to Peshawar:" << endl;
    searchByRoute(trains, count, "Karachi", "Peshawar");

    cout << endl << "Trains from Lahore to Karachi:" << endl;
    searchByRoute(trains, count, "Lahore", "Karachi");

    cout << endl << "Sorted by departure:" << endl;
    sortByDeparture(trains, count);
    showAll(trains, count);

    cout << endl << "Sorted by fare:" << endl;
    sortByFare(trains, count);
    showAll(trains, count);
    return 0;
}