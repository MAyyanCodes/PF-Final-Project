#include <iostream>    
#include <vector>      
#include <fstream>      
#include <string>       
#include <iomanip>     
#include <ctime>       
#include <sstream>     
#include <cstdlib>     
#include <limits>
#include "Flight_management_system.h"
#include "common.h"

using namespace std;   

namespace Flight {
     
 using ::getCurrentDateTime;        
using ::clearScreen;
using ::displayCrossSystemBookings;

struct Seat {
    int row;           
    int column;        
    char status;       
    string seatClass;  
    int price;         
       
    Seat() {
        row = 0;           
        column = 0;        
        status = 'F';      
        seatClass = "";    
        price = 0;         
        };  
};

struct FlightBooking {
    int bookingID;          
    string passengerName;   
    string passengerPhone;  
    string passengerEmail;  
    string passengerCNIC;
    int row;                
    int column;             
    int price;              
    string seatClass;       
    string bookingTime;     
    bool isActive;          
    
    
    FlightBooking() {
        bookingID = 0;
        passengerName = "";
        passengerPhone = "";
        passengerEmail = "";
        passengerCNIC = "";
        row = 0;
        column = 0;
        price = 0;
        seatClass = "";
        bookingTime = "";
        isActive = true;    
    };
};

struct Flight {
    string flightNumber;     
    string flightRoute;      
    int totalRows;           
    int totalColumns;        
    int freeSeats;           
    int reservedSeats;       
    double totalRevenue;        
    string departureTime;    
    string arrivalTime;      
    
