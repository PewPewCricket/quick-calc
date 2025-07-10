//
// Created by pewpewcricket on 7/10/25.
//

#include "mathSolver.h"

#include <sstream>
#include <string>
#include <stack>
#include <vector>

std::vector<std::string> tokenizeString(const std::string& input) {
    std::vector<std::string> tokens;
    std::istringstream iss(input);
    std::string token;

    while (std::getline(iss, token, ' ')) {
        tokens.push_back(token);
    }
    return tokens;
}

float solvePostfix(const std::vector<std::string>& problem) {
    std::stack<float> stack;
    for (std::string token : problem) {
        if (std::isdigit(token[0]) || (token.size() > 1 && token[0] == 'u' && token[1] == '-')) {
            if (token[0] == 'u' && token[1] == '-') {
                token.erase(0, 1);
            }
            stack.push(std::stof(token));
        } else {
            if (stack.size() < 2) {
                throw std::runtime_error("Invalid postfix expression: Not enough operands for operator " + token);
            }

            const float a = stack.top();
            stack.pop();
            const float b = stack.top();
            stack.pop();

            if (token == "+")
                stack.push(b + a);
            else if (token == "-")
                stack.push(b - a);
            else if (token == "*")
                stack.push(b * a);
            else if (token == "/")
                stack.push(b / a);
            else
                throw std::runtime_error("Unknown operator: " + token);
        }
    }

    if (stack.size() != 1) {
        throw std::runtime_error("Invalid postfix expression: Resulting stack size is not 1.");
    }
    return stack.top();
}

static int getPrecedence(const std::string& op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    return 0;
}

static bool isOperator(const std::string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/";
}

std::vector<std::string> toPostfix(const std::string& infix) {
    std::vector<std::string> outputQueue;
    std::stack<std::string> operatorStack;

    for (const std::string& token : tokenizeString(infix)) {
        if (std::isdigit(token[0]) || (token.size() > 1 && token[0] == 'u' && token[1] == '-' && std::isdigit(token[2]))) {
            outputQueue.push_back(token);
        } else if (isOperator(token)) {
            while (!operatorStack.empty() && isOperator(operatorStack.top()) &&
                   getPrecedence(operatorStack.top()) >= getPrecedence(token)) {
                outputQueue.push_back(operatorStack.top());
                operatorStack.pop();
            }
            operatorStack.push(token);
        } else if (token == "(") {
            operatorStack.push(token);
        } else if (token == ")") {
            while (!operatorStack.empty() && operatorStack.top() != "(") {
                outputQueue.push_back(operatorStack.top());
                operatorStack.pop();
            }

            if (!operatorStack.empty() && operatorStack.top() == "(") {
                operatorStack.pop();
            } else {
                throw std::runtime_error("Mismatched parentheses: Missing opening parenthesis.");
            }
        } else {
            throw std::runtime_error("Unexpected token in infix expression: " + token);
        }
    }

    while (!operatorStack.empty()) {
        if (operatorStack.top() == "(" || operatorStack.top() == ")") {
            throw std::runtime_error("Mismatched parentheses: Unclosed opening parenthesis.");
        }
        outputQueue.push_back(operatorStack.top());
        operatorStack.pop();
    }

    return outputQueue;
}