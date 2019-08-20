#include "database.h"
#include "date.h"
#include "condition_parser.h"
#include "node.h"
#include "test_runner.h"

#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <regex>

using namespace std;

string ParseEvent(istream &is) {
    string value;
    getline(is, value);

    value = regex_replace(value, regex("^ +| +$|( ) +"), "$1");

    return value;
}

void TestAll();

int main() {
    TestAll();

    Database db;

    for (string line; getline(cin, line);) {
        istringstream is(line);

        string command;
        is >> command;
        if (command == "Add") {
            const auto date = ParseDate(is);
            const auto event = ParseEvent(is);
            db.Add(date, event);
        } else if (command == "Print") {
            db.Print(cout);
        } else if (command == "Del") {
            auto condition = ParseCondition(is);
            auto predicate = [condition](const Date &date, const string &event) {
                return condition->Evaluate(date, event);
            };
            int count = db.RemoveIf(predicate);
            cout << "Removed " << count << " entries" << endl;
        } else if (command == "Find") {
            auto condition = ParseCondition(is);
            auto predicate = [condition](const Date &date, const string &event) {
                return condition->Evaluate(date, event);
            };

            const auto entries = db.FindIf(predicate);
            for (const auto &entry : entries) {
                cout << entry << endl;
            }
            cout << "Found " << entries.size() << " entries" << endl;
        } else if (command == "Last") {
            try {
                const Result &last = db.Last(ParseDate(is));
                cout << last << endl;
            } catch (invalid_argument &) {
                cout << "No entries" << endl;
            }
        } else if (command.empty()) {
            continue;
        } else {
            throw logic_error("Unknown command: " + command);
        }
    }

    return 0;
}

void TestParseEvent() {
    {
        istringstream is("event");
        AssertEqual(ParseEvent(is), "event", "Parse event without leading spaces");
    }
    {
        istringstream is("   sport event ");
        AssertEqual(ParseEvent(is), "sport event", "Parse event with leading spaces");
    }
    {
        istringstream is("   sport    event ");
        AssertEqual(ParseEvent(is), "sport event", "Parse event with leading spaces");
    }
    {
        istringstream is("  date event  \n  event event");
        vector<string> events;
        events.push_back(ParseEvent(is));
        events.push_back(ParseEvent(is));
    }
}

void TestParseDate() {
    {
        istringstream is("0001-01-01");
        Date dat = ParseDate(is);
        Assert(dat == Date(1, 1, 1), "Parse date 0001-01-01");

        ostringstream os;
        os << dat;
        AssertEqual(os.str(), "0001-01-01", "Parse date 0001-01-01");
    }
    {
        istringstream is("2017-11-07");
        Assert(ParseDate(is) == Date(2017, 11, 7), "Parse date 2017-11-07");
    }
    {
        istringstream is("0-2-31");
        Assert(ParseDate(is) == Date(0, 2, 31), "Parse date 0-2-31");
    }
}

void TestPrintDate() {
    {
        ostringstream os;
        auto date = Date(2017, 11, 7);
        os << date;

        AssertEqual(os.str(), "2017-11-07", "Out date 2017-11-07");
    }
    {
        ostringstream os;
        auto date = Date(1, 1, 1);
        os << date;

        AssertEqual(os.str(), "0001-01-01", "Out date 0001-01-01");
    }
}

