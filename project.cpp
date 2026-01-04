#include <iostream>
#include <fstream>
#include <cstring>
#include <limits>
#include <string>
#include <cctype>
#include <cstdlib>
#include <sstream>
#include <windows.h>

using namespace std;
string CURRENT_ROLE = "";

const int rows = 3;
const int cols = 5;

/* ===================== PROTOTYPES ===================== */
bool validpassword(string);
bool validemail(string);
bool validphonenumber(string);
string signup();
string login();
int neonMenu();

int getSafeInt();
double getSafeDouble();
string getSafeString();
bool isAdmin();
bool handleLogin();
void delay(int ms);
void neonPrint(string txt, string color, int speed = 3);
void pulse(string txt);
string neonMessage(string msg);
string neonHeader(string msg);

int loadstudents(int[], char[][50], char[][20], char[][10], char[][5], char[][12], double[], int&);
bool savestudents(int[], char[][50], char[][20], char[][10], char[][5], char[][12], double[], int);
string studentmanagement(int arrid[], char namarr[][50], char contactarr[][20],
                         char roomarr[][10], char planearr[][5], char dataarr[][12],
                         double duesarr[], int &count);

string takeattendance(int arrid[], char namarr[][50], int count) ;

bool loadmenuitems(string[rows][cols]);
bool savemenu(string[rows][cols]);
bool loadprices(double&, double&, double&);
bool saveprices(double, double, double);
bool updatemenu();
bool updateprices();
string getmenu();
int menumanagement();

int findStudentIndex(int[], int, int);
int countmeal(string, string, string, int&, int&, int&);
double generatebill(int, int, int, double, double, double);
bool savebill(int, string, string, double);
double billmanagement(int arrid[], char namarr[][50], int count) ;

double loadlastbill(int);
int generatepaymentid();
bool savepayment(int, int, double, double, string, string, string, double);
string generaterecipt(int, int, double, double, string, string, string, double);
bool saverecipt(string);
string paymentmodule(int);

double totalrevenue(string);
int countmealtoday(string);
int countstudentdues();
int countstudents();
int totalbillgenerate();
string lastpaymentsummary();
string dashboard();
int main(){
/* ===================== main  ===================== */

int arrid[200];
char namarr[200][50];
char contactarr[200][20];
char roomarr[200][10];
char planearr[200][5];
char dataarr[200][12];
double duesarr[200];
int count = 0;


loadstudents(arrid, namarr, contactarr, roomarr, planearr, dataarr, duesarr, count);

while (true) {
    int choice = neonMenu();

    if (choice == 1) {
        cout << signup() << endl;
        system("pause");
    }
    else if (choice == 2) {
    handleLogin();
    system("pause");
}


    else if (choice == 3) {
    if (isAdmin())
        cout << studentmanagement(arrid, namarr, contactarr, roomarr, planearr, dataarr, duesarr, count);
    system("pause");
}

else if (choice == 4) {
    if (CURRENT_ROLE == "admin" || CURRENT_ROLE == "user")
        cout << takeattendance(arrid, namarr, count);
    else
        cout << neonMessage("Please login first.\n");
    system("pause");
}


else if (choice == 5) {
    if (isAdmin())
        menumanagement();
    system("pause");
}

else if (choice == 6) {
    if (isAdmin())
        cout << billmanagement(arrid, namarr, count);
    system("pause");
}

else if (choice == 7) {
    if (isAdmin()) {
        int sid;
        cout << "Enter student ID: ";
        cin >> sid;
        cout << paymentmodule(sid);
    }
    system("pause");
}

else if (choice == 8) {
    if (CURRENT_ROLE == "admin" || CURRENT_ROLE == "user")
        cout << dashboard();
    else
        cout << neonMessage("Please login first.\n");
    system("pause");
}



        
    else if (choice == 9) {
        break;
    }
}
}


/* ===================== VALIDATION ===================== */
bool validpassword(string pass) {
    if (pass.length() < 7) 
    return false;
    bool d = false, s = false;
    for (char c : pass) {
        if (isdigit(c)) 
        d = true;
        if (c == '@' || c == '#' || c == '$' || c == '*') 
        s = true;
    }
    return d && s;
}

bool validemail(string email) {
    return email.size() > 10 && email.substr(email.size() - 10) == "@gmail.com";
}

