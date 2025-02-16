#include <iostream>
#include <sstream>
#include <unordered_map>
#include <string>
#include <cctype>

using namespace std;

unordered_map<string, double> variables;

// Function to trim whitespace
string trim(const string &str) {
    size_t first = str.find_first_not_of(" \t");
    size_t last = str.find_last_not_of(" \t");
    return (first == string::npos || last == string::npos) ? "" : str.substr(first, last - first + 1);
}

// Function to add spaces around operators for proper tokenization
string addSpacesToExpression(const string &expression) {
    string spacedExpr;
    for (char c : expression) {
        if (c == '+' || c == '-' || c == '*' || c == '/') {
            spacedExpr += " ";
            spacedExpr += c;
            spacedExpr += " ";
        } else {
            spacedExpr += c;
        }
    }
    return spacedExpr;
}

// Function to evaluate an arithmetic expression
double evaluateExpression(string expression) {
    expression = addSpacesToExpression(expression);
    istringstream iss(expression);
    double result, num;
    char op = '+';

    if (!(iss >> result)) {
        cerr << "Error: Invalid expression." << endl;
        return 0;
    }

    while (iss >> op >> num) {
        switch (op) {
            case '+': result += num; break;
            case '-': result -= num; break;
            case '*': result *= num; break;
            case '/': 
                if (num != 0) result /= num;
                else {
                    cerr << "Error: Division by zero!" << endl;
                    return 0;
                }
                break;
            default:
                cerr << "Error: Unsupported operator " << op << endl;
                return 0;
        }
    }
    return result;
}

// Function to process function calls like add(a, b)
double processFunctionCall(string input) {
    size_t openParen = input.find('(');
    size_t closeParen = input.find(')');

    if (openParen == string::npos || closeParen == string::npos || openParen > closeParen) {
        cerr << "Error: Invalid function syntax." << endl;
        return 0;
    }

    string functionName = trim(input.substr(0, openParen));
    string args = trim(input.substr(openParen + 1, closeParen - openParen - 1));

    size_t comma = args.find(',');
    if (comma == string::npos) {
        cerr << "Error: Function requires two arguments." << endl;
        return 0;
    }

    string arg1 = trim(args.substr(0, comma));
    string arg2 = trim(args.substr(comma + 1));

    // Replace variables with values if they exist
    double num1 = (variables.count(arg1)) ? variables[arg1] : stod(arg1);
    double num2 = (variables.count(arg2)) ? variables[arg2] : stod(arg2);

    if (functionName == "add") {
        return num1 + num2;
    } 
    else if (functionName == "sub") {
        return num1 - num2;
    }
    else if (functionName == "mul") {
        return num1 * num2;
    }
    else if (functionName == "div") {
        if (num2 != 0) return num1 / num2;
        else {
            cerr << "Error: Division by zero!" << endl;
            return 0;
        }
    }
    else {
        cerr << "Error: Unsupported function '" << functionName << "'" << endl;
        return 0;
    }
}

// Function to process user input and handle variable assignments
void processInput(const string &input) {
    string trimmedInput = trim(input);

    if (trimmedInput.empty()) return;

    if (variables.count(trimmedInput)) {
        // If input is a variable name, return its stored value
        cout << trimmedInput << " = " << variables[trimmedInput] << endl;
        return;
    }

    if (trimmedInput.find('=') != string::npos) {
        // Handle variable assignment
        size_t eqPos = trimmedInput.find('=');
        string varName = trim(trimmedInput.substr(0, eqPos));
        string expr = trim(trimmedInput.substr(eqPos + 1));

        // Replace variables in the expression
        for (auto &var : variables) {
            size_t pos;
            while ((pos = expr.find(var.first)) != string::npos) {
                expr.replace(pos, var.first.length(), to_string(var.second));
            }
        }

        double value = evaluateExpression(expr);
        variables[varName] = value;
        cout << varName << " = " << value << endl;
    } 
    else if (trimmedInput.find('(') != string::npos && trimmedInput.find(')') != string::npos) {
        // Handle function calls
        cout << processFunctionCall(trimmedInput) << endl;
    } 
    else {
        // Replace variables in the input expression
        string expression = trimmedInput;
        for (auto &var : variables) {
            size_t pos;
            while ((pos = expression.find(var.first)) != string::npos) {
                expression.replace(pos, var.first.length(), to_string(var.second));
            }
        }

        cout << evaluateExpression(expression) << endl;
    }
}

int main() {
    string input;
    cout << "Simple C++ Interpreter (type 'exit' to quit)\n";

    while (true) {
        cout << "> ";
        getline(cin, input);

        if (input == "exit") break;

        processInput(input);
    }

    return 0;
}