void TestDB() {
    {
        Database db = Database();

        ostringstream os;
        db.Print(os);

        AssertEqual(os.str(), "", "test print 1");
    }
    {
        Database db = Database();
        db.Add(Date(2017, 1, 1), "Holiday");

        ostringstream os;
        db.Print(os);

        AssertEqual(os.str(), "2017-01-01 Holiday\n", "test add 1");
    }
    {
        Database db = Database();
        db.Add(Date(2017, 1, 1), "Holiday");
        db.Add(Date(2017, 3, 8), "Holiday");
        db.Add(Date(2017, 1, 1), "New Year");
        db.Add(Date(2017, 1, 1), "New Year");

        ostringstream os;
        db.Print(os);

        AssertEqual(os.str(), "2017-01-01 Holiday\n"
                              "2017-01-01 New Year\n"
                              "2017-03-08 Holiday\n", "test add 2");
    }
    {
        Database db = Database();
        db.Add(Date(2017, 6, 1), "Someone's birthday");
        db.Add(Date(2017, 7, 8), "Someone's birthday");
        db.Add(Date(2017, 7, 8), "8th of July");

        istringstream is("event == \"Someone's birthday\"");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const string &event) {
            return condition->Evaluate(date, event);
        };

        AssertEqual(db.RemoveIf(predicate), 2, "remove by event name");

        ostringstream os;
        db.Print(os);

        AssertEqual(os.str(), "2017-07-08 8th of July\n", "print ok after remove by event name");

        AssertEqual(db.RemoveIf(predicate), 0, "remove by event name");
    }
    {
        Database db = Database();
        db.Add(Date(2017, 6, 1), "1st of June");
        db.Add(Date(2017, 7, 8), "8th of July");
        db.Add(Date(2017, 7, 8), "Someone's birthday");

        istringstream is("date == 2017-07-08");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const string &event) {
            return condition->Evaluate(date, event);
        };

        AssertEqual(db.RemoveIf(predicate), 2, "remove some");

        ostringstream os;
        db.Print(os);

        AssertEqual(os.str(), "2017-06-01 1st of June\n", "print ok");

        istringstream is2("date == 2017-06-01");
        auto condition2 = ParseCondition(is2);
        auto predicate2 = [condition2](const Date &date, const string &event) {
            return condition2->Evaluate(date, event);
        };

        AssertEqual(db.RemoveIf(predicate2), 1, "remove last");
        AssertEqual(db.RemoveIf(predicate), 0, "remove nothing");
        AssertEqual(db.RemoveIf(predicate2), 0, "remove nothing");
    }
    {
        Database db = Database();
        db.Add(Date(2017, 6, 1), "1st of June");
        db.Add(Date(2017, 7, 8), "8th of July");
        db.Add(Date(2017, 7, 8), "Someone's birthday");

        istringstream is("");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const string &event) {
            return condition->Evaluate(date, event);
        };

        AssertEqual(db.RemoveIf(predicate), 3, "remove all");

        ostringstream os;
        db.Print(os);

        AssertEqual(os.str(), "", "print nothing");
    }
    {
        Database db = Database();

        db.Add(Date(2017, 1, 1), "Holiday");
        db.Add(Date(2017, 3, 8), "Holiday");
        db.Add(Date(2017, 1, 1), "New Year");

        istringstream is("event != \"working day\"");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const string &event) {
            return condition->Evaluate(date, event);
        };

        const auto &count = db.FindIf(predicate);

        AssertEqual(count.size(), 3, "count 3");
    }
    {
        Database db = Database();

        db.Add(Date(2017, 1, 1), "Holiday");
        db.Add(Date(2017, 3, 8), "Holiday");
        db.Add(Date(2017, 1, 1), "New Year");

        istringstream is("");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const string &event) {
            return condition->Evaluate(date, event);
        };

        const auto &count = db.FindIf(predicate);

        AssertEqual(count.size(), 3, "count 3");
    }
    {
        Database db = Database();

        db.Add(Date(2017, 1, 1), "Holiday");
        db.Add(Date(2017, 3, 8), "Holiday");
        db.Add(Date(2017, 1, 1), "New Year");

        istringstream is("date > 2017-1-1");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const string &event) {
            return condition->Evaluate(date, event);
        };

        const auto &count = db.FindIf(predicate);

        AssertEqual(count.size(), 1, "count 1");
    }
    {
        Database db = Database();

        db.Add(Date(2017, 1, 1), "New Year");
        db.Add(Date(2017, 3, 8), "Holiday");
        db.Add(Date(2017, 1, 1), "Holiday");

        bool no_entries = false;
        try {
            db.Last(Date(2016, 12, 31));
        }
        catch (invalid_argument &) {
            no_entries = true;
        }

        Assert(no_entries, "expect no entries");

        Result p = db.Last(Date(2017, 1, 1));

        AssertEqual(Date(2017, 1, 1), p.date, "last 1");
        AssertEqual("Holiday", p.event, "last 1");

        Result p2 = db.Last(Date(2017, 6, 1));

        AssertEqual(Date(2017, 3, 8), p2.date, "last 2");
        AssertEqual("Holiday", p2.event, "last 2");
    }
    {
        Database db = Database();

        db.Add(Date(2017, 1, 1), "New Year");

        istringstream is("event == \"New Year\"");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const string &event) {
            return condition->Evaluate(date, event);
        };

        db.RemoveIf(predicate);

        bool no_entries = false;
        try {
            db.Last(Date(2016, 12, 31));
        }
        catch (invalid_argument &) {
            no_entries = true;
        }

        Assert(no_entries, "expect no entries");

        db.RemoveIf(predicate);
    }
    {
        Database db = Database();

        bool no_entries = false;
        try {
            db.Last(Date(2016, 12, 31));
        }
        catch (invalid_argument &) {
            no_entries = true;
        }
        Assert(no_entries, "expect no entries");

        db.Add(Date(2017, 1, 1), "New Year");
        db.Add(Date(2017, 1, 1), "New Year 2");

        no_entries = false;
        try {
            db.Last(Date(2016, 12, 31));
        }
        catch (invalid_argument &) {
            no_entries = true;
        }

        Assert(no_entries, "expect no entries");

        Result r1 = db.Last(Date(2019, 12, 31));
        AssertEqual(r1.event, "New Year 2", "");

        Result r2 = db.Last(Date(2017, 1, 1));
        AssertEqual(r2.event, "New Year 2", "");
    }
}

