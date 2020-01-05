//
//  Polynomial.hpp
//  [Backend] Операции с многочленами
//
//  Created by Андрей Москалёв on 14/04/2019.
//  Copyright © 2019 Андрей Москалёв. All rights reserved.
//

#ifndef Polynomial_hpp
#define Polynomial_hpp

#include <stdio.h>
#include <vector>
#include "exceptions.hpp"
#include "utils.hpp"
#include <cmath>

struct Node {
    Node * next = nullptr, * prev = nullptr;
    int power = 0;
    char x = '0';
    double cf = 0;
};

struct Binary {
    Node * start, * finish;
    Binary() {

    }
    Binary (Node * start, Node * finish) {
        this->start = start;
        this->finish = finish;
    }
};

struct Polynomial {
    Node * pol_head = nullptr;
    Node * pol_tail = nullptr;
    int len = 0;

    Polynomial() {}

    Polynomial(std::string input) {
        if (!((input.back() >= '0' && input.back() <= '9') || input.back() == 'x')) {
            throw InputValidationException("Unexpected end of line");
        }
        input = remove_spaces(input);
        if (input.size() == 0) {
            throw InputValidationException("There are nothing to parse (len(input) == 0)");
        }
        int state = 0;
        /*
         state description
         0 - coefficient or x or sign (+/-) expected-
         -1 - sign "-" given, next digit or x expected
         1 - sign "+", next digit or x expected
         2 - sign, coefficient and variable given, ^ or + expected
         3 - next digit, make cf below zero
         4 - next digit, make cf above zero
         5 - ^ given, integer expected
         6 - making integer (power)
         7 - adding to answer
         */
        int cf = -1e9;
        char x = '0';
        int power = -1e9;
        bool next_with_minus = false;
        for (int i = 0; i < input.size(); ++i) {
            char c = input[i];
            switch (state) {
                case 0:
                    if (c >= '0' && c <= '9') {
                        if (cf == -1e9) {
                            cf = 0;
                        }
                        cf = cf * 10 + (c - '0');
                        if (next_with_minus) {
                            if (cf > 0)
                                cf = -cf;
                            next_with_minus = false;
                        }
                        state = 4;
                        if (i == input.size() - 1) {
                            state = 7;
                        }
                    }
                    else if (c == '+') {
                        state = 1;
                    }
                    else if (c == '-') {
                        state = -1;
                    }
                    else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                        x = tolower(c);
                        state = 2;
                        if (next_with_minus) {
                            cf = -1;
                            next_with_minus = false;
                        }
                        if (i == input.size() - 1) {
                            state = 7;
                        }
                    }
                    else {
                        std::string err = "sign or coefficient or variable expected, but \"";
                        err += c;
                        err += "\" given.";
                        throw InputValidationException(err);
                    }
                    break;
                case -1:
                    if (c >= '0' && c <= '9') {
                        if (cf == -1e9) {
                            cf = 0;
                        }
                        cf = cf * 10 + (c - '0');
                        if (cf > 0)
                            cf = -cf;
                        state = 3;
                    }
                    else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                        x = tolower(c);
                        cf = -1;
                        state = 2;
                        if (i == input.size() - 1) {
                            state = 7;
                        }
                    }
                    else {
                        std::string err = "Expected digit, but \"";
                        err += c;
                        err += "\" given";
                        throw InputValidationException(err);
                    }
                    if (i == input.size() - 1) {
                        state = 7;
                    }
                    break;
                case 1:
                    if (c >= '0' && c <= '9') {
                        if (cf == -1e9) {
                            cf = 0;
                        }
                        bool lower_zero = cf < 0;
                        cf = fabs(cf) * 10 + (c - '0');
                        if (lower_zero) {
                            cf = -cf;
                        }
                        state = 4;
                    }
                    else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                        x = tolower(c);
                        state = 2;
                        if (i == input.size() - 1) {
                            state = 7;
                        }
                    }
                    else {
                        std::string err = "Expected digit, but \"";
                        err += c;
                        err += "\" given";
                        throw InputValidationException(err);
                    }
                    break;
                case 2:
                    if (c == '^') {
                        state = 5;
                    }
                    else if (c == '+') {
                        state = 7;
                    }
                    else if (c == '-') {
                        state = 7;
                        next_with_minus = true;
                    }
                    else {
                        std::string err = "Expected ^ or \"+\" or end, but \"";
                        err += c;
                        err += "\" given";
                        throw InputValidationException(err);
                    }
                    break;
                case 3:
                    if (c >= '0' && c <= '9') {
                        if (cf == -1e9) {
                            cf = 0;
                        }
                        //                        bool lower_zero = cf < 0;
                        cf = fabs(cf) * 10 + (c - '0');
                        if (cf > 0) {
                            cf = -cf;
                        }
                        state = 3;
                    }