    Flight() {
        flightNumber = "PK-000";            
        flightRoute = "Not Set";        
        totalRows = 0;
        totalColumns = 0;
        freeSeats = 0;
        reservedSeats = 0;
        totalRevenue = 0.0;             
        departureTime = "Not Set";
        arrivalTime = "Not Set";
    };
};

static int nextBookingID = 2001;
static const string ADMIN_PASSWORD = "123";
static int adminAttempts = 0;
static bool adminLocked = false;
  
void initializeFlight(vector<vector<Seat>>& airplane, Flight& flight);
void displayMenu();
void displaySeatLayout(const vector<vector<Seat>>& airplane);
void displayGraphicalSeatMap(const vector<vector<Seat>>& airplane);
void bookSeat(vector<vector<Seat>>& airplane, Flight& flight, vector<FlightBooking>& history);
void cancelBooking(vector<vector<Seat>>& airplane, Flight& flight, vector<FlightBooking>& history);
void searchBooking(const vector<FlightBooking>& history);
void displayBookingHistory(const vector<FlightBooking>& history);
void adminPanel(vector<vector<Seat>>& airplane, Flight& flight, vector<FlightBooking>& history);
void saveBookingsToFile(const vector<FlightBooking>& history);
void loadBookingsFromFile(vector<FlightBooking>& history);
void saveFlightState(const vector<vector<Seat>>& airplane, const Flight& flight);
void loadFlightState(vector<vector<Seat>>& airplane, Flight& flight);
void generateTicket(const FlightBooking& booking, const Flight& flight);
void viewRevenueReport(const Flight& flight, const vector<FlightBooking>& history);

void mainFlightManagementSystem(){
    vector<vector<Seat>> airplane;      
    Flight flight;                   
    vector<FlightBooking> bookingHistory;  
    
    cout << "\n========================================\n";
    cout << "   WELCOME TO FLIGHT BOOKING SYSTEM\n";
    cout << "========================================\n";

    loadFlightState(airplane, flight);          
    loadBookingsFromFile(bookingHistory);   

    cout << "\n Airplane size: " << airplane.size() << endl;
    cout << " Total rows: " << flight.totalRows << endl;
    cout << " Total columns: " << flight.totalColumns << endl;

    if (!airplane.empty() && flight.totalRows > 0 && flight.totalColumns > 0) {
        cout << "\n[INFO] Existing flight data found!\n";
        cout << "========================================\n";
        cout << "Flight Number: " << flight.flightNumber << endl;
        cout << "Route: " << flight.flightRoute << endl;
        cout << "Departure: " << flight.departureTime << endl;
        cout << "Arrival: " << flight.arrivalTime << endl;
        cout << "Configuration: " << flight.totalRows << " rows x " 
             << flight.totalColumns << " seats per row" << endl;
        cout << "Total Seats: " << (flight.totalRows * flight.totalColumns) << endl;
        cout << "Booked Seats: " << flight.reservedSeats << endl;
        cout << "Available Seats: " << flight.freeSeats << endl;
        cout << "Total Revenue: Rs. " << fixed << setprecision(2) 
             << flight.totalRevenue << endl;
        cout << "Total Bookings: " << bookingHistory.size() << endl;
        cout << "========================================\n\n";
        
        cout << "What would you like to do?\n";
        cout << "1. Load Existing Flight\n";
        cout << "2. Initialize New Flight (WARNING: This will delete all existing data!)\n";
        cout << "Enter your choice (1 or 2): ";
        
        int startupChoice;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> startupChoice;
        
        if (startupChoice == 2) {
            char confirm;
            cout << "\n[WARNING] Initializing a new flight will permanently delete:\n";
            cout << "- All flight configurations\n";
            cout << "- All seat bookings\n";
            cout << "- All booking history\n";
            cout << "- All revenue data\n";
            cout << "Are you absolutely sure? (y/n): ";
            cin >> confirm;
            
            if (confirm == 'y' || confirm == 'Y') {
                airplane.clear();
                bookingHistory.clear();
                nextBookingID = 2001;
                flight = Flight(); // Reset flight to default
                
                cout << "\n[INFO] Existing data cleared. Starting fresh initialization...\n";
                initializeFlight(airplane, flight);
            } else {
                cout << "\n[INFO] Cancelled. Loading existing flight...\n";
            }
        } else {
            cout << "\n[SUCCESS] Existing flight loaded successfully!\n";
        }
    } else {
    
        cout << "\n[INFO] No existing flight data found.\n";
        cout << "[INFO] Starting flight initialization...\n";
        initializeFlight(airplane, flight);
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
        bookSeat(airplane, flight, bookingHistory);
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
        clearScreen();  
        break;
        
    case 2:
        displaySeatLayout(airplane);
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
        clearScreen();  
        break;
        
    case 3:
        displayGraphicalSeatMap(airplane);
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
        clearScreen();  
        break;
        
    case 4:
        cancelBooking(airplane, flight, bookingHistory);
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
        clearScreen();
        break;
        
    case 5:
        cout << "\n========================================\n";
        cout << "       SEAT STATISTICS\n";
        cout << "========================================\n";
        cout << "Total Seats: " << (flight.totalRows * flight.totalColumns) << endl;
        cout << "Available Seats: " << flight.freeSeats << endl;
        cout << "Booked Seats: " << flight.reservedSeats << endl;
        cout << "Occupancy: " << fixed << setprecision(1) 
             << (flight.reservedSeats * 100.0 / (flight.totalRows * flight.totalColumns)) 
             << "%" << endl;
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
        clearScreen();  
        break;
        
    case 6:
        searchBooking(bookingHistory);
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
        clearScreen();  
        break;
        
    case 7:
        displayBookingHistory(bookingHistory);
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
        clearScreen();  
        break;
        
    case 8:
        adminPanel(airplane, flight, bookingHistory);
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
        clearScreen();  
        break;
        
    case 9:
        cout << "\n========================================\n";
        cout << "Thank you for using our Flight Booking System!\n";
        cout << "Have a safe journey!\n";
        cout << "========================================\n";
        saveFlightState(airplane, flight);
        saveBookingsToFile(bookingHistory);
        return;
        
    default:
        cout << "\n[ERROR] Invalid choice! Please enter 1-9.\n";
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
        clearScreen(); 
}
    }
    