void TestDatabase() {
    istringstream empty_is("");
    auto empty_condition = ParseCondition(empty_is);
    auto empty_predicate = [empty_condition](const Date &date, const string &event) {
        return empty_condition->Evaluate(date, event);
    };

    // Add 2 - Del 1 - Add deleted again
    {
        Database db;
        Date d(2019, 1, 1);
        db.Add(d, "e1");
        db.Add(d, "e2");
        istringstream is(R"(event == "e1")");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const string &event) {
            return condition->Evaluate(date, event);
        };
        AssertEqual(db.RemoveIf(predicate), 1, "Db Add2-Del-Add 1");
        db.Add(d, "e1");
        AssertEqual(db.FindIf(empty_predicate).size(), 2, "Db Add2-Del-Add 2");
    }

    // Add
    {
        Database db;
        Date d(2019, 1, 1);
        db.Add(d, "e1");
        db.Add(d, "e1");
        istringstream is("date == 2019-01-01");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const string &event) {
            return condition->Evaluate(date, event);
        };
        AssertEqual(db.FindIf(predicate).size(), 1, "Db Add Duplicates 1");
    }

    // Last
    {
        Database db;
        Date d(2019, 1, 1);
        Date d1(2019, 1, 2);
        Date d2(2018, 12, 22);
        db.Add(d1, "e1");
        db.Add(d2, "e2");
        AssertEqual(db.Last(d).Print(), "2018-12-22 e2", "Db Last 1");
        Date d3(2018, 12, 24);
        db.Add(d3, "e3");
        AssertEqual(db.Last(d).Print(), "2018-12-24 e3", "Db Last 2");

        // Get last event for date before date event
        try {
            Date d4(2017, 2, 2);
            db.Last(d4);
            Assert(false, "Db Last 3");
        } catch (invalid_argument e) {
            // Pass
        }

        // Delete event and get last
        istringstream is("date == 2018-12-24");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const string &event) {
            return condition->Evaluate(date, event);
        };
        db.RemoveIf(predicate);
        AssertEqual(db.Last(d).Print(), "2018-12-22 e2", "Db Last 4");

        AssertEqual(db.Last(d1).Print(), "2019-01-02 e1", "Db Last 5");
        db.Add(d2, "e4");
        AssertEqual(db.Last(d2).Print(), "2018-12-22 e4", "Db Last 6");
    }

    // Del
    {
        Database db;
        db.Add({2019, 1, 1}, "e1");
        db.Add({2019, 1, 1}, "e2");
        db.Add({2018, 1, 7}, "e3");
        db.Add({2018, 1, 7}, "e4");
        istringstream is("date == 2018-01-07");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const string &event) {
            return condition->Evaluate(date, event);
        };
        AssertEqual(db.RemoveIf(predicate), 2, "Db Del 1");
    }
    {
        Database db;
        db.Add({2019, 1, 1}, "e1");
        db.Add({2019, 1, 1}, "e2");
        db.Add({2018, 1, 7}, "e3");
        db.Add({2018, 1, 7}, "e4");
        istringstream is("date >= 2018-01-07 AND date <= 2020-01-01");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const string &event) {
            return condition->Evaluate(date, event);
        };
        AssertEqual(db.RemoveIf(predicate), 4, "Db Del 2");
    }
    {
        Database db;
        db.Add({2019, 1, 1}, "e1");
        db.Add({2019, 1, 1}, "e2");
        db.Add({2018, 1, 7}, "e3");
        db.Add({2018, 1, 7}, "e4");
        AssertEqual(db.RemoveIf(empty_predicate), 4, "Db Del 3");
    }
    {
        Database db;
        db.Add({2019, 1, 1}, "e1");
        db.Add({2019, 1, 1}, "e2");
        db.Add({2018, 1, 7}, "e3");
        db.Add({2018, 1, 7}, "e4");
        istringstream is(R"(event == "e1")");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const string &event) {
            return condition->Evaluate(date, event);
        };
        AssertEqual(db.RemoveIf(predicate), 1, "Db Del 4");
    }

    {
        Database db;
        db.Add({2019, 1, 1}, "e1");
        db.Add({2019, 1, 1}, "e2");
        db.Add({2018, 1, 7}, "e3");
        db.Add({2018, 1, 7}, "e4");
        istringstream is(R"(event == "e1" OR date == 2019-01-01)");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const string &event) {
            return condition->Evaluate(date, event);
        };
        AssertEqual(db.RemoveIf(predicate), 2, "Db Del 5");
    }

    // Find
    {
        Database db;
        db.Add({2019, 1, 1}, "e1");
        db.Add({2019, 1, 1}, "e2");
        db.Add({2018, 1, 7}, "e3");
        db.Add({2018, 1, 7}, "e4");
        istringstream is("date == 2018-01-07");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const string &event) {
            return condition->Evaluate(date, event);
        };
        AssertEqual(db.FindIf(predicate).size(), 2, "Db Find 1");
    }
    {
        Database db;
        db.Add({2019, 1, 1}, "e1");
        db.Add({2019, 1, 1}, "e2");
        db.Add({2018, 1, 7}, "e3");
        db.Add({2018, 1, 7}, "e4");
        istringstream is("date >= 2018-01-07 AND date <= 2020-01-01");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const string &event) {
            return condition->Evaluate(date, event);
        };
        AssertEqual(db.FindIf(predicate).size(), 4, "Db Find 2");
    }
    {
        Database db;
        db.Add({2019, 1, 1}, "e1");
        db.Add({2019, 1, 1}, "e2");
        db.Add({2018, 1, 7}, "e3");
        db.Add({2018, 1, 7}, "e4");
        AssertEqual(db.FindIf(empty_predicate).size(), 4, "Db Find 3");
    }
    {
        Database db;
        db.Add({2019, 1, 1}, "e1");
        db.Add({2019, 1, 1}, "e2");
        db.Add({2018, 1, 7}, "e3");
        db.Add({2018, 1, 7}, "e4");
        istringstream is(R"(event == "e1")");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const string &event) {
            return condition->Evaluate(date, event);
        };
        AssertEqual(db.FindIf(predicate).size(), 1, "Db Find 4");
    }

    {
        Database db;
        db.Add({2019, 1, 1}, "e1");
        db.Add({2019, 1, 1}, "e2");
        db.Add({2018, 1, 7}, "e3");
        db.Add({2018, 1, 7}, "e4");
        istringstream is(R"(event == "e1" OR date == 2019-01-01)");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const string &event) {
            return condition->Evaluate(date, event);
        };
        AssertEqual(db.FindIf(predicate).size(), 2, "Db Find 5");
    }

    // Add - Del - Add - Del
    {
        Database db;
        db.Add({2019, 1, 1}, "e1");
        db.Add({2019, 1, 1}, "e2");
        AssertEqual(db.RemoveIf(empty_predicate), 2, "Db Add-Del-Add-Del 1");

        db.Add({2019, 1, 1}, "e1");
        db.Add({2019, 1, 1}, "e2");
        AssertEqual(db.RemoveIf(empty_predicate), 2, "Db Add-Del-Add-Del 1");
    }
    {
        Database db;
        db.Add({2017, 1, 1}, "date");
        db.Add({2017, 1, 1}, "event");
        db.Add({2017, 1, 1}, "third");
        db.Add({2017, 1, 1}, "fourth");
        db.Add({2017, 1, 1}, "five");

        istringstream is(R"(event == "event" OR event == "fourth")");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const string &event) {
            return condition->Evaluate(date, event);
        };

        AssertEqual(2, db.RemoveIf(predicate), "Remove several");

        ostringstream os;
        db.Print(os);

        AssertEqual("2017-01-01 date\n2017-01-01 third\n2017-01-01 five\n", os.str(),
                    "Check print after remove several- 3");
    }
    {
        Database db;
        db.Add({2018, 3, 8}, "preved");
        db.Add({2018, 8, 3}, "medved");

        istringstream is("event !=\"medved\"");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const string &event) {
            return condition->Evaluate(date, event);
        };

        AssertEqual(1, db.RemoveIf(predicate), "Remove only one");

        db.Add({2018, 3, 8}, "krasavcheg");
        Result p = db.Last(Date(2018, 3, 8));

        AssertEqual(Date(2018, 3, 8), p.date, "last 1");
        AssertEqual("krasavcheg", p.event, "last 1");

        db.Add({2018, 3, 8}, "krasavcheg");
        Result p2 = db.Last(Date(2018, 3, 8));

        AssertEqual(Date(2018, 3, 8), p2.date, "last 1");
        AssertEqual("krasavcheg", p2.event, "last 1");
    }
    {
        Database db = Database();

        db.Add(Date(2017, 1, 1), "New Year");
        db.Add(Date(2017, 3, 8), "Holiday");

        Result p = db.Last(Date(2017, 2, 1));

        AssertEqual(Date(2017, 1, 1), p.date, "last 1");
        AssertEqual("New Year", p.event, "last 1");
    }
    {
        Database db = Database();

        db.Add(Date(2017, 1, 1), "New Year");

        Result p = db.Last(Date(2017, 2, 1));

        AssertEqual(Date(2017, 1, 1), p.date, "last 1");
        AssertEqual("New Year", p.event, "last 1");
    }
    {
        Database db = Database();

        db.Add(Date(2017, 1, 2), "New Year");

        bool no_entries = false;
        try {
            db.Last(Date(2017, 1, 1));
        }
        catch (invalid_argument &) {
            no_entries = true;
        }

        Assert(no_entries, "expect no entries");
    }
    {
        Database db = Database();

        db.Add(Date(2017, 11, 21), "Tuesday");
        db.Add(Date(2017, 11, 20), "Monday");
        db.Add(Date(2017, 11, 21), "Weekly meeting");

        istringstream is("event != \"Weekly meeting\"");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date &date, const string &event) {
            return condition->Evaluate(date, event);
        };
        AssertEqual(db.FindIf(predicate).size(), 2, "Db Find 2");
    }
}

