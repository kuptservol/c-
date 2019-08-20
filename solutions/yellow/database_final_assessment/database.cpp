#include "database.h"

void Database::Add(const Date &date, const string &event) {
    if (events_by_date_unique[date].count(event) == 0) {
        events_by_date[date].push_back(event);
        events_by_date_unique[date].insert(event);
    }
}

Result Database::Last(const Date &date) const {
    auto it = events_by_date.upper_bound(date);
    if (it == events_by_date.begin()) {
        throw invalid_argument("");
    } else {
        --it;
    }

    const auto &pair = *it;
    return {pair.first, pair.second[pair.second.size() - 1]};
}

void Database::Print(ostream &os) const {
    for (const auto &[date, events] : events_by_date) {
        for (const auto &event : events) {
            os << date << " " << event << endl;
        }
    }
}

ostream &operator<<(ostream &os, const Result &res) {
    os << res.date << " " << res.event;
    return os;
}

bool operator==(const Result &res1, const Result &res2) {
    return res1.date == res2.date && res1.event == res2.event;
}

bool operator!=(const Result &res1, const Result &res2) {
    return !(res1 == res2);
}