bool validphonenumber(string ph) {
    if (ph.length() != 12 || ph[4] != '-') 
    return false;
    for (int i = 0; i < 12; i++)
        if (i != 4 && !isdigit(ph[i])) 
        return false;
    return true;
}


/* ===================== SAFE INPUT ===================== */
int getSafeInt() {
    int x;
    while (!(cin >> x)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input. Enter again: ";
    }
    return x;
}

double getSafeDouble() {
    double x;
    while (!(cin >> x)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input. Enter again: ";
    }
    return x;
}

string getSafeString() {
    string s;
    cin >> s;
    return s;
}

/* ===================== LOGIN / SIGNUP ===================== */

string signup() {
    string username, password, email, phone, role;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // USERNAME (FULL NAME)
    while (true) {
        bool exist = false;
        cout << neonMessage("Enter full name: ");
        getline(cin, username);

        ifstream fin("users.txt");
        string line;
        while (getline(fin, line)) {
            string u;
            stringstream ss(line);
            getline(ss, u, '|');
            if (u == username) {
                exist = true;
                break;
            }
        }
        fin.close();

        if (!exist) break;
        cout << neonMessage("Username already exists.\n");
    }

    // PASSWORD
    while (true) {
        cout << neonMessage("Enter password: ");
        cin >> password;
        if (validpassword(password)) break;
        cout << neonMessage("Invalid password.\n");
    }

    // EMAIL
    while (true) {
        cout << neonMessage("Enter email: ");
        cin >> email;
        if (validemail(email)) break;
        cout << neonMessage("Invalid email.\n");
    }

    // PHONE
    while (true) {
        cout << neonMessage("Enter phone: ");
        cin >> phone;
        if (validphonenumber(phone)) break;
        cout << neonMessage("Invalid phone.\n");
    }

    // ROLE
    while (true) {
        cout << neonMessage("Enter role (admin/user): ");
        cin >> role;
        if (role == "admin" || role == "user") break;
        cout << neonMessage("Only admin or user allowed.\n");
    }

    ofstream fout("users.txt", ios::app);
    fout << username << "|" << password << "|" << email
         << "|" << phone << "|" << role << endl;
    fout.close();

    return "Signup successful.";
}

string login() {
    string input, pass;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << neonMessage("Enter full name or email: ");
    getline(cin, input);

    cout << neonMessage("Enter password: ");
    cin >> pass;

    ifstream fin("users.txt");
    if (!fin) return "invalid";

    string line;
    while (getline(fin, line)) {
        string u, p, e, ph, role;
        stringstream ss(line);

        getline(ss, u, '|');
        getline(ss, p, '|');
        getline(ss, e, '|');
        getline(ss, ph, '|');
        getline(ss, role);

        if ((input == u || input == e) && pass == p) {
            return role;
        }
    }
    return "invalid";
}


bool isAdmin() {
    if (CURRENT_ROLE != "admin") {
        cout << neonMessage("ACCESS DENIED: Admin only section\n");
        return false;
    }
    return true;
}
bool handleLogin() {
    string role = login();

    if (role == "admin") {
        CURRENT_ROLE = "admin";
        cout << neonMessage("Admin login successful\n");
        return true;
    }
    else if (role == "user") {
        CURRENT_ROLE = "user";
        cout << neonMessage("User login successful\n");
        return true;
    }
    else {
        CURRENT_ROLE = "";
        cout << neonMessage("Invalid login\n");
        return false;
    }
}





/* =====================  MENU ===================== */
void delay(int ms) {
    for (volatile long long i = 0; i < ms * 15000; i++);
}

void neonPrint(string txt, string color, int speed) {
    for (char c : txt) {
        cout << color << c << "\033[0m" << flush;
        delay(speed);
    }
}

void pulse(string txt) {
    string color[] = { "\033[95m", "\033[94m", "\033[96m", "\033[95m" };
    for (int i = 0; i < 4; i++) {
        cout << "\r" << color[i] << txt << "\033[0m" << flush;
        delay(200);
    }
    cout << endl;
}

