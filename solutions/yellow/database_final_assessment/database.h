#pragma once

#include "date.h"
#include "node.h"
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

struct Result {
    Date date;
    string event;

    string Print() {
        return date.Print() + " " + event;
    }
};

class Database {
public:
    void Add(const Date &date, const string &event);

    template<typename UnaryPredicate>
    int RemoveIf(const UnaryPredicate &predicate) {

        int remove_count = 0;

        for (auto events_by_date_it = events_by_date.begin(); events_by_date_it != events_by_date.end();) {

            const Date &date = events_by_date_it->first;
            vector<string> &events = events_by_date_it->second;

            const auto partition = stable_partition(events.begin(), events.end(),
                                                    [predicate, date](const string &val) {
                                                        return !predicate(date, val);
                                                    });

            for (auto events_it = partition; events_it != events.end();) {
                events_by_date_unique[date].erase(*events_it);
                events_it = events.erase(events_it);
                remove_count++;
            }

            if (events.empty()) {
                events_by_date_it = events_by_date.erase(events_by_date_it);
                events_by_date_unique.erase(date);
            } else {
                events_by_date_it++;
            }
        }

        return remove_count;
    }


    template<typename UnaryPredicate>
    vector<Result> FindIf(UnaryPredicate predicate) const {
        vector<Result> found;

        for (const auto &[date, events] : events_by_date) {
            for (const auto &event : events) {
                if (predicate(date, event)) {
                    found.push_back({date, event});
                }
            }
        }
        return found;
    }

    Result Last(const Date &date) const;

    void Print(ostream &os) const;

private:
    map<Date, vector<string>> events_by_date;
    map<Date, set<string>> events_by_date_unique;
};

ostream &operator<<(ostream &os, const Result &res);

bool operator==(const Result &res1, const Result &res2);

bool operator!=(const Result &res1, const Result &res2);