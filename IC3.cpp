/*
 * IC3.cpp
 *
 *  Created on: Jan 30, 2017
 *      Author: dureja
 */

#include "IC3.h"

namespace IC3 {

IC3::IC3(Model *M) {
	M->show_init();
	M->show_trans();
}

bool IC3::prove() {
	std::cout << "de-Morgan example\n";
	z3::context c;

	z3::expr x = c.bool_const("x");
	z3::expr y = c.bool_const("y");
	z3::expr conjecture = (!(x && y)) == (!x || !y);

	z3::solver s(c);
	// adding the negation of the conjecture as a constraint.
	s.add(!conjecture);
	std::cout << s << "\n";
	std::cout << s.to_smt2() << "\n";
	switch (s.check()) {
	case z3::unsat:   std::cout << "de-Morgan is valid\n"; break;
	case z3::sat:     std::cout << "de-Morgan is not valid\n"; break;
	case z3::unknown: std::cout << "unknown\n"; break;
	}
	return true;
}

IC3::~IC3() {
	// TODO Auto-generated destructor stub
}

} /* namespace IC3 */