int neonMenu() {
    system("cls");
    


    pulse("=================================================");
    pulse("       MESS BILL MANAGEMENT SYSTEM               ");
    pulse("=================================================");

    neonPrint(" [1] SIGN UP\n", "\033[95m");
    neonPrint(" [2] LOGIN\n", "\033[95m");

    if (CURRENT_ROLE == "admin") {
    neonPrint(" [3] STUDENT MANAGEMENT\n", "\033[92m");
    neonPrint(" [4] ATTENDANCE\n", "\033[92m");
    neonPrint(" [5] MENU & PRICE SETTINGS\n", "\033[92m");
    neonPrint(" [6] BILL MANAGEMENT\n", "\033[92m");
    neonPrint(" [7] PAYMENTS & RECEIPTS\n", "\033[92m");
    neonPrint(" [8] DASHBOARD\n", "\033[92m");
}
else if (CURRENT_ROLE == "user") {
    neonPrint(" [4] ATTENDANCE\n", "\033[96m");
    neonPrint(" [8] DASHBOARD\n", "\033[96m");
}

    neonPrint(" [9] EXIT\n\n", "\033[91m");

    cout << "\033[1mENTER CHOICE: \033[0m";
    return getSafeInt();
}


string neonMessage(string msg) {
    string colors[] = {"\033[95m", "\033[96m", "\033[92m", "\033[93m", "\033[91m"};
    string result = "";
    for (int i = 0; i < msg.size(); i++) {
        result += colors[i % 5] + string(1, msg[i]) + "\033[0m";
    }
    return result + "\n";
}

string neonHeader(string msg) {
    string header = "";
    string line = "==================================================";
    header += neonMessage(line);
    header += neonMessage(msg);
    header += neonMessage(line);
    return header;
}
/* ===================== STUDENT MANAGEMENT ===================== */
int loadstudents(int arrid[], char namarr[][50], char contactarr[][20], char roomarr[][10],
                 char planearr[][5], char dataarr[][12], double duesarr[], int &count) {
    ifstream fin("students.txt");
    if (!fin) 
    return 0;

    string id, name, contact, room, plane, data, dues;
    while (fin >> id >> name >> contact >> room >> plane >> data >> dues) {
        arrid[count] = stoi(id);
        strncpy(namarr[count], name.c_str(), 49);
        strncpy(contactarr[count], contact.c_str(), 19);
        strncpy(roomarr[count], room.c_str(), 9);
        strncpy(planearr[count], plane.c_str(), 4);
        strncpy(dataarr[count], data.c_str(), 11);
        duesarr[count] = stod(dues);
        count++;
    }
    fin.close();
    return count;
}

bool savestudents(int arrid[], char namarr[][50], char contactarr[][20], char roomarr[][10],
                  char planearr[][5], char dataarr[][12], double duesarr[], int count) {
    ofstream fout("students.txt");
    if (!fout) return false;

    for (int i = 0; i < count; i++) {
        fout << arrid[i] << " " << namarr[i] << " " << contactarr[i] << " "
             << roomarr[i] << " " << planearr[i] << " " << dataarr[i] << " " << duesarr[i] << endl;
    }
    fout.close();
    return true;
}

