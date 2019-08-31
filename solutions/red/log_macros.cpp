#include "tests/test_runner.h"
#include <string>
#include <ostream>
using namespace std;

class Logger {
public:
    explicit Logger(ostream& output_stream) : os(output_stream) {
    }

    void SetLogLine(bool value) { log_line = value; }
    void SetLogFile(bool value) { log_file= value; }

    void Log(const string& message);

    void SetFile(const string& filename) {
        file = filename;
    }

    void SetLine(int line_number) {
        line = line_number;
    }

private:
    ostream& os;
    bool log_line = false;
    bool log_file = false;
    string file;
    int line;
};

void Logger::Log(const string& message) {
    if (log_file && log_line) {
        os << file << ':' << line << ' ';
    } else if (log_file) {
        os << file << ' ';
    } else if (log_line) {
        os << "Line " << line << ' ';
    }
    os << message << '\n';
}

#define LOG(logger, message) \
  logger.SetFile(__FILE__);  \
  logger.SetLine(__LINE__);  \
  logger.Log(message);

void TestLog() {

    ostringstream logs;
    Logger l(logs);
    LOG(l, "hello");

    l.SetLogFile(true);
    LOG(l, "hello");

    l.SetLogLine(true);
    LOG(l, "hello");

    l.SetLogFile(false);
    LOG(l, "hello");

    string expected = "hello\n";
    expected += "logger.cpp hello\n";
    expected += "logger.cpp:10 hello\n";
    expected += "Line 13 hello\n";
    ASSERT_EQUAL(logs.str(), expected);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestLog);
}