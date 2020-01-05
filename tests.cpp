//
//  tests.cpp
//  [Backend] Операции с многочленами
//
//  Created by Андрей Москалёв on 12/04/2019.
//  Copyright © 2019 Андрей Москалёв. All rights reserved.
//

#include "tests.hpp"
#include "utils.hpp"
#include "exceptions.hpp"
#include "Polynomial.hpp"
#include "polynomial_funcs.hpp"

void test_remove_spaces() {
    if (remove_spaces("fkefe frwg  egwrgfr") != "fkefefrwgegwrgfr") {
        throw TestFailedException("remove_spaces() - 1");
    }
    if (remove_spaces(" fkefe frwg  egwrgfr") != "fkefefrwgegwrgfr") {
        throw TestFailedException("remove_spaces() - 2");
    }
    if (remove_spaces("    ") != "") {
        throw TestFailedException("remove_spaces() - 3");
    }
    if (remove_spaces(" aabgr ") != "aabgr") {
        throw TestFailedException("remove_spaces() - 4");
    }
}

void test_init_polynomial() {
    std::vector<std::pair<std::string, std::string>> tests = {
//        {"x^2+-x^2+-x^2+x^2", ""},,'
        {"-12-2", "-14"},
        {"0+0x+0x^1-1+1x^0", "0"},
        {"x^2-11+14", "x^2+3"},
        {"1+x^3+1-1", "x^3+1"},
        {"0x^3+1+0x-1", "0"},
        {"1-x", "-x+1"},
        {"2x-0", "2x"},
        {"12-12", "0"},
        {"2x^0", "2"},
        {"x^2-2x^2+x^2", "0"},
        {"-x-", ""},
        {"-77x^23+22x^2-222x+88", "-77x^23+22x^2-222x+88"},
        {"-11+14", "3"},
        {"0+-1", "-1"},
        {"20+x^2+x^4+48x+4x^4", "5x^4+x^2+48x+20"},
        {"2^2", "4"},
        {"2x^+x", ""},
        {"x^2+x^2", "2x^2"},
        {"x^48+x+x^4+4888+x^3", "x^48+x^4+x^3+x+4888"},
        {"20x", "20x"},
        {"228x+224x^4", "224x^4+228x"},
        {"666x^666+48x^2+4", "666x^666+48x^2+4"},
        {"kek", ""},
        {"666x^666+0+48x^2+4", "666x^666+48x^2+4"},
    };
    for (int i = 0; i < tests.size(); ++i) {
        try {
            std::string cur = Polynomial(tests[i].first).str();
            if (cur != tests[i].second) {
                throw TestFailedException("test_init_polynomial() - "+std::to_string(i)+"\nInput: "+tests[i].first+"\nExpected: "+tests[i].second+"\nResult: "+cur+"\n");
            }
        }
        catch (InputValidationException e) {
            if (tests[i].second.size() != 0) {
                throw TestFailedException("test_init_polynomial() - "+std::to_string(i)+"\nExpected: "+tests[i].second+"\nResult: "+e.reason());
            }
        }
    }
}

struct BinaryPolynomials {
    Polynomial a;
    Polynomial b;
    BinaryPolynomials() {

    }
    BinaryPolynomials(Polynomial a, Polynomial b) {
        this->a = a;
        this->b = b;
    }
};

