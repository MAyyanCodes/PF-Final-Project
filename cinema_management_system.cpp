#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <cstdlib>
#include <limits>
#include "cinema_management_system.h"
#include "common.h"
using namespace std;
namespace Cinema {
    
 using ::getCurrentDateTime;
using ::clearScreen;
using ::displayCrossSystemBookings;

struct Seat {
    int row;
    int column;
    char status;      
    string category;  
    int price;
    
    Seat() {
    row = 0;
    column = 0;
    status = 'F';
    category = "";
    price = 0;
    };
};


struct Booking {
    int bookingID;
    string customerName;
    string customerPhone;
    string customerCNIC;
    int row;
    int column;
    int price;
    string category;
    string bookingTime;
    bool isActive;          
    
    Booking() {
    bookingID = 0;
    customerName = "";
    customerPhone = ""; 
    customerCNIC = "";
    row = 0;
    column = 0;
    price = 0;
    category = "";
    bookingTime = "";
    isActive = true;
    };
};

struct CinemaHall {
    string hallName;
    int totalRows;
    int totalColumns;
    int freeSeats;
    int reservedSeats;
    double totalRevenue;
    
    CinemaHall() {
        hallName = "Main Hall";
        totalRows = 0;
        totalColumns = 0;
        freeSeats = 0;
        reservedSeats = 0;
        totalRevenue = 0.0;
    };
};

static int nextBookingID = 1001;
static string ADMIN_PASSWORD = "123#";
static int adminAttempts = 0;
static bool adminLocked = false;

void initializeCinema(vector<vector<Seat>>& cinema, CinemaHall& hall);
void displayMenu();
void displaySeatLayout(const vector<vector<Seat>>& cinema);
void displayGraphicalSeatMap(const vector<vector<Seat>>& cinema);
void bookSeat(vector<vector<Seat>>& cinema, CinemaHall& hall, vector<Booking>& history);
void cancelSeat(vector<vector<Seat>>& cinema, CinemaHall& hall, vector<Booking>& history);
void searchBooking(const vector<Booking>& history);
void displayBookingHistory(const vector<Booking>& history);
void adminPanel(vector<vector<Seat>>& cinema, CinemaHall& hall, vector<Booking>& history);
void saveBookingsToFile(const vector<Booking>& history);
void loadBookingsFromFile(vector<Booking>& history);
void saveCinemaState(const vector<vector<Seat>>& cinema, const CinemaHall& hall);
void loadCinemaState(vector<vector<Seat>>& cinema, CinemaHall& hall);
void generateReceipt(const Booking& booking);
void viewRevenueReport(const CinemaHall& hall, const vector<Booking>& history);

void mainCinemaManagementSystem(){
    system("cls");
    vector<vector<Seat>> cinema;
    CinemaHall hall;
    vector<Booking> bookingHistory;
    
    cout << "\n========================================\n";
    cout << "   WELCOME TO CINEMA BOOKING SYSTEM\n";
    cout << "========================================\n";
    
    loadCinemaState(cinema, hall);
    loadBookingsFromFile(bookingHistory);
    
    cout << "\n Cinema size: " << cinema.size() << endl;
    cout << " Total rows: " << hall.totalRows << endl;
    cout << " Total columns: " << hall.totalColumns << endl;
    
    if (!cinema.empty() && hall.totalRows > 0 && hall.totalColumns > 0) {
        cout << "\n[INFO] Existing cinema data found!\n";
        cout << "========================================\n";
        cout << "Cinema Hall: " << hall.hallName << endl;
        cout << "Configuration: " << hall.totalRows << " rows x " 
             << hall.totalColumns << " columns" << endl;
        cout << "Total Seats: " << (hall.totalRows * hall.totalColumns) << endl;
        cout << "Reserved Seats: " << hall.reservedSeats << endl;
        cout << "Available Seats: " << hall.freeSeats << endl;
        cout << "Total Revenue: Rs. " << fixed << setprecision(2) 
             << hall.totalRevenue << endl;
        cout << "Total Bookings: " << bookingHistory.size() << endl;
        cout << "========================================\n\n";
        
        cout << "What would you like to do?\n";
        cout << "1. Load Existing Cinema\n";
        cout << "2. Initialize New Cinema (WARNING: This will delete all existing data!)\n";
        cout << "Enter your choice (1 or 2): ";
        
        int startupChoice;
       
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> startupChoice;
        
        if (startupChoice == 2) {
            char confirm;
            cout << "\n[WARNING] Initializing a new cinema will permanently delete:\n";
            cout << "- All seat configurations\n";
            cout << "- All booking history\n";
            cout << "- All revenue data\n";
            cout << "Are you absolutely sure? (y/n): ";
            cin >> confirm;
            
            if (confirm == 'y' || confirm == 'Y') {
               
                cinema.clear();
                bookingHistory.clear();
                nextBookingID = 1001;
                hall = CinemaHall(); // here i am reseting hall to default 
                
                cout << "\n[INFO] Existing data cleared. Starting fresh initialization...\n";
                initializeCinema(cinema, hall);
            } else {
                cout << "\n[INFO] Cancelled. Loading existing cinema...\n";
            }
        } else {
            cout << "\n[SUCCESS] Existing cinema loaded successfully!\n";
        }
    } else {
       
        cout << "\n[INFO] No existing cinema data found.\n";
        cout << "[INFO] Starting cinema initialization...\n";
        initializeCinema(cinema, hall);
    }

    int choice;
    while (true) {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n[ERROR] Invalid input! Please enter a number.\n";
            continue;
        }
        
        switch (choice) {
            case 1:
                bookSeat(cinema, hall, bookingHistory);
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
                system("cls");
                break;
            case 2:
                displaySeatLayout(cinema);
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
                system("cls");
                break;
            case 3:
                displayGraphicalSeatMap(cinema);
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
                system("cls");
                break;
            case 4:
                cancelSeat(cinema, hall, bookingHistory);
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
                system("cls");
                break;
            case 5:
                cout << "\nFree Seats: " << hall.freeSeats << endl;
                cout << "Reserved Seats: " << hall.reservedSeats << endl;
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
                system("cls");
                break;
            case 6:
                searchBooking(bookingHistory);
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
                system("cls");
                break;
            case 7:
                displayBookingHistory(bookingHistory);
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
                system("cls");
                break;
            case 8:
                adminPanel(cinema, hall, bookingHistory);
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
                system("cls");
                break;
            case 9:
                cout << "\n========================================\n";
                cout << "Thank you for using our system!\n";
                cout << "Have a great movie experience!\n";
                cout << "========================================\n";
                
                saveCinemaState(cinema, hall);
                saveBookingsToFile(bookingHistory);
                return;
            default:
                cout << "\n[ERROR] Invalid choice! Please try again.\n";
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
            }
        }
    