string studentmanagement(int arrid[], char namarr[][50], char contactarr[][20],
                         char roomarr[][10], char planearr[][5], char dataarr[][12],
                         double duesarr[], int &count) {
    while (true) {
        cout << "---------------------------------------------------------------------\n";
        cout <<neonHeader( "-------------------STUDENT MANAGEMENT SYSTEM-------------------------\n");
        cout << "---------------------------------------------------------------------\n";
        cout << neonMessage("1. ADD STUDENTS\n");
        cout << neonMessage("2. SEARCH STUDENTS\n");
        cout << neonMessage("3. UPDATE STUDENTS\n");
        cout << neonMessage("4. DELETE STUDENTS\n");
        cout << neonMessage("5. BACK TO MAIN MENU\n");
        cout << neonMessage("Enter choice: ");
        int ch = getSafeInt();

        if (ch == 1) {
            if (count >= 200) {
                cout << "Student limit reached.\n";
                continue;
            }
            cout <<neonMessage( "Enter ID: ");
            arrid[count] = getSafeInt();
            cout << "Enter name: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.getline(namarr[count], 50);

            cout <<neonMessage("Enter contact: ");
            cin >> contactarr[count];
            cout <<neonMessage ("Enter room number: ");
            cin >> roomarr[count];
            cout << neonMessage("Enter meal plan:(A,B,C) ");
            cin >> planearr[count];
            cout <<neonMessage ("Enter join date (dd/mm/yy): ");
            cin >> dataarr[count];
            duesarr[count] = 0;
            count++;
            savestudents(arrid, namarr, contactarr, roomarr, planearr, dataarr, duesarr, count);
            cout <<neonMessage( "Student added successfully.\n");
        }
        else if (ch == 2) {
            cout << neonMessage("Enter student ID to search: ");
            int id = getSafeInt();
            bool found = false;
            for (int i = 0; i < count; i++) {
                if (arrid[i] == id) {
                    cout << neonMessage("Name: ") << namarr[i] << "\n";
                    cout << neonMessage("Contact: " )<< contactarr[i] << "\n";
                    cout << neonMessage("Room: " )<< roomarr[i] << "\n";
                    cout << neonMessage("Meal Plan: " )<< planearr[i] << "\n";
                    cout << neonMessage("Join Date: ") << dataarr[i] << "\n";
                    cout << neonMessage("Dues: " )<< duesarr[i] << "\n";
                    found = true;
                    break;
                }
            }
            if (!found) 
            cout <<neonMessage( "Student not found.\n");
        }
        else if (ch == 3) {
            cout <<neonMessage( "Enter student ID to update: ");
            int id = getSafeInt();
            int pos = findStudentIndex(arrid, count, id);
            if (pos == -1) {
                cout << "Student not found.\n";
                continue;
            }
           cout << "Enter new name: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.getline(namarr[pos], 50);

            cout << neonMessage("Enter new contact: ");
            cin >> contactarr[pos];
            cout << neonMessage("Enter new room number: ");
            cin >> roomarr[pos];
            cout << neonMessage("Enter new meal plan: ");
            cin >> planearr[pos];
            cout << neonMessage("Enter new join date: ");
            cin >> dataarr[pos];
            savestudents(arrid, namarr, contactarr, roomarr, planearr, dataarr, duesarr, count);
            cout <<neonMessage ("Student updated successfully.\n");
        }
        else if (ch == 4) {
            cout << neonMessage("Enter student ID to delete: ");
            int id = getSafeInt();
            int pos = findStudentIndex(arrid, count, id);
            if (pos == -1) {
                cout <<neonMessage ("Student not found.\n");
                continue;
            }
            // Shift arrays to remove student
            for (int i = pos; i < count - 1; i++) {
                arrid[i] = arrid[i + 1];
                strcpy(namarr[i], namarr[i + 1]);
                strcpy(contactarr[i], contactarr[i + 1]);
                strcpy(roomarr[i], roomarr[i + 1]);
                strcpy(planearr[i], planearr[i + 1]);
                strcpy(dataarr[i], dataarr[i + 1]);
                duesarr[i] = duesarr[i + 1];
            }
            count--;
            savestudents(arrid, namarr, contactarr, roomarr, planearr, dataarr, duesarr, count);
            cout <<neonMessage ("Student deleted successfully.\n");
        }
        else if (ch == 5) {
            return "Returning to Main Menu.\n";
        }
        else {
            cout << "Invalid choice. Try again.\n";
        }
    }
}


/* ===================== ATTENDANCE ===================== */

string takeattendance(int arrid[], char namarr[][50], int count) {
    cout << neonHeader("ATTENDANCE");

    if (count == 0)
        return "No students found. First add students.";

    string adddate;
    cout << neonMessage("Enter date (dd/mm/yy): ");
    adddate = getSafeString();

    ofstream fout("attendance.txt", ios::app);
    if (!fout)
        return "Error opening attendance file.";

    for (int i = 0; i < count; i++) {
        char b, l, d;

        cout << neonMessage("\nStudent ID: ") << arrid[i]
             << neonMessage(" | Name: ") << namarr[i] << endl;

        cout << neonMessage("Want breakfast (Y/N)? ");
        cin >> b;
        cin.ignore(1000, '\n');

        cout << neonMessage("Want lunch (Y/N)? ");
        cin >> l;
        cin.ignore(1000, '\n');

        cout << neonMessage("Want dinner (Y/N)? ");
        cin >> d;
        cin.ignore(1000, '\n');

        fout << adddate << " " << arrid[i] << " "
             << b << " " << l << " " << d << endl;
    }

    fout.close();
    return "Attendance saved successfully.";
}


