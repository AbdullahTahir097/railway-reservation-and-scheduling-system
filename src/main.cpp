#include <iostream>
#include <string>
#include <fstream>
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
    string owner;       // username of the passenger who booked it
    Ticket* next;
};

// number of buckets in the PNR hash map (a prime number)
const int HASH_SIZE = 101;

// one node inside a hash bucket: a PNR and a pointer to that ticket
class HashNode {
public:
    int pnr;
    Ticket* ticket;
    HashNode* next;
};

class TicketList {
public:
    Ticket* head;
    HashNode* table[HASH_SIZE];   // hash map: PNR -> ticket, for fast lookup

    TicketList() {
        head = NULL;
        for (int i = 0; i < HASH_SIZE; i++) {
            table[i] = NULL;
        }
    }

    // hash function: turns a PNR into a bucket number
    int hashFunction(int pnr) {
        return pnr % HASH_SIZE;
    }

    // add a PNR and its ticket to the hash map
    void hashInsert(int pnr, Ticket* ticket) {
        int index = hashFunction(pnr);
        HashNode* newNode = new HashNode;
        newNode->pnr = pnr;
        newNode->ticket = ticket;
        newNode->next = table[index];
        table[index] = newNode;
    }

    // find a ticket using the hash map, returns NULL if not found
    Ticket* hashSearch(int pnr) {
        int index = hashFunction(pnr);
        HashNode* temp = table[index];
        while (temp != NULL) {
            if (temp->pnr == pnr) {
                return temp->ticket;
            }
            temp = temp->next;
        }
        return NULL;
    }

    // remove a PNR from the hash map
    void hashRemove(int pnr) {
        int index = hashFunction(pnr);
        HashNode* prev = NULL;
        HashNode* cur = table[index];
        while (cur != NULL) {
            if (cur->pnr == pnr) {
                if (prev == NULL) {
                    table[index] = cur->next;
                } else {
                    prev->next = cur->next;
                }
                delete cur;
                return;
            }
            prev = cur;
            cur = cur->next;
        }
    }

    // print the buckets that are not empty
    void displayHash() {
        cout << "--- PNR hash map (non-empty buckets) ---" << endl;
        for (int i = 0; i < HASH_SIZE; i++) {
            if (table[i] != NULL) {
                cout << "Bucket " << i << ": ";
                HashNode* temp = table[i];
                while (temp != NULL) {
                    cout << temp->pnr;
                    if (temp->next != NULL) cout << " -> ";
                    temp = temp->next;
                }
                cout << endl;
            }
        }
    }

