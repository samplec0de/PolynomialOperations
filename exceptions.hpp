//
//  exceptions.hpp
//  [Backend] Операции с многочленами
//
//  Created by Андрей Москалёв on 12/04/2019.
//  Copyright © 2019 Андрей Москалёв. All rights reserved.
//

#ifndef exceptions_hpp
#define exceptions_hpp

#include <stdio.h>
#include <iostream>

struct InputValidationException {
    std::string meta;
    InputValidationException() {
        this->meta = "not defined";
    }
    InputValidationException(std::string reason) {
        this->meta = reason;
    }
    std::string reason() const throw () {
        return "Error parsing polynomial: " + meta + "\n";
    }
};

struct TestFailedException {
    std::string meta;
    TestFailedException() {
        this->meta = "Failed test";
    }
    TestFailedException(std::string reason) {
        this->meta = reason;
    }
    std::string reason() const throw () {
        return "Failed test " + meta + "\n";
    }
};

struct DataBaseException {
    std::string meta;
    DataBaseException() {
        this->meta = "DataBase: wrong behavior!";
    }
    DataBaseException(std::string reason) {
        this->meta = reason;
    }
    std::string reason() const throw () {
        return "DataBase exception: " + meta + "\n";
    }
};

struct OpearionException {
    std::string meta;
    OpearionException() {
        this->meta = "Operation: error!";
    }
    OpearionException(std::string reason) {
        this->meta = reason;
    }
    std::string reason() const throw () {
        return "Operatiobn exception: " + meta + "\n";
    }
};

#endif /* exceptions_hpp */