void TestDatabaseAddAndPrint() {
    {
        Database db;
        db.Add({2017, 11, 15}, "Wednesday");
        db.Add({2017, 11, 16}, "Thursday");
        db.Add({2017, 11, 17}, "Friday");

        ostringstream os;
        db.Print(os);

        const string expected = "2017-11-15 Wednesday\n"
                                "2017-11-16 Thursday\n"
                                "2017-11-17 Friday\n";
        AssertEqual(os.str(), expected, "Database print: 3 simple events");
    }
    {
        Database db;
        db.Add({2017, 3, 1}, "1st of March");
        db.Add({2017, 2, 1}, "1st of February");
        db.Add({2017, 1, 1}, "1st of January");

        ostringstream os;
        db.Print(os);

        const string expected = "2017-01-01 1st of January\n"
                                "2017-02-01 1st of February\n"
                                "2017-03-01 1st of March\n";
        AssertEqual(os.str(), expected, "Database print: events should sorted by date");
    }
    {
        Database db;
        db.Add({2017, 3, 1}, "01.03 1");
        db.Add({2017, 3, 5}, "05.03 1");
        db.Add({2017, 3, 1}, "01.03 2");
        db.Add({2017, 3, 1}, "01.03 3");
        db.Add({2017, 3, 5}, "05.03 2");

        ostringstream os;
        db.Print(os);

        const string expected = "2017-03-01 01.03 1\n"
                                "2017-03-01 01.03 2\n"
                                "2017-03-01 01.03 3\n"
                                "2017-03-05 05.03 1\n"
                                "2017-03-05 05.03 2\n";
        AssertEqual(os.str(), expected, "Database print: multiple events at single date");
    }
    {
        Database db;
        db.Add({2017, 3, 1}, "01.03 1");
        db.Add({2017, 3, 1}, "01.03 2");
        db.Add({2017, 3, 1}, "01.03 1");
        db.Add({2017, 3, 1}, "01.03 1");

        ostringstream os;
        db.Print(os);

        const string expected = "2017-03-01 01.03 1\n"
                                "2017-03-01 01.03 2\n";
        AssertEqual(os.str(), expected, "Database print: identical events at the given date must be ignored");
    }
}

