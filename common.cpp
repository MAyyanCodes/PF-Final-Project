#include "common.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <sstream>
using namespace std;

string getCurrentDateTime() {
    time_t now = time(0);
    tm* localTime = localtime(&now);
    
    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localTime);             // strftime = string format time function
    
    return string(buffer);
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int countCinemaBookingsByCNIC(const string& cnic) {   
    ifstream file("bookings.txt");
    if (!file) return 0;
    
    int count = 0;
    string line;
    
    while (getline(file, line)) {
        stringstream ss(line);
        string temp;
        
        getline(ss, temp, '|');
        getline(ss, temp, '|');
        getline(ss, temp, '|');
    

        string bookingCNIC;
        getline(ss, bookingCNIC, '|');
        
        
            for (int i = 0; i < 5; i++) {
            getline(ss, temp, '|');
        }
        
        string isActiveStr;
        getline(ss, isActiveStr, '|');
        bool isActive = (isActiveStr == "1");
        
        if (bookingCNIC == cnic && isActive) {
            count++;
        }
    }
    
    file.close();
    return count;
}


int countFlightBookingsByCNIC(const string& cnic) { 
    ifstream file("flight_bookings.txt");
    if (!file) return 0;
    
    int count = 0;
    string line;
    
    while (getline(file, line)) {
        stringstream ss(line);
        string temp;
        
        
        getline(ss, temp, '|');
        getline(ss, temp, '|');
        getline(ss, temp, '|');
        getline(ss, temp, '|');
        
    
        string bookingCNIC;
        getline(ss, bookingCNIC, '|');
        

        for (int i = 0; i < 5; i++) {
            getline(ss, temp, '|');
        }
        
        string isActiveStr;
        getline(ss, isActiveStr, '|');
        bool isActive = (isActiveStr == "1");
        
        if (bookingCNIC == cnic && isActive) {
            count++;
        }
    }
    
    file.close();
    return count;
}

void displayCrossSystemBookings(const string& cnic, const string& currentSystem) {
    int cinemaBookings = countCinemaBookingsByCNIC(cnic);
    int flightBookings = countFlightBookingsByCNIC(cnic);
    
    if (cinemaBookings > 0 || flightBookings > 0) {
        cout << "\n========================================\n";
        cout << "   CROSS-SYSTEM BOOKING INFORMATION\n";
        cout << "========================================\n";
        cout << "CNIC: " << cnic << "\n\n";
        
        if (cinemaBookings > 0 && flightBookings > 0) {
            cout << "[INFO] This customer has bookings in BOTH systems!\n\n";
        }
        
        cout << "Cinema Management System:  " << cinemaBookings << " active booking(s)\n";
        cout << "Flight Management System:  " << flightBookings << " active booking(s)\n";
        cout << "Total Active Bookings:     " << (cinemaBookings + flightBookings) << "\n";
        cout << "========================================\n";
    } else {
        cout << "\n[INFO] No active bookings found in either system for CNIC: " << cnic << "\n";
    }
}