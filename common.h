#ifndef COMMON_H
#define COMMON_H

#include <string>
using namespace std;

string getCurrentDateTime();
void clearScreen();

int countCinemaBookingsByCNIC(const string& cnic);
int countFlightBookingsByCNIC(const string& cnic);
void displayCrossSystemBookings(const string& cnic, const string& currentSystem);

#endif