void TestDateComparisonNode() {
    const Date november_1{2017, 11, 1};
    const Date november_2{2017, 11, 2};
    const Date november_3{2017, 11, 3};

    DateComparisonNode less(Comparison::Less, november_2);
    Assert(less.Evaluate(november_1, ""), "DateComparisonNode: less 1");
    Assert(!less.Evaluate(november_3, ""), "DateComparisonNode: less 2");

    DateComparisonNode not_equal(Comparison::NotEqual, november_2);
    Assert(not_equal.Evaluate(november_1, ""), "DateComparisonNode: not_equal 1");
    Assert(!not_equal.Evaluate(november_2, ""), "DateComparisonNode: not_equal 2");

    DateComparisonNode greater_eq(Comparison::GreaterOrEqual, november_2);
    Assert(greater_eq.Evaluate(november_2, ""), "DateComparisonNode: greater_eq 1");
    Assert(greater_eq.Evaluate(november_3, ""), "DateComparisonNode: greater_eq 2");
    Assert(!greater_eq.Evaluate(november_1, ""), "DateComparisonNode: greater_eq 3");
}

void TestEventComparisonNode() {
    const Date january_1{1970, 1, 1};

    EventComparisonNode less_eq(Comparison::LessOrEqual, "b");
    Assert(less_eq.Evaluate(january_1, "a"), "EventComparisonNode: less_eq 1");
    Assert(!less_eq.Evaluate(january_1, "c"), "EventComparisonNode: less_eq 2");
    Assert(less_eq.Evaluate(january_1, "b"), "EventComparisonNode: less_eq 3");

    EventComparisonNode equal(Comparison::Equal, "b");
    Assert(equal.Evaluate(january_1, "b"), "EventComparisonNode: equal 1");
    Assert(!equal.Evaluate(january_1, "a"), "EventComparisonNode: equal 2");
    Assert(!equal.Evaluate(january_1, "c"), "EventComparisonNode: equal 3");

    EventComparisonNode greater(Comparison::Greater, "b");
    Assert(greater.Evaluate(january_1, "c"), "EventComparisonNode: greater 1");
    Assert(!greater.Evaluate(january_1, "b"), "EventComparisonNode: greater 2");
    Assert(!greater.Evaluate(january_1, "a"), "EventComparisonNode: greater 3");
}