                    else if (c == '^') {
                        state = 5;
                    }
                    else if (c == '+') {
                        state = 7;
                    }
                    else if (c == '-') {
                        next_with_minus = true;
                        state = 7;
                    }
                    else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                        x = tolower(c);
                        state = 2;
                        if (i == input.size() - 1) {
                            state = 7;
                        }
                    }
                    else {
                        std::string err = "Expected ^ or \"+\" or end or 0-9, but \"";
                        err += c;
                        err += "\" given";
                        throw InputValidationException(err);
                    }
                    if (i == input.size() - 1) {
                        state = 7;
                    }
                    break;
                case 4:
                    if (c >= '0' && c <= '9') {
                        if (cf == -1e9) {
                            cf = 0;
                        }
                        bool lower_zero = cf < 0;
                        cf = fabs(cf) * 10 + (c - '0');
                        if (lower_zero)
                            cf = -cf;
                        state = 4;
                        if (i == input.size() - 1) {
                            state = 7;
                        }
                        else if (c == '+') {
                            state = 7;
                        }
                        else if (c == '-') {
                            next_with_minus = true;
                            state = 7;
                        }
                    }
                    else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                        x = tolower(c);
                        state = 2;
                        if (i == input.size() - 1) {
                            state = 7;
                        }
                    }
                    else if (c == '^') {
                        state = 5;
                    }
                    else if (c == '+') {
                        state = 7;
                    }
                    else if (c == '-') {
                        state = 7;
                        next_with_minus = true;
                    }
                    else {
                        std::string err = "Expected ^ or 0-9, but \"";
                        err += c;
                        err += "\" given";
                        throw InputValidationException(err);
                    }
                    break;
                case 5:
                    if (c >= '0' && c <= '9') {
                        if (power == -1e9) {
                            power = 0;
                        }
                        power = power * 10 + (c - '0');
                        state = 6;
                        if (i == input.size() - 1) {
                            state = 7;
                        }
                    }
                    else {
                        std::string err = "Expected 0-9, but \"";
                        err += c;
                        err += "\" given";
                        throw InputValidationException(err);
                    }
                    break;
                case 6:
                    if (c >= '0' && c <= '9') {
                        if (power == -1e9) {
                            power = 0;
                        }
                        power = power * 10 + (c - '0');
                        state = 6;
                        if (i == input.size() - 1) {
                            state = 7;
                        }
                    }
                    else if (c == '+') {
                        state = 7;
                    }
                    else if (c == '-') {
                        next_with_minus = true;
                        state = 7;
                    }
                    else {
                        std::string err = "Expected \"+\" or end, but \"";
                        err += c;
                        err += "\" given";
                        throw InputValidationException(err);
                    }
                    break;
                default:
                    break;
            }
            if (state == 7) {
                if (c == '^') {
                    throw InputValidationException("Unexpected end of line");
                }
                if (cf == -1e9) {
                    cf = 1;
                }
                else if (fabs(cf) <= 1e-7) {
                    x = '0';
                    power = -1e9;
                }
                if (power == 0) {
                    x = '0';
                    power = -1e9;
                }
                if (x != '0') {
                    if (power == -1e9) {
                        power = 1;
                    }
                }
                if (power == 0) {
                    x = '0';
                }
                if (x == '0' && power != -1e9) {
                    cf = pow(cf, power);
                    power = 0;
                }
                Node * q = new Node;
                q->cf = cf;
                q->power = power;
                if (power == 0) {
                    x = '0';
                }
                q->x = x;
                q->next = nullptr;
                if (!pol_head) {
                    pol_head = pol_tail = q;
                }
                else {
                    q->prev = pol_tail;
                    pol_tail->next = q;
                    pol_tail = pol_tail->next;
                }
                x = '0';
                cf = -1e9;
                power = -1e9;
                state = 0;
                len += 1;
            }
        }
        if (!pol_head) {
            throw InputValidationException("Format error, a part of correct polynomial given, but end expected");
        }
        bring_similar_terms();
    }

    std::string str() {
        std::string res = "";
        Node * q = pol_head;
        if (q != nullptr) {
            if (q->cf < 0) {
                res += "-";
            }
        }
        for (q = pol_head; q != nullptr; q = q->next) {
            if (q->x == '0') {
                res += dtos(fabs(q->cf));
            }
            else {
                if (fabs(q->cf) != 1)
                    res += dtos(fabs(q->cf));
                res += q->x;
                if (q->power != -1e9 && q->power != 1) {
                    res += "^";
                    res += std::to_string(q->power);
                }
            }
            if (q == pol_tail) {
                break;
            }
            if (q->next && q->next->cf >= 0) {
                res += "+";
            }
            else if (q->next) {
                res += "-";
            }
        }
        return res;
    }

    void bring_similar_terms() {
        Binary res = merge_sort(Binary(pol_head, pol_tail), len);
        pol_head = res.start;
        pol_tail = res.finish;
        pol_tail->next = nullptr;
        for (Node * q = pol_head; q != nullptr; q = q->next) {
            if ((q->power == 0 && q->x != '0') || fabs(q->cf) <= 1e-7) {
                q->x = '0';
            }
            if (q->next == nullptr || q == pol_tail) {
                break;
            }
        }
        for (Node * q = pol_head; q != nullptr && len > 1; q = q->next) {
            if (fabs(q->cf) <= 1e-7 && len > 1) {
                Node * t = q;
                Node * p = q->next;
                if (q->prev) {
                    q->prev->next = p;
                }
                if (p) {
                    p->prev = q->prev;
                }
                if (q == pol_tail) {
                    pol_tail = q->prev;
                }
                q = q->next;
                if (t == pol_head) {
                    pol_head = q;
                }
                delete t;
                --len;
                if (q == nullptr) {
                    break;
                }
                continue;
            }
            if (q->next == nullptr || q == pol_tail) {
                break;
            }
        }
        for (Node * q = pol_head; len > 1 && q != nullptr; ) {
            if (q->power == 0) {
                q->x = '0';
                q->power = -1e9;
            }
            if (q->power == 1 && q->x == '0') {
                q->power = -1e9;
            }
            if (q->next != nullptr && q->power == q->next->power) {
                q->cf += q->next->cf;
                Node * p = q->next;
                if (p == pol_tail) {
                    pol_tail = q;
                }
                q->next = p->next;
                if (q->next && q->next->next) {
                    q->next->next->prev = q;
                }
                --len;
                delete p;
                if (fabs(q->cf) <= 1e-7 && len > 1) {
                    Node * t = q;
                    q = q->next;
                    if (t == pol_head) {
                        pol_head = q;
                    }
                    delete t;
                    --len;
                    if (q == pol_tail)
                        break;
                    continue;
                }
                continue;
            }
            if (fabs(q->cf) <= 1e-7 && len > 1) {
                Node * t = q;
                q = q->next;
                if (t == pol_head) {
                    pol_head = q;
                }
                delete t;
                --len;
                if (q == pol_tail)
                    break;
//                --len;
                continue;
            }
            if (q->next == nullptr || q->next == pol_tail || q == pol_tail) {
                break;
            }
            q = q->next;
        }
        for (Node * q = pol_head; q != nullptr && len > 1; q = q->next) {
            if (fabs(q->cf) <= 1e-7 && len > 1) {
                Node * t = q;
                Node * p = q->next;
                if (q->prev) {
                    q->prev->next = p;
                }
                if (p) {
                    p->prev = q->prev;
                }
                if (q == pol_tail) {
                    pol_tail = q->prev;
                }
                q = q->next;
                if (t == pol_head) {
                    pol_head = q;
                }
//                delete t;
                --len;
                if (q == nullptr) {
                    break;
                }
                continue;
            }
            if (q->next == nullptr || q == pol_tail) {
                break;
            }
        }
        if (pol_head && fabs(pol_head->cf) <= 1e-7) {
            pol_head->x = '0';
        }
        return;
    }

    double value(double point) {
        double res = 0;
        for (Node * q = pol_head; ; q = q->next) {
            if (q->power == 0 || q->x == '0') {
                res += q->cf;
            }
            else {
                res += q->cf * pow(point, q->power);
            }
            if (q == pol_tail) {
                break;
            }
        }
        return res;
    }

    Binary merge(Binary a, Binary b, int len_a, int len_b) {
        Node * q = a.start, * p = b.start, * prev = nullptr;
        Node * ans = nullptr, * ans_finish = nullptr;
        int cnt_a = 0, cnt_b = 0;
        while (cnt_a < len_a && cnt_b < len_b) {
            if (p->power >= q->power) {
                if (ans) {
                    ans_finish->next = p;
                    ans_finish = ans_finish->next;
                }
                else {
                    ans = ans_finish = p;
                }
                if (prev) {
                    ans_finish->prev = prev;
                }
                prev = ans_finish;
                ++cnt_b;
                if (cnt_b < len_b) {
                    p = p->next;
                }
            }
            else {
                if (ans) {
                    ans_finish->next = q;
                    ans_finish = ans_finish->next;
                }
                else {
                    ans = ans_finish = q;
                }
                if (prev) {
                    ans_finish->prev = prev;
                }
                prev = ans_finish;
                ++cnt_a;
                if (cnt_a < len_a) {
                    q = q->next;
                }
            }
        }
        while (cnt_b < len_b) {
            if (ans) {
                ans_finish->next = p;
                ans_finish = ans_finish->next;
            }
            else {
                ans = ans_finish = p;
            }
            if (prev) {
                ans_finish->prev = prev;
            }
            prev = ans_finish;
            ++cnt_b;
            if (cnt_b < len_b) {
                p = p->next;
            }
        }
        while (cnt_a < len_a) {
            if (ans) {
                ans_finish->next = q;
                ans_finish = ans_finish->next;
            }
            else {
                ans = ans_finish = q;
            }
            if (prev) {
                ans_finish->prev = prev;
            }
            prev = ans_finish;
            ++cnt_a;
            if (cnt_a < len_a) {
                q = q->next;
            }
        }
        return Binary(ans, ans_finish);
    }

    Binary merge_sort(Binary info, int len) {
        if (len <= 1) {
            return info;
        }
        if (len == 2) {
            if (info.start->power <= info.finish->power) {
                info.finish->next = info.start;
                info.start = info.finish;
                info.start->prev = info.finish->prev;
                info.finish = info.finish->next;
                info.finish->prev = info.start;
                return info;
            }
            else {
                return info;
            }
        }
        Node * finish_left = info.start;
        int cnt = 1;
        for (cnt = 0; cnt < len / 2; ++cnt) {
            if (cnt != len / 2 - 1)
                finish_left = finish_left->next;
        }
        Node * start_right = finish_left->next;
        Binary left = merge_sort(Binary(info.start, finish_left), cnt);
        Binary right = merge_sort(Binary(start_right, info.finish), len - cnt);
        Binary res = merge(left, right, cnt, len - cnt);
        res.start->prev = nullptr;
        res.finish->next = nullptr;
        return res;
    }
    void add(double cf, int power, char x) {
        if (!pol_head) {
            pol_head = pol_tail = new Node;
        }
        else {
            pol_tail->next = new Node;
            pol_tail->next->prev = pol_tail;
            pol_tail = pol_tail->next;
        }
        pol_tail->power = power;
        pol_tail->cf = cf;
        pol_tail->x = x;
        ++len;
        bring_similar_terms();
    }
    std::vector<int> roots(int a = -1e8, int b = 1e8) {
        std::vector<int> ans;
        for (int i = a; i <= b; ++i) {
            if (value(i) == 0) {
                ans.push_back(i);
            }
        }
        return ans;
    }
    Polynomial derivative() {
        Polynomial res = Polynomial();
        Node * q = pol_head;
        int len_src = len;
        for (int i = 0; i < len_src; ++i) {
            if (q->power != -1e9) {
                res.add(q->cf * q->power, q->power - 1, q->x);
            }
            if (q == pol_tail) {
                break;
            }
            q = q->next;
        }
        if (res.len == 0) {
            res.add(0, -1e9, '0');
        }
        return res;
    }
    Polynomial n_derivative(int n) {
        Polynomial ans = *this;
        for (int i = 0; i < n; ++i) {
            ans = ans.derivative();
        }
        return ans;
    }
};

#endif /* Polynomial_hpp */
