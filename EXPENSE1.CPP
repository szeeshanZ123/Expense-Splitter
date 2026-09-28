

#include <iostream.h>
#include <conio.h>
#include <iomanip.h>
#include <string.h>
#include <stdio.h>

#define MAX 20
#define MAX_EXPENSES 100
#define STACK_SIZE 50
#define QUEUE_SIZE 50


/* ==========================================================
                     EXPENSE STRUCTURE
   ========================================================== */

struct Expense
{
    int id;
    int personIndex;
    char description[30];
    float amount;
};


/* ==========================================================
                  LINKED LIST NODE
   ========================================================== */

struct Node
{
    Expense data;
    Node *next;
};


/* ==========================================================
                  SETTLEMENT STRUCTURE
   ========================================================== */

struct Transaction
{
    char payer[30];
    char receiver[30];
    float amount;
};


/* ==========================================================
                EXPENSE SPLITTER CLASS
   ========================================================== */

class ExpenseSplitter
{
private:

    /* ================= ARRAY ================= */

    int n;
    char names[MAX][30];
    float spent[MAX];
    float balance[MAX];

    float total;
    float equalShare;

    /* ================= LINKED LIST ================= */

    Node *head;

    /* ================= STACK ================= */

    Expense stack[STACK_SIZE];
    int top;

    /* ================= QUEUE ================= */

    Transaction queue[QUEUE_SIZE];
    int front;
    int rear;


public:

    /* ======================================================
                        CONSTRUCTOR
       ====================================================== */

    ExpenseSplitter()
    {
        n = 0;
        total = 0;
        equalShare = 0;

        head = NULL;

        top = -1;

        front = -1;
        rear = -1;
    }


    /* ======================================================
                       FUNCTION LIST
       ====================================================== */

    void getData();
    void addExpense(int personIndex, char description[], float amount);

    void calculate();

    void showSummary();
    void showAllExpenses();

    void searchPerson();
    void sortPeople();

    void displayLinkedList();

    void stackPush(Expense e);
    void stackPop();
    void stackPeek();
    void displayStack();

    void enqueue(Transaction t);
    void dequeue();
    void displayQueue();

    void settlement();

    void clearData();

    void pause();
};


/* ==========================================================
                     GET DATA
   ========================================================== */

void ExpenseSplitter::getData()
{
    int i;
    float amount;
    char description[30];

    clrscr();

    cout << "============================================\n";
    cout << "           ENTER EXPENSE DETAILS\n";
    cout << "============================================\n\n";

    cout << "Enter number of people (max 20): ";
    cin >> n;

    if(n <= 0 || n > MAX)
    {
        cout << "\nInvalid number of people!";
        n = 0;
        pause();
        return;
    }

    /* Reset previous data */

    total = 0;
    equalShare = 0;

    head = NULL;
    top = -1;
    front = -1;
    rear = -1;


    /* Enter people */

    for(i = 0; i < n; i++)
    {
        spent[i] = 0;
        balance[i] = 0;

        cout << "\nEnter name of person "
             << i + 1 << ": ";

        cin >> names[i];

        cout << "\nEnter expenses for "
             << names[i];

        cout << "\nEnter -1 when finished.\n";

        while(1)
        {
            cout << "\nExpense description: ";
            cin >> description;

            cout << "Expense amount: Rs. ";
            cin >> amount;

            if(amount == -1)
                break;

            if(amount < 0)
            {
                cout << "Invalid amount!";
                continue;
            }

            spent[i] = spent[i] + amount;

            addExpense(i, description, amount);
        }
    }

    calculate();

    cout << "\n\nAll expenses entered successfully!";

    pause();
}


/* ==========================================================
                  ADD EXPENSE
        ARRAY + LINKED LIST + STACK
   ========================================================== */

