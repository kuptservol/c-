#include "airline_ticket.h"
#include "test_runner.h"

using namespace std;

bool operator<(const Date &lhs, const Date &rhs) {
    return tie(lhs.year, lhs.month, lhs.day) < tie(rhs.year, rhs.month, rhs.day);
}

bool operator==(const Date &lhs, const Date &rhs) {
    return tie(lhs.year, lhs.month, lhs.day) == tie(rhs.year, rhs.month, rhs.day);
}

bool operator!=(const Date &lhs, const Date &rhs) {
    return tie(lhs.year, lhs.month, lhs.day) != tie(rhs.year, rhs.month, rhs.day);
}

bool operator!=(const Time &lhs, const Time &rhs) {
    return tie(lhs.hours, lhs.minutes) != tie(rhs.hours, rhs.minutes);
}

bool operator<(const Time &lhs, const Time &rhs) {
    return tie(lhs.hours, lhs.minutes) < tie(rhs.hours, rhs.minutes);
}

bool operator==(const Time &lhs, const Time &rhs) {
    return tie(lhs.hours, lhs.minutes) == tie(rhs.hours, rhs.minutes);
}

ostream &operator<<(ostream &os, const Date &d) {
    return os << d.year << '-' << d.month << '-' << d.day;
}

istream &operator>>(istream &stream, Date &date) {
    int year, month, day;

    stream >> year;
    stream.ignore(1);
    stream >> month;
    stream.ignore(1);
    stream >> day;

    date.year = year;
    date.month = month;
    date.day = day;

    return stream;
}

istream &operator>>(istream &stream, Time &time) {
    int hours, minutes;

    stream >> hours;
    stream.ignore(1);
    stream >> minutes;

    time.hours = hours;
    time.minutes = minutes;
    return stream;
}

ostream &operator<<(ostream &os, const Time &t) {
    return os << t.hours << ':' << t.minutes;
}

#define UPDATE_FIELD(ticket, field, updates) \
    { \
        auto it = updates.find(#field);    \
        if (it != updates.end()) {  \
            istringstream is;   \
		    is.str(it->second);	\
		    is >> ticket.field; \
        } \
    }

void TestUpdate() {
    AirlineTicket t;
    t.price = 0;

    const map<string, string> updates1 = {
            {"departure_date", "2018-2-28"},
            {"departure_time", "17:40"},
    };
    UPDATE_FIELD(t, departure_date, updates1);
    UPDATE_FIELD(t, departure_time, updates1);
    UPDATE_FIELD(t, price, updates1);

    ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
    ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
    ASSERT_EQUAL(t.price, 0);

    const map<string, string> updates2 = {
            {"price",        "12550"},
            {"arrival_time", "20:33"},
    };
    UPDATE_FIELD(t, departure_date, updates2);
    UPDATE_FIELD(t, departure_time, updates2);
    UPDATE_FIELD(t, arrival_time, updates2);
    UPDATE_FIELD(t, price, updates2);

    ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
    ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
    ASSERT_EQUAL(t.price, 12550);
    ASSERT_EQUAL(t.arrival_time, (Time{20, 33}));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestUpdate);
}