    return;  
}
void initializeFlight(vector<vector<Seat>>& airplane, Flight& flight) {
    cout << "\n=== FLIGHT SETUP ===\n";
    cin.ignore();   
    cout << "Enter Flight Number (e.g., PK-301): ";
    getline(cin, flight.flightNumber);  
    
    cout << "Enter Flight Route (e.g., Karachi to Dubai): ";
    getline(cin, flight.flightRoute);
    
    cout << "Enter Departure Time (e.g., 10:00 AM): ";
    getline(cin, flight.departureTime);
    
    cout << "Enter Arrival Time (e.g., 02:00 PM): ";
    getline(cin, flight.arrivalTime);
    
    cout << "\nEnter number of seat rows: ";
    cin >> flight.totalRows;
    cout << "Enter number of seats per row: ";
    cin >> flight.totalColumns;
    
    airplane.resize(flight.totalRows, vector<Seat>(flight.totalColumns));
    
    for (int i = 0; i < flight.totalRows; i++) {
    
        for (int j = 0; j < flight.totalColumns; j++) {
            
            airplane[i][j].row = i + 1;         
            airplane[i][j].column = j + 1;   
            airplane[i][j].status = 'F';     
            
              if (i < 2) {
                airplane[i][j].seatClass = "FIRST CLASS";
                airplane[i][j].price = 25000; 
            } 

            else if (i < 6) {
                airplane[i][j].seatClass = "BUSINESS";
                airplane[i][j].price = 15000;  
                    } 
        
            else {
                airplane[i][j].seatClass = "ECONOMY";
                airplane[i][j].price = 8000;  
            }
        }
    }
    
    flight.freeSeats = flight.totalRows * flight.totalColumns;
    flight.reservedSeats = 0;  
    flight.totalRevenue = 0.0; 
    
    cout << "\n[SUCCESS] Flight initialized with " << flight.freeSeats << " seats!\n";
    
    saveFlightState(airplane, flight);
    cout << "[INFO] Flight configuration saved.\n";
}

void displayMenu() {
    cout << "\n========================================\n";
    cout << "            MAIN MENU\n";
    cout << "========================================\n";
    cout << "1. Book a Flight Seat\n";
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

void displaySeatLayout(const vector<vector<Seat>>& airplane) {
    cout << "\n========================================\n";
    cout << "      DETAILED SEAT LAYOUT\n";
    cout << "========================================\n";
    cout << "F = Free | R = Reserved\n\n";
    
    for (size_t i = 0; i < airplane.size(); i++) {
        cout << "Row " << setw(2) << (i+1) << ": ";

        for (size_t j = 0; j < airplane[i].size(); j++) {
            cout << "(" << airplane[i][j].row << "-" 
                 << airplane[i][j].column << " " 
                 << airplane[i][j].status << ") ";
        }
        cout << " [" << airplane[i][0].seatClass << "]\n";
    }
    cout << "========================================\n";
}

void displayGraphicalSeatMap(const vector<vector<Seat>>& airplane) {
    cout << "\n========================================\n";
    cout << "      GRAPHICAL SEAT MAP\n";
    cout << "========================================\n";
    cout << "[O] = Available  |  [X] = Booked\n\n";
    cout << "         COCKPIT (Front)\n";
    cout << "    ========================\n\n";
    
    for (size_t i = 0; i < airplane.size(); i++) {
        cout << "Row " << setw(2) << (i+1) << ": ";
        
        for (size_t j = 0; j < airplane[i].size(); j++) {
            if (airplane[i][j].status == 'F') {
                cout << "[O] ";  
            } else {
                cout << "[X] ";  
            }
            
            if (j == (airplane[i].size() / 2 - 1)) {
                cout << " | "; 
            }
        }
        cout << " [" << airplane[i][0].seatClass 
             << " - Rs." << airplane[i][0].price << "]\n";
    }
    cout << "\n========================================\n";
}

void bookSeat(vector<vector<Seat>>& airplane, Flight& flight, vector<FlightBooking>& history) {
    cout << "\n========================================\n";
    cout << "        BOOK A FLIGHT SEAT\n";
    cout << "========================================\n";
    cout << "Flight: " << flight.flightNumber << " | " << flight.flightRoute << endl;
    cout << "Departure: " << flight.departureTime << " | Arrival: " << flight.arrivalTime << endl;

    cout << "\nSEAT CLASSES:\n";
    cout << "1. FIRST CLASS (Rows 1-2)   - Rs. 25,000\n";
    cout << "2. BUSINESS    (Rows 3-6)   - Rs. 15,000\n";
    cout << "3. ECONOMY     (Row 7+)     - Rs. 8,000\n\n";
    
    displayGraphicalSeatMap(airplane);
    
    int row, column;
    cout << "\nEnter row number: ";
    cin >> row;
    cout << "Enter seat number (column): ";
    cin >> column;
    
    if (row < 1 || row > flight.totalRows || column < 1 || column > flight.totalColumns) {
        cout << "\n[ERROR] Invalid seat! Please choose within range.\n";
        cout << "Valid rows: 1-" << flight.totalRows << endl;
        cout << "Valid columns: 1-" << flight.totalColumns << endl;
        return;  
    }
    
    int r = row - 1;     
    int c = column - 1;
    if (airplane[r][c].status == 'R') {
        cout << "\n[ERROR] Seat " << row << "-" << column << " is already booked!\n";
        cout << "Please choose another seat.\n";
        return;
    }
    
    FlightBooking newBooking;  
    cin.ignore();  
    
    cout << "\n--- PASSENGER INFORMATION ---\n";
    cout << "Enter passenger name: ";
    getline(cin, newBooking.passengerName);  
    
    cout << "Enter phone number: ";
    getline(cin, newBooking.passengerPhone);
    
    cout << "Enter email address: ";
    getline(cin, newBooking.passengerEmail);

    cout << "Enter CNIC (without dashes): ";
    getline(cin, newBooking.passengerCNIC);
    
    newBooking.bookingID = nextBookingID++;  
    newBooking.row = row;
    newBooking.column = column;
    newBooking.seatClass = airplane[r][c].seatClass;  
    newBooking.price = airplane[r][c].price;          
    newBooking.bookingTime = getCurrentDateTime();    
    newBooking.isActive = true;                       
    
    airplane[r][c].status = 'R';      
    
    flight.freeSeats--;      
    flight.reservedSeats++;  
    flight.totalRevenue += newBooking.price;  
    
    history.push_back(newBooking);  
    
    generateTicket(newBooking, flight);
    
    cout << "\n[SUCCESS] Seat booked successfully!\n";
    cout << "Booking ID: " << newBooking.bookingID << endl;
    cout << "Seats remaining: " << flight.freeSeats << endl;
    
    saveBookingsToFile(history);
    saveFlightState(airplane, flight);
}

void cancelBooking(vector<vector<Seat>>& airplane, Flight& flight, vector<FlightBooking>& history) {
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
        
            airplane[r][c].status = 'F';
            
            flight.freeSeats++;
            flight.reservedSeats--;
            
            
            double refund = booking.price * 0.9;
            flight.totalRevenue -= refund;  
            
            booking.isActive = false;
            
            cout << "\n[SUCCESS] Booking cancelled!\n";
            cout << "Passenger: " << booking.passengerName << endl;
            cout << "Seat: Row " << booking.row << ", Column " << booking.column << endl;
            cout << "Original Price: Rs. " << booking.price << endl;
            cout << "Refund Amount (90%): Rs. " << fixed << setprecision(2) << refund << endl;
            cout << "Cancellation Fee (10%): Rs. " << (booking.price * 0.1) << endl;
            
            found = true;  
            
            saveBookingsToFile(history);
            saveFlightState(airplane, flight);
            
            break;  
        }
    }
    
    if (!found) {  
        cout << "\n[ERROR] Booking ID not found or already cancelled!\n";
    }
}