void ExpenseSplitter::addExpense(int personIndex,
                                 char description[],
                                 float amount)
{
    static int id = 1;

    Expense e;

    e.id = id++;

    e.personIndex = personIndex;

    strcpy(e.description, description);

    e.amount = amount;


    /* ================================================
                    LINKED LIST
       ================================================ */

    Node *newNode;
    Node *temp;

    newNode = new Node;

    newNode->data = e;
    newNode->next = NULL;

    if(head == NULL)
    {
        head = newNode;
    }
    else
    {
        temp = head;

        while(temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = newNode;
    }


    /* ================================================
                         STACK
       ================================================ */

    stackPush(e);
}


/* ==========================================================
                       CALCULATE
   ========================================================== */

void ExpenseSplitter::calculate()
{
    int i;

    total = 0;

    for(i = 0; i < n; i++)
    {
        total = total + spent[i];
    }

    if(n > 0)
        equalShare = total / n;
    else
        equalShare = 0;


    for(i = 0; i < n; i++)
    {
        balance[i] = spent[i] - equalShare;
    }
}


/* ==========================================================
                     SHOW SUMMARY
   ========================================================== */

void ExpenseSplitter::showSummary()
{
    int i;

    clrscr();

    cout << "============================================================\n";
    cout << "                     EXPENSE SUMMARY\n";
    cout << "============================================================\n\n";

    if(n == 0)
    {
        cout << "No data available.";
        pause();
        return;
    }

    cout << setiosflags(ios::fixed) << setprecision(2);

    cout << "Total Expense          : Rs. "
         << total << endl;

    cout << "Each Person Should Pay : Rs. "
         << equalShare << endl;

    cout << "\n------------------------------------------------------------\n";

    cout << setw(15) << "Name"
         << setw(15) << "Spent"
         << setw(20) << "Status"
         << endl;

    cout << "------------------------------------------------------------\n";


    for(i = 0; i < n; i++)
    {
        cout << setw(15) << names[i]
             << setw(15) << spent[i];

        if(balance[i] > 0)
        {
            cout << setw(20)
                 << "Receive Rs.";

            cout << balance[i];
        }
        else if(balance[i] < 0)
        {
            cout << setw(20)
                 << "Pay Rs.";

            cout << -balance[i];
        }
        else
        {
            cout << setw(20)
                 << "Settled";
        }

        cout << endl;
    }

    cout << "------------------------------------------------------------\n";

    pause();
}


/* ==========================================================
                  SHOW ALL EXPENSES
                    ARRAY VIEW
   ========================================================== */

void ExpenseSplitter::showAllExpenses()
{
    Node *temp;

    clrscr();

    cout << "============================================================\n";
    cout << "                    ALL EXPENSES\n";
    cout << "============================================================\n";

    if(head == NULL)
    {
        cout << "\nNo expenses available.";
        pause();
        return;
    }

    temp = head;

    cout << "\nID"
         << "\tPerson"
         << "\tDescription"
         << "\tAmount";

    cout << "\n------------------------------------------------------------";

    while(temp != NULL)
    {
        cout << "\n"
             << temp->data.id
             << "\t"
             << names[temp->data.personIndex]
             << "\t"
             << temp->data.description
             << "\t\tRs. "
             << temp->data.amount;

        temp = temp->next;
    }

    cout << "\n------------------------------------------------------------";

    pause();
}


/* ==========================================================
                    SEARCH PERSON
                     LINEAR SEARCH
   ========================================================== */

void ExpenseSplitter::searchPerson()
{
    char searchName[30];
    int i;
    int found = 0;

    clrscr();

    cout << "============================================\n";
    cout << "              SEARCH PERSON\n";
    cout << "============================================\n\n";

    if(n == 0)
    {
        cout << "No data available.";
        pause();
        return;
    }

    cout << "Enter person name: ";
    cin >> searchName;

    for(i = 0; i < n; i++)
    {
        if(strcmp(names[i], searchName) == 0)
        {
            cout << "\nPerson Found!\n";

            cout << "\nName       : "
                 << names[i];

            cout << "\nTotal Spent: Rs. "
                 << spent[i];

            if(balance[i] > 0)
            {
                cout << "\nStatus     : Receive Rs. "
                     << balance[i];
            }
            else if(balance[i] < 0)
            {
                cout << "\nStatus     : Pay Rs. "
                     << -balance[i];
            }
            else
            {
                cout << "\nStatus     : Settled";
            }

            found = 1;
            break;
        }
    }

    if(found == 0)
    {
        cout << "\nPerson not found!";
    }

    pause();
}


/* ==========================================================
                       SORT PEOPLE
                     BUBBLE SORT
   ========================================================== */

void ExpenseSplitter::sortPeople()
{
    int i;
    int j;

    char tempName[30];
    float tempSpent;
    float tempBalance;

    clrscr();

    cout << "============================================\n";
    cout << "          SORT PEOPLE BY EXPENSE\n";
    cout << "============================================\n\n";

    if(n == 0)
    {
        cout << "No data available.";
        pause();
        return;
    }

    cout << "1. Lowest Expense First\n";
    cout << "2. Highest Expense First\n";

    int choice;

    cout << "\nEnter choice: ";
    cin >> choice;


    /* Bubble Sort */

    for(i = 0; i < n - 1; i++)
    {
        for(j = 0; j < n - i - 1; j++)
        {
            if(choice == 1)
            {
                if(spent[j] > spent[j + 1])
                {
                    strcpy(tempName, names[j]);
                    strcpy(names[j], names[j + 1]);
                    strcpy(names[j + 1], tempName);

                    tempSpent = spent[j];
                    spent[j] = spent[j + 1];
                    spent[j + 1] = tempSpent;

                    tempBalance = balance[j];
                    balance[j] = balance[j + 1];
                    balance[j + 1] = tempBalance;
                }
            }
            else if(choice == 2)
            {
                if(spent[j] < spent[j + 1])
                {
                    strcpy(tempName, names[j]);
                    strcpy(names[j], names[j + 1]);
                    strcpy(names[j + 1], tempName);

                    tempSpent = spent[j];
                    spent[j] = spent[j + 1];
                    spent[j + 1] = tempSpent;

                    tempBalance = balance[j];
                    balance[j] = balance[j + 1];
                    balance[j + 1] = tempBalance;
                }
            }
        }
    }


    if(choice == 1 || choice == 2)
    {
        cout << "\nSorted Successfully!\n\n";

        cout << setw(15) << "Name"
             << setw(15) << "Spent"
             << endl;

        cout << "--------------------------------\n";

        for(i = 0; i < n; i++)
        {
            cout << setw(15) << names[i]
                 << setw(15) << spent[i]
                 << endl;
        }
    }
    else
    {
        cout << "\nInvalid choice!";
    }

    pause();
}


/* ==========================================================
                  LINKED LIST DISPLAY
   ========================================================== */

void ExpenseSplitter::displayLinkedList()
{
    Node *temp;

    clrscr();

    cout << "============================================\n";
    cout << "          LINKED LIST EXPENSES\n";
    cout << "============================================\n";

    if(head == NULL)
    {
        cout << "\nLinked List is empty.";
        pause();
        return;
    }

    temp = head;

    cout << "\nHEAD";

    while(temp != NULL)
    {
        cout << " -> ["
             << temp->data.id
             << ", ";

        cout << names[temp->data.personIndex]
             << ", Rs."
             << temp->data.amount
             << "]";

        temp = temp->next;
    }

    cout << " -> NULL";

    cout << "\n\nLinked List stores each individual expense.";

    pause();
}


/* ==========================================================
                         STACK PUSH
   ========================================================== */

void ExpenseSplitter::stackPush(Expense e)
{
    if(top == STACK_SIZE - 1)
    {
        return;
    }

    top++;

    stack[top] = e;
}


/* ==========================================================
                         STACK POP
   ========================================================== */

void ExpenseSplitter::stackPop()
{
    clrscr();

    cout << "============================================\n";
    cout << "                STACK POP\n";
    cout << "============================================\n";

    if(top == -1)
    {
        cout << "\nStack is empty.";
        pause();
        return;
    }

    cout << "\nRemoved Recent Expense:\n";

    cout << "\nPerson      : "
         << names[stack[top].personIndex];

    cout << "\nDescription : "
         << stack[top].description;

    cout << "\nAmount      : Rs. "
         << stack[top].amount;

    top--;

    pause();
}


/* ==========================================================
                         STACK PEEK
   ========================================================== */

void ExpenseSplitter::stackPeek()
{
    clrscr();

    cout << "============================================\n";
    cout << "                STACK PEEK\n";
    cout << "============================================\n";

    if(top == -1)
    {
        cout << "\nStack is empty.";
        pause();
        return;
    }

    cout << "\nMost Recent Expense:\n";

    cout << "\nPerson      : "
         << names[stack[top].personIndex];

    cout << "\nDescription : "
         << stack[top].description;

    cout << "\nAmount      : Rs. "
         << stack[top].amount;

    pause();
}


/* ==========================================================
                    DISPLAY STACK
   ========================================================== */

void ExpenseSplitter::displayStack()
{
    int i;

    clrscr();

    cout << "============================================\n";
    cout << "          RECENT EXPENSES - STACK\n";
    cout << "============================================\n";

    if(top == -1)
    {
        cout << "\nStack is empty.";
        pause();
        return;
    }

    cout << "\nTOP\n";
    cout << "--------------------------------------------";

    for(i = top; i >= 0; i--)
    {
        cout << "\n"
             << names[stack[i].personIndex]
             << " | "
             << stack[i].description
             << " | Rs."
             << stack[i].amount;
    }

    cout << "\n--------------------------------------------";
    cout << "\nLIFO: Last In First Out";

    pause();
}


/* ==========================================================
                       QUEUE ENQUEUE
   ========================================================== */

void ExpenseSplitter::enqueue(Transaction t)
{
    if(rear == QUEUE_SIZE - 1)
    {
        return;
    }

    if(front == -1)
    {
        front = 0;
    }

    rear++;

    queue[rear] = t;
}


/* ==========================================================
                       QUEUE DEQUEUE
   ========================================================== */

void ExpenseSplitter::dequeue()
{
    clrscr();

    cout << "============================================\n";
    cout << "             PROCESS SETTLEMENT\n";
    cout << "============================================\n";

    if(front == -1 || front > rear)
    {
        cout << "\nSettlement Queue is empty.";
        pause();
        return;
    }

    cout << "\nProcessed Transaction:\n";

    cout << "\n"
         << queue[front].payer
         << " pays Rs. "
         << queue[front].amount
         << " to "
         << queue[front].receiver;

    front++;

    if(front > rear)
    {
        front = -1;
        rear = -1;
    }

    pause();
}


/* ==========================================================
                    DISPLAY QUEUE
   ========================================================== */

void ExpenseSplitter::displayQueue()
{
    int i;

    clrscr();

    cout << "============================================\n";
    cout << "           SETTLEMENT QUEUE\n";
    cout << "============================================\n";

    if(front == -1 || front > rear)
    {
        cout << "\nQueue is empty.";
        pause();
        return;
    }

    cout << "\nFRONT\n";
    cout << "--------------------------------------------";

    for(i = front; i <= rear; i++)
    {
        cout << "\n"
             << queue[i].payer
             << " -> "
             << queue[i].receiver
             << " : Rs."
             << queue[i].amount;
    }

    cout << "\n--------------------------------------------";
    cout << "\nFIFO: First In First Out";

    pause();
}


/* ==========================================================
                       SETTLEMENT
                  QUEUE + ARRAY
   ========================================================== */

void ExpenseSplitter::settlement()
{
    int i;
    int j;

    float bal[MAX];
    float amount;

    Transaction t;

    clrscr();

    cout << "============================================================\n";
    cout << "                    SETTLEMENT\n";
    cout << "============================================================\n\n";

    if(n == 0)
    {
        cout << "No data available.";
        pause();
        return;
    }


    /* Clear old settlement queue */

    front = -1;
    rear = -1;


    /* Copy balances */

    for(i = 0; i < n; i++)
    {
        bal[i] = balance[i];
    }


    /* Calculate settlement */

    for(i = 0; i < n; i++)
    {
        if(bal[i] < 0)
        {
            for(j = 0; j < n; j++)
            {
                if(bal[j] > 0)
                {
                    if(-bal[i] < bal[j])
                    {
                        amount = -bal[i];
                    }
                    else
                    {
                        amount = bal[j];
                    }


                    bal[i] = bal[i] + amount;
                    bal[j] = bal[j] - amount;


                    strcpy(t.payer, names[i]);
                    strcpy(t.receiver, names[j]);

                    t.amount = amount;


                    /* Add settlement to Queue */

                    enqueue(t);


                    if(bal[i] == 0)
                    {
                        break;
                    }
                }
            }
        }
    }


    /* Display settlement */

    if(front == -1)
    {
        cout << "Everyone is already settled.";
    }
    else
    {
        cout << "Settlement Transactions:\n";
        cout << "------------------------------------------------------------\n";

        for(i = front; i <= rear; i++)
        {
            cout << queue[i].payer
                 << " pays Rs. "
                 << queue[i].amount
                 << " to "
                 << queue[i].receiver
                 << endl;
        }

        cout << "------------------------------------------------------------";
    }

    pause();
}


/* ==========================================================
                    CLEAR DATA
   ========================================================== */

void ExpenseSplitter::clearData()
{
    Node *temp;

    while(head != NULL)
    {
        temp = head;

        head = head->next;

        delete temp;
    }

    n = 0;

    total = 0;

    equalShare = 0;

    top = -1;

    front = -1;

    rear = -1;
}


/* ==========================================================
                       PAUSE
   ========================================================== */

void ExpenseSplitter::pause()
{
    cout << "\n\nPress any key to continue...";

    getch();
}


/* ==========================================================
                           MAIN
   ========================================================== */

void main()
{
    ExpenseSplitter obj;

    int choice;

    do
    {
        clrscr();

        cout << "****************************************************\n";
        cout << "*                                                  *\n";
        cout << "*              EXPENSE SPLITTER                   *\n";
        cout << "*            DATA STRUCTURES PROJECT              *\n";
        cout << "*                                                  *\n";
        cout << "****************************************************\n";

        cout << "\n";

        cout << "================ MAIN MENU =================\n";

        cout << "\n1. Enter People & Expenses";
        cout << "\n2. Show Expense Summary";
        cout << "\n3. Show All Expenses";
        cout << "\n4. Search Person";
        cout << "\n5. Sort People by Expense";
        cout << "\n6. Show Linked List";

        cout << "\n\n------------- STACK OPERATIONS -------------";

        cout << "\n7. Show Recent Expenses";
        cout << "\n8. Stack POP";
        cout << "\n9. Stack PEEK";

        cout << "\n\n------------- QUEUE OPERATIONS -------------";

        cout << "\n10. Show Settlement Queue";
        cout << "\n11. Process Settlement Queue";

        cout << "\n\n12. Generate Settlement";
        cout << "\n13. Clear Data";
        cout << "\n14. Exit";

        cout << "\n\nEnter your choice: ";
        cin >> choice;


        switch(choice)
        {
            case 1:
                obj.getData();
                break;

            case 2:
                obj.showSummary();
                break;

            case 3:
                obj.showAllExpenses();
                break;

            case 4:
                obj.searchPerson();
                break;

            case 5:
                obj.sortPeople();
                break;

            case 6:
                obj.displayLinkedList();
                break;

            case 7:
                obj.displayStack();
                break;

            case 8:
                obj.stackPop();
                break;

            case 9:
                obj.stackPeek();
                break;

            case 10:
                obj.displayQueue();
                break;

            case 11:
                obj.dequeue();
                break;

            case 12:
                obj.settlement();
                break;

            case 13:
                obj.clearData();

                cout << "\nAll data cleared.";

                obj.pause();
                break;

            case 14:
                clrscr();

                cout << "\n\n";
                cout << "****************************************\n";
                cout << "*       THANK YOU FOR USING            *\n";
                cout << "*          EXPENSE SPLITTER             *\n";
                cout << "****************************************\n";

                break;

            default:
                cout << "\nInvalid choice!";

                obj.pause();
        }

    } while(choice != 14);

    getch();

}