struct FalseNode : Node {
    bool Evaluate(const Date &, const string &) const override {
        return false;
    };
};

void TestDateOutput() {
    {
        ostringstream os;
        os << Date{2017, 11, 15};
        AssertEqual(os.str(), "2017-11-15", "Date output: simple");
    }
    {
        ostringstream os;
        os << Date{2017, 1, 1};
        AssertEqual(os.str(), "2017-01-01", "Date output: day and month less than 10");
    }
    {
        ostringstream os;
        os << Date{2, 10, 10};
        AssertEqual(os.str(), "0002-10-10", "Date output: year less than 1000");
    }
}

void TestParseDate2() {
    istringstream is("2017-11-15");
    const Date date = ParseDate(is);

    AssertEqual(date.GetYear(), 2017, "Parse date: year");
    AssertEqual(date.GetMonth(), 11, "Parse date: month");
    AssertEqual(date.GetDay(), 15, "Parse date: day");
}

void TestLogicalOperationNode() {
    {
        map<bool, shared_ptr<Node>> bool_to_node = {
                {false, make_shared<FalseNode>()},
                {true,  make_shared<EmptyNode>()}
        };

        for (bool value_one : {false, true}) {
            for (bool value_two : {false, true}) {
                LogicalOperationNode or_node(
                        LogicalOperation::Or, bool_to_node[value_one], bool_to_node[value_two]
                );
                LogicalOperationNode and_node(
                        LogicalOperation::And, bool_to_node[value_one], bool_to_node[value_two]
                );

                AssertEqual(or_node.Evaluate({1, 1, 1}, ""), value_one || value_two, "LogicalOperationNode Or");
                AssertEqual(and_node.Evaluate({1, 1, 1}, ""), value_one && value_two, "LogicalOperationNode And");
            }
        }
    }
    {
        LogicalOperationNode root(
                LogicalOperation::And,
                make_shared<DateComparisonNode>(Comparison::Equal, Date{2017, 11, 18}),
                make_shared<EventComparisonNode>(Comparison::Equal, "Saturday")
        );

        Assert(root.Evaluate({2017, 11, 18}, "Saturday"), "LogicalOperationNode propagates arguments 1");
        Assert(!root.Evaluate({2017, 11, 18}, "Friday"), "LogicalOperationNode propagates arguments 2");
        Assert(!root.Evaluate({2017, 11, 1}, "Saturday"), "LogicalOperationNode propagates arguments 1");
    }
}

void TestDatabaseFind() {
    {
        const vector<Result> expected = {
                {{2017, 11, 17}, "Friday"},
                {{2017, 11, 18}, "Saturday"},
                {{2017, 11, 19}, "Sunday"}
        };

        Database db;
        for (const auto &e : expected) {
            db.Add(e.date, e.event);
        }

        auto alwaysTrue = [](const Date &, const string &) { return true; };
        AssertEqual(db.FindIf(alwaysTrue), expected, "Database find: All entries should be found for alwaysTrue");
    }
    {
        Database db;
        db.Add({2017, 11, 17}, "Friday");
        db.Add({2016, 11, 17}, "Thursday");
        db.Add({2015, 11, 17}, "Tuesday");
        db.Add({2014, 11, 17}, "Monday");

        auto complexCondition = [](const Date &date, const string &event) {
            return date.GetYear() == 2016 || event == "Monday";
        };
        const vector<Result> expected = {{{2014, 11, 17}, "Monday"},
                                         {{2016, 11, 17}, "Thursday"}};
        AssertEqual(db.FindIf(complexCondition), expected, "Database find: complex condition");
    }
    {
        Database db;
        db.Add({2017, 11, 17}, "Friday");
        db.Add({2016, 11, 17}, "Thursday");
        db.Add({2015, 11, 17}, "Tuesday");
        db.Add({2014, 11, 17}, "Monday");

        auto acceptsNothing = [](const Date &date, const string &event) {
            return date.GetYear() == 2016 && event == "Monday";
        };
        AssertEqual(db.FindIf(acceptsNothing), vector<Result>(), "Database find: accepts nothing");
    }
}