void searchBooking(const vector<FlightBooking>& history) {
    cout << "\n========================================\n";
    cout << "         SEARCH BOOKING\n";
    cout << "========================================\n";
    cout << "1. Search by Booking ID\n";
    cout << "2. Search by Phone Number\n";
    cout << "3. Search by Passenger Name\n";
    cout << "4. Search by CNIC\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    bool found = false;

    if (choice == 1) {
        int bookingID;
        cout << "Enter Booking ID: ";
        cin >> bookingID;

        for (const auto& booking : history) {
            if (booking.bookingID == bookingID) {
                cout << "\n--- BOOKING FOUND ---\n";
                cout << "Passenger: " << booking.passengerName << endl;
                cout << "Phone: " << booking.passengerPhone << endl;
                cout << "Email: " << booking.passengerEmail << endl;
                cout << "CNIC: " << booking.passengerCNIC << endl;
                cout << "Seat: Row " << booking.row << ", Column " << booking.column << endl;
                cout << "Class: " << booking.seatClass << endl;
                cout << "Price: Rs. " << booking.price << endl;
                cout << "Status: " << (booking.isActive ? "ACTIVE" : "CANCELLED") << endl;
                return;
            }
        }
        cout << "\n[ERROR] Booking not found!\n";
    }

    else if (choice == 2) {
        string phone;
        cout << "Enter Phone Number: ";
        getline(cin, phone);

        for (const auto& booking : history) {
            if (booking.passengerPhone == phone) {
                cout << "ID: " << booking.bookingID
                     << " | Seat: " << booking.row << "-" << booking.column
                     << " | Status: " << (booking.isActive ? "ACTIVE" : "CANCELLED") << endl;
                found = true;
            }
        }
        if (!found) cout << "No bookings found!\n";
    }

    else if (choice == 3) {
        string name;
        cout << "Enter Passenger Name: ";
        getline(cin, name);

        for (const auto& booking : history) {
            if (booking.passengerName.find(name) != string::npos) {        // string::npos is a special constant meaning "not found 
                cout << "ID: " << booking.bookingID
                     << " | Seat: " << booking.row << "-" << booking.column
                     << " | Status: " << (booking.isActive ? "ACTIVE" : "CANCELLED") << endl;
                found = true;
            }
        }
        if (!found) cout << "No bookings found!\n";
    }

    else if (choice == 4) {
        string cnic;
        cout << "Enter CNIC: ";
        getline(cin, cnic);

        cout << "\n--- FLIGHT BOOKINGS FOR CNIC: " << cnic << " ---\n";
        
        for (const auto& booking : history) {
            if (booking.passengerCNIC == cnic) {
                cout << "ID: " << booking.bookingID
                     << " | Name: " << booking.passengerName
                     << " | Seat: " << booking.row << "-" << booking.column
                     << " | Status: " << (booking.isActive ? "ACTIVE" : "CANCELLED") << endl;
                found = true;
            }
        }
        
        if (!found) {
            cout << "No bookings found in Flight System!\n";
        }
        
        displayCrossSystemBookings(cnic, "Flight");
    }

    else {
        cout << "\n[ERROR] Invalid choice!\n";
    }
}

