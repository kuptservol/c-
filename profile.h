#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>

using namespace std;
using namespace std::chrono;

class LogDuration {
public:
    explicit LogDuration(const string &msg = "")
            : message(msg + ": "), start(steady_clock::now()) {
    }

    ~ LogDuration() {
        auto finish = steady_clock::now();
        auto dur = finish - start;
        cerr << message << duration_cast<std::chrono::milliseconds>(dur).count() << " ms" << endl;
    }

private:
    string message;
    steady_clock::time_point start;
};

#define LOG_DURATION(message) LogDuration UNIQ_ID( __LINE__){message};