    return;
}

void initializeCinema(vector<vector<Seat>>& cinema, CinemaHall& hall) {
    cout << "\n=== CINEMA HALL SETUP ===\n";
    cout << "Enter number of rows: ";
    cin >> hall.totalRows;
    cout << "Enter number of columns: ";
    cin >> hall.totalColumns;
    
    cinema.resize(hall.totalRows, vector<Seat>(hall.totalColumns));
    
    for (int i = 0; i < hall.totalRows; i++) {
        for (int j = 0; j < hall.totalColumns; j++) {
            cinema[i][j].row = i + 1;
            cinema[i][j].column = j + 1;
            cinema[i][j].status = 'F';
            
            
            if (i < 2) {  
                cinema[i][j].category = "VIP";
                cinema[i][j].price = 1600;
            } else if (i < 8) {  
                cinema[i][j].category = "GOLD";
                cinema[i][j].price = 1000;
            } else {  
                cinema[i][j].category = "STANDARD";
                cinema[i][j].price = 700;
            }
        }
    }
    
    hall.freeSeats = hall.totalRows * hall.totalColumns;
    hall.reservedSeats = 0;
    hall.totalRevenue = 0.0;
    
    cout << "\n[SUCCESS] Cinema initialized with " << hall.freeSeats << " seats!\n";
    
    saveCinemaState(cinema, hall);
    cout << "[INFO] Cinema configuration saved.\n";
}

