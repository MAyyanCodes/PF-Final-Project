#include <iostream>
#include "cinema_management_system.h"
#include "Flight_management_system.h"
#include "common.h"

using namespace std;

int main() {
    int choice;
    
    while (true) {
        cout << "\n========================================\n";
        cout << "    TRANSPORTATION BOOKING SYSTEM\n";
        cout << "========================================\n";
        cout << "1. Cinema Management System\n";
        cout << "2. Flight Management System\n";
        cout << "3. Exit\n";
        cout << "========================================\n";
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                Cinema::mainCinemaManagementSystem();  // Use namespace
                break;
            case 2:
                Flight::mainFlightManagementSystem();  // Use namespace
                break;
            case 3:
                cout << "\nThank you for using our system!\n";
                return 0;
            default:
                cout << "\nInvalid choice! Please try again.\n";
        }
    }
    
    return 0;
}