/* ===================== MENU MANAGEMENT ======================*/
bool loadmenuitems(string items[rows][cols]) {
    ifstream fin("menu.txt");
    if (!fin) 
    return false;

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            fin >> items[i][j];
    fin.close();
    return true;
}

bool savemenu(string items[rows][cols]) {
    ofstream fout("menu.txt");
    if (!fout) return false;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) fout << items[i][j] << " ";
        fout << endl;
    }
    fout.close();
    return true;
}

bool loadprices(double &b, double &l, double &d) {
    ifstream fin("prices.txt");
    if (!fin) return false;
    fin >> b >> l >> d;
    fin.close();
    return true;
}

bool saveprices(double b, double l, double d) {
    ofstream fout("prices.txt");
    if (!fout) return false;
    fout << b << " " << l << " " << d;
    fout.close();
    return true;
}

bool updatemenu() {
    string items[rows][cols];
    cout << "Enter 5 items for Breakfast: ";
    for (int j = 0; j < cols; j++) cin >> items[0][j];
    cout << "Enter 5 items for Lunch: ";
    for (int j = 0; j < cols; j++) cin >> items[1][j];
    cout << "Enter 5 items for Dinner: ";
    for (int j = 0; j < cols; j++) cin >> items[2][j];
    return savemenu(items);
}

bool updateprices() {
    double b, l, d;
    cout << "Enter Breakfast price: "; b = getSafeDouble();
    cout << "Enter Lunch price: "; l = getSafeDouble();
    cout << "Enter Dinner price: "; d = getSafeDouble();
    return saveprices(b, l, d);
}

string getmenu() {
    string items[rows][cols];
    double b, l, d;
    bool ok1 = loadmenuitems(items);
    bool ok2 = loadprices(b, l, d);

    string out = "-----------------MENU----------------\n";

    if (!ok1) out += "Menu file not found.\n";
    else {
        out += "Breakfast: ";
        for (int j = 0; j < cols; j++) out += items[0][j] + " ";
        out += "\nLunch: ";
        for (int j = 0; j < cols; j++) out += items[1][j] + " ";
        out += "\nDinner: ";
        for (int j = 0; j < cols; j++) out += items[2][j] + " ";
    }

    out += "\n----------Prices----------------\n";
    if (!ok2) out += "Price file not found\n";
    else {
        out += "Breakfast: " + to_string(b) + "\n";
        out += "Lunch: " + to_string(l) + "\n";
        out += "Dinner: " + to_string(d) + "\n";
    }

    return out;
}

int menumanagement() {
    cout << neonHeader("MENU & PRICE SETTINGS");
    cout <<neonMessage( "1. View Menu & Prices\n2. Update Menu\n3. Update Prices\n4. Back\nEnter choice: ");
    int ch = getSafeInt();

    if (ch == 1) { 
        cout <<neonMessage( getmenu()); 
        system("pause");
        return 1; 
        }
    else if(ch==2){
        if(updatemenu())
        cout<<neonMessage("---succesfully updated menu .\n ");
        else
        cout<<neonMessage("error found in update process .\n");
    
    system("pause");
    return 2;
    }
    else if(ch==3){
        if(updateprices())
        cout<<neonMessage("----successfully updated prices .")<<endl;
        else
        cout<<neonMessage("error in saving upadte file.")<<endl;
        system("pause");
        return 3;
    }
    else if(ch==4){
        cout<<" back ----------------------:"<<endl;
        return 4;
    }
    cout<<"invalid choices ."<<endl;
    return 0;
}

/* ===================== FIND STUDENT INDEX ===================== */
int findStudentIndex(int arrid[], int count, int sid) {
    for (int i = 0; i < count; i++)
        if (arrid[i] == sid) 
        return i;
    return -1;
}

/* ===================== COUNT MEALS ===================== */
int countmeal(string strsid, string sdate, string edate, int &B, int &L, int &D) {
    string date, id;
    char b, l, d;
    B = L = D = 0;

    ifstream fin("attendance.txt");
    if (!fin) return 0;

    while (fin >> date >> id >> b >> l >> d) {
        if (id == strsid && date >= sdate && date <= edate) {
            if (b == 'Y' || b == 'y') B++;
            if (l == 'Y' || l == 'y') L++;
            if (d == 'Y' || d == 'y') D++;
        }
    }
    fin.close();
    return 1;
}

