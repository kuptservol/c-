#pragma once

#include <string>

using namespace std;

struct Date {
    int year, month, day;
};

struct Time {
    int hours, minutes;
};


bool operator<(const Date &date1, const Date &date2);
bool operator!=(const Date &date1, const Date &date2);
bool operator==(const Date &date1, const Date &date2);
bool operator<(const Time &time1, const Time &time2);
ostream& operator<<(ostream& os, const Date& date);

struct AirlineTicket {
    string from;
    string to;
    string airline;
    Date departure_date;
    Time departure_time;
    Date arrival_date;
    Time arrival_time;
    int price;
};