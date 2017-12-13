#include <cctype>
#include <ios>
#include <iostream>
#include <map>
#include <sstream>
#include <stack>
#include <string>
#include <typeinfo>
#include <variant>
#include <vector>

enum class base { d = 'd', o = 'o', x = 'x', X = 'X', s = 's' };

using valType = std::variant<int, std::string>;

auto isTruthy(valType arg) {
  if (std::holds_alternative<int>(arg)) {
    return std::get<int>(arg) != 0 ? true : false;
  } else if (std::holds_alternative<std::string>(arg)) {
    return std::get<std::string>(arg).size() > 0 ? true : false;
  }
}

std::string evaluateString(const std::string &notEvaluated,
                           std::vector<valType> args = {}) {

  std::string answer;
  // std::stack<int> s;
  std::stack<valType> s;
  std::stack<bool> ifElseRes;
  std::map<std::string, valType> vars;

  for (auto curr = notEvaluated.begin(); curr != notEvaluated.end();) {
    // std::cout << "\n\n    current char is: " << *curr;

    if (*curr != '%') {

      answer += *curr;
      // curr++;
    } else {
      curr++;

      switch (*curr) {

      case '\'': {
        // auto constCharASCIIVal = static_cast<int>(*++curr) - 48 - 1;
        // s.push(constCharASCIIVal);
        auto constChar = std::string() + *++curr;
        s.push(valType(constChar));
        curr++;
        break;
      }

      // case 'd': {
      //   // auto top = std::to_string(s.top());
      //   // s.pop();
      //   // answer += top;
      //   auto top = s.top();
      //   s.pop();

      //   if (std::holds_alternative<int>(top)) {
      //     auto res = std::to_string(std::get<int>(top));
      //     answer += res;
      //   } else {
      //     // TODO: what should happen if top of the stack is not an integer ?
      //     throw std::runtime_error("Oops.. top of the stack not an integer");
      //   }
      //   break;
      // }

      case 't': {
        if (!s.empty()) { // TODO: what should happen if stack is empty

          auto top = s.top();
          s.pop();

          if (isTruthy(top)) {
            // push a true onto the stack to let the corresponding else_if's and
            // else's to know that some branch before them executed do nothing
            ifElseRes.top() = true;
            // and let the following expressions parse
          } else {
            // move curr untill it starts pointing to this then's else
            // maintain integer nesting variable to count inner if-else's if any
            auto nesting = 0;
            for (;; curr++) {
              if (*curr == '%') {
                auto next = *(curr + 1);

                if (next == '?') {
                  nesting++;
                } else if ((next == 'e' or next == ';') and nesting) {
                  nesting--;
                } else if ((next == 'e' or next == ';') and not nesting) {
                  std::advance(curr, -1); // check for UB
                  break;                  // check if this break works ?
                }
              }
            }
          }
        } else {
          throw std::runtime_error("Stack empty.. Invalid expression to parse");
        }
        break;
      }

      case 'e': {
        if (!ifElseRes.empty()) {
          auto someBranchBeforeExecuted = ifElseRes.top();

          if (someBranchBeforeExecuted) {
            // move curr unitll it starts pointing to this else's end
            // maintain integer nesting variable to count inner if-else's
            auto nesting = 0;
            for (;; curr++) {
              if (*curr == '%') {
                auto next = *(curr + 1); // what should happen if this fails

                if (next == '?') {
                  nesting++;
                } else if (next == ';' and nesting) {
                  nesting--;
                } else if ((next == ';' or next == 'e') and not nesting) {
                  std::advance(curr, -1); // check for UB
                  break;                  // check if this break works ?
                }
              }
            }
          }
        } else {
          throw std::runtime_error("Invalid else found");
        }
        break;
      }

      case ';': {
        if (not ifElseRes.empty()) {
          ifElseRes.pop(); // what should happen if this fails ?
        } else {
          throw std::runtime_error("Invalid ending of if-else found");
        }
        break;
      }

      case '%': {
        answer += '%';
        break;
      }

      case 'p': {
        auto paramNum = static_cast<int>(*++curr) - 48 - 1;
        if (paramNum < args.size()) { // TODO: what should happen if this fails
          // s.push(args[paramNum]);
          s.push(args[paramNum]);
        } else {
          throw std::runtime_error(
              "Parameter number out of range.. Invalid expression to parse");
        }
        break;
      }

      case 'P': {
        auto varName = std::string(1, *++curr);
        if (!s.empty()) { // TODO: what should happen if stack is empty
          auto top = s.top();
          s.pop();
          vars[varName] = top;
        } else {
          throw std::runtime_error("Stack empty.. Invalid expression to parse");
        }
        break;
      }

      case 'g': { // Both 'g' cases handled here
        auto varName = std::string(1, *++curr);
        auto keyIter = vars.find(varName);
        if (keyIter != vars.end()) { // TODO: what happens if this fails
          s.push(keyIter->second);
        } else {
          throw std::runtime_error(
              "Invalid Variable.. Invalid expression to parse");
        }
        break;
      }

      case '+': {
        if (s.size() >= 2) { // TODO: what should happen if this fails
          // auto operand1 = s.top();
          // s.pop();
          // auto operand2 = s.top();
          // s.pop();
          // s.push(operand1 + operand2);
          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.pop();

          // Oooh... the plus operation is valid for both integer and strings..
          // what to do now ?
          if (operand1.index() ==
              operand2.index()) // if they are holding values of same type
          {
            // improve the readibility of the code that follows
            if (std::holds_alternative<std::string>(operand1) and
                std::holds_alternative<std::string>(operand2)) {
              auto res = std::get<std::string>(operand1) +
                         std::get<std::string>(operand2);
              s.push(valType(res));
            } else if (std::holds_alternative<int>(operand1) and
                       std::holds_alternative<int>(operand2)) {
              auto res = std::get<int>(operand1) + std::get<int>(operand2);
              s.push(valType(res));
            }
          } else { // TODO: what should happen if they are of different types
            throw std::runtime_error("Addition operation applied on two "
                                     "different types of operands");
          }

        } else {
          throw std::runtime_error(
              "Number of operands less than 2.. Invalid expression to parse");
        }
        break;
      }

      case '-': {
        if (s.size() >= 2) { // TODO: what should happen if this fails
          // auto operand1 = s.top();
          // s.pop();
          // auto operand2 = s.top();
          // s.pop();
          // s.push(operand1 - operand2);
          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.pop();

          if (std::holds_alternative<int>(operand1) and
              std::holds_alternative<int>(operand2)) {
            auto res = std::get<int>(operand2) - std::get<int>(operand1);
            s.push(valType(res));
          } else { // TODO: what should happen if either or both of them are
                   // string types ?
            throw std::runtime_error(
                "Invlalid operands for subtraction operation");
          }
        } else {
          throw std::runtime_error(
              "Number of operands less than 2.. Invalid expression to parse");
        }
        break;
      }

      case '*': {
        if (s.size() >= 2) { // TODO: what should happen if this fails
          // auto operand1 = s.top();
          // s.pop();
          // auto operand2 = s.top();
          // s.pop();
          // s.push(operand1 * operand2);
          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.pop();

          if (std::holds_alternative<int>(operand1) and
              std::holds_alternative<int>(operand2)) {
            auto res = std::get<int>(operand1) * std::get<int>(operand2);
            s.push(valType(res));
          } else { // TODO: what should happen if either or both of them are
                   // string types ?
            throw std::runtime_error(
                "Invlalid operands for multiplication operation");
          }
        } else {
          throw std::runtime_error(
              "Number of operands less than 2.. Invalid expression to parse");
        }
        break;
      }

      case '/': {
        if (s.size() >= 2) { // TODO: what should happen if this fails
          // auto operand1 = s.top();
          // s.pop();
          // auto operand2 = s.top();
          // s.pop();
          // if (operand1 == 0) {
          //   throw std::runtime_error("Division by zero attempted");
          // }
          // s.push(operand2 / operand1);
          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.pop();

          if (std::holds_alternative<int>(operand1) and
              std::holds_alternative<int>(operand2)) {
            if (std::get<int>(operand1) == 0) {
              throw std::runtime_error("Division by zero attempted");
            } else {
              auto res = std::get<int>(operand2) / std::get<int>(operand1);
              s.push(valType(res));
            }
          } else { // TODO: what should happen if either or both of them are
                   // string types ?
            throw std::runtime_error(
                "Invlalid operands for division operation");
          }
        } else {
          throw std::runtime_error(
              "Number of operands less than 2.. Invalid expression to parse");
        }
        break;
      }

      case 'm': // NOTE: special char for mod
      {
        if (s.size() >= 2) { // TODO: what should happen if this fails
          // auto operand1 = s.top();
          // s.pop();
          // auto operand2 = s.top();
          // s.pop();
          // s.push(operand1 % operand2);
          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.pop();

          if (std::holds_alternative<int>(operand1) and
              std::holds_alternative<int>(operand2)) {
            auto res = std::get<int>(operand2) % std::get<int>(operand1);
            s.push(valType(res));
          } else { // TODO: what should happen if either or both of them are
                   // string types ?
            throw std::runtime_error("Invlalid operands for mod operation");
          }
        } else {
          throw std::runtime_error(
              "Number of operands less than 2.. Invalid expression to parse");
        }
        break;
      }

      case '&': {
        if (s.size() >= 2) { // TODO: what should happen if this fails
          // auto operand1 = s.top();
          // s.pop();
          // auto operand2 = s.top();
          // s.pop();
          // s.push(operand1 & operand2);

          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.pop();

          if (std::holds_alternative<int>(operand1) and
              std::holds_alternative<int>(operand2)) {
            auto res = std::get<int>(operand1) & std::get<int>(operand2);
            s.push(valType(res));
          } else { // TODO: what should happen if either or both of them are
                   // string types ?
            throw std::runtime_error(
                "Invlalid operands for bitwise-and operation");
          }
        } else {
          throw std::runtime_error(
              "Number of operands less than 2.. Invalid expression to parse");
        }
        break;
      }

      case '|': {
        if (s.size() >= 2) { // TODO: what should happen if this fails
          // auto operand1 = s.top();
          // s.pop();
          // auto operand2 = s.top();
          // s.pop();
          // s.push(operand1 | operand2);

          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.pop();

          if (std::holds_alternative<int>(operand1) and
              std::holds_alternative<int>(operand2)) {
            auto res = std::get<int>(operand1) | std::get<int>(operand2);
            s.push(valType(res));
          } else { // TODO: what should happen if either or both of them are
                   // string types ?
            throw std::runtime_error(
                "Invlalid operands for bitwise-or operation");
          }
        } else {
          throw std::runtime_error(
              "Number of operands less than 2.. Invalid expression to parse");
        }
        break;
      }

      case '^': {
        if (s.size() >= 2) { // TODO: what should happen if this fails
          // auto operand1 = s.top();
          // s.pop();
          // auto operand2 = s.top();
          // s.pop();
          // s.push(operand1 ^ operand2);

          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.pop();

          if (std::holds_alternative<int>(operand1) and
              std::holds_alternative<int>(operand2)) {
            auto res = std::get<int>(operand1) ^ std::get<int>(operand2);
            s.push(valType(res));
          } else { // TODO: what should happen if either or both of them are
                   // string types ?
            throw std::runtime_error("Invlalid operands for xor operation");
          }
        } else {
          throw std::runtime_error(
              "Number of operands less than 2.. Invalid expression to parse");
        }
        break;
      }

      case 'i': {
        if (args.size() >= 1) { // TODO: what should happen if this fails
          // Should do by using .at() or use custom if-else's ?
          // try {
          // args.at(0) += 1;
          // args.at(1) += 1;
          // } catch (const std::out_of_range) {
          //   // Should anything be done with this exception, I don't think so.
          // }
          try {
            if (std::holds_alternative<int>(args.at(0))) {
              std::get<int>(args.at(0)) += 1;
            }
            if (std::holds_alternative<int>(args.at(1))) {
              std::get<int>(args.at(1)) += 1;
            }
          } catch (const std::out_of_range) {
            // should anything be done with this exception, I don't think so.
          }
        } else {
          throw std::runtime_error(
              "Parameter number out of range.. Invalid expression to parse");
        }
        break;
      }

      case '=': {
        if (s.size() >= 2) { // TODO: what should happen if this fails
          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.pop();
          s.push(operand1 == operand2);
        } else {
          throw std::runtime_error(
              "Number of operands less than 2.. Invalid expression to parse");
        }
        break;
      }

      case '<': {
        if (s.size() >= 2) { // TODO: what should happen if this fails
          // auto operand1 = s.top();
          // s.pop();
          // auto operand2 = s.top();
          // s.pop();
          // s.push(operand1 < operand2);

          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.pop();

          if (operand1.index() ==
              operand2.index()) { // if they are of same type
            auto res = operand2 < operand1;
            s.push(valType(res));
          } else {
            throw std::runtime_error(
                "Invalid operands for greater-than opearator");
          }
        } else {
          throw std::runtime_error(
              "Number of operands less than 2.. Invalid expression to parse");
        }
        break;
      }

      case '>': {
        if (s.size() >= 2) { // TODO: what should happen if this fails
          // auto operand1 = s.top();
          // s.pop();
          // auto operand2 = s.top();
          // s.pop();
          // s.push(operand1 > operand2);

          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.pop();

          if (operand1.index() ==
              operand2.index()) { // if they are of same type
            auto res = operand2 > operand1;
            s.push(valType(res));
          } else {
            throw std::runtime_error(
                "Invalid operands for lesser-than opearator");
          }
        } else {
          throw std::runtime_error(
              "Number of operands less than 2.. Invalid expression to parse");
        }
        break;
      }

      case '!': {
        // if (s.size() >= 1) { // TODO: what should happen if this fails
        //   // auto operand = s.top();
        //   // s.pop();
        //   // s.push(!operand); // TODO: confirm this !operand
        //   auto operand = s.top();
        //   s.pop();

        //   if (std::holds_alternative<int>(operand)){
        //     s.push(valType(!operand));
        //   } else {
        //     throw std::runtime_error("Invalid operand i.e. string for not(!)
        //     operator")
        //   }
        // } else {
        //   throw std::runtime_error(
        //       "Number of operands less than 1.. Invalid expression to
        //       parse");
        // }
        if (s.size() >= 1) {
          auto top = s.top();
          s.pop();
          s.push(valType(!isTruthy(top)));
        } else {
          throw std::runtime_error(
              "Number of operands less than 1.. Invalid expression to parse");
        }
        // std::cout << "logical-not not yet implemented\n";
        break;
      }

      case '~': {
        if (s.size() >= 1) { // TODO: what should happen if this fails
          // auto operand = s.top();
          // s.pop();
          // s.push(~operand); // TODO: confirm this ~operand and need to use
          //                   // unsigned int

          auto operand = s.top();
          s.pop();

          if (std::holds_alternative<int>(operand)) {
            auto res = ~(std::get<int>(operand));
            s.push(valType(res));
          } else {
            throw std::runtime_error(
                "Invalid operand(string) for bitwise-not operator");
          }
        } else {
          throw std::runtime_error(
              "Number of operands less than 1.. Invalid expression to parse");
        }
        break;
      }

      case '{': {
        // std::string decimalNumberToBe;
        // while (*++curr != '}') {
        //   decimalNumberToBe += *curr;
        // }
        // s.push(std::stoi(decimalNumberToBe));

        std::string decimalNumberToBe;
        while (*++curr != '}') {
          decimalNumberToBe += *curr;
        }
        s.push(valType(std::stoi(decimalNumberToBe)));
        break;
      }

      case 'c': {
        // DOUBT: According to the new man page the top-most stack value should
        // be popped as character and appended to answer. So if top of stack
        // contains 3 then '3' should be appended to output or ascii value
        // corresponding to 3 ? for now appending the ascii value.

        // auto charCorrespondingToTop = static_cast<char>(s.top());
        // s.pop();
        // answer += charCorrespondingToTop;

        auto top = s.top();
        s.pop();

        if (std::holds_alternative<int>(top)) {
          auto charCorrespondingToTop = static_cast<char>(std::get<int>(top));
          answer += charCorrespondingToTop;
        } else {
          throw std::runtime_error("Cannot convert string to char");
        }
        break;
      }

      case 's': {
        // DOUBT: Should pop the whole stack or only the topmost item ?
        // for now poppping the whole stack.
        // DOUBT: same doubt as case 'c'
        // while (!s.empty()) {
        //   answer += static_cast<char>(s.top());
        //   s.pop();
        // }

        auto top = s.top();
        s.pop();

        if (std::holds_alternative<std::string>(top)) {
          answer += std::get<std::string>(top);
        } else {
          throw std::runtime_error("invalid operand for %s encoding");
        }
        break;
      }

      case 'l': {
        // auto poppedVal = std::to_string(s.top());
        // s.pop();
        // s.push(poppedVal.size());

        auto top = s.top();
        s.pop();

        if (std::holds_alternative<std::string>(top)) {
          auto res = std::get<std::string>(top).size();
          s.push(valType(res));
        } else {
          throw std::runtime_error("Bad operand(int) for calculating length");
        }
        break;
      }

      case ':':
      case ' ':
      case '#': {
        // TODO: check the validity of this parsing algo
        std::string numberToBe;
        curr++;
        if (*curr == '+' || *curr == '-') {
          numberToBe += *curr;
          curr++;
        }
        char base{'d'};
        while (*curr != '%' && curr != notEvaluated.end()) {
          if (*curr == 'd' || *curr == 'o' || *curr == 'x' || *curr == 'X' ||
              *curr == 's') {
            base = *curr;
            break;
          }
          numberToBe += *curr;
          curr++;
        }
        // here we got the numberToBe and the base, now convert the number to
        // that base and append to answer

        break;
      }

      case 'A': {
        auto operand1 = s.top();
        s.pop();
        auto operand2 = s.top();
        s.pop();

        s.push(valType(isTruthy(operand1) and isTruthy(operand2)));
        break;
      }

      case 'O': {
        auto operand1 = s.top();
        s.pop();
        auto operand2 = s.top();
        s.pop();

        s.push(valType(isTruthy(operand1) or isTruthy(operand2)));
        break;
      }

      case '?': {
        // let's parse
        ifElseRes.push(false);
        break;
      }

      default: {
        if (*curr == ':' or *curr == ' ' or *curr == '#') {
          if (*curr == ':') {
            // parse + or - next and set flag
            curr++;
            if (*curr == '+') {
              // set flag to std::showpos
            } else if (*curr == '-') {
              // set flag to std::left
            } else {
              throw std::runtime_error("Invalid char found after ':'");
            }
          } else if (*curr == ' ') {
            // set flag
            curr++;
          } else if (*curr == '#') {
            // set flag
            curr++;
          }
          // one curr++ remaining somewhere above
        }

        if (std::isdigit(*curr)) {
          // parse width untill '.' is reached
          std::string widthToBe;
          while (std::isdigit(*curr)) {
            widthToBe += *curr;
            curr++;
          }
          // std::cout << "[line 737] widthToBe:" << widthToBe << '\n';
        }

        if (*curr == '.') {
          // parse precision untill you reach one of base specifiers
          curr++;
          std::string precisionToBe;
          while (std::isdigit(*curr)) {
            precisionToBe += *curr;
            curr++;
          }
          // std::cout << "[line 748] precisionToBe:" << precisionToBe << '\n';
        }

        if (*curr == 'd' or *curr == 's' or *curr == 'x' or *curr == 'X' or
            *curr == 'o') {
          // std::cout << "inside base if\n";
          // set flag on string stream accordingly
          // but first also pop the stack to get the value that is to be printed
          // check if the value popped is compatible with base found
          auto top = s.top();
          s.pop();

          std::stringstream formattingStream(
              std::ios_base::out); // VERIFY: this
                                   // stream's
                                   // destructor must
                                   // be called when
                                   // this scope exits

          switch (*curr) {
          case 'd': { /* set std::dec on stringstream */
            if (std::holds_alternative<int>(top)) {
              auto res = std::to_string(std::get<int>(top));
              // TODO: before appending format what is to be printed
              answer += res;
            } else {
              throw std::runtime_error(
                  "Oops.. top of the stack not an integer");
            }
            break;
          }

          case 'x': { /* set std::hex on stringstream */
            if (std::holds_alternative<int>(top)) {
              std::hex(formattingStream);
              // TODO: before appending format what is to be printed
              formattingStream << std::get<int>(top);
              answer += formattingStream.str();
            } else {
              throw std::runtime_error(
                  "Oops.. top of the stack not an integer");
            }
            break;
          }

          case 'X': { /* set std::uppercase on stringstream */
            if (std::holds_alternative<int>(top)) {
              std::uppercase(formattingStream);
              std::hex(formattingStream);
              // TODO: before appending format what is to be printed
              formattingStream << std::get<int>(top);
              answer += formattingStream.str();
            } else {
              throw std::runtime_error(
                  "Oops.. top of the stack not an integer");
            }
            break;
          }

          case 'o': { /* set std::oct on stringstream */
            if (std::holds_alternative<int>(top)) {
              std::oct(formattingStream);
              // TODO: before appending format what is to be printed
              formattingStream << std::get<int>(top);
              answer += formattingStream.str();
            } else {
              throw std::runtime_error(
                  "Oops.. top of the stack not an integer");
            }
            break;
          }

          case 's': {
            if (std::holds_alternative<std::string>(top)) {
              auto res = std::get<std::string>(top);
              // TODO: before appending format what is to be printed
              formattingStream << res;
              answer += formattingStream.str();
            } else {
              throw std::runtime_error("Oops.. top of the stack not an string");
            }
            break;
          }
          }
        } else {
          throw std::runtime_error("Invalid % encoding found");
        }

        // print to string stream and get back the res from it and append it to
        // answer string
      }

        // default: {

        //   // set width, flag, precision and base according to ncurses
        //   if (std::isdigit(*curr)) {
        //     // parse width
        //     std::string widthStr;
        //     while (std::isdigit(*curr)) {
        //       widthStr += *curr;
        //       curr++;
        //     }
        //     // set width to the parsed one
        //   } else {
        //     switch (*curr) {
        //     case ':':{
        //       auto next = *(curr + 1);
        //       if (next == '+' or next == '-'){

        //         // set flag accordingly
        //       } else {
        //         throw std::runtime_error("Invalid character found after
        //         ':'");
        //       }
        //       break;
        //     }
        //     case ' ':
        //     case '#':
        //     case '.': {
        //       // parse precision
        //       std::string precisionStr;
        //       while (std::isdigit(*curr)) {
        //         precisionStr += *curr;
        //         curr++;
        //       }
        //       // set precision to the parsed one
        //       break;
        //     }
        //     case 'd':
        //     case 's':
        //     case 'x':
        //     case 'X':
        //     case 'o':
        //     default: // raise error
        //     }
        //   }
        // }
        // *********************************************************************
        // default: {
        //   // std::cout << " hello";
        //   // answer += *curr;
        //   switch(*curr){
        //     // initialize width, precision, flags and repr flag with default
        //     values according to ncurses int width; case ':':{ break;} case
        //     '#':{ break;} case ' ':{ break;} case '.':{
        //       // parse precision
        //       std::string precisionStr;
        //       while(std::isdigit(*curr)){
        //         precisionStr += *curr;
        //         curr++;
        //       }
        //       precision = std::stoi(precisionStr);
        //       break;
        //     }
        //     case 'd':{
        //       break;
        //     }
        //     case 'o':{
        //       break;
        //     }
        //     case 'x': {
        //       break;
        //     }
        //     case 'X': {
        //       break;
        //     }
        //     case 's': {break;}
        //     default: {
        //       // check if it is a digit
        //       if (std::isdigit(*curr)){
        //         // find width and/or precision
        //         std::string widthStr;
        //         while (std::isdigit(*curr)){
        //           widthStr += *curr;
        //           curr++;
        //         }
        //         auto width = std::stoi(widthStr);

        //       } else {
        //         throw std::runtime_error("Invalid % encoding found");
        //       }
        //       break;
        //     }
        //   }
        //   break;
        // }

        // std::cout << "\n  answer is: " << answer;
        // *********************************************************************
      }
    }
    curr++;
  }

  return answer;
}

// int main()
// {

//   std::string str = "\\E]4;%p1%d;rgb\\:%p2%{255}%*%{1000}%/%2.2X/"
//                     "%p3%{255}%*%{1000}%/%2.2X/%p4%{255}%*%{1000}%/%2.2X\E\\";

//   // = "%tushar";
//   // std::cout << "Enter the string to parse: ";
//   // std::cin >> str;

//   auto res = evaluateString(str, {1, 2, 3, 4});

//   std::cout << "    evaluated String res is: " << res << '\n';

//   return 0;
// }