/* ===================== GENERATE BILL ===================== */
double generatebill(int B, int L, int D, double bp, double lp, double dp) {
    return (B * bp) + (L * lp) + (D * dp);
}

bool savebill(int sid, string start, string end, double amount) {
    ofstream fout("bills.txt", ios::app);
    if (!fout) 
    return false;
    fout << sid << " " << start << " " << end << " " << amount << endl;
    fout.close();
    return true;
}

/* ===================== BILL MANAGEMENT ===================== */

double billmanagement(int arrid[], char namarr[][50], int count) {
    cout << neonHeader("BILL MANAGEMENT");
    cout <<neonMessage("Enter student ID: ");
    int sid = getSafeInt();
    int index = findStudentIndex(arrid, count, sid);
    if (index == -1) {
        cout <<neonMessage( "Student not found.\n");
        return -1;
    }

    string sdate, edate;
    cout << neonMessage("Enter start date (dd/mm/yy): "); 
    cin >> sdate;
    cout <<neonMessage( "Enter end date (dd/mm/yy): "); 
    cin >> edate;

    double bprice, lprice, dprice;
    if (!loadprices(bprice, lprice, dprice)) {
        cout << "Prices not found.\n";
        return -1;
    }

    int B = 0, L = 0, D = 0;
    countmeal(to_string(sid), sdate, edate, B, L, D);
    double bill = generatebill(B, L, D, bprice, lprice, dprice);

    cout << neonHeader("\n---Bill Summary---\n");
    cout << neonMessage("Student ID: " )<< sid << "\n";
    cout << neonMessage("Breakfasts: " )<< B << neonMessage("\nLunches: ") << L << neonMessage("\nDinners: ") << D << "\n";
    cout <<neonMessage( "Total Amount: ") << bill << " PKR\n";

    savebill(sid, sdate, edate, bill);
    return bill;
}


/* ===================== LOAD LAST BILL ===================== */
double loadlastbill(int sid) {
    ifstream fin("bills.txt");
    if (!fin) return -1;

    int id;
    string sdate, edate;
    double bill, lastbill = -1;

    while (fin >> id >> sdate >> edate >> bill) {
        if (sid == id) lastbill = bill;
    }
    fin.close();
    return lastbill;
}

/* ===================== PAYMENT MODULE ===================== */
int generatepaymentid() {
    ifstream fin("payments.txt");
    int last = 0;
    if (fin) {
        int pid, sid;
        double bill, paid, bal;
        string method, txn, date;
        while (fin >> pid >> sid >> bill >> paid >> method >> txn >> date >> bal) last = pid;
        fin.close();
    }
    return last + 1;
}

bool savepayment(int pid, int sid, double bill, double paid, string method, string txn, string date, double bal) {
    ofstream fout("payments.txt", ios::app);
    if (!fout) return false;
    fout << pid << " " << sid << " " << bill << " " << paid << " " << method << " " << txn << " " << date << " " << bal << endl;
    fout.close();
    return true;
}

string generaterecipt(int pid, int sid, double bill, double paid, string method, string txn, string date, double bal) {
    string r = "\n-------------------- RECEIPT --------------------\n";
    r += "Payment ID: " + to_string(pid) + "\n";
    r += "Student ID: " + to_string(sid) + "\n";
    r += "Bill: " + to_string(bill) + "\n";
    r += "Paid Amount: " + to_string(paid) + "\n";
    r += "Method: " + method + "\n";
    r += "Transaction ID: " + txn + "\n";
    r += "Date: " + date + "\n";
    r += "Balance: " + to_string(bal) + "\n";
    r += "-------------------------------------------------\n";
    return r;
}

bool saverecipt(string recipt) {
    ofstream fout("recipt.txt", ios::app);
    if (!fout) 
    return false;
    fout << recipt << endl;
    fout.close();
    return true;
}

