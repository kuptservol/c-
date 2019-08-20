#pragma once

#include <ostream>
#include <istream>
#include <iostream>
#include <sstream>

using namespace std;

class Date {
public:
    Date(const int &y, const int &m, const int &d);

    int GetYear() const;

    int GetMonth() const;

    int GetDay() const;

    string Print() const;

private:
    const int year;
    const int month;
    const int day;
};

Date ParseDate(istream &is);

bool operator==(const Date &one, const Date &another);

bool operator!=(const Date &one, const Date &another);

bool operator<(const Date &one, const Date &another);

bool operator>(const Date &one, const Date &another);

ostream &operator<<(ostream &os, const Date &date);