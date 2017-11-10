#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <typeinfo>
#include <vector>

enum class base
{
  d = 'd',
  o = 'o',
  x = 'x',
  X = 'X',
  s = 's'
};



std::string evaluateString(const std::string &notEvaluated,
                           std::vector<int> args = {})
{

  std::string answer;
  std::stack<int> s;
  std::map<std::string, int> vars;

  for (auto curr = notEvaluated.begin(); curr != notEvaluated.end();)
  {
    // std::cout << "\n\n    current char is: " << *curr;

    if (*curr != '%')
    {

      answer += *curr;
      // curr++;
    }
    else
    {
      curr++;

      switch (*curr)
      {

      case '\'':
      {
        auto constCharASCIIVal = static_cast<int>(*++curr) - 48 - 1;
        s.push(constCharASCIIVal);
        curr++;
        break;
      }

      case 'd':
      {
        // TODO: Are you sure to add 48 ?
        auto top = std::to_string(s.top()); 
        s.pop();
        answer += top;
        break;
      }

      case 't':
      {
        if (!s.empty())
        { // TODO: what should happen if stack is empty
          auto top = s.top();
          s.pop();
        }
        else
        {
          throw std::runtime_error("Stack empty.. Invalid expression to parse");
        }
        break;
      }

      case '%':
      {
        answer += '%';
        break;
      }

      case 'p':
      {
        auto paramNum = static_cast<int>(*++curr) - 48 - 1;
        if (paramNum < args.size())
        { // TODO: what should happen if this fails
          s.push(args[paramNum]);
        }
        else
        {
          throw std::runtime_error(
              "Parameter number out of range.. Invalid expression to parse");
        }
        break;
      }

      case 'P':
      {
        auto varName = std::string(1, *++curr);
        if (!s.empty())
        { // TODO: what should happen if stack is empty
          auto top = s.top();
          s.pop();
          vars[varName] = top;
        }
        else
        {
          throw std::runtime_error("Stack empty.. Invalid expression to parse");
        }
        break;
      }

      case 'g':
      { // Both 'g' cases handled here
        auto varName = std::string(1, *++curr);
        auto keyIter = vars.find(varName);
        if (keyIter != vars.end())
        { // TODO: what happens if this fails
          s.push(keyIter->second);
        }
        else
        {
          throw std::runtime_error(
              "Invalid Variable.. Invalid expression to parse");
        }
        break;
      }

      case '+':
      {
        if (s.size() >= 2)
        { // TODO: what should happen if this fails
          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.pop();
          s.push(operand1 + operand2);
        }
        else
        {
          throw std::runtime_error(
              "Number of operands less than 2.. Invalid expression to parse");
        }
        break;
      }

      case '-':
      {
        if (s.size() >= 2)
        { // TODO: what should happen if this fails
          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.pop();
          s.push(operand1 - operand2);
        }
        else
        {
          throw std::runtime_error(
              "Number of operands less than 2.. Invalid expression to parse");
        }
        break;
      }

      case '*':
      {
        if (s.size() >= 2)
        { // TODO: what should happen if this fails
          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.pop();
          s.push(operand1 * operand2);
        }
        else
        {
          throw std::runtime_error(
              "Number of operands less than 2.. Invalid expression to parse");
        }
        break;
      }

      case '/':
      {
        if (s.size() >= 2)
        { // TODO: what should happen if this fails
          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.pop();
          s.push(operand1 / operand2);
        }
        else
        {
          throw std::runtime_error(
              "Number of operands less than 2.. Invalid expression to parse");
        }
        break;
      }

      case 'm': // NOTE: special char for mod
      {
        if (s.size() >= 2)
        { // TODO: what should happen if this fails
          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.pop();
          s.push(operand1 % operand2);
        }
        else
        {
          throw std::runtime_error(
              "Number of operands less than 2.. Invalid expression to parse");
        }
        break;
      }

      case '&':
      {
        if (s.size() >= 2)
        { // TODO: what should happen if this fails
          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.pop();
          s.push(operand1 & operand2);
        }
        else
        {
          throw std::runtime_error(
              "Number of operands less than 2.. Invalid expression to parse");
        }
        break;
      }

      case '|':
      {
        if (s.size() >= 2)
        { // TODO: what should happen if this fails
          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.pop();
          s.push(operand1 | operand2);
        }
        else
        {
          throw std::runtime_error(
              "Number of operands less than 2.. Invalid expression to parse");
        }
        break;
      }

      case '^':
      {
        if (s.size() >= 2)
        { // TODO: what should happen if this fails
          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.pop();
          s.push(operand1 ^ operand2);
        }
        else
        {
          throw std::runtime_error(
              "Number of operands less than 2.. Invalid expression to parse");
        }
        break;
      }

      case 'i':
      {
        if (args.size() >= 2)
        { // TODO: what should happen if this fails
          args[0] += 1;
          args[1] += 1;
        }
        else
        {
          throw std::runtime_error(
              "Parameter number out of range.. Invalid expression to parse");
        }
        break;
      }

      case '=':
      {
        if (s.size() >= 2)
        { // TODO: what should happen if this fails
          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.push(operand1 == operand2);
        }
        else
        {
          throw std::runtime_error(
              "Number of operands less than 2.. Invalid expression to parse");
        }
        break;
      }

      case '<':
      {
        if (s.size() >= 2)
        { // TODO: what should happen if this fails
          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.push(operand1 < operand2);
        }
        else
        {
          throw std::runtime_error(
              "Number of operands less than 2.. Invalid expression to parse");
        }
        break;
      }

      case '>':
      {
        if (s.size() >= 2)
        { // TODO: what should happen if this fails
          auto operand1 = s.top();
          s.pop();
          auto operand2 = s.top();
          s.push(operand1 > operand2);
        }
        else
        {
          throw std::runtime_error(
              "Number of operands less than 2.. Invalid expression to parse");
        }
        break;
      }

      case '!':
      {
        if (s.size() >= 1)
        { // TODO: what should happen if this fails
          auto operand = s.top();
          s.pop();
          s.push(!operand); // TODO: confirm this !operand
        }
        else
        {
          throw std::runtime_error(
              "Number of operands less than 1.. Invalid expression to parse");
        }
        break;
      }

      case '~':
      {
        if (s.size() >= 1)
        { // TODO: what should happen if this fails
          auto operand = s.top();
          s.pop();
          s.push(~operand); // TODO: confirm this ~operand and need to use
                            // unsigned int
        }
        else
        {
          throw std::runtime_error(
              "Number of operands less than 1.. Invalid expression to parse");
        }
        break;
      }

      case '{':
      {
        std::string decimalNumberToBe;
        while (*++curr != '}')
        {
          decimalNumberToBe += *curr;
        }
        s.push(std::stoi(decimalNumberToBe)); // DOUBT: should this
                                              // 'decimalNumberToBe' pushed to
                                              // stack or appended to answer ?
        break;
      }

      case 'c':
      {
        // DOUBT: According to the new man page the top-most stack value should
        // be popped as character and appended to answer. So if top of stack
        // contains 3 then '3' should be appended to output or ascii value
        // corresponding to 3 ? for now appending the ascii value.
        auto charCorrespondingToTop = static_cast<char>(s.top());
        s.pop();
        answer += charCorrespondingToTop;
        break;
      }

      case 's':
      {
        // DOUBT: Should pop the whole stack or only the topmost item ?
        // for now poppping the whole stack.
        // DOUBT: same doubt as case 'c'
        while (!s.empty())
        {
          answer += static_cast<char>(s.top());
          s.pop();
        }
        break;
      }

      case 'l':
      {
        auto poppedVal = std::to_string(s.top());
        s.pop();
        s.push(poppedVal.size());
      }

      case ':':
      case ' ':
      case '#':
      {
        // TODO: check the validity of this parsing algo
        std::string numberToBe;
        curr++;
        if (*curr == '+' || *curr == '-')
        {
          numberToBe += *curr;
          curr++;
        }
        char base{'d'};
        while (*curr != '%' && curr != notEvaluated.end())
        {
          if (*curr == 'd' || *curr == 'o' || *curr == 'x' || *curr == 'X' ||
              *curr == 's')
          {
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

      case '?':
      {
        throw std::runtime_error("If-else not yet implemented !!");
      }

      default:
        std::cout << " hello";
        answer += *curr;
        break;
      }

      // std::cout << "\n  answer is: " << answer;
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