void displayMenu() {
    cout << "\n========================================\n";
    cout << "            MAIN MENU\n";
    cout << "========================================\n";
    cout << "1. Book a Seat\n";
    cout << "2. View Seat Layout (Detailed)\n";
    cout << "3. View Graphical Seat Map\n";
    cout << "4. Cancel a Booking\n";
    cout << "5. Show Seat Statistics\n";
    cout << "6. Search Booking\n";
    cout << "7. View Booking History\n";
    cout << "8. Admin Panel\n";
    cout << "9. Exit\n";
    cout << "========================================\n"; 
}

void displaySeatLayout(const vector<vector<Seat>>& cinema) {
    cout << "\n========================================\n";
    cout << "         SEAT LAYOUT (DETAILED)\n";
    cout << "========================================\n";
    cout << "F = Free | R = Reserved\n\n";
    
    for (size_t i = 0; i < cinema.size(); i++) {
        for (size_t j = 0; j < cinema[i].size(); j++) {
            cout << "(" << cinema[i][j].row << "-" << cinema[i][j].column 
                 << " " << cinema[i][j].status << ") ";
        }
        cout << "  [" << cinema[i][0].category << "]\n";
    }
    cout << "========================================\n";
}

void displayGraphicalSeatMap(const vector<vector<Seat>>& cinema) {
    cout << "\n========================================\n";
    cout << "       GRAPHICAL SEAT MAP\n";
    cout << "========================================\n";
    cout << "[O] = Available  |  [X] = Reserved\n\n";
    cout << "              SCREEN\n";
    cout << "    ========================\n\n";
    
    for (size_t i = 0; i < cinema.size(); i++) {
        cout << "Row " << setw(2) << (i+1) << ": ";
        for (size_t j = 0; j < cinema[i].size(); j++) {
            if (cinema[i][j].status == 'F') {
                cout << "[O] ";
            } else {
                cout << "[X] ";
            }
        }
        cout << " [" << cinema[i][0].category << " - Rs." << cinema[i][0].price << "]\n";
    }
    cout << "\n========================================\n";
}

void bookSeat(vector<vector<Seat>>& cinema, CinemaHall& hall, vector<Booking>& history) {
    cout << "\n========================================\n";
    cout << "            BOOK A SEAT\n";
    cout << "========================================\n";

    cout << "\nSEAT CATEGORIES:\n";
    cout << "1. VIP      (Rows 1-2)   - Rs. 1600\n";
    cout << "2. GOLD     (Rows 3-8)   - Rs. 1000\n";
    cout << "3. STANDARD (Row 9+)     - Rs. 700\n\n";
    
    displayGraphicalSeatMap(cinema);
    
    int row, column;
    cout << "\nEnter row number: ";
    cin >> row;
    cout << "Enter seat number (column): ";
    cin >> column;
    
    if (row < 1 || row > hall.totalRows || column < 1 || column > hall.totalColumns) {
        cout << "\n[ERROR] Invalid seat selection! Please choose within range.\n";
        return;
    }
    
    int r = row - 1;
    int c = column - 1;
    
    if (cinema[r][c].status == 'R') {
        cout << "\n[ERROR] This seat is already reserved! Please choose another.\n";
        return;
    }
    
    Booking newBooking;
    cin.ignore();  
    cout << "\nEnter customer name: ";
    getline(cin, newBooking.customerName);
    cout << "Enter customer phone: ";
    getline(cin, newBooking.customerPhone);
    cout << "Enter customer CNIC (without dashes): ";
    getline(cin, newBooking.customerCNIC);
    
    newBooking.bookingID = nextBookingID++;
    newBooking.row = row;
    newBooking.column = column;
    newBooking.category = cinema[r][c].category;
    newBooking.price = cinema[r][c].price;
    newBooking.bookingTime = getCurrentDateTime();
    newBooking.isActive = true;
    
    cinema[r][c].status = 'R';
    hall.freeSeats--;
    hall.reservedSeats++;
    hall.totalRevenue += newBooking.price;

    history.push_back(newBooking);
    
    generateReceipt(newBooking);
    
    cout << "\n[SUCCESS] Seat booked successfully!\n";
    cout << "Booking ID: " << newBooking.bookingID << endl;
    cout << "Available seats now: " << hall.freeSeats << endl;    

    saveBookingsToFile(history);
    saveCinemaState(cinema, hall);
}