void displayBookingHistory(const vector<FlightBooking>& history) {
    cout << "\n========================================\n";
    cout << "       BOOKING HISTORY\n";
    cout << "========================================\n";
    
    if (history.empty()) {
        cout << "No bookings yet!\n";
        return;
    }
    cout << left << setw(8) << "ID" 
         << setw(20) << "Passenger" 
         << setw(15) << "Phone"
         << setw(10) << "Seat" 
         << setw(15) << "Class"
         << setw(10) << "Price" 
         << setw(12) << "Status" << endl;
    
    cout << string(90, '-') << endl;

    for (const auto& booking : history) {
        cout << left 
             << setw(8) << booking.bookingID 
             << setw(20) << booking.passengerName 
             << setw(15) << booking.passengerPhone 
             << setw(10) << ("(" + to_string(booking.row) + "-" + to_string(booking.column) + ")")
             << setw(15) << booking.seatClass
             << setw(10) << booking.price 
             << setw(12) << (booking.isActive ? "ACTIVE" : "CANCELLED") << endl;
    }
}

void adminPanel(vector<vector<Seat>>& airplane, Flight& flight, vector<FlightBooking>& history) {
    if (adminLocked) {
        cout << "\n[LOCKED] Admin panel locked due to failed attempts.\n";
        cout << "Please restart the program or wait.\n";
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
        cout << "\n[ERROR] Incorrect password!\n";
        cout << "Attempts remaining: " << (3 - adminAttempts) << endl;
        
        if (adminAttempts >= 3) {
            adminLocked = true;
            cout << "[LOCKED] Too many failed attempts! Admin panel locked.\n";
        }
        return;
    }
    
    adminAttempts = 0;
    
    cout << "\n[ACCESS GRANTED] Welcome, Administrator!\n";
    
    int adminChoice;
    while (true) {
        cout << "\n========================================\n";
        cout << "         ADMIN PANEL\n";
        cout << "========================================\n";
        cout << "1. View Revenue Report\n";
        cout << "2. Reset All Seats\n";
        cout << "3. Modify Flight Details\n";
        cout << "4. View All Bookings\n";
        cout << "5. Force Cancel Booking\n";
        cout << "6. Generate Summary Report\n";
        cout << "7. Exit Admin Panel\n";
        cout << "========================================\n";
        cout << "Enter choice: ";
        cin >> adminChoice;
        
        switch (adminChoice) {
            case 1:
                viewRevenueReport(flight, history);
                break;
                
            case 2: {
                char confirm;
                cout << "\n[WARNING] This will reset ALL seats to available!\n";
                cout << "All current bookings will be lost!\n";
                cout << "Are you sure? (y/n): ";
                cin >> confirm;
                
                if (confirm == 'y' || confirm == 'Y') {
                    for (auto& row : airplane) {      
                        for (auto& seat : row) {      
                            seat.status = 'F';        
                        }
                    }
                    
                    flight.freeSeats = flight.totalRows * flight.totalColumns;
                    flight.reservedSeats = 0;                    
                    cout << "\n[SUCCESS] All seats reset!\n";
                    saveFlightState(airplane, flight);
                } else {
                    cout << "\n[CANCELLED] Operation cancelled.\n";
                }
                break;
            }
            
            case 3: {
                cin.ignore();
                cout << "\n--- MODIFY FLIGHT DETAILS ---\n";
                cout << "Current Flight Number: " << flight.flightNumber << endl;
                cout << "Enter new Flight Number (or press Enter to skip): ";
                string temp;
                getline(cin, temp);
                if (!temp.empty()) { 
                    flight.flightNumber = temp;
                }

                cout << "\nCurrent Route: " << flight.flightRoute << endl;
                cout << "Enter new Route (or press Enter to skip): ";
                getline(cin, temp);
                if (!temp.empty()) {
                    flight.flightRoute = temp;
                }
                
                cout << "\nCurrent Departure: " << flight.departureTime << endl;
                cout << "Enter new Departure Time (or press Enter to skip): ";
                getline(cin, temp);
                if (!temp.empty()) {
                    flight.departureTime = temp;
                }
                
                cout << "\nCurrent Arrival: " << flight.arrivalTime << endl;
                cout << "Enter new Arrival Time (or press Enter to skip): ";
                getline(cin, temp);
                if (!temp.empty()) {
                    flight.arrivalTime = temp;
                }
                
                cout << "\n[SUCCESS] Flight details updated!\n";
                saveFlightState(airplane, flight);
                break;
            }
            
            case 4:
                displayBookingHistory(history);
                break;
            
            case 5: {
                int row, col;
                cout << "\n--- FORCE CANCEL SEAT ---\n";
                cout << "Enter row: ";
                cin >> row;
                cout << "Enter column: ";
                cin >> col;

                if (row > 0 && row <= flight.totalRows && 
                    col > 0 && col <= flight.totalColumns) {
                    
                    int r = row - 1;
                    int c = col - 1;
                    
                    if (airplane[r][c].status == 'R') {
                        airplane[r][c].status = 'F';
                        flight.freeSeats++;
                        flight.reservedSeats--;
                        
                        cout << "\n[SUCCESS] Seat force cancelled!\n";
                        saveFlightState(airplane, flight);
                    } else {
                        cout << "\n[INFO] Seat is already free!\n";
                    }
                } else {
                    cout << "\n[ERROR] Invalid seat position!\n";
                }
                break;
            }
            
            case 6: {
                ofstream report("flight_summary_report.txt");
                if (report) {
                    report << "FLIGHT MANAGEMENT SYSTEM - SUMMARY REPORT\n";
                    report << "==========================================\n\n";
                    report << "Flight Number: " << flight.flightNumber << "\n";
                    report << "Route: " << flight.flightRoute << "\n";
                    report << "Departure: " << flight.departureTime << "\n";
                    report << "Arrival: " << flight.arrivalTime << "\n\n";
                    report << "Total Seats: " << (flight.totalRows * flight.totalColumns) << "\n";
                    report << "Available: " << flight.freeSeats << "\n";
                    report << "Booked: " << flight.reservedSeats << "\n";
                    report << "Revenue: Rs. " << fixed << setprecision(2) << flight.totalRevenue << "\n";
                    report.close();
                    cout << "\n[SUCCESS] Report generated: flight_summary_report.txt\n";
                } else {
                    cout << "\n[ERROR] Could not create report file!\n";
                }
                break;
            }
            
            case 7:
                cout << "\n[INFO] Exiting admin panel...\n";
                return;  
                
            default:
                cout << "\n[ERROR] Invalid choice!\n";
        }
    }
}

