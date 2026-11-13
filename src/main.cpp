#include <iostream>
#include <string>
using namespace std;

class Train {
public:
    int id;
    string name;
    string source;
    string destination;
    int departure;   // HHMM format: 1830 means 18:30
    int fare;
    int seats;

    void display() {
        int h = departure / 100;
        int m = departure % 100;
        cout << id << " | " << name << " | " << source << " to " << destination
             << " | " << (h < 10 ? "0" : "") << h << ":" << (m < 10 ? "0" : "") << m
             << " | Rs " << fare << " | seats: " << seats << endl;
    }
};

int main() {
    const int MAX = 10;
    Train trains[MAX];
    int count = 0;

    trains[count++] = {1, "Green Line", "Karachi", "Rawalpindi", 600, 3600, 3};
    trains[count++] = {2, "Tezgam", "Karachi", "Peshawar", 1830, 4500, 2};
    trains[count++] = {3, "Awam Express", "Lahore", "Peshawar", 915, 1800, 1};
    trains[count++] = {4, "Shalimar", "Multan", "Lahore", 745, 1000, 4};

    cout << "All trains (" << count << "):" << endl;
    for (int i = 0; i < count; i++) {
        trains[i].display();
    }
    return 0;
}