void cancelSeat(vector<vector<Seat>>& cinema, CinemaHall& hall, vector<Booking>& history) {
    cout << "\n========================================\n";
    cout << "         CANCEL BOOKING\n";
    cout << "========================================\n";
    
    int bookingID;
    cout << "Enter Booking ID to cancel: ";
    cin >> bookingID;
    
    bool found = false;
    for (auto& booking : history) {
        if (booking.bookingID == bookingID && booking.isActive) {
            int r = booking.row - 1;
            int c = booking.column - 1;
        
            cinema[r][c].status = 'F';
            hall.freeSeats++;
            hall.reservedSeats--;
            
            double refund = booking.price * 0.8;
            hall.totalRevenue -= refund;
            
            booking.isActive = false;
            
            cout << "\n[SUCCESS] Booking cancelled!\n";
            cout << "Refund amount: Rs. " << fixed << setprecision(2) << refund << endl;
            cout << "Seat (" << booking.row << "-" << booking.column << ") is now available.\n";
            
            found = true;
             
            saveBookingsToFile(history);
            saveCinemaState(cinema, hall);
            break;
        }
    }
    
    if (!found) {
        cout << "\n[ERROR] Booking ID not found or already cancelled!\n";
    }
}

void searchBooking(const vector<Booking>& history) {
    cout << "\n========================================\n";
    cout << "         SEARCH BOOKING\n";
    cout << "========================================\n";
    cout << "1. Search by Booking ID\n";
    cout << "2. Search by Phone Number\n";
    cout << "3. Search by Customer Name\n";
    cout << "4. Search by CNIC\n";
    cout << "Enter choice: ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    if (choice == 1) {
        int bookingID;
        cout << "Enter Booking ID: ";
        cin >> bookingID;
        
        for (const auto& booking : history) {
            if (booking.bookingID == bookingID) {
                cout << "\n--- BOOKING FOUND ---\n";
                cout << "Booking ID: " << booking.bookingID << endl;
                cout << "Customer: " << booking.customerName << endl;
                cout << "Phone: " << booking.customerPhone << endl;
                cout << "CNIC: " << booking.customerCNIC << endl;
                cout << "Seat: Row " << booking.row << ", Column " << booking.column << endl;
                cout << "Category: " << booking.category << endl;
                cout << "Price: Rs. " << booking.price << endl;
                cout << "Time: " << booking.bookingTime << endl;
                cout << "Status: " << (booking.isActive ? "ACTIVE" : "CANCELLED") << endl;
                return;
            }
        }
        cout << "\n[ERROR] Booking not found!\n";
        
    } else if (choice == 2) {
        string phone;
        cout << "Enter Phone Number: ";
        getline(cin, phone);
        
        bool found = false;
        cout << "\n--- BOOKINGS FOR " << phone << " ---\n";
        
        for (const auto& booking : history) {
            if (booking.customerPhone == phone) {
                cout << "ID: " << booking.bookingID 
                     << " | Seat: (" << booking.row << "-" << booking.column 
                     << ") | Status: " << (booking.isActive ? "ACTIVE" : "CANCELLED") << endl;
                found = true;
            }
        }
        
        if (!found) {
            cout << "No bookings found!\n";
        }
        
    } else if (choice == 3) {
        string name;
        cout << "Enter Customer Name: ";
        getline(cin, name);
        
        bool found = false;
        cout << "\n--- BOOKINGS FOR " << name << " ---\n";
        
        for (const auto& booking : history) {
            if (booking.customerName.find(name) != string::npos) {          // string::npos is a special constant meaning "not found"
                cout << "ID: " << booking.bookingID 
                     << " | Seat: " << booking.row << "-" << booking.column
                     << " | Status: " << (booking.isActive ? "ACTIVE" : "CANCELLED") << endl;
                found = true;
            }
        }
        
        if (!found) {
            cout << "No bookings found!\n";
        }
    }
    else if (choice == 4) {
        string cnic;
        cout << "Enter CNIC: ";
        getline(cin, cnic);

        bool found = false;
        cout << "\n--- CINEMA BOOKINGS FOR CNIC: " << cnic << " ---\n";
        
        for (const auto& booking : history) {
            if (booking.customerCNIC == cnic) {
                cout << "ID: " << booking.bookingID
                     << " | Name: " << booking.customerName
                     << " | Seat: " << booking.row << "-" << booking.column
                     << " | Status: " << (booking.isActive ? "ACTIVE" : "CANCELLED") << endl;
                found = true;
            }
        }
        
        if (!found) {
            cout << "No bookings found in Cinema System!\n";
        }
        
        displayCrossSystemBookings(cnic, "Cinema");
    }
}

