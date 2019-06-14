#include "tests.h"

void Test() {

    PhoneNumber phone("+7-495-111-22-33");
    AssertEqual(phone.GetCityCode(), "495", "");
    AssertEqual(phone.GetCountryCode(), "7", "");
    AssertEqual(phone.GetCityCode(), "111-22-33", "");
}

void TestAll() {
    TestRunner tr;
    tr.RunTest(Test, "Test");
}