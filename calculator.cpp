#include <iostream>
#include <string>
#include <vector>
#include <regex>

bool validExpression = true;
std::string errorMessage = "";

bool isNumber(char num) {
    std::regex digit("\\d");
    return std::regex_match(std::string(1, num), digit);
}

bool isOperator(char op) {
    return op == '+' || op == '-' || op == '*' || op == '/' || op == '%';
}

// Fix this function
double evaluate(std::vector<std::string> inputTokens, std::vector<int> inputTokenTypes) {
    std::vector<std::string> tokens;
    std::vector<int> tokenTypes; // 0 for number, 1 for standard operator, 2 for single number operator

    tokens = inputTokens;
    tokenTypes = inputTokenTypes;
    
    // Distinguishes between standard operators and single number operators
    std::string token = "";
    for (int i = 0; i < tokens.size(); i++) {
        token = tokens[i];

        if (token == "++" or token == "--") {
            tokenTypes[i] = 2;
        }
    }

    // Checks if first token is a number
    if (tokenTypes[0] != 0) {
        errorMessage = "Syntax Error: Invalid expression";
        validExpression = false;
        return 0;
    }

    // Checks if the tokens are in the correct order
    for (int i = 0; i < tokens.size(); i++) {
        if (i == 0) {
            continue;
        }

        switch (tokenTypes[i]) {
            case 0:
                if (tokenTypes[i - 1] == 0) {
                    errorMessage = "Syntax Error: Invalid expression";
                    validExpression = false;
                    return 0;
                }
                break;
            case 1:
                if (i == tokens.size() - 1) {
                    errorMessage = "Syntax Error: Invalid expression";
                    validExpression = false;
                    return 0;
                }
                if (tokenTypes[i - 1] != 0 || tokenTypes[i + 1] != 0) {
                    errorMessage = "Syntax Error: Invalid expression";
                    validExpression = false;
                    return 0;
                }
                break;
            
            case 2:
                if (tokenTypes[i - 1] != 0) {
                    errorMessage = "Syntax Error: Invalid expression";
                    validExpression = false;
                    return 0;
                }
                if (i == tokens.size() - 1) {
                    break;
                }
                errorMessage = "Syntax Error: Invalid expression";
                validExpression = false;
                return 0;
        }
    }

    // Order of Operations
    std::vector<int> tokenPriority; // 0-3 with 1 being first, 3 being last and 0 being a number
    for (int i = 0; i < tokenTypes.size(); i++) {
        switch (tokenTypes[i]) {
            case 0:
                tokenPriority.push_back(0);
                break;
            
            case 1:
                if (tokens[i] == "+" or tokens[i] == "-") {
                    tokenPriority.push_back(3);
                } else {
                    tokenPriority.push_back(2);
                }
                break;
            
            case 2:
                tokenPriority.push_back(1);
                break;
        }
    }

    // Evaluates the expression
    std::vector<std::string> newTokens = tokens;
    std::vector<int> newTokenTypes = tokenTypes;
    std::vector<int> newTokenPriority = tokenPriority;
    bool completed = false;
    bool changed = false;

    int priority = 1;
    while (!completed) {
        int i = 0;
        while (i < tokens.size()) {
            if (changed) {
                tokens = newTokens;
                tokenTypes = newTokenTypes;
                tokenPriority = newTokenPriority;
                newTokens = tokens;
                newTokenTypes = tokenTypes;
                newTokenPriority = tokenPriority;
                changed = false;
            }

            if (tokenPriority[i] != priority) {
                i++;
                continue;
            }

            switch (tokenTypes[i]) {
                case 1:
                    if (tokens[i] == "//") {
                        newTokens[i - 1] = std::to_string((int) (std::stod(tokens[i - 1]) / std::stod(tokens[i + 1])));
                        newTokens.erase(newTokens.begin() + i, newTokens.begin() + i + 2);
                        newTokenTypes.erase(newTokenTypes.begin() + i, newTokenTypes.begin() + i + 2);
                        newTokenPriority.erase(newTokenPriority.begin() + i, newTokenPriority.begin() + i + 2);
                        changed = true;
                        i = 0;
                        continue;
                    }
                    switch (tokens[i][0]) {
                        case '+':
                            newTokens[i - 1] = std::to_string(std::stod(tokens[i - 1]) + std::stod(tokens[i + 1]));
                            newTokens.erase(newTokens.begin() + i, newTokens.begin() + i + 2);
                            newTokenTypes.erase(newTokenTypes.begin() + i, newTokenTypes.begin() + i + 2);
                            newTokenPriority.erase(newTokenPriority.begin() + i, newTokenPriority.begin() + i + 2);
                            changed = true;
                            i = 0;
                            continue;
                        
                        case '-':
                            newTokens[i - 1] = std::to_string(std::stod(tokens[i - 1]) - std::stod(tokens[i + 1]));
                            newTokens.erase(newTokens.begin() + i, newTokens.begin() + i + 2);
                            newTokenTypes.erase(newTokenTypes.begin() + i, newTokenTypes.begin() + i + 2);
                            newTokenPriority.erase(newTokenPriority.begin() + i, newTokenPriority.begin() + i + 2);
                            changed = true;
                            i = 0;
                            continue;

                        case '*':
                            newTokens[i - 1] = std::to_string(std::stod(tokens[i - 1]) * std::stod(tokens[i + 1]));
                            newTokens.erase(newTokens.begin() + i, newTokens.begin() + i + 2);
                            newTokenTypes.erase(newTokenTypes.begin() + i, newTokenTypes.begin() + i + 2);
                            newTokenPriority.erase(newTokenPriority.begin() + i, newTokenPriority.begin() + i + 2);
                            changed = true;
                            i = 0;
                            continue;

                        case '/':
                            newTokens[i - 1] = std::to_string(std::stod(tokens[i - 1]) / std::stod(tokens[i + 1]));
                            newTokens.erase(newTokens.begin() + i, newTokens.begin() + i + 2);
                            newTokenTypes.erase(newTokenTypes.begin() + i, newTokenTypes.begin() + i + 2);
                            newTokenPriority.erase(newTokenPriority.begin() + i, newTokenPriority.begin() + i + 2);
                            changed = true;
                            i = 0;
                            continue;

                        case '%':
                            newTokens[i - 1] = std::to_string((int) std::stod(tokens[i - 1]) % (int) std::stod(tokens[i + 1]));
                            newTokens.erase(newTokens.begin() + i, newTokens.begin() + i + 2);
                            newTokenTypes.erase(newTokenTypes.begin() + i, newTokenTypes.begin() + i + 2);
                            newTokenPriority.erase(newTokenPriority.begin() + i, newTokenPriority.begin() + i + 2);
                            changed = true;
                            i = 0;
                            continue;
                    }
                    break;
                
                case 2:
                    if (tokens[i] == "++") {
                        newTokens[i - 1] = std::to_string(std::stod(tokens[i - 1]) + 1);
                        newTokens.erase(newTokens.begin() + i);
                        newTokenTypes.erase(newTokenTypes.begin() + i);
                        newTokenPriority.erase(newTokenPriority.begin() + i);
                        changed = true;
                        i = 0;
                        continue;
                    } else {
                        newTokens[i - 1] = std::to_string(std::stod(tokens[i - 1]) - 1);
                        newTokens.erase(newTokens.begin() + i);
                        newTokenTypes.erase(newTokenTypes.begin() + i);
                        newTokenPriority.erase(newTokenPriority.begin() + i);
                        changed = true;
                        i = 0;
                        continue;
                    }
                    break;
            }

            // Shouldn't need to be used but just in case
            i++;
        }

        if (tokens.size() <= 1) {
            completed = true;
            continue;
        }

        priority++;
    }

    return std::stod(tokens[0]);
}