string paymentmodule(int sid) {
    cout << neonHeader("PAYMENTS & RECEIPTS");
    double bill = loadlastbill(sid);
    if (bill < 0) 
    return "No bill found.";

    cout << neonMessage("Outstanding bill: ") << bill << endl;
    cout <<neonMessage( "Enter payment amount: ");
    double payment = getSafeDouble();
    if (payment <= 0) 
    return "Invalid amount.";
    if (payment > bill) payment = bill;

    cout << neonMessage( "Enter payment method (cash / easypaisa / card / bank): ");
    string method = getSafeString();
    string txn = "none";
    if (method != "Cash" && method != "cash") {
        cout <<neonMessage( "Enter transaction ID: ");
        txn = getSafeString();
    }

    cout <<neonMessage( "Enter payment date (dd/mm/yy): ");
    string date = getSafeString();
    double bal = bill - payment;
    int pid = generatepaymentid();

    if (!savepayment(pid, sid, bill, payment, method, txn, date, bal)) 
        return "Error saving payment.";

    string receipt = generaterecipt(pid, sid, bill, payment, method, txn, date, bal);
    saverecipt(receipt);
    return receipt;
}


/* ===================== DASHBOARD & REPORTS ===================== */
double totalrevenue(string month) {
    ifstream fin("payments.txt");
    if (!fin) 
    return 0;

    int pid, sid;
    double bill, paid, bal;
    string method, txn, date;
    double total = 0;

    while (fin >> pid >> sid >> bill >> paid >> method >> txn >> date >> bal) {
        if (date.substr(3, 2) == month.substr(3, 2)) total += paid;
    }
    fin.close();
    return total;
}

int countmealtoday(string today) {
    ifstream fin("attendance.txt");
    if (!fin) 
    return 0;

    string date, id;
    char b, l, d;
    int count = 0;
    while (fin >> date >> id >> b >> l >> d) {
        if (date == today) {
            if (b == 'Y' || b == 'y') count++;
            if (l == 'Y' || l == 'y') count++;
            if (d == 'Y' || d == 'y') count++;
        }
    }
    fin.close();
    return count;
}

int countstudentdues() {
    ifstream fin("students.txt");
    if (!fin) 
    return 0;

    int count = 0;
    string id, name, room, plan, date;
    double dues;
    while (fin >> id >> name >> room >> plan >> date >> dues) {
        if (dues > 0) count++;
    }
    fin.close();
    return count;
}

int countstudents() {
    ifstream fin("students.txt");
    if (!fin) 
    return 0;

    int count = 0;
    string id, name, room, plan, date;
    double dues;
    while (fin >> id >> name >> room >> plan >> date >> dues) count++;
    fin.close();
    return count;
}

int totalbillgenerate() {
    ifstream fin("bills.txt");
    if (!fin) 
    return 0;

    int count = 0, sid;
    string sdate, edate;
    double bill;
    while (fin >> sid >> sdate >> edate >> bill) count++;
    fin.close();
    return count;
}

string lastpaymentsummary() {
    ifstream fin("payments.txt");
    if (!fin) 
    return "No payments yet.";

    int pid, sid;
    double bill, paid, bal;
    string method, txn, date;
    string summary = "";

    while (fin >> pid >> sid >> bill >> paid >> method >> txn >> date >> bal) {
        summary = "Payment ID: " + to_string(pid) +
                  "\nStudent ID: " + to_string(sid) +
                  "\nPaid: " + to_string(paid) +
                  "\nMethod: " + method +
                  "\nDate: " + date +
                  "\nBalance: " + to_string(bal) + "\n";
    }
    fin.close();

    return summary;
}
string dashboard() {
    
    string today;
    cout << "Enter today's date (dd/mm/yy): ";
    today = getSafeString();

    double revenue = totalrevenue(today);
    int meals = countmealtoday(today);
    int dues = countstudentdues();
    int students = countstudents();
    int totalBills = totalbillgenerate();
    string lastPayment = lastpaymentsummary();

    string out = "-------------- MESS DASHBOARD ----------------------\n";
    out += "Total Students: " + to_string(students) + "\n";
    out += "Total Bills Generated: " + to_string(totalBills) + "\n";
    out += "Total Meals Today: " + to_string(meals) + "\n";
    out += "Total Revenue: " + to_string(revenue) + "\n";
    out += "------------ Last Payment --------------------\n";
    out += lastPayment + "\n";
    out += "---------------------------------------------------\n";

    return out;
}




