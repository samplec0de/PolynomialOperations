//
//  polynomial_funcs.hpp
//  [Backend] Операции с многочленами
//
//  Created by Андрей Москалёв on 14/04/2019.
//  Copyright © 2019 Андрей Москалёв. All rights reserved.
//

#ifndef polynomial_funcs_hpp
#define polynomial_funcs_hpp

#include <stdio.h>
#include "Polynomial.hpp"
#include "exceptions.hpp"

Polynomial sum(const Polynomial &a, const Polynomial &b, bool minus=false);
Polynomial multiply(const Polynomial &a, const Polynomial &b);
std::pair<Polynomial, Polynomial> divide(Polynomial a, Polynomial b);
bool equals(const Polynomial &a, const Polynomial &b);

#endif /* polynomial_funcs_hpp */