double simplifyExpression(std::string expression) {
    std::regex space("\\s*");
    expression = std::regex_replace(expression, space, "");

    std::vector<int> tokenTypes; // 0 for number, 1 for operator, -1 for invalid
    std::vector<std::string> tokens;
    
    int currentType = -1; // 0 for number, 1 for operator, -1 for invalid
    int prevType = -1; // 0 for number, 1 for operator, -1 for invalid
    std::string currentToken = "";
    std::string prevChar = "";
    bool isNegative = false;
    
    for (int i = 0; i < expression.length() && validExpression; i++) {
        // Determines type of current token
        if (isNumber(expression[i])) {
            currentType = 0;
        } else if (isOperator(expression[i])) {
            currentType = 1;
        } else {
            currentType = -1;
        }

        // Adds token to list if the type changed
        if (currentType != prevType && prevType != -1) {
            tokens.push_back(currentToken);
            tokenTypes.push_back(prevType);
            currentToken = "";
        }

        // Updates current token
        switch (currentType) {
            case 0:
                if (isNegative) {
                    currentToken = "-" + std::string(1, expression[i]);
                    isNegative = false;
                } else {
                    currentToken += expression[i];
                }
                break;
            
            case 1:
                if (prevType != 1 && prevType != -1) {
                    currentToken = expression[i];
                    break;
                }
                if (expression[i] == '*' or expression[i] == '%') {
                    errorMessage = "Syntax Error: Invalid operator";
                    validExpression = false;
                    continue;
                }
                if ((currentToken.length() == 1 || currentToken == "//") && expression[i] == '-') {
                    isNegative = true;
                    break;
                }
                if (currentToken.length() == 1 && currentToken[0] == expression[i]) {
                    currentToken += expression[i];
                    break;
                }
                if (prevType == -1 && expression[i] == '-') {
                    isNegative = true;
                    currentType = -1;
                    break;
                } else {
                    errorMessage = "Syntax Error: Invalid operator";
                    validExpression = false;
                    isNegative = false;
                    continue;
                }
            
            default:
                errorMessage = "Syntax Error: Invalid character";
                validExpression = false;
                continue;
        }

        prevType = currentType;
        prevChar = expression[i];
    }
    // Adds last token to list
    if (isNegative) {
        if (currentToken == "-") {
            currentToken = "--";
        } else {
            errorMessage = "Syntax Error: Invalid operator";
            validExpression = false;
        }
    }
    tokens.push_back(currentToken);
    tokenTypes.push_back(prevType);

    if (!validExpression) {
        return 0;
    }

    double result = evaluate(tokens, tokenTypes);
    return result;
}

int main() {
    std::string input;
    double output;

    std::cout << "Enter an expression: ";
    getline(std::cin, input);

    output = simplifyExpression(input);

    if (!validExpression) {
        std::cout << errorMessage << std::endl;
        return 0;
    }

    std::cout << output << std::endl;
    return 1;
}

// Standard Operators:
// "+" Addition
// "-" Subtraction
// "*" Multiplication
// "/" Division
// "//" Floor Division (Truncate towards zero)
// "%" Modulus

// Single Number Operators:
// "++" Increment (2++ = 3)
// "--" Decrement (2-- = 1)

// Order of Operations:
// 1. Increment/Decrement
// 2. Multiplication/Division/Floor Division/Modulus
// 3. Addition/Subtraction

// Edge Cases:
// "1+-2" = "1 + (-2)"
// "1-+2" invalid - syntax error
// "1--2" = "1 - (-2)"
// "1--" = "1 decrement"
// "1++2" invalid - eval error
// "1*-2" = "1 * (-2)"
// "1//-2" = "1 // (-2)"
// "1++-2" invalid
// "-1+-2" = "(-1) + (-2)"

// TODO
// Extras:
// 4. Add support for decimal numbers
// 5. Add support for parentheses