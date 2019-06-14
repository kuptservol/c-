#pragma once

#include <string>
#include <sstream>
#include "phone_number.h"

using namespace std;

string PhoneNumber::GetCountryCode() const {
    return country_code_;
}

string PhoneNumber::GetCityCode() const {
    return city_code_;
}

string PhoneNumber::GetLocalNumber() const {
    return local_number_;
}

string PhoneNumber::GetInternationalNumber() const {
    //+7-495-111-22-33
    return "+" + country_code_ + "-" + city_code_ + "-" + local_number_;
}

PhoneNumber::PhoneNumber(const string &international_number) {
    if (international_number[0] != '+') {
        throw invalid_argument("");
    }

    bool country_code_set = false;
    int country_code_set_i;
    bool city_code_set = false;
    for (int i = 1; i < international_number.size(); i++) {
        if (international_number[i] == '-') {
            if (!country_code_set) {
                country_code_set = true;
                country_code_set_i = i + 1;
                country_code_ = international_number.substr(1, i - 1);
            } else if (!city_code_set) {
                city_code_set = true;
                city_code_ = international_number.substr(country_code_set_i, i - country_code_set_i);
                continue;
            }
        }

        if (country_code_set && city_code_set) {
            local_number_ += international_number[i];
        }
    }

    if (!city_code_set || !country_code_set || local_number_.size() == 0) {
        throw invalid_argument("");
    }
}