void displayBookingHistory(const vector<Booking>& history) {
    cout << "\n========================================\n";
    cout << "       BOOKING HISTORY\n";
    cout << "========================================\n";
    
    if (history.empty()) {
        cout << "No bookings yet!\n";
        return;
    }
    
    cout << left << setw(10) << "ID" << setw(20) << "Customer" 
         << setw(15) << "Phone" << setw(10) << "Seat" 
         << setw(12) << "Category" << setw(10) << "Price" 
         << setw(10) << "Status" << endl;
    cout << string(87, '-') << endl;  // it is creating the string with 87 dashes 
    
    for (const auto& booking : history) {
        cout << left << setw(10) << booking.bookingID 
             << setw(20) << booking.customerName 
             << setw(15) << booking.customerPhone 
             << setw(10) << ("(" + to_string(booking.row) + "-" + to_string(booking.column) + ")")
             << setw(12) << booking.category 
             << setw(10) << booking.price 
             << setw(10) << (booking.isActive ? "ACTIVE" : "CANCELLED") << endl;
    }
}

void adminPanel(vector<vector<Seat>>& cinema, CinemaHall& hall, vector<Booking>& history) {
    if (adminLocked) {
        cout << "\n[LOCKED] Admin panel locked due to too many failed attempts.\n";
        cout << "Please wait 10 minutes or restart the program.\n";
        return;
    }
    
    string password;
    cout << "\n========================================\n";
    cout << "       ADMIN AUTHENTICATION\n";
    cout << "========================================\n";
    cout << "Enter Admin Password: ";
    cin >> password;
    
    if (password != ADMIN_PASSWORD) {
        adminAttempts++;
        cout << "\n[ERROR] Incorrect password! Attempts remaining: " << (3 - adminAttempts) << endl;
        
        if (adminAttempts >= 3) {
            adminLocked = true;
            cout << "[LOCKED] Admin panel locked!\n";
        }
        return;
    }
    
    adminAttempts = 0;
    
    cout << "\n========================================\n";
    cout << "       ADMIN PANEL ACCESS GRANTED\n";
    cout << "========================================\n";
    
    int adminChoice;
    while (true) {
        cout << "\n--- ADMIN MENU ---\n";
        cout << "1. View Revenue Report\n";
        cout << "2. Reset All Seats\n";
        cout << "3. Force Cancel Seat\n";
        cout << "4. View All Bookings\n";
        cout << "5. Export Reports\n";
        cout << "6. Exit Admin Panel\n";
        cout << "Enter choice: ";
        cin >> adminChoice;
        
        switch (adminChoice) {
            case 1:
                viewRevenueReport(hall, history);
                break;
            case 2: {
                char confirm;
                cout << "\n[WARNING] This will reset all seats! Continue? (y/n): ";
                cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    for (auto& row : cinema) {
                        for (auto& seat : row) {
                            seat.status = 'F';
                        }
                    }
                    hall.freeSeats = hall.totalRows * hall.totalColumns;
                    hall.reservedSeats = 0;
                    cout << "\n[SUCCESS] All seats reset to free!\n";
                    saveCinemaState(cinema, hall);
                }
                break;
            }
            case 3: {
                int row, col;
                cout << "Enter row: ";
                cin >> row;
                cout << "Enter column: ";
                cin >> col;
                
                if (row > 0 && row <= hall.totalRows && col > 0 && col <= hall.totalColumns) {
                    if (cinema[row-1][col-1].status == 'R') {
                        cinema[row-1][col-1].status = 'F';
                        hall.freeSeats++;
                        hall.reservedSeats--;
                        cout << "\n[SUCCESS] Seat force cancelled!\n";
                        saveCinemaState(cinema, hall);
                    } else {
                        cout << "\n[ERROR] Seat is already free!\n";
                    }
                } else {
                    cout << "\n[ERROR] Invalid seat!\n";
                }
                break;
            }
            case 4:
                displayBookingHistory(history);
                break;
            case 5:
                cout << "\n[INFO] Reports exported to revenue_report.txt\n";
                break;
            case 6:
                cout << "\n[INFO] Exiting admin panel...\n";
                return;
            default:
                cout << "\n[ERROR] Invalid choice!\n";
        }
    }
}

