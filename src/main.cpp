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
// one node of the linked list = one ticket
class Ticket {
public:
    int pnr;
    string passengerName;
    int trainId;
    string source;
    string destination;
    int fare;
    string status;      // CONFIRMED or WAITING
    Ticket* next;
};

class TicketList {
public:
    Ticket* head;

    TicketList() {
        head = NULL;
    }

    // add a new ticket at the end of the list
    void addTicket(int pnr, string name, int trainId, string source, string destination, int fare, string status) {
        Ticket* newTicket = new Ticket;
        newTicket->pnr = pnr;
        newTicket->passengerName = name;
        newTicket->trainId = trainId;
        newTicket->source = source;
        newTicket->destination = destination;
        newTicket->fare = fare;
        newTicket->status = status;
        newTicket->next = NULL;

        if (head == NULL) {
            head = newTicket;
        } else {
            Ticket* temp = head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newTicket;
        }
    }
    // print every ticket from head to the end
    void display() {
        if (head == NULL) {
            cout << "No tickets" << endl;
            return;
        }
        Ticket* temp = head;
        while (temp != NULL) {
            cout << "PNR " << temp->pnr << " | " << temp->passengerName
                 << " | Train " << temp->trainId << " | " << temp->source
                 << " to " << temp->destination << " | Rs " << temp->fare
                 << " | " << temp->status << endl;
            temp = temp->next;
        }
    }

    // find a ticket by PNR, returns NULL if not found
    Ticket* findByPnr(int pnr) {
        Ticket* temp = head;
        while (temp != NULL) {
            if (temp->pnr == pnr) {
                return temp;
            }
            temp = temp->next;
        }
        return NULL;
    }