void viewRevenueReport(const Flight& flight, const vector<FlightBooking>& history) {
    cout << "\n========================================\n";
    cout << "         REVENUE REPORT\n";
    cout << "========================================\n";
    cout << "Flight: " << flight.flightNumber << " (" << flight.flightRoute << ")\n\n";
    
    cout << "Total Revenue: Rs. " << fixed << setprecision(2) << flight.totalRevenue << endl;
    cout << "Total Bookings Made: " << history.size() << endl;
    cout << "Active Bookings: " << flight.reservedSeats << endl;
    cout << "Cancelled Bookings: " << (history.size() - flight.reservedSeats) << endl;
    
    int firstClassCount = 0, businessCount = 0, economyCount = 0;
    double firstClassRevenue = 0, businessRevenue = 0, economyRevenue = 0;
    
    for (const auto& booking : history) {
        if (booking.isActive) {  
            if (booking.seatClass == "FIRST CLASS") {
                firstClassCount++;
                firstClassRevenue += booking.price;
            } else if (booking.seatClass == "BUSINESS") {
                businessCount++;
                businessRevenue += booking.price;
            } else if (booking.seatClass == "ECONOMY") {
                economyCount++;
                economyRevenue += booking.price;
            }
        }
    }
    
    cout << "\n--- CLASS-WISE BREAKDOWN ---\n";
    cout << "First Class:  " << firstClassCount << " seats | Rs. " << firstClassRevenue << endl;
    cout << "Business:     " << businessCount << " seats | Rs. " << businessRevenue << endl;
    cout << "Economy:      " << economyCount << " seats | Rs. " << economyRevenue << endl;
    
        double occupancyRate = (flight.reservedSeats * 100.0) / (flight.totalRows * flight.totalColumns);
    cout << "\nOccupancy Rate: " << fixed << setprecision(1) << occupancyRate << "%\n";
    
    if (flight.reservedSeats > 0) {
        double avgRevenue = flight.totalRevenue / flight.reservedSeats;
        cout << "Average Revenue per Booking: Rs. " << fixed << setprecision(2) << avgRevenue << endl;
    }
    
    cout << "========================================\n";
}