void viewRevenueReport(const CinemaHall& hall, const vector<Booking>& history) {
    cout << "\n========================================\n";
    cout << "         REVENUE REPORT\n";
    cout << "========================================\n";
    cout << "Total Revenue: Rs. " << fixed << setprecision(2) << hall.totalRevenue << endl;
    cout << "Total Bookings: " << history.size() << endl;
    cout << "Active Bookings: " << hall.reservedSeats << endl;
    
    int vipCount = 0, goldCount = 0, standardCount = 0;
    double vipRevenue = 0, goldRevenue = 0, standardRevenue = 0;
    
    for (const auto& booking : history) {
        if (booking.isActive) {
            if (booking.category == "VIP") {
                vipCount++;
                vipRevenue += booking.price;
            } else if (booking.category == "GOLD") {
                goldCount++;
                goldRevenue += booking.price;
            } else {
                standardCount++;
                standardRevenue += booking.price;
            }
        }
    }
    
    cout << "\n--- Category-wise Revenue ---\n";
    cout << "VIP:      " << vipCount << " seats | Rs. " << vipRevenue << endl;
    cout << "GOLD:     " << goldCount << " seats | Rs. " << goldRevenue << endl;
    cout << "STANDARD: " << standardCount << " seats | Rs. " << standardRevenue << endl;
    
    double occupancyRate = (hall.reservedSeats * 100.0) / (hall.totalRows * hall.totalColumns);
    cout << "\nOccupancy Rate: " << fixed << setprecision(2) << occupancyRate << "%\n";
}

void saveBookingsToFile(const vector<Booking>& history) {
    ofstream file("bookings.txt");
    if (!file) {
        cout << "\n[ERROR] Could not save bookings!\n";
        return;
    }
    
    for (const auto& booking : history) {
        file << booking.bookingID << "|" 
             << booking.customerName << "|"
             << booking.customerPhone << "|"
             << booking.customerCNIC << "|"
             << booking.row << "|"
             << booking.column << "|"
             << booking.price << "|"
             << booking.category << "|"
             << booking.bookingTime << "|"
             << booking.isActive << "\n";
    }
    
    file.close();
}

