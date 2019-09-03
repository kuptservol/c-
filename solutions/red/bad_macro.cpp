#include "test_runner.h"

#include <ostream>

using namespace std;

#define PRINT_VALUES(out, x, y)   do { out << (x) << endl; out << (y) << endl;} while(0)

int main() {
    TestRunner tr;
    tr.RunTest([] {
        ostringstream output;
        if (true) PRINT_VALUES(output,  5, "red belt");
        else PRINT_VALUES(output,  "\n", "\n");

        PRINT_VALUES(output, 5, "red belt");
        ASSERT_EQUAL(output.str(), "5\nred belt\n5\nred belt\n");
    }, "PRINT_VALUES usage example");
}