#include <iostream>
#include <string>
using namespace std;

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

int main() {
    TicketList list;

    list.addTicket(1001, "Ali", 2, "Karachi", "Peshawar", 4500, "CONFIRMED");
    list.addTicket(1002, "Sara", 3, "Lahore", "Peshawar", 1800, "CONFIRMED");
    list.addTicket(1003, "Usman", 3, "Lahore", "Peshawar", 1800, "WAITING");

    cout << "All tickets:" << endl;
    list.display();

    cout << endl << "Find PNR 1002:" << endl;
    Ticket* t = list.findByPnr(1002);
    if (t != NULL) {
        cout << t->passengerName << " is " << t->status << endl;
    }

    cout << endl << "Remove PNR 1002 (middle):" << endl;
    list.removeByPnr(1002);
    list.display();

    cout << endl << "Remove PNR 9999 (does not exist):" << endl;
    if (list.removeByPnr(9999) == false) {
        cout << "Ticket not found" << endl;
    }

    cout << endl << "Remove PNR 1001 (first):" << endl;
    list.removeByPnr(1001);
    list.display();
    return 0;
}