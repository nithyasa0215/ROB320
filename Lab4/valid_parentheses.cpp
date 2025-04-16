#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>


// Given a string s containing just the characters '(', ')', '{', '}', '['
// and ']', determine if the input string is valid.

// An input string is valid if:
//   * Open brackets must be closed by the same type of brackets.
//   * Open brackets must be closed in the correct order.
//   * Every close bracket has a corresponding open bracket of the same type.

// Examples:
// 1. Input: s = "()"
//    Output: true
// 2. Input: s = "()[]{}"
//    Output: true
// 3. Input: s = "(]"
//    Output: false
// 4. Input: s = "([)]"
//    Output: false

bool is_valid(std::string s) {

    // implentation of STACK!!! 
    // use 370 learning to help !!
    // TODO: Implement the function
    std::stack<char> stack;

    /// cratea a map to hold the brackets
    std::unordered_map<char, char> brackets = {{')', '('}, {'}', '{'}, {']', '['}};
    
    // go through all of brackerts
    for (char c : s) {

        if (c == '(' || c == '{' || c == '[') {
            // this will add the brackets to the stack
            stack.push(c);
        } else {
            if (stack.empty() || stack.top() != brackets[c]) {
                //if the stack is empty tthen will return false
                return false;
            }
            // this will pop off the stocks
            stack.pop();
        }
    }
    
    return stack.empty();
}

int main() {
    std::string s = "()";
    std::cout << is_valid(s) << std::endl;
    s = "()[]{}";
    std::cout << is_valid(s) << std::endl;
    s = "(]";
    std::cout << is_valid(s) << std::endl;
    s = "([)]";
    std::cout << is_valid(s) << std::endl;
    s = "{[]}";
    std::cout << is_valid(s) << std::endl;
    return 0;
}