void loadBookingsFromFile(vector<Booking>& history) {
    ifstream file("bookings.txt");
    if (!file) return;  

    string line;
    while (getline(file, line)) {
        Booking booking;
        stringstream ss(line);
        string temp;
        
        getline(ss, temp, '|'); booking.bookingID = stoi(temp);
        getline(ss, booking.customerName, '|');
        getline(ss, booking.customerPhone, '|');
        getline(ss, booking.customerCNIC, '|');
        getline(ss, temp, '|'); booking.row = stoi(temp);
        getline(ss, temp, '|'); booking.column = stoi(temp);
        getline(ss, temp, '|'); booking.price = stoi(temp);
        getline(ss, booking.category, '|');
        getline(ss, booking.bookingTime, '|');
        getline(ss, temp, '|'); booking.isActive = (temp == "1");
        
        history.push_back(booking);
        
        if (booking.bookingID >= nextBookingID) {
            nextBookingID = booking.bookingID + 1;
        }
    }
    
    file.close();
}

void saveCinemaState(const vector<vector<Seat>>& cinema, const CinemaHall& hall) {
    ofstream file("cinema_state.txt");
    if (!file) return;
    
    file << hall.totalRows << " " << hall.totalColumns << " " 
         << hall.freeSeats << " " << hall.reservedSeats << " " 
         << hall.totalRevenue << "\n";
    
    for (const auto& row : cinema) {
        for (const auto& seat : row) {
            file << seat.status;
        }
        file << "\n";
    }
    
    file.close();
}

void loadCinemaState(vector<vector<Seat>>& cinema, CinemaHall& hall) {
    ifstream file("cinema_state.txt");
    if (!file) return;
    
    file >> hall.totalRows >> hall.totalColumns 
         >> hall.freeSeats >> hall.reservedSeats 
         >> hall.totalRevenue;
    
    cinema.resize(hall.totalRows, vector<Seat>(hall.totalColumns));
    
    string line;
    getline(file, line);  
    
    for (int i = 0; i < hall.totalRows; i++) {
        getline(file, line);
        for (int j = 0; j < hall.totalColumns; j++) {
            cinema[i][j].row = i + 1;
            cinema[i][j].column = j + 1;
            cinema[i][j].status = line[j];
            
            if (i < 2) {
                cinema[i][j].category = "VIP";
                cinema[i][j].price = 1600;
            } else if (i < 8) {
                cinema[i][j].category = "GOLD";
                cinema[i][j].price = 1000;
            } else {
                cinema[i][j].category = "STANDARD";
                cinema[i][j].price = 700;
            }
        }
    }
    
    file.close();
}

void generateReceipt(const Booking& booking) {
    cout << "\n========================================\n";
    cout << "            BOOKING RECEIPT\n";
    cout << "========================================\n";
    cout << "Booking ID:    " << booking.bookingID << endl;
    cout << "Customer Name: " << booking.customerName << endl;
    cout << "Phone Number:  " << booking.customerPhone << endl;
    cout << "Seat Location: Row " << booking.row << ", Column " << booking.column << endl;
    cout << "Category:      " << booking.category << endl;
    cout << "Price:         Rs. " << booking.price << endl;
    cout << "Booking Time:  " << booking.bookingTime << endl;
    cout << "========================================\n";
    cout << "     Thank you for your booking!\n";
    cout << "========================================\n";
    
    ofstream receiptFile("receipt_" + to_string(booking.bookingID) + ".txt");
    if (receiptFile) {   // check if file opened successfully
        receiptFile << "CINEMA BOOKING RECEIPT\n";
        receiptFile << "================================\n";
        receiptFile << "Booking ID: " << booking.bookingID << "\n";
        receiptFile << "Customer: " << booking.customerName << "\n";
        receiptFile << "Phone: " << booking.customerPhone << "\n";
        receiptFile << "Seat: Row " << booking.row << ", Column " << booking.column << "\n";
        receiptFile << "Category: " << booking.category << "\n";
        receiptFile << "Price: Rs. " << booking.price << "\n";
        receiptFile << "Time: " << booking.bookingTime << "\n";
        receiptFile.close();
    }
}
}