//
//  DataBase.hpp
//  [Backend] Операции с многочленами
//
//  Created by Андрей Москалёв on 23/04/2019.
//  Copyright © 2019 Андрей Москалёв. All rights reserved.
//

#ifndef DataBase_hpp
#define DataBase_hpp

#include <stdio.h>
#include "Polynomial.hpp"
#include "polynomial_funcs.hpp"
#include "exceptions.hpp"

struct ElemDB {
    ElemDB * next = nullptr, * prev = nullptr;
    Polynomial * polynomial = nullptr;
    ElemDB() {}
    ElemDB(Polynomial * p) {
        polynomial = p;
    }
};

struct DataBase {
    int count = 0;
    ElemDB * elem = nullptr;

    void add(Polynomial * p) {
        ++count;
        if (!elem) {
            elem = new ElemDB(p);
            return;
        }
        ElemDB * q = elem;
        for (int i = 0; i < count - 2; ++i) {
            q = q->next;
        }
        q->next = new ElemDB(p);
        q->next->prev = q;
    }

    void remove(Polynomial p) {
        ElemDB * q = elem;
        for (int i = 0; i < count; ++i) {
            if (equals(*q->polynomial, p)) {
                if (q->prev) {
                    q->prev->next = q->next;
                }
                if (q->next) {
                    q->next->prev = q->prev;
                }
                delete q;
                return;
            }
            q = q->next;
        }
        throw DataBaseException("This polynomial does not exist.");
    }

    void remove(int n) {
        n -= 1;
        if (n < 0) {
            throw DataBaseException("number of polynomial can`t be lower zero.");
        }
        if (n > count) {
            throw DataBaseException("This polynomial does not exist.");
        }
        if (n == 0) {
            ElemDB * q = elem;
            elem = elem->next;
            delete q;
            --count;
            return;
        }
        ElemDB * q = elem;
        for (int i = 0; i < n && q->next != nullptr; ++i) {
            q = q->next;
        }
        if (q->prev) {
            q->prev->next = q->next;
        }
        if (q->next) {
            q->next->prev = q->prev;
        }
        delete q;
        --count;
    }

    Polynomial get(int n) {
        n -= 1;
        if (n < 0) {
            throw DataBaseException("number of polynomial can`t be lower zero.");
        }
        if (n > count) {
            throw DataBaseException("This polynomial does not exist.");
        }
        ElemDB * q = elem;
        for (int i = 0; i < n && q->next != nullptr; ++i) {
            q = q->next;
        }
        return *q->polynomial;
    }

    std::vector<Polynomial *> getAll() {
        std::vector<Polynomial *> ans;
        ElemDB * q = elem;
        for (int i = 0; i < count; ++i) {
            ans.push_back(q->polynomial);
            q = q->next;
        }
        return ans;
    }

};


#endif /* DataBase_hpp */
