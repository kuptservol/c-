#include "date.h"
#include <iomanip>

Date::Date(const int &y, const int &m, const int &d) : year(y), month(m), day(d) {
}

int Date::GetYear() const {
    return year;
}

int Date::GetMonth() const {
    return month;
}

int Date::GetDay() const {
    return day;
}

string Date::Print() const {
    string os;
    string year = "000" + to_string(GetYear());
    string month = "0" + to_string(GetMonth());
    string day = "0" + to_string(GetDay());
    os = year.substr(year.size() - 4, 4) + "-" + month.substr(month.size() - 2, 2) + "-"
         + day.substr(day.size() - 2, 2);

    return os;
}

ostream& operator<<(ostream& os, const Date& date)
{
    os << setfill('0') << setw(4) << date.GetYear() << '-';
    os << setfill('0') << setw(2) << date.GetMonth() << '-';
    os << setfill('0') << setw(2) << date.GetDay();
    return os;
}

bool operator<(const Date &one, const Date &another) {
    if (one.GetYear() == another.GetYear()) {
        if (one.GetMonth() == another.GetMonth()) {
            return one.GetDay() < another.GetDay();
        } else {
            return one.GetMonth() < another.GetMonth();
        }
    } else {
        return one.GetYear() < another.GetYear();
    }
}

bool operator>(const Date &one, const Date &another) {
    if (one.GetYear() == another.GetYear()) {
        if (one.GetMonth() == another.GetMonth()) {
            return one.GetDay() > another.GetDay();
        } else {
            return one.GetMonth() > another.GetMonth();
        }
    } else {
        return one.GetYear() > another.GetYear();
    }
}

bool operator==(const Date &one, const Date &another) {
    return one.GetYear() == another.GetYear() &&
           one.GetMonth() == another.GetMonth() &&
           one.GetDay() == another.GetDay();
}

bool operator!=(const Date &one, const Date &another) {
    return !(one == another);
}

Date ParseDate(istream &is) {
    string s_year, s_month;
    getline(is, s_year, '-');
    getline(is, s_month, '-');
    int day, month, year;
    is >> day;
    month = stoi(s_month);
    year = stoi(s_year);
    if (day < 1 || day > 31 || month < 1 || month > 12)
        throw invalid_argument("Invalid date: " + s_year + "-" + s_month + "-" + to_string(day));
    return Date(year, month, day);
}