void test_sum_polynomials() {
    std::vector<std::pair<BinaryPolynomials, std::string>> tests = {
        {BinaryPolynomials(Polynomial("x"), Polynomial("x")), "2x"},
        {BinaryPolynomials(Polynomial("2x^2"), Polynomial("x")), "2x^2+x"},
        {BinaryPolynomials(Polynomial("2x^48+8"), Polynomial("48x^48+480")), "50x^48+488"},
        {BinaryPolynomials(Polynomial("3x^4+8"), Polynomial("48x^48+480")), "48x^48+3x^4+488"},
    };
    for (int i = 0; i < tests.size(); ++i) {
        Polynomial res = sum(tests[i].first.a, tests[i].first.b);
        std::string res_str = res.str();
        if (res_str != tests[i].second) {
            throw TestFailedException("test_sum_polynomials() - "+std::to_string(i)+"\nInput: "+tests[i].first.a.str()+"+"+tests[i].first.b.str()+"\nExpected: "+tests[i].second+"\nResult: "+res_str+"\n");
        }
        int len = int(std::count(res_str.begin(), res_str.end(), '+')+1);
        if (len != res.len) {
            throw TestFailedException("test_sum_polynomials() (len) - "+std::to_string(i)+"\nInput: "+tests[i].first.a.str()+"+"+tests[i].first.b.str()+"\nExpected: "+tests[i].second+" ("+std::to_string(len)+")"+"\nResult: "+res_str+" ("+std::to_string(res.len)+")"+"\n");
        }
    }
}

void test_minus_polynomials() {
    std::vector<std::pair<BinaryPolynomials, std::string>> tests = {
        {BinaryPolynomials(Polynomial("x^2-3x-4"), Polynomial("x^2-3x-4")), "0"},
        {BinaryPolynomials(Polynomial("2x^2+x"), Polynomial("1-x")), "2x^2+2x-1"},
        {BinaryPolynomials(Polynomial("x-2"), Polynomial("x-3")), "1"},
        {BinaryPolynomials(Polynomial("x"), Polynomial("x")), "0"},
        {BinaryPolynomials(Polynomial("2x^2"), Polynomial("x")), "2x^2-x"},
        {BinaryPolynomials(Polynomial("2x^48+8"), Polynomial("48x^48+480")), "-46x^48-472"},
        {BinaryPolynomials(Polynomial("3x^4+8"), Polynomial("48x^48+480")), "-48x^48+3x^4-472"},
    };
    for (int i = 0; i < tests.size(); ++i) {
        Polynomial res = sum(tests[i].first.a, tests[i].first.b, true);
        std::string res_str = res.str();
        if (res_str != tests[i].second) {
            throw TestFailedException("test_minus_polynomials() - "+std::to_string(i)+"\nInput: "+tests[i].first.a.str()+"-"+tests[i].first.b.str()+"\nExpected: "+tests[i].second+"\nResult: "+res_str+"\n");
        }
        int len = int(std::count(res_str.begin(), res_str.end(), '+')+std::count(res_str.begin(), res_str.end(), '-')+1);
        if (res_str[0] == '-') {
            --len;
        }
        if (len != res.len) {
            throw TestFailedException("test_minus_polynomials() (len) - "+std::to_string(i)+"\nInput: "+tests[i].first.a.str()+"-"+tests[i].first.b.str()+"\nExpected: "+tests[i].second+" ("+std::to_string(len)+")"+"\nResult: "+res_str+" ("+std::to_string(res.len)+")"+"\n");
        }
    }
}

struct NodeValues {
    int power = 0;
    double cf = 0;
    char x = '0';
    NodeValues() {

    }
    NodeValues(double cf, int power, char x) {
        this->cf = cf;
        this->power = power;
        this->x = x;
    }
    std::string str() {
        if (power == 0) {
            return std::to_string(cf);
        }
        std::string ans = "";
        ans += std::to_string(cf);
        if (power == 1) {
            ans += x;
            return ans;
        }
        ans += "";
        ans += x;
        ans += "^";
        ans += std::to_string(power);
        return ans;
    }
};

