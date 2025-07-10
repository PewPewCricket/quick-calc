//
// Created by pewpewcricket on 7/10/25.
//

#ifndef MATHSOLVER_H
#define MATHSOLVER_H

#include <string>
#include <stack>
#include <vector>

float solvePostfix(const std::vector<std::string>& problem);
std::vector<std::string> toPostfix(const std::string& infix);

#endif //MATHSOLVER_H
