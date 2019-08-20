#pragma once

#include "date.h"
#include "comparison_type.h"
#include "logical_op.h"
#include <memory>

using namespace std;

class Node {
public:
    virtual bool Evaluate(const Date &date, const string &event) const = 0;
};

class DateComparisonNode : public Node {
public:

    DateComparisonNode(const Comparison &cmp_, const Date &date_) : cmp(cmp_), date(date_) {}

    bool Evaluate(const Date &date_, const string &event_) const override {
        if (cmp == Comparison::Less) {
            return !(date_ == date) && date_ < date;
        } else if (cmp == Comparison::LessOrEqual) {
            return date_ == date || date_ < date;
        } else if (cmp == Comparison::Greater) {
            return !(date_ == date) && date_ > date;
        } else if (cmp == Comparison::GreaterOrEqual) {
            return date_ == date || date_ > date;
        } else if (cmp == Comparison::Equal) {
            return date_ == date;
        } else if (cmp == Comparison::NotEqual) {
            return !(date_ == date);
        }

        return false;
    }

private:
    const Comparison cmp;
    const Date date;
};

class EventComparisonNode : public Node {
public:
    EventComparisonNode(const Comparison &cmp_, const string &event_) : cmp(cmp_), event(event_) {}


    bool Evaluate(const Date &date_, const string &event_) const override {
        if (cmp == Comparison::Less) {
            return event_ < event;
        } else if (cmp == Comparison::LessOrEqual) {
            return event_ == event || event_ < event;
        } else if (cmp == Comparison::Greater) {
            return event_ > event;
        } else if (cmp == Comparison::GreaterOrEqual) {
            return event_ == event || event_ > event;
        } else if (cmp == Comparison::Equal) {
            return event_ == event;
        } else if (cmp == Comparison::NotEqual) {
            return !(event_ == event);
        }

        return false;
    }

private:
    const Comparison cmp;
    const string event;
};

class LogicalOperationNode : public Node {
public:
    LogicalOperationNode(
            const LogicalOperation &op_,
            const shared_ptr<Node> &left_,
            const shared_ptr<Node> &right_)
            : op(op_),
              left(left_),
              right(right_) {}

    bool Evaluate(const Date &date, const string &event) const override {
        if (op == LogicalOperation::And) {
            return left->Evaluate(date, event) && right->Evaluate(date, event);
        } else if (op == LogicalOperation::Or) {
            return left->Evaluate(date, event) || right->Evaluate(date, event);
        }

        return false;
    }

private:
    const LogicalOperation op;
    const shared_ptr<Node> left;
    const shared_ptr<Node> right;

};

class EmptyNode : public Node {

    bool Evaluate(const Date &date, const string &event) const override {
        return true;
    }

};