void test_add() {
    std::vector<std::pair<std::pair<NodeValues, Polynomial>, std::string>> tests = {
        {{NodeValues(48, 2, 'x'), Polynomial("x")}, "48x^2+x"},
        {{NodeValues(2, 1, 'x'), Polynomial("x")}, "3x"},
        {{NodeValues(3, 10, 'x'), Polynomial("48x^48+480")}, "48x^48+3x^10+480"},
        {{NodeValues(10, 20, 'y'), Polynomial("48x^48+480")}, "48x^48+10y^20+480"},
    };
    for (int i = 0; i < tests.size(); ++i) {
        NodeValues cur = tests[i].first.first;
        Polynomial src = tests[i].first.second;
        tests[i].first.second.add(cur.cf, cur.power, cur.x);
        if (tests[i].first.second.str() != tests[i].second) {
            throw TestFailedException("test_add() - "+std::to_string(i)+"\nInput: "+src.str()+"+"+tests[i].first.second.str()+"\nExpected: "+tests[i].second+"\nResult: "+tests[i].first.second.str());
        }
    }
}

void test_multiply() {
    std::vector<std::pair<BinaryPolynomials, std::string>> tests = {
        {BinaryPolynomials(Polynomial("x-2"), Polynomial("0x")), "0"},
        {BinaryPolynomials(Polynomial("x^2+48x+24x^5"), Polynomial("x^3+33x^5+24x")), "792x^10+24x^8+33x^7+2160x^6+x^5+48x^4+24x^3+1152x^2"},
        {BinaryPolynomials(Polynomial("x+2x^3+77x^7+12"), Polynomial("2x^2+33x^7+90")), "2541x^14+66x^10+154x^9+33x^8+7326x^7+4x^5+182x^3+24x^2+90x+1080"},
        {BinaryPolynomials(Polynomial("47x^3+33x+2"), Polynomial("2")), "94x^3+66x+4"},
        {BinaryPolynomials(Polynomial("x"), Polynomial("x")), "x^2"},

    };
    for (int i = 0; i < tests.size(); ++i) {
        Polynomial res = multiply(tests[i].first.a, tests[i].first.b);
        if (res.str() != tests[i].second) {
            throw TestFailedException("test_multiply() - "+std::to_string(i)+"\nInput: "+tests[i].first.a.str()+"  "+tests[i].first.b.str()+"\nExpected: "+tests[i].second+"\nResult: "+res.str());
        }
    }
}

void test_division() {
    std::vector<std::pair<BinaryPolynomials, BinaryPolynomials>> tests = {
//        {BinaryPolynomials(Polynomial("x^3"), Polynomial("3x")), BinaryPolynomials(Polynomial("0.333333x^2"), Polynomial("0"))},
        {BinaryPolynomials(Polynomial("12"), Polynomial("6")), BinaryPolynomials(Polynomial("2"), Polynomial("0"))},
        {BinaryPolynomials(Polynomial("x"), Polynomial("x^7+2")), BinaryPolynomials(Polynomial("0"), Polynomial("x^7+2"))},
        {BinaryPolynomials(Polynomial("x^2"), Polynomial("x")), BinaryPolynomials(Polynomial("x"), Polynomial("0"))},
    };
    for (int i = 0; i < tests.size(); ++i) {
        std::pair<Polynomial, Polynomial> res = divide(tests[i].first.a, tests[i].first.b);
        if (!equals(res.first, tests[i].second.a) || !equals(res.second, tests[i].second.b)) {
            throw TestFailedException("test_divsision() - "+std::to_string(i)+"\nInput: "+tests[i].first.a.str()+" / "+tests[i].first.b.str()+"\nExpected: " + tests[i].second.a.str() + ", " + tests[i].second.b.str() +"\nResult: " + res.first.str() + ", " + res.second.str());
        }
    }
}

void test_value() {
    std::vector<std::pair<std::pair<Polynomial, double>, double>> tests = {
        {{Polynomial("792x^10+24x^8+33x^7+2160x^6+x^5+48x^4+24x^3+1152x^2"), 0.5},
            328.90625},
        {{Polynomial("22x+13x^3+488x^5"), 100}, 4880013002200},
        {{Polynomial("31x^10+552x^6+2220x^9"), 0.13},
            0.00268798},
        {{Polynomial("x^14+32x^10+x^10+44x+12x^6"), 5.125},
            9036852707.733536},
    };
    for (int i = 0; i < tests.size(); ++i) {
        double predict = tests[i].first.first.value(tests[i].first.second);
        double ans = tests[i].second;
        if (fabs(predict - ans) > 1e-6) {
            throw TestFailedException("test_value() - "+std::to_string(i)+"\nInput: "+tests[i].first.first.str()+", where x="+dtos(tests[i].first.second)+"\nExpected: "+dtos(ans)+"\nResult: "+dtos(predict)+" (difference: "+dtos(fabs(predict - ans))+")");
        }
    }
}

