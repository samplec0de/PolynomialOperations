#include "mainwindow.h"
#include <iostream>
#include <QApplication>
#include "exceptions.hpp"
#include "utils.hpp"
#include "tests.hpp"
#include "Polynomial.hpp"
#include "polynomial_funcs.hpp"

void stress_test() {
    test_remove_spaces();
    std::cout << "test_remove_spaces(): ok\n";
    test_init_polynomial();
    std::cout << "test_init_polynomial(): ok\n";
    test_sum_polynomials();
    std::cout << "test_sum_polynomials(): ok\n";
    test_add();
    std::cout << "test_add(): ok\n";
    test_multiply();
    std::cout << "test_multiply(): ok\n";
    test_value();
    std::cout << "test_value(): ok\n";
    test_equal();
    std::cout << "test_equal(): ok\n";
    test_derivative();
    std::cout << "test_derivative(): ok\n";
    test_n_derivative();
    std::cout << "test_n_derivative(): ok\n";
    test_roots();
    std::cout << "test_roots(): ok\n";
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    bool all_tests_passed = true;
//    try {
//        stress_test();
//    }
//    catch (TestFailedException e) {
//        std::cout << e.reason();
//        all_tests_passed = false;
//    }
//    std::cout << (all_tests_passed ? "All tests passed!\n" : "CHECK TESTING ERRORS!!!\n");
    if (!all_tests_passed) {
        return 0;
    }
    MainWindow w;
    w.show();
    return a.exec();
}