void TestDatabaseRemove() {
    {
        Database db;
        db.Add({2017, 11, 17}, "Friday");
        db.Add({2016, 11, 17}, "Thursday");
        db.Add({2015, 11, 17}, "Tuesday");
        db.Add({2014, 11, 17}, "Monday");

        auto alwaysTrue = [](const Date &, const string &) { return true; };
        AssertEqual(db.RemoveIf(alwaysTrue), 4, "Database remove: alwaysTrue removes all 1");
        AssertEqual(db.FindIf(alwaysTrue), vector<Result>(), "Database remove: alwaysTrue removes all 2");
    }
    {
        Database db;
        db.Add({2017, 11, 17}, "Friday");
        db.Add({2016, 11, 17}, "Thursday");
        db.Add({2015, 11, 17}, "Tuesday");
        db.Add({2014, 11, 17}, "Monday");

        auto complexCondition = [](const Date &date, const string &event) {
            return date.GetYear() == 2016 || event == "Monday";
        };
        AssertEqual(db.RemoveIf(complexCondition), 2, "Database remove: complex condition 1");
        AssertEqual(db.FindIf(complexCondition), vector<Result>(), "Database remove: complex condition 2");
    }
    {
        const Date date = {2017, 11, 24};

        Database db;
        db.Add(date, "abc");
        db.Add(date, "bca");
        db.Add(date, "abd");
        db.Add(date, "cba");

        ostringstream os;
        db.Print(os);
        const string expected_one =
                "2017-11-24 abc\n"
                "2017-11-24 bca\n"
                "2017-11-24 abd\n"
                "2017-11-24 cba\n";
        AssertEqual(os.str(), expected_one, "Database print after remove 1");

        db.RemoveIf([](const Date &, const string &event) { return event[0] == 'a'; });

        os.str("");
        db.Print(os);
        const string expected_two =
                "2017-11-24 bca\n"
                "2017-11-24 cba\n";
        AssertEqual(os.str(), expected_two,
                    "Database print after remove 2: entries should be printed in order of addition");
    }
}

void TestDatabaseLast() {
    {
        Database db;
        db.Add({2017, 11, 17}, "Friday");
        db.Add({2017, 11, 17}, "One more event");
        db.Add({2016, 11, 17}, "Thursday");
        db.Add({2015, 11, 17}, "Tuesday");
        db.Add({2014, 11, 17}, "Monday");

        AssertEqual(db.Last({2017, 11, 17}), Result{{2017, 11, 17}, "One more event"}, "Database last: successful 1");
        AssertEqual(db.Last({2017, 11, 16}), Result{{2016, 11, 17}, "Thursday"}, "Database last: successful 2");
        AssertEqual(db.Last({2016, 11, 17}), Result{{2016, 11, 17}, "Thursday"}, "Database last: successful 3");
        AssertEqual(db.Last({2016, 11, 18}), Result{{2016, 11, 17}, "Thursday"}, "Database last: successful 4");
        AssertEqual(db.Last({2014, 11, 18}), Result{{2014, 11, 17}, "Monday"}, "Database last: successful 5");
        AssertEqual(db.Last({2014, 11, 17}), Result{{2014, 11, 17}, "Monday"}, "Database last: successful 6");

        bool wasException = false;
        try {
            db.Last({1, 1, 1});
        } catch (invalid_argument &) {
            wasException = true;
        } catch (exception &e) {
            Assert(false, "Database last: unexpected exception " + string(e.what()));
        }
        Assert(wasException, "Database last: invalid argument wasn't thrown");
    }
    {
        Database db;
        db.Add({2017, 11, 17}, "One more event");
        db.Add({2017, 11, 17}, "Friday");
        AssertEqual(db.Last({2017, 11, 17}), Result{{2017, 11, 17}, "Friday"}, "Database last and remove 1");
        db.RemoveIf([](const Date &, const string &event) { return event == "Friday"; });
        AssertEqual(db.Last({2017, 11, 17}), Result{{2017, 11, 17}, "One more event"}, "Database last and remove 2");
    }
    {
        Database db;
        db.Add({2017, 11, 17}, "One more event");
        db.Add({2017, 11, 17}, "Friday");
        db.Add({2016, 11, 17}, "Thursday");
        db.RemoveIf([](const Date &date, const string &) { return date == Date{2017, 11, 17}; });
        AssertEqual(db.Last({2017, 11, 17}), Result{{2016, 11, 17}, "Thursday"}, "Database last and remove 2");
    }
}