void saveBookingsToFile(const vector<FlightBooking>& history) {
    ofstream file("flight_bookings.txt");
    
    if (!file) {  
        cout << "\n[ERROR] Could not save bookings to file!\n";
        return;
    }
    
    for (const auto& booking : history) {

        file << booking.bookingID << "|" 
             << booking.passengerName << "|"
             << booking.passengerPhone << "|"
             << booking.passengerEmail << "|"
             << booking.passengerCNIC << "|"
             << booking.row << "|"
             << booking.column << "|"
             << booking.price << "|"
             << booking.seatClass << "|"
             << booking.bookingTime << "|"
             << booking.isActive << "\n";  
    }
    
    file.close();  
}

void loadBookingsFromFile(vector<FlightBooking>& history) {
    
    ifstream file("flight_bookings.txt");
    
    if (!file) {  
        return;   
        }
    
    string line;
    
    while (getline(file, line)) {
        FlightBooking booking;
        
        stringstream ss(line);
        string temp;          
        
        getline(ss, temp, '|');  
        booking.bookingID = stoi(temp);  
        
        getline(ss, booking.passengerName, '|');
        getline(ss, booking.passengerPhone, '|');
        getline(ss, booking.passengerEmail, '|');
        getline(ss, booking.passengerCNIC, '|');
        
        getline(ss, temp, '|');
        booking.row = stoi(temp);
        
        getline(ss, temp, '|');
        booking.column = stoi(temp);
        
        getline(ss, temp, '|');
        booking.price = stoi(temp);
        
        getline(ss, booking.seatClass, '|');
        getline(ss, booking.bookingTime, '|');
        
        getline(ss, temp, '|');
        booking.isActive = (temp == "1");  
        history.push_back(booking);
         
        if (booking.bookingID >= nextBookingID) {
            nextBookingID = booking.bookingID + 1;
        }
    }
    
    file.close();
}

void saveFlightState(const vector<vector<Seat>>& airplane, const Flight& flight) {
    ofstream file("flight_state.txt");
    
    if (!file) {
        return;  
    }
    
    file << flight.flightNumber << "|"
         << flight.flightRoute << "|"
         << flight.totalRows << "|"
         << flight.totalColumns << "|"
         << flight.freeSeats << "|"
         << flight.reservedSeats << "|"
         << flight.totalRevenue << "|"
         << flight.departureTime << "|"
         << flight.arrivalTime << "\n";
    
    for (const auto& row : airplane) {
        for (const auto& seat : row) {
            file << seat.status;  
        }
        file << "\n";
    }
    
    file.close();
}

