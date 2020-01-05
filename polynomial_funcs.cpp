//
//  polynomial_funcs.cpp
//  [Backend] Операции с многочленами
//
//  Created by Андрей Москалёв on 14/04/2019.
//  Copyright © 2019 Андрей Москалёв. All rights reserved.
//

#include "polynomial_funcs.hpp"

Polynomial sum(const Polynomial &a, const Polynomial &b, bool minus) {
    Polynomial res;
    int len = 0;
    Node * head = nullptr, * tail = nullptr;
    // Copying a to res
    for (Node * p = a.pol_head; p != nullptr; p = p->next) {
        if (!head) {
            head = tail = new Node;
            *head = *p;
            *tail = *p;
        }
        else {
            tail->next = new Node;
            *(tail->next) = *p;
            tail = tail->next;
        }
        ++len;
        if (p == a.pol_tail) {
            break;
        }
    }
    // Adding b to the tail
    for (Node * p = b.pol_head; p != nullptr; p = p->next) {
        if (!head) {
            head = tail = new Node;
            *head = *p;
            *tail = *p;
        }
        else {
            tail->next = new Node;
            *(tail->next) = *p;
            tail->next->prev = tail;
            tail = tail->next;
        }
        ++len;
        if (minus) {
            tail->cf = tail->cf * -1;
        }
        if (p == b.pol_tail) {
            break;
        }
    }
    res.pol_head = head;
    res.pol_tail = tail;
    res.len = len;
    res.bring_similar_terms();
    return res;
}

Polynomial multiply(const Polynomial &a, const Polynomial &b) {
    if (!a.pol_head) {
        return b;
    }
    if (!b.pol_head) {
        return a;
    }
    if ((a.len == 1 && a.pol_head->cf == 0) || (b.len == 1 && fabs(b.pol_head->cf) <= 1e-7)) {
        return Polynomial("0");
    }
    Node * q = a.pol_head;
    Polynomial res = Polynomial();
    res.pol_head = res.pol_tail = new Node;
    res.pol_head->cf = a.pol_head->cf * b.pol_head->cf;
    res.pol_head->x = a.pol_head->x;
    if (a.pol_head->x == '0' && b.pol_head->x == '0') {
        res.pol_head->x = '0';
    }
    else if (a.pol_head->x == '0' && b.pol_head->x != '0') {
        res.pol_head->x = b.pol_head->x;
    }
    else {
        res.pol_head->x = a.pol_head->x;
    }
    res.pol_head->power = a.pol_head->power + b.pol_head->power;
    int pw1 = a.pol_head->power, pw2 = b.pol_head->power;
    if (pw1 == -1e9 && pw2 == -1e9) {
        res.pol_head->power = -1e9;
    }
    else if (pw1 == -1e9 && pw2 != -1e9) {
        res.pol_head->power = pw2;
    }
    else if (pw1 != -1e9 && pw2 == -1e9) {
        res.pol_head->power = pw1;
    }
    else {
        res.pol_head->power = pw1 + pw2;
    }
    res.len = 1;
    bool first = true;
    for (int i = 0; i < a.len && q; ++i) {
        Node * p = b.pol_head;
        if (first) {
            p = p->next;
            first = false;
        }
        for (int j = 0; j < b.len && p; ++j) {
            pw1 = q->power;
            pw2 = p->power;
            int pw = 0;
            if (pw1 == -1e9 && pw2 == -1e9) {
                pw = -1e9;
            }
            else if (pw1 == -1e9 && pw2 != -1e9) {
                pw = pw2;
            }
            else if (pw1 != -1e9 && pw2 == -1e9) {
                pw = pw1;
            }
            else {
                pw = pw1 + pw2;
            }
            char cx = '0';
            if (q->x == '0' && p->x == '0') {
                cx = '0';
            }
            else if (q->x == '0' && p->x != '0') {
                cx = p->x;
            }
            else {
                cx = q->x;
            }
            res.add(q->cf * p->cf, pw, cx);
            p = p->next;
        }
        q = q->next;
    }
    return res;
}

bool equals(const Polynomial &a, const Polynomial &b) {
    if (a.len != b.len) {
        return false;
    }
    Node * q = a.pol_head;
    Node * p = b.pol_head;
    int len = a.len;
    while (len--) {
        if (q->power == 0) {
            q->x = '0';
            q->power = -1e9;
        }
        if (p->power == 0) {
            p->x = '0';
            p->power = -1e9;
        }
        if (p->power == 1 && p->x == '0') {
            p->power = -1e9;
        }
        if (q->power == 1 && q->x == '0') {
            q->power = -1e9;
        }
        if (fabs(q->cf) <= 1e-7) {
            q->power = -1e9;
            p->x = '0';
        }
        if (fabs(p->cf) <= 1e-7) {
            p->x = '0';
            p->power = -1e9;
        }
        if (q->cf != p->cf || q->power != p->power || q->x != p->x) {
            return false;
        }
        p = p->next;
        q = q->next;
    }
    return true;
}

std::pair<Polynomial, Polynomial> divide(Polynomial a, Polynomial b) {
    Polynomial ans = Polynomial();
    if (b.len == 1 && fabs(b.pol_head->cf) <= 1e-7) {
        throw OpearionException("Division by zero");
    }
//    if (b.len == 1 && b.pol_head->cf == 1) {
//        return {a, Polynomial("0")};
//    }
    if (b.len == 1 && b.pol_head->cf == -1) {
        return {multiply(a, Polynomial("-1")), Polynomial("0")};
    }
    bool one = false;
    if (a.len == 1 && b.len == 1) {
        one = true;
    }
    bool was = false;
    while (true) {
        if ((a.len == 1 && fabs(a.pol_head->cf) <= 1e-7) || a.pol_head->power < b.pol_head->power) {
            break;
        }
        was = true;
//        std::cout << a.str() << "\n";
        double multiply_by_cf = -1e9;
        double multiply_by_power = 1;
        bool xxx = (a.pol_head->x == 'x' || b.pol_head->x == 'x');
        if (!(b.pol_head->power == -1e9 || b.pol_head->power == 0)) {
            multiply_by_power = a.pol_head->power - b.pol_head->power;
        }
        else {
            multiply_by_power = a.pol_head->power;
        }
        multiply_by_cf = a.pol_head->cf / b.pol_head->cf;
        Polynomial cur_add = Polynomial("0");
        cur_add.pol_head->cf = multiply_by_cf;
        cur_add.pol_head->power = multiply_by_power;
        if (xxx)
            cur_add.pol_head->x = 'x';
//        std::cout << "min " <<  multiply(b, cur_add).str() << "\n";
        Polynomial tmp = multiply(b, cur_add);
        a = sum(a, tmp, true);
        ans.add(multiply_by_cf, multiply_by_power, tmp.pol_head->x);
        if (one) {
            break;
        }
    }
    if (!was) {
        if (ans.pol_tail == nullptr) {
            ans.add(0, -1e9, '0');
        }
        ans.bring_similar_terms();
        b.bring_similar_terms();
        return {ans, a};
    }
    ans.bring_similar_terms();
    b.bring_similar_terms();
    return {ans, a};
}