void TestParseCondition() {
    {
        istringstream is("date != 2017-11-18");
        shared_ptr<Node> root = ParseCondition(is);
        Assert(root->Evaluate({2017, 1, 1}, ""), "Parse condition 1");
        Assert(!root->Evaluate({2017, 11, 18}, ""), "Parse condition 2");
    }
    {
        istringstream is(R"(event == "sport event")");
        shared_ptr<Node> root = ParseCondition(is);
        Assert(root->Evaluate({2017, 1, 1}, "sport event"), "Parse condition 3");
        Assert(!root->Evaluate({2017, 1, 1}, "holiday"), "Parse condition 4");
    }
    {
        istringstream is("date >= 2017-01-01 AND date < 2017-07-01");
        shared_ptr<Node> root = ParseCondition(is);
        Assert(root->Evaluate({2017, 1, 1}, ""), "Parse condition 5");
        Assert(root->Evaluate({2017, 3, 1}, ""), "Parse condition 6");
        Assert(root->Evaluate({2017, 6, 30}, ""), "Parse condition 7");
        Assert(!root->Evaluate({2017, 7, 1}, ""), "Parse condition 8");
        Assert(!root->Evaluate({2016, 12, 31}, ""), "Parse condition 9");
    }
    {
        istringstream is(R"(event != "sport event" AND event != "Wednesday")");
        shared_ptr<Node> root = ParseCondition(is);
        Assert(root->Evaluate({2017, 1, 1}, "holiday"), "Parse condition 10");
        Assert(!root->Evaluate({2017, 1, 1}, "sport event"), "Parse condition 11");
        Assert(!root->Evaluate({2017, 1, 1}, "Wednesday"), "Parse condition 12");
    }
    {
        istringstream is(R"(event == "holiday AND date == 2017-11-18")");
        shared_ptr<Node> root = ParseCondition(is);
        Assert(!root->Evaluate({2017, 11, 18}, "holiday"), "Parse condition 13");
        Assert(!root->Evaluate({2017, 11, 18}, "work day"), "Parse condition 14");
        Assert(root->Evaluate({1, 1, 1}, "holiday AND date == 2017-11-18"), "Parse condition 15");
    }
    {
        istringstream is(R"(((event == "holiday" AND date == 2017-01-01)))");
        shared_ptr<Node> root = ParseCondition(is);
        Assert(root->Evaluate({2017, 1, 1}, "holiday"), "Parse condition 16");
        Assert(!root->Evaluate({2017, 1, 2}, "holiday"), "Parse condition 17");
    }
    {
        istringstream is(R"(date > 2017-01-01 AND (event == "holiday" OR date < 2017-07-01))");
        shared_ptr<Node> root = ParseCondition(is);
        Assert(!root->Evaluate({2016, 1, 1}, "holiday"), "Parse condition 18");
        Assert(root->Evaluate({2017, 1, 2}, "holiday"), "Parse condition 19");
        Assert(root->Evaluate({2017, 1, 2}, "workday"), "Parse condition 20");
        Assert(!root->Evaluate({2018, 1, 2}, "workday"), "Parse condition 21");
    }
    {
        istringstream is(R"(date > 2017-01-01 AND event == "holiday" OR date < 2017-07-01)");
        shared_ptr<Node> root = ParseCondition(is);
        Assert(root->Evaluate({2016, 1, 1}, "event"), "Parse condition 22");
        Assert(root->Evaluate({2017, 1, 2}, "holiday"), "Parse condition 23");
        Assert(root->Evaluate({2017, 1, 2}, "workday"), "Parse condition 24");
        Assert(!root->Evaluate({2018, 1, 2}, "workday"), "Parse condition 25");
    }
    {
        istringstream is(R"(((date == 2017-01-01 AND event == "holiday")))");
        shared_ptr<Node> root = ParseCondition(is);
        Assert(root->Evaluate({2017, 1, 1}, "holiday"), "Parse condition 26");
        Assert(!root->Evaluate({2017, 1, 2}, "holiday"), "Parse condition 27");
    }
    {
        istringstream is(R"(((event == "2017-01-01" OR date > 2016-01-01)))");
        shared_ptr<Node> root = ParseCondition(is);
        Assert(root->Evaluate({1, 1, 1}, "2017-01-01"), "Parse condition 28");
        Assert(!root->Evaluate({2016, 1, 1}, "event"), "Parse condition 29");
        Assert(root->Evaluate({2016, 1, 2}, "event"), "Parse condition 30");
    }
}

void TestAll() {
    TestRunner tr;
    tr.RunTest(TestParseEvent, "TestParseEvent");
    tr.RunTest(TestParseDate, "TestParseDate");
    tr.RunTest(TestPrintDate, "TestPrintDate");
    tr.RunTest(TestParseCondition, "TestParseCondition");
    tr.RunTest(TestDB, "TestDB");
    tr.RunTest(TestDatabase, "TestDatabase");

    tr.RunTest(TestParseEvent, "TestParseEvent");
    tr.RunTest(TestDateOutput, "TestDateOutput");
    tr.RunTest(TestParseDate2, "TestParseDate");
    tr.RunTest(TestDatabaseAddAndPrint, "TestDatabaseAddAndPrint");
    tr.RunTest(TestDatabaseFind, "TestDatabaseFind");
    tr.RunTest(TestDatabaseRemove, "TestDatabaseRemove");
    tr.RunTest(TestDatabaseLast, "TestDatabaseLast");
    tr.RunTest(TestDateComparisonNode, "TestDateComparisonNode");
    tr.RunTest(TestEventComparisonNode, "TestEventComparisonNode");
    tr.RunTest(TestLogicalOperationNode, "TestLogicalOperationNode");
}