void test_equal() {
    std::vector<std::pair<std::pair<Polynomial, Polynomial>, bool>> tests = {
        {{Polynomial("x-2"), Polynomial("x-3")}, false},
        {{Polynomial("x"), Polynomial("x+0")}, true},
        {{Polynomial("x^48+x^4+20"), Polynomial("20+x^4+x^48")}, true},
        {{Polynomial("x^48+x^4+20"), Polynomial("x^4+x^48")}, false},
        {{Polynomial("x"), Polynomial("2x")}, false},
    };
    for (int i = 0; i < tests.size(); ++i) {
        bool eq = equals(tests[i].first.first, tests[i].first.second);
        bool ans = tests[i].second;
        if (eq != ans) {
            throw TestFailedException("test_equal() - "+std::to_string(i)+"\nInput: "+tests[i].first.second.str()+" == "+tests[i].first.first.str()+"\nExpected: "+(ans ? "True" : "False")+"\nResult: "+(eq ? "True" : "False"));
        }
    }
}

void test_derivative() {
    std::vector<std::pair<Polynomial, Polynomial>> tests = {
        {Polynomial("4x^4+2x^8+10"), Polynomial("16x^3+16x^7")},
        {Polynomial("120x"), Polynomial("120")},
        {Polynomial("x"), Polynomial("1")},
        {Polynomial("2033"), Polynomial("0")},
    };
    for (int i = 0; i < tests.size(); ++i) {
        Polynomial derivative = tests[i].first.derivative();
        if (!equals(derivative, tests[i].second)) {
            throw TestFailedException("test_derivative() - "+std::to_string(i)+"\nInput: "+tests[i].first.str()+"\nExpected: "+tests[i].second.str()+"\nResult: "+derivative.str());
        }
    }
}

void test_n_derivative() {
    std::vector<std::pair<std::pair<Polynomial, int>, Polynomial>> tests = {
        {{Polynomial("2x^2"), 1}, Polynomial("4x")},
        {{Polynomial("0"), 5}, Polynomial("0")},
        {{Polynomial("4x^23"), 6}, Polynomial("290727360x^17")},
        {{Polynomial("5x^5+10x^3"), 5}, Polynomial("600")},
    };
}

void test_roots() {
    std::vector<std::pair<Polynomial, std::vector<int>>> tests = {
        {Polynomial("x"), {0}},
        {Polynomial("x^2+88x^12+10x^4"), {0}},
        {Polynomial("x^2+88x^12+10x^4+48"), {}},
        {Polynomial("x^4+-10000"), {-10, 10}},
    };
    for (int i = 0; i < tests.size(); ++i) {
        std::vector<int> ans = tests[i].first.roots(-1000, 1000);
        if (ans != tests[i].second) {
            std::string roots_correct = "{";
            for (int j = 0; j < tests[i].second.size(); ++j) {
                roots_correct += std::to_string(tests[i].second[j]);
                if (j != tests[i].second.size() - 1) {
                    roots_correct += ", ";
                }
            }
            roots_correct += "}";
            std::string roots_result = "{";
            for (int j = 0; j < ans.size(); ++j) {
                roots_result += std::to_string(ans[j]);
                if (j != ans.size() - 1) {
                    roots_result += ", ";
                }
            }
            roots_result += "}";
            throw TestFailedException("test_roots() - "+std::to_string(i)+"\nInput: "+tests[i].first.str()+" = 0"+"\nExpected: "+roots_correct+"\nResult: "+roots_result);
        }
    }
}
