#include <utility>

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <memory>

using namespace std;

class Figure {
public:
    virtual string Name() const = 0;

    virtual double Perimeter() const = 0;

    virtual double Area() const = 0;

protected:
    double pi = 3.14;
};

class Rect : public Figure {
public:
    Rect(string name_, const double &width, const double &height)
            : name(std::move(name_)), area(height * width), perimetr(2 * width + 2 * height) {}

    string Name() const override {
        return name;
    }

    double Perimeter() const override {
        return perimetr;
    }

    double Area() const override {
        return area;
    }

private:
    const string name;
    const double perimetr;
    const double area;
};

class Triangle : public Figure {
public:
    Triangle(string name_, const double &a, const double &b, const double &c)
            : name(std::move(name_)), perimetr(a + b + c),
              area(sqrt(perimetr / 2 * (perimetr / 2 - a) * (perimetr / 2 - b) * (perimetr / 2 - c))) {}

    string Name() const override {
        return name;
    }

    double Perimeter() const override {
        return perimetr;
    }

    double Area() const override {
        return area;
    }

private:
    const string name;
    const double perimetr;
    const double area;
};

class Circle : public Figure {
public:
    Circle(string name_, const double &r)
            : name(std::move(name_)), perimetr(2 * pi * r),
              area(pi * r * r) {}

    string Name() const override {
        return name;
    }

    double Perimeter() const override {
        return perimetr;
    }

    double Area() const override {
        return area;
    }

private:
    const string name;
    const double perimetr;
    const double area;
};

shared_ptr<Figure> CreateFigure(istringstream &is) {
    string type;
    is >> type;

    if (type == "RECT") {
        int width, height;
        is >> width;
        is >> height;

        Rect rect(type, width, height);

        return make_shared<Rect>(rect);
    } else if (type == "TRIANGLE") {
        //ADD TRIANGLE 3 4 5

        int a, b, c;
        is >> a;
        is >> b;
        is >> c;

        Triangle triangle(type, a, b, c);

        return make_shared<Triangle>(triangle);

    } else {
        int r;
        is >> r;

        Circle circle(type, r);

        return make_shared<Circle>(circle);
    }
}


int main() {
    vector<shared_ptr<Figure>> figures;
    for (string line; getline(cin, line);) {
        istringstream is(line);

        string command;
        is >> command;

        if (command == "ADD") {
            figures.push_back(CreateFigure(is));
        } else if (command == "PRINT") {
            for (const auto &current_figure : figures) {
                cout << fixed << setprecision(3)
                     << current_figure->Name() << " "
                     << current_figure->Perimeter() << " "
                     << current_figure->Area() << endl;
            }
        }
    }
    return 0;
}

//Compile error: b"/tmp/submission47u9v1ge/tmp2l_xawqu.cpp:95:1: fatal error: no template named 'shared_ptr'\nshared_ptr<Figure> CreateFigure(istringstream &is) {\n^\n1 error generated.\n"