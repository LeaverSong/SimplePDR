/*************************************************************************
 * Copyright (C) 2017 by Rohit Dureja                                    *
 *                                                                       *
 * This file is part of SimplePDR.                                       *
 *                                                                       *
 *  SimplePDR is free software: you can redistribute it and/or modify    *
 *  it under the terms of the GNU General Public License as published by *
 *  the Free Software Foundation, either version 3 of the License, or    *
 *  (at your option) any later version.                                  *
 *                                                                       *
 *  SimplePDR is distributed in the hope that it will be useful,         *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 *  GNU General Public License for more details.                         *
 *                                                                       *
 *  You should have received a copy of the GNU General Public License    *
 *  along with SimplePDR.  If not, see <http://www.gnu.org/licenses/>.   *
 *************************************************************************/

#include "IC3.h"

namespace IC3 {

IC3::IC3(Model *M) {
	M->show_init();
	M->show_trans();
	solver = new Solver::Solver();


	std::vector<std::string> * vars;
	vars = M->get_variables();
	for (unsigned int i = 0 ; i < vars->size() ; ++i)
		solver->add_symbol((*vars)[i], Solver::Boolean);

	solver->add_assertion("(assert (= x y))");
	solver->add_assertion("(assert (= (not x) z))");

	Solver::result res;
	res = solver->check_sat();
	if (res == Solver::sat) {
		std::cout << solver->get_model() << std::endl;
	}

	solver->push(1);
	solver->add_assertion("(assert (= x z))");
	res = solver->check_sat();
	if (res == Solver::sat) {
			std::cout << solver->get_model() << std::endl;
	}

	solver->pop(1);
	solver->add_assertion("(assert (= x y z))");
	res = solver->check_sat();
	if (res == Solver::sat) {
			std::cout << solver->get_model() << std::endl;
	}
}

bool IC3::prove() {
	return true;
}

IC3::~IC3() {
	delete solver;
}

} /* namespace IC3 */