void loadFlightState(vector<vector<Seat>>& airplane, Flight& flight) {
    ifstream file("flight_state.txt");
    
    if (!file) {
        return;  
    }
    
    string line;
    getline(file, line);  
    
    stringstream ss(line);
    string temp;
    
    getline(ss, flight.flightNumber, '|');
    getline(ss, flight.flightRoute, '|');
    
    getline(ss, temp, '|');
    flight.totalRows = stoi(temp);
    
    getline(ss, temp, '|');
    flight.totalColumns = stoi(temp);
    
    getline(ss, temp, '|');
    flight.freeSeats = stoi(temp);
    
    getline(ss, temp, '|');
    flight.reservedSeats = stoi(temp);
    
    getline(ss, temp, '|');
    flight.totalRevenue = stod(temp);  // stod is used for converion of tring to double 
    
    getline(ss, flight.departureTime, '|');
    getline(ss, flight.arrivalTime, '|');
    
    airplane.resize(flight.totalRows, vector<Seat>(flight.totalColumns));
    
    for (int i = 0; i < flight.totalRows; i++) {
        getline(file, line);
        
        for (int j = 0; j < flight.totalColumns; j++) {
            airplane[i][j].row = i + 1;
            airplane[i][j].column = j + 1;
            airplane[i][j].status = line[j];  // Now we are getting character at position j
        
            if (i < 2) {
                airplane[i][j].seatClass = "FIRST CLASS";
                airplane[i][j].price = 25000;
            } else if (i < 6) {
                airplane[i][j].seatClass = "BUSINESS";
                airplane[i][j].price = 15000;
            } else {
                airplane[i][j].seatClass = "ECONOMY";
                airplane[i][j].price = 8000;
            }
        }
    }
    
    file.close();
}

void generateTicket(const FlightBooking& booking, const Flight& flight) {
    cout << "\n========================================\n";
    cout << "         BOARDING PASS\n";
    cout << "========================================\n";
    cout << "Booking ID:     " << booking.bookingID << endl;
    cout << "Passenger Name: " << booking.passengerName << endl;
    cout << "Phone:          " << booking.passengerPhone << endl;
    cout << "Email:          " << booking.passengerEmail << endl;
    cout << "----------------------------------------\n";
    cout << "Flight:         " << flight.flightNumber << endl;
    cout << "Route:          " << flight.flightRoute << endl;
    cout << "Departure:      " << flight.departureTime << endl;
    cout << "Arrival:        " << flight.arrivalTime << endl;
    cout << "----------------------------------------\n";
    cout << "Seat:           Row " << booking.row << ", Seat " << booking.column << endl;
    cout << "Class:          " << booking.seatClass << endl;
    cout << "Fare:           Rs. " << booking.price << endl;
    cout << "Booking Time:   " << booking.bookingTime << endl;
    cout << "========================================\n";
    cout << "   Please arrive 2 hours before departure\n";
    cout << "========================================\n";
    
    string filename = "ticket_" + to_string(booking.bookingID) + ".txt";
    ofstream ticketFile(filename);
    
    if (ticketFile) {
        ticketFile << "======================================\n";
        ticketFile << "         FLIGHT BOARDING PASS\n";
        ticketFile << "======================================\n";
        ticketFile << "Booking ID: " << booking.bookingID << "\n";
        ticketFile << "Passenger: " << booking.passengerName << "\n";
        ticketFile << "Phone: " << booking.passengerPhone << "\n";
        ticketFile << "Email: " << booking.passengerEmail << "\n\n";
        ticketFile << "Flight: " << flight.flightNumber << "\n";
        ticketFile << "Route: " << flight.flightRoute << "\n";
        ticketFile << "Departure: " << flight.departureTime << "\n";
        ticketFile << "Arrival: " << flight.arrivalTime << "\n\n";
        ticketFile << "Seat: Row " << booking.row << ", Seat " << booking.column << "\n";
        ticketFile << "Class: " << booking.seatClass << "\n";
        ticketFile << "Fare: Rs. " << booking.price << "\n";
        ticketFile << "Booked: " << booking.bookingTime << "\n";
        ticketFile << "======================================\n";
        ticketFile.close();
        
        cout << "[INFO] Ticket saved as: " << filename << endl;
    }
}
}