    // add a new ticket at the end of the list
    void addTicket(int pnr, string name, int trainId, string source, string destination, int fare, string status, string owner) {
        Ticket* newTicket = new Ticket;
        newTicket->pnr = pnr;
        newTicket->passengerName = name;
        newTicket->trainId = trainId;
        newTicket->source = source;
        newTicket->destination = destination;
        newTicket->fare = fare;
        newTicket->status = status;
        newTicket->owner = owner;
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

        // also store it in the hash map
        hashInsert(pnr, newTicket);
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

    // print only the tickets booked by one passenger
    void displayByOwner(string owner) {
        bool found = false;
        Ticket* temp = head;
        while (temp != NULL) {
            if (temp->owner == owner) {
                cout << "PNR " << temp->pnr << " | " << temp->passengerName
                     << " | Train " << temp->trainId << " | " << temp->source
                     << " to " << temp->destination << " | Rs " << temp->fare
                     << " | " << temp->status << endl;
                found = true;
            }
            temp = temp->next;
        }
        if (found == false) {
            cout << "You have no tickets" << endl;
        }
    }

    // count how many tickets a passenger has
    int countByOwner(string owner) {
        int total = 0;
        Ticket* temp = head;
        while (temp != NULL) {
            if (temp->owner == owner) {
                total++;
            }
            temp = temp->next;
        }
        return total;
    }

    // find a ticket by PNR, returns NULL if not found
    // uses the hash map, so no need to walk through the whole list
    Ticket* findByPnr(int pnr) {
        return hashSearch(pnr);
    }

    // remove a ticket by PNR, returns true if it was removed
    bool removeByPnr(int pnr) {
        if (head == NULL) {
            return false;
        }
        // quick check with the hash map, then remove it from the hash map too
        if (hashSearch(pnr) == NULL) {
            return false;
        }
        hashRemove(pnr);

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

// one node in the undo stack
class StackNode {
public:
    int pnr;
    StackNode* next;
};

// LIFO stack for undo history
// every booking pushes onto the stack, undo pops the most recent one
class UndoStack {
public:
    StackNode* top;
    int size;

    UndoStack() {
        top = NULL;
        size = 0;
    }

    bool isEmpty() {
        return top == NULL;
    }

    // push a PNR onto the top of the stack
    void push(int pnr) {
        StackNode* newNode = new StackNode;
        newNode->pnr = pnr;
        newNode->next = top;
        top = newNode;
        size++;
    }

    // remove and return the PNR at the top of the stack
    int pop() {
        if (isEmpty()) {
            return -1;
        }
        StackNode* temp = top;
        int pnr = temp->pnr;
        top = top->next;
        delete temp;
        size--;
        return pnr;
    }

    // see the top PNR without removing it
    int peek() {
        if (isEmpty()) {
            return -1;
        }
        return top->pnr;
    }

    // remove a specific PNR from anywhere in the stack
    // needed when a ticket is cancelled manually (not via undo)
    bool remove(int pnr) {
        if (isEmpty()) return false;

        if (top->pnr == pnr) {
            pop();
            return true;
        }

        StackNode* prev = top;
        StackNode* cur = top->next;
        while (cur != NULL) {
            if (cur->pnr == pnr) {
                prev->next = cur->next;
                delete cur;
                size--;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false;
    }

    // print all PNRs from top to bottom
    void display() {
        if (isEmpty()) {
            cout << "  (empty)" << endl;
            return;
        }
        cout << "  Top -> ";
        StackNode* temp = top;
        while (temp != NULL) {
            cout << "PNR " << temp->pnr;
            if (temp->next != NULL) cout << " -> ";
            temp = temp->next;
        }
        cout << endl;
    }
};

// number of buckets in the users hash map
const int USER_TABLE_SIZE = 13;

class User {
public:
    string name;
    string username;
    string password;
    string role;      // PASSENGER or ADMIN
};

// one node inside a bucket of the users hash map
class UserNode {
public:
    User user;
    UserNode* next;
};

// hash map of users, the key is the username
class UserMap {
public:
    UserNode* table[USER_TABLE_SIZE];

    UserMap() {
        for (int i = 0; i < USER_TABLE_SIZE; i++) {
            table[i] = NULL;
        }
    }

    // hash function for text: add up the character codes, then take % size
    int hashFunction(string username) {
        int sum = 0;
        for (int i = 0; i < (int)username.size(); i++) {
            sum = sum + (unsigned char)username[i];
        }
        return sum % USER_TABLE_SIZE;
    }

    // find a user by username, returns NULL if there is no such user
    User* search(string username) {
        int index = hashFunction(username);
        UserNode* temp = table[index];
        while (temp != NULL) {
            if (temp->user.username == username) {
                return &(temp->user);
            }
            temp = temp->next;
        }
        return NULL;
    }

    // add a user, returns false if the username is already taken
    bool insert(User u) {
        if (search(u.username) != NULL) {
            return false;
        }
        int index = hashFunction(u.username);
        UserNode* newNode = new UserNode;
        newNode->user = u;
        newNode->next = table[index];
        table[index] = newNode;
        return true;
    }

    // remove a user by username, returns true if it was removed
    bool remove(string username) {
        int index = hashFunction(username);
        UserNode* prev = NULL;
        UserNode* cur = table[index];
        while (cur != NULL) {
            if (cur->user.username == username) {
                if (prev == NULL) {
                    table[index] = cur->next;
                } else {
                    prev->next = cur->next;
                }
                delete cur;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false;
    }

    // print all passengers (not the admin)
    void displayPassengers() {
        int total = 0;
        for (int i = 0; i < USER_TABLE_SIZE; i++) {
            UserNode* temp = table[i];
            while (temp != NULL) {
                if (temp->user.role == "PASSENGER") {
                    cout << temp->user.name << " | username: " << temp->user.username << endl;
                    total++;
                }
                temp = temp->next;
            }
        }
        if (total == 0) {
            cout << "No passengers registered" << endl;
        }
    }
};

// show the waiting list for every train
// the queues are indexed by train id, so sorting the trains does not mix them up
void showWaitingLists(Train trains[], int trainCount, WaitingQueue waitQueues[]) {
    cout << "\n--- Waiting Lists ---" << endl;
    for (int i = 0; i < trainCount; i++) {
        cout << trains[i].name << " (ID " << trains[i].id << "):" << endl;
        waitQueues[trains[i].id].display();
    }
}

// book a ticket for the logged in passenger
// the & means the original list and counter are changed, not copies
void bookTicket(Train trains[], int trainCount, TicketList &list, int &nextPnr, WaitingQueue waitQueues[], UndoStack &undoStack, User* me) {
    int id;
    cout << "Enter train id: ";
    cin >> id;

    int pos = linearSearch(trains, trainCount, id);
    if (pos == -1) {
        cout << "Train not found" << endl;
        return;
    }

    string status;
    if (trains[pos].seats > 0) {
        trains[pos].seats--;
        status = "CONFIRMED";
    } else {
        status = "WAITING";
    }

    list.addTicket(nextPnr, me->name, id, trains[pos].source, trains[pos].destination, trains[pos].fare, status, me->username);

    if (status == "WAITING") {
        waitQueues[id].enqueue(nextPnr);
        cout << "Train is full. You have been added to the waiting list." << endl;
        cout << "Your position in queue: " << waitQueues[id].size << endl;
    }

    undoStack.push(nextPnr);
    cout << "Ticket booked. Your PNR is " << nextPnr << " (" << status << ")" << endl;
    nextPnr++;
}

// cancel a ticket using its PNR
// requiredOwner is the passenger's username, the admin passes "" and can cancel any ticket
void cancelTicket(Train trains[], int trainCount, TicketList &list, WaitingQueue waitQueues[], UndoStack &undoStack, string requiredOwner) {
    int pnr;
    cout << "Enter PNR: ";
    cin >> pnr;

    Ticket* t = list.findByPnr(pnr);
    if (t == NULL) {
        cout << "Ticket not found" << endl;
        return;
    }

    // a passenger can only cancel his own tickets
    if (requiredOwner != "" && t->owner != requiredOwner) {
        cout << "This ticket does not belong to you" << endl;
        return;
    }

    if (t->status == "CONFIRMED") {
        int pos = linearSearch(trains, trainCount, t->trainId);

        // check if someone is waiting in the queue for this train
        if (!waitQueues[t->trainId].isEmpty()) {
            // promote the front of the waiting list to confirmed
            int promotedPnr = waitQueues[t->trainId].dequeue();
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
        waitQueues[t->trainId].remove(pnr);
    }

    undoStack.remove(pnr);
    list.removeByPnr(pnr);
    cout << "Ticket " << pnr << " cancelled" << endl;
}

// undo the most recent booking using the stack
void undoLastBooking(Train trains[], int trainCount, TicketList &list, WaitingQueue waitQueues[], UndoStack &undoStack) {
    if (undoStack.isEmpty()) {
        cout << "Nothing to undo" << endl;
        return;
    }

    int pnr = undoStack.pop();
    Ticket* t = list.findByPnr(pnr);
    if (t == NULL) {
        cout << "Ticket already removed" << endl;
        return;
    }

    cout << "Undoing booking: PNR " << pnr << " (" << t->passengerName << ")" << endl;

    if (t->status == "CONFIRMED") {
        int pos = linearSearch(trains, trainCount, t->trainId);

        if (!waitQueues[t->trainId].isEmpty()) {
            int promotedPnr = waitQueues[t->trainId].dequeue();
            Ticket* promoted = list.findByPnr(promotedPnr);
            if (promoted != NULL) {
                promoted->status = "CONFIRMED";
                cout << "PNR " << promotedPnr << " has been promoted from waiting list to CONFIRMED" << endl;
            }
        } else {
            trains[pos].seats++;
        }
    } else if (t->status == "WAITING") {
        waitQueues[t->trainId].remove(pnr);
    }

    list.removeByPnr(pnr);
    cout << "Booking undone successfully" << endl;
}

// ---------- file handling ----------
// every record is one line, fields are separated by |

// write all trains into data/trains.txt
void saveTrains(Train trains[], int count) {
    ofstream out("data/trains.txt");
    if (!out) {
        cout << "Could not save trains (is there a data folder?)" << endl;
        return;
    }
    for (int i = 0; i < count; i++) {
        out << trains[i].id << "|" << trains[i].name << "|" << trains[i].source << "|"
            << trains[i].destination << "|" << trains[i].departure << "|"
            << trains[i].fare << "|" << trains[i].seats << endl;
    }
    out.close();
}

// read trains from data/trains.txt, returns how many were loaded (0 if no file)
int loadTrains(Train trains[], int maxTrains) {
    ifstream in("data/trains.txt");
    if (!in) {
        return 0;
    }
    int count = 0;
    string idText, name, source, destination, depText, fareText, seatsText;
    while (count < maxTrains && getline(in, idText, '|')) {
        getline(in, name, '|');
        getline(in, source, '|');
        getline(in, destination, '|');
        getline(in, depText, '|');
        getline(in, fareText, '|');
        getline(in, seatsText);

        trains[count].id = stoi(idText);
        trains[count].name = name;
        trains[count].source = source;
        trains[count].destination = destination;
        trains[count].departure = stoi(depText);
        trains[count].fare = stoi(fareText);
        trains[count].seats = stoi(seatsText);
        count++;
    }
    in.close();
    return count;
}

// write all tickets into data/tickets.txt
void saveTickets(TicketList &list) {
    ofstream out("data/tickets.txt");
    if (!out) {
        cout << "Could not save tickets (is there a data folder?)" << endl;
        return;
    }
    Ticket* temp = list.head;
    while (temp != NULL) {
        out << temp->pnr << "|" << temp->passengerName << "|" << temp->trainId << "|"
            << temp->source << "|" << temp->destination << "|" << temp->fare << "|"
            << temp->status << "|" << temp->owner << endl;
        temp = temp->next;
    }
    out.close();
}

// read tickets from data/tickets.txt
// waiting tickets are put back into their train's queue, in the same order
void loadTickets(TicketList &list, WaitingQueue waitQueues[], int &nextPnr) {
    ifstream in("data/tickets.txt");
    if (!in) {
        return;
    }
    string pnrText, name, trainIdText, source, destination, fareText, status, owner;
    while (getline(in, pnrText, '|')) {
        getline(in, name, '|');
        getline(in, trainIdText, '|');
        getline(in, source, '|');
        getline(in, destination, '|');
        getline(in, fareText, '|');
        getline(in, status, '|');
        getline(in, owner);

        int pnr = stoi(pnrText);
        int trainId = stoi(trainIdText);
        int fare = stoi(fareText);

        list.addTicket(pnr, name, trainId, source, destination, fare, status, owner);

        if (status == "WAITING") {
            waitQueues[trainId].enqueue(pnr);
        }
        if (pnr >= nextPnr) {
            nextPnr = pnr + 1;
        }
    }
    in.close();
}

// write all passengers into data/users.txt (the admin is built into the program, so not saved)
void saveUsers(UserMap &users) {
    ofstream out("data/users.txt");
    if (!out) {
        cout << "Could not save users (is there a data folder?)" << endl;
        return;
    }
    for (int i = 0; i < USER_TABLE_SIZE; i++) {
        UserNode* temp = users.table[i];
        while (temp != NULL) {
            if (temp->user.role == "PASSENGER") {
                out << temp->user.name << "|" << temp->user.username << "|"
                    << temp->user.password << "|" << temp->user.role << endl;
            }
            temp = temp->next;
        }
    }
    out.close();
}

// read the passengers from data/users.txt
void loadUsers(UserMap &users) {
    ifstream in("data/users.txt");
    if (!in) {
        return;
    }
    User u;
    while (getline(in, u.name, '|')) {
        getline(in, u.username, '|');
        getline(in, u.password, '|');
        getline(in, u.role);
        users.insert(u);
    }
    in.close();
}

// save everything (called after every booking, cancel and undo)
void saveData(Train trains[], int count, TicketList &list) {
    saveTrains(trains, count);
    saveTickets(list);
}

// check that text does not contain the | character (it is used to separate fields in the files)
bool hasBar(string text) {
    for (int i = 0; i < (int)text.size(); i++) {
        if (text[i] == '|') {
            return true;
        }
    }
    return false;
}

// create a new passenger account
void registerUser(UserMap &users) {
    string name, username, password;
    cout << "Enter your full name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Choose a username: ";
    cin >> username;
    cout << "Choose a password: ";
    cin >> password;

    if (name == "" || hasBar(name) || hasBar(username) || hasBar(password)) {
        cout << "Invalid input. The name cannot be empty and nothing can contain the | character." << endl;
        return;
    }

    User u;
    u.name = name;
    u.username = username;
    u.password = password;
    u.role = "PASSENGER";

    if (users.insert(u) == false) {
        cout << "That username is already taken" << endl;
        return;
    }
    saveUsers(users);
    cout << "Account created. You can login now." << endl;
}

// ask for username and password, returns the user or NULL if login failed
User* loginUser(UserMap &users) {
    string username, password;
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    User* u = users.search(username);
    if (u == NULL || u->password != password) {
        cout << "Wrong username or password" << endl;
        return NULL;
    }
    cout << "Welcome, " << u->name << " (" << u->role << ")" << endl;
    return u;
}

// menu for a logged in passenger
void passengerMenu(User* me, Train trains[], int count, TicketList &tickets, WaitingQueue waitQueues[], UndoStack &undoStack, int &nextPnr) {
    int choice = -1;
    while (choice != 0) {
        cout << endl;
        cout << "===== Passenger Menu: " << me->name << " =====" << endl;
        cout << "1. View all trains" << endl;
        cout << "2. Sort trains by departure time" << endl;
        cout << "3. Sort trains by fare" << endl;
        cout << "4. Search trains by route" << endl;
        cout << "5. Book a ticket" << endl;
        cout << "6. View my tickets" << endl;
        cout << "7. Find my ticket by PNR" << endl;
        cout << "8. Cancel my ticket" << endl;
        cout << "9. Undo my last booking" << endl;
        cout << "10. View undo history" << endl;
        cout << "0. Logout" << endl;
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
            bookTicket(trains, count, tickets, nextPnr, waitQueues, undoStack, me);
            saveData(trains, count, tickets);
        } else if (choice == 6) {
            tickets.displayByOwner(me->username);
        } else if (choice == 7) {
            int pnr;
            cout << "Enter PNR: ";
            cin >> pnr;
            Ticket* t = tickets.findByPnr(pnr);
            if (t == NULL || t->owner != me->username) {
                cout << "Ticket not found" << endl;
            } else {
                cout << "PNR " << t->pnr << " | " << t->passengerName
                     << " | Train " << t->trainId << " | " << t->source
                     << " to " << t->destination << " | Rs " << t->fare
                     << " | " << t->status << endl;
            }
        } else if (choice == 8) {
            cancelTicket(trains, count, tickets, waitQueues, undoStack, me->username);
            saveData(trains, count, tickets);
        } else if (choice == 9) {
            undoLastBooking(trains, count, tickets, waitQueues, undoStack);
            saveData(trains, count, tickets);
        } else if (choice == 10) {
            cout << "\n--- Undo History (most recent first) ---" << endl;
            undoStack.display();
        } else if (choice == 0) {
            cout << "Logged out" << endl;
        } else {
            cout << "Invalid choice, try again" << endl;
        }
    }
}

// menu for the admin
void adminMenu(Train trains[], int count, TicketList &tickets, WaitingQueue waitQueues[], UndoStack &undoStack, UserMap &users) {
    int choice = -1;
    while (choice != 0) {
        cout << endl;
        cout << "===== Admin Menu =====" << endl;
        cout << "1. View all trains" << endl;
        cout << "2. View all tickets" << endl;
        cout << "3. View waiting lists" << endl;
        cout << "4. View all passengers" << endl;
        cout << "5. Cancel any ticket" << endl;
        cout << "6. Delete a passenger account" << endl;
        cout << "7. View PNR hash map" << endl;
        cout << "0. Logout" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            showAll(trains, count);
        } else if (choice == 2) {
            tickets.display();
        } else if (choice == 3) {
            showWaitingLists(trains, count, waitQueues);
        } else if (choice == 4) {
            users.displayPassengers();
        } else if (choice == 5) {
            cancelTicket(trains, count, tickets, waitQueues, undoStack, "");
            saveData(trains, count, tickets);
        } else if (choice == 6) {
            string username;
            cout << "Enter the username to delete: ";
            cin >> username;
            User* u = users.search(username);
            if (u == NULL || u->role != "PASSENGER") {
                cout << "No passenger with that username" << endl;
            } else if (tickets.countByOwner(username) > 0) {
                cout << "This passenger still has tickets. Cancel them first." << endl;
            } else {
                users.remove(username);
                saveUsers(users);
                cout << "Passenger " << username << " deleted" << endl;
            }
        } else if (choice == 7) {
            tickets.displayHash();
        } else if (choice == 0) {
            cout << "Logged out" << endl;
        } else {
            cout << "Invalid choice, try again" << endl;
        }
    }
}

int main() {
    const int MAX = 10;
    Train trains[MAX];
    int count = 0;
    TicketList tickets;
    int nextPnr = 1001;
    WaitingQueue waitQueues[MAX + 1];   // indexed by train id (ids start from 1)
    UndoStack undoStack;
    UserMap users;

    // load saved data; if there is no trains file yet, start with these 4 trains
    count = loadTrains(trains, MAX);
    if (count == 0) {
        trains[count++] = {1, "Green Line", "Karachi", "Rawalpindi", 600, 3600, 3};
        trains[count++] = {2, "Tezgam", "Karachi", "Peshawar", 1830, 4500, 2};
        trains[count++] = {3, "Awam Express", "Lahore", "Peshawar", 915, 1800, 1};
        trains[count++] = {4, "Shalimar", "Multan", "Lahore", 745, 1000, 4};
        saveTrains(trains, count);
    }
    loadTickets(tickets, waitQueues, nextPnr);
    loadUsers(users);

    // the admin account is built into the program
    User admin;
    admin.name = "System Admin";
    admin.username = "admin";
    admin.password = "admin123";
    admin.role = "ADMIN";
    users.insert(admin);

    int choice = -1;
    while (choice != 0) {
        cout << endl;
        cout << "===== Railway Reservation System =====" << endl;
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            registerUser(users);
        } else if (choice == 2) {
            User* me = loginUser(users);
            if (me != NULL) {
                // undo history belongs to one login, so start with an empty stack
                while (!undoStack.isEmpty()) {
                    undoStack.pop();
                }
                // check the role and show the right menu
                if (me->role == "ADMIN") {
                    adminMenu(trains, count, tickets, waitQueues, undoStack, users);
                } else {
                    passengerMenu(me, trains, count, tickets, waitQueues, undoStack, nextPnr);
                }
            }
        } else if (choice == 0) {
            cout << "Goodbye" << endl;
        } else {
            cout << "Invalid choice, try again" << endl;
        }
    }
    return 0;
}