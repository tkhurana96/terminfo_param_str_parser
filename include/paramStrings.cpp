#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <typeinfo>
#include <vector>

std::string evaluateString(std::string &notEvaluated, std::vector<int> args) {

  std::string answer;
  std::stack<int> s;
  std::map<std::string, int> vars;

  for (auto curr = notEvaluated.begin(); curr != notEvaluated.end();) {
    std::cout << "\n\n    current char is: " << *curr;

    if (*curr != '%') {

      answer += *curr;
      curr++;

    } else {
      curr++;

      switch (*curr) {

      case '\'': {
        auto constCharASCIIVal = static_cast<int>(*++curr);
        s.push(constCharASCIIVal);
      }

      case 't': {
        if (!s.empty()) { // TODO: what should happen if stack is empty
          auto top = s.top();
          s.pop();
        }
        break;
      }

      case '%': {
        answer += '%';
        break;
      }

      case 'p': {
        auto paramNum = static_cast<int>(*curr++);
        if (paramNum < args.size()) { // TODO: what should happen if this fails
          s.push(args[paramNum]);
        }
        break;
      }

      case 'P': {
        auto varName = std::string(1, *++curr);
        if (!s.empty()) { // TODO: what should happen if stack is empty
          auto top = s.top();
          s.pop();
          vars[varName] = top;
        }
        break;
      }

      case 'g': { // Both 'g' cases handled here
        auto varName = std::string(1, *++curr);
        auto keyIter = vars.find(varName);
        if (keyIter != vars.end()) { // TODO: what happens if this fails
          s.push(keyIter->second);
        }
        break;
      }

      case '+': {
        if (s.size() >= 2) { // TODO: what should happen if this fails
          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.pop();
          s.push(operand1 + operand2);
          // TODO: have both operands and I know the operation from *curr, apply
          // operation to operands and push result
        }
        break;
      }

      case '-': {
        if (s.size() >= 2) { // TODO: what should happen if this fails
          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.pop();
          s.push(operand1 - operand2);
          // TODO: have both operands and I know the operation from *curr, apply
          // operation to operands and push result
        }
        break;
      }

      case '*': {
        if (s.size() >= 2) { // TODO: what should happen if this fails
          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.pop();
          s.push(operand1 * operand2);
          // TODO: have both operands and I know the operation from *curr, apply
          // operation to operands and push result
        }
        break;
      }

      case '/': {
        if (s.size() >= 2) { // TODO: what should happen if this fails
          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.pop();
          s.push(operand1 / operand2);
          // TODO: have both operands and I know the operation from *curr, apply
          // operation to operands and push result
        }
        break;
      }

      case 'm': // NOTE: special char for mod
      {
        if (s.size() >= 2) { // TODO: what should happen if this fails
          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.pop();
          s.push(operand1 % operand2);
          // TODO: have both operands and I know the operation from *curr, apply
          // operation to operands and push result
        }
        break;
      }

      case '&': {
        if (s.size() >= 2) { // TODO: what should happen if this fails
          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.pop();
          s.push(operand1 & operand2);
          // TODO: have both operands and I know the operation from *curr, apply
          // operation to operands and push result
        }
        break;
      }

      case '|': {
        if (s.size() >= 2) { // TODO: what should happen if this fails
          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.pop();
          s.push(operand1 | operand2);
          // TODO: have both operands and I know the operation from *curr, apply
          // operation to operands and push result
        }
        break;
      }

      case '^': {
        if (s.size() >= 2) { // TODO: what should happen if this fails
          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.pop();
          s.push(operand1 ^ operand2);
          // TODO: have both operands and I know the operation from *curr, apply
          // operation to operands and push result
        }
        break;
      }

      case 'i': {
        if (args.size() >= 2) { // TODO: what should happen if this fails
          args[0] += 1;
          args[1] += 1;
        }
        break;
      }

      case '=': {
        if (s.size() >= 2) { // TODO: what should happen if this fails
          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.push(operand1 == operand2);
        }
        break;
      }

      case '<': {
        if (s.size() >= 2) { // TODO: what should happen if this fails
          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.push(operand1 < operand2);
        }
        break;
      }

      case '>': {
        if (s.size() >= 2) { // TODO: what should happen if this fails
          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.push(operand1 > operand2);
        }
        break;
      }

      case '!': {
        if (s.size() >= 1) { // TODO: what should happen if this fails
          auto operand = s.top();
          s.pop();
          s.push(!operand);
        }
        break;
      }

      case '~': {
        if (s.size() >= 1) { // TODO: what should happen if this fails
          auto operand = s.top();
          s.pop();
          s.push(~operand);
        }
        break;
      }

      case '{': {
        std::string decimalNumberToBe;
        while (*curr != '}') {
          decimalNumberToBe += *curr;
          curr++;
        }
        s.push(std::stoi(decimalNumberToBe)); // DOUBT: should this
                                              // 'decimalNumberToBe' pushed to
                                              // stack or appended to answer ?
        break;
      }

      case 'c': {
        auto poppedVal = std::to_string(s.top());
        s.pop();
        answer += poppedVal;
        break;
      }

      case 's': {
        // DOUBT: Should pop the whole stack or only the topmost item ?
        // for now poppping the whole stack.
        while (!s.empty()) {
          answer += std::to_string(s.top());
          s.pop();
        }
        break;
      }

      case 'l': {
        auto poppedVal = std::to_string(s.top());
        s.pop();
        s.push(static_cast<int>(poppedVal.size()));
      }

      default:
        std::cout << " hello";
        break;
      }
    }
  }

  return answer;
}

int main() {

  std::string str = "%tushar";

  auto res = evaluateString(str, {1, 2, 3});

  std::cout << "    evaluated String res is: " << res << '\n';

  return 0;
}