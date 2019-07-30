#include <iostream>
#include <string>

using namespace std;


void SendSms(const string &number, const string &message) {
    cout << "Send '" << message << "' to number " << number << endl;
}

void SendEmail(const string &email, const string &message) {
    cout << "Send '" << message << "' to e-mail " << email << endl;
}

class INotifier {
public:
    INotifier() {}

    virtual void Notify(const string &message) const = 0;
};

class SmsNotifier : public INotifier {
public:
    SmsNotifier(const string &number_) : number(number_) {}

    virtual void Notify(const string &message) const override {
        SendSms(number, message);
    }

private:
    string number;
};

class EmailNotifier : public INotifier {
public:
    EmailNotifier(const string &email_) : email(email_) {}

    virtual void Notify(const string &message) const override {
        SendEmail(email, message);
    }

private:
    string email;
};

/*
 Реализуйте здесь классы INotifier, SmsNotifier, EmailNotifier
 */

void Notify(INotifier &notifier, const string &message) {
    notifier.Notify(message);
}


int main() {
    EmailNotifier email{"na-derevnyu@dedushke.ru"};
    SmsNotifier sms{"+7-495-777-77-77"};

    Notify(sms, "I have White belt in C++");
    Notify(email, "And want a Yellow one");
    return 0;
}