    // remove a ticket by PNR, returns true if it was removed
    bool removeByPnr(int pnr) {
        if (head == NULL) {
            return false;
        }
        // case 1: the ticket is the first one
        if (head->pnr == pnr) {
            Ticket* toDelete = head;
            head = head->next;
            delete toDelete;
            return true;
        }
        // case 2: the ticket is somewhere after the first
        Ticket* prev = head;
        Ticket* cur = head->next;
        while (cur != NULL) {
            if (cur->pnr == pnr) {
                prev->next = cur->next;
                delete cur;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false;
    }
};
// one node in the waiting queue
class QueueNode {
public:
    int pnr;
    QueueNode* next;
};

// FIFO queue for the waiting list of each train
// passengers who cant get a seat join the back of the queue
// when a seat opens up, the passenger at the front gets it
class WaitingQueue {
public:
    QueueNode* front;
    QueueNode* rear;
    int size;

    WaitingQueue() {
        front = NULL;
        rear = NULL;
        size = 0;
    }

    bool isEmpty() {
        return front == NULL;
    }

    // add a passenger's PNR to the back of the queue
    void enqueue(int pnr) {
        QueueNode* newNode = new QueueNode;
        newNode->pnr = pnr;
        newNode->next = NULL;

        if (rear == NULL) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        size++;
    }

    // remove and return the PNR at the front of the queue
    int dequeue() {
        if (isEmpty()) {
            return -1;
        }
        QueueNode* temp = front;
        int pnr = temp->pnr;
        front = front->next;
        if (front == NULL) {
            rear = NULL;
        }
        delete temp;
        size--;
        return pnr;
    }

    // see the front PNR without removing it
    int peek() {
        if (isEmpty()) {
            return -1;
        }
        return front->pnr;
    }

    // print all PNRs in the queue from front to back
    void display() {
        if (isEmpty()) {
            cout << "  (empty)" << endl;
            return;
        }
        QueueNode* temp = front;
        int pos = 1;
        while (temp != NULL) {
            cout << "  " << pos << ". PNR " << temp->pnr << endl;
            temp = temp->next;
            pos++;
        }
    }

    // remove a specific PNR from anywhere in the queue
    // needed when a waiting passenger cancels their ticket
    bool remove(int pnr) {
        if (isEmpty()) return false;

        if (front->pnr == pnr) {
            dequeue();
            return true;
        }

        QueueNode* prev = front;
        QueueNode* cur = front->next;
        while (cur != NULL) {
            if (cur->pnr == pnr) {
                prev->next = cur->next;
                if (cur == rear) {
                    rear = prev;
                }
                delete cur;
                size--;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false;
    }
};

// show the waiting list for every train
void showWaitingLists(Train trains[], int trainCount, WaitingQueue waitQueues[]) {
    cout << "\n--- Waiting Lists ---" << endl;
    for (int i = 0; i < trainCount; i++) {
        cout << trains[i].name << " (ID " << trains[i].id << "):" << endl;
        waitQueues[i].display();
    }
}

// book a ticket for a train
// the & means the original list and counter are changed, not copies
void bookTicket(Train trains[], int trainCount, TicketList &list, int &nextPnr, WaitingQueue waitQueues[]) {
    int id;
    string name;
    cout << "Enter train id: ";
    cin >> id;

    int pos = linearSearch(trains, trainCount, id);
    if (pos == -1) {
        cout << "Train not found" << endl;
        return;
    }

    cout << "Enter passenger name: ";
    cin.ignore();
    getline(cin, name);

    string status;
    if (trains[pos].seats > 0) {
        trains[pos].seats--;
        status = "CONFIRMED";
    } else {
        status = "WAITING";
    }

    list.addTicket(nextPnr, name, id, trains[pos].source, trains[pos].destination, trains[pos].fare, status);

    if (status == "WAITING") {
        waitQueues[pos].enqueue(nextPnr);
        cout << "Train is full. You have been added to the waiting list." << endl;
        cout << "Your position in queue: " << waitQueues[pos].size << endl;
    }

    cout << "Ticket booked. Your PNR is " << nextPnr << " (" << status << ")" << endl;
    nextPnr++;
}

// cancel a ticket using its PNR
void cancelTicket(Train trains[], int trainCount, TicketList &list, WaitingQueue waitQueues[]) {
    int pnr;
    cout << "Enter PNR: ";
    cin >> pnr;

    Ticket* t = list.findByPnr(pnr);
    if (t == NULL) {
        cout << "Ticket not found" << endl;
        return;
    }

    if (t->status == "CONFIRMED") {
        int pos = linearSearch(trains, trainCount, t->trainId);

        // check if someone is waiting in the queue for this train
        if (!waitQueues[pos].isEmpty()) {
            // promote the front of the waiting list to confirmed
            int promotedPnr = waitQueues[pos].dequeue();
            Ticket* promoted = list.findByPnr(promotedPnr);
            if (promoted != NULL) {
                promoted->status = "CONFIRMED";
                cout << "PNR " << promotedPnr << " has been promoted from waiting list to CONFIRMED" << endl;
            }
        } else {
            // no one waiting, seat goes back to the train
            trains[pos].seats++;
        }
    } else if (t->status == "WAITING") {
        // remove from the waiting queue
        int pos = linearSearch(trains, trainCount, t->trainId);
        waitQueues[pos].remove(pnr);
    }

    list.removeByPnr(pnr);
    cout << "Ticket " << pnr << " cancelled" << endl;
}

int main() {
    const int MAX = 10;
    Train trains[MAX];
    int count = 0;
    TicketList tickets;
    int nextPnr = 1001;
    WaitingQueue waitQueues[MAX];

    trains[count++] = {1, "Green Line", "Karachi", "Rawalpindi", 600, 3600, 3};
    trains[count++] = {2, "Tezgam", "Karachi", "Peshawar", 1830, 4500, 2};
    trains[count++] = {3, "Awam Express", "Lahore", "Peshawar", 915, 1800, 1};
    trains[count++] = {4, "Shalimar", "Multan", "Lahore", 745, 1000, 4};

    int choice = -1;
    while (choice != 0) {
        cout << endl;
        cout << "===== Railway Reservation System =====" << endl;
        cout << "1. View all trains" << endl;
        cout << "2. Sort trains by departure time" << endl;
        cout << "3. Sort trains by fare" << endl;
        cout << "4. Search trains by route" << endl;
        cout << "5. Book a ticket" << endl;
        cout << "6. View all tickets" << endl;
        cout << "7. Cancel a ticket" << endl;
        cout << "8. View waiting lists" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            showAll(trains, count);
        } else if (choice == 2) {
            sortByDeparture(trains, count);
            showAll(trains, count);
        } else if (choice == 3) {
            sortByFare(trains, count);
            showAll(trains, count);
        } else if (choice == 4) {
            string from, to;
            cout << "From: ";
            cin >> from;
            cout << "To: ";
            cin >> to;
            searchByRoute(trains, count, from, to);
        } else if (choice == 5) {
            bookTicket(trains, count, tickets, nextPnr, waitQueues);
        } else if (choice == 6) {
            tickets.display();
        } else if (choice == 7) {
            cancelTicket(trains, count, tickets, waitQueues);
        } else if (choice == 8) {
            showWaitingLists(trains, count, waitQueues);
        } else if (choice == 0) {
            cout << "Goodbye" << endl;
        } else {
            cout << "Invalid choice, try again" << endl;
        }
    }
    return 0;
}