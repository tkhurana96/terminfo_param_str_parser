#include <iostream>
#include <map>
#include <ncurses.h>
#include "paramStrings.cpp"

// template<size_t> struct counter { };

// template<size_t N, typename... Args>
// int func_from_vector(const std::string& s, counter<N>, const std::vector<int>& v, Args... args)
// {
// 	if(N < v.size())
// 		return func_from_vector(s, counter<N + 1>(), v, args..., v[N]);
// 	else
// 		return tparm(s.data(), args...);
// }

// // terminate infinity
// constexpr size_t end = 10;

// template<typename... Args>
// auto func_from_vector(const std::string& s, counter<end>, const std::vector<int>& v, Args... args)
// {
// 	if(end < v.size())
// 		return tparm(s.data(), args..., v[end]);
// 	else
// 		return tparm(s.data(), args...);
// }

// auto func_from_vector(const std::string& s, const std::vector<int>& v)
// {
// 	return func_from_vector(s, counter<0>(), v);
// }

int main(){

    std::map<std::string, std::string> no_param_caps{
        {"make_visible", "\E[?12l\E[?25h"},
        {"make_visible_2", "\E[?12;25h"},
        {"make_invisible", "\E[?25l"},
        {"clear", "\E[H\E[2J"},
        {"clear_to_screen_end", "\E[J"},
        {"clear_to_eol", "\E[K"},
        {"clear_to_bol", "\E[1K"},
        {"blink","\E[5m"},
        {"bold", "\E[1m"},
        {"back_tab", "\E[Z"},
        {"carriage_return", "\r"},
        {"bell", "^G"},
        {"cursor_down", "\n"},
        {"cursor_left", "^H"},
        {"cursor_right", "\E[C"},
        {"cursor_up", "\E[A"},
        {"cursor_home", "\E[H"},
        {"delete_char", "\E[P"},
        {"dim_mode", "\E[2m"},
        {"delete_line", "\E[M"},
        {"flash_screen", "\E[?5h$<100/>\E[?5l"},
        {"tab", "^I"},
        {"set_tab", "\EH"},
        {"insert_line", "\E[L"},
        {"scroll_text_up","\n"},
        {"blank_mode", "\E[8m"},
        {"init_string", "\E[!p\E[?3;4l\E[4l\E>"},
        {"shifted_del_char", "\E[3;2~"}
        // TODO: add more no param strings
    };

    // TODO: The expected output of the with param tests should be taken from ncurses or tparm
    // Observation1: Negative values don't work with cuf, cup, cuu, hpa, il, indn, rin caps(maybe -ve values don't work at all)
    // Observation2: Goto_col(hpa) cap only needs one arg, but %i increments params if atleast 2 params are present,
    // i think %i should check existence individually for each of the first two args [FIXED].
    std::map<std::string, std::tuple<std::string, std::vector<int>, std::string>> param_caps{
        // {"test_name", {"test_input", test_input_args, "expected_output"}}
        {"cursor_left(cub)", {"\E[%p1%dD", {5}, "\E[5D"}},
        {"cursor_down(cud)", {"\E[%p1%dB", {12}, "\E[12B"}},
        {"cursor_right(cuf)_1", {"\E[%p1%dC", {4}, "\E[4C"}},
        {"cursor_right(cuf)_2", {"\E[%p1%dC", {-19}, "\E[-19C"}}, // Wrong test if not compared with ncurses behavior
        {"cursor_up(cuu)_1", {"\E[%p1%dA", {17}, "\E[17A"}},
        {"cursor_up(cuu)_2", {"\E[%p1%dA", {-20}, "\E[-20A"}}, // Wrong test if not compared with ncurses behavior
        {"move_cursor(cup)_1", {"\E[%i%p1%d;%p2%dH", {10, 20}, "\E[11;21H"}},
        {"move_cursor(cup)_2", {"\E[%i%p1%d;%p2%dH", {-5, -45}, "\E[-4;-44H"}}, // Wrong test if not compared with ncurses behavior
        {"delete_char(dch)_1", {"\E[%p1%dP", {5}, "\E[5P"}}, // Wrong test if not compared with ncurses behavior
        {"delete_char(dch)_2", {"\E[%p1%dP", {-7}, "\E[-7P"}}, // Wrong test if not compared with ncurses behavior
        {"delete_char(dch)_3", {"\E[%p1%dP", {100}, "\E[100P"}}, // Wrong test if not compared with ncurses behavior
        {"delete_lines(dl)_1", {"\E[%p1%dM", {6}, "\E[6M"}}, // Wrong test if not compared with ncurses behavior
        {"delete_lines(dl)_2", {"\E[%p1%dM", {-8}, "\E[-8M"}}, // Wrong test if not compared with ncurses behavior
        {"delete_lines(dl)_3", {"\E[%p1%dM", {100}, "\E[100M"}}, // Wrong test if not compared with ncurses behavior
        {"erase_chars(ech)_1", {"\E[%p1%dX", {10}, "\E[10X"}}, // Wrong test if not compared with ncurses behavior
        {"erase_chars(ech)_2", {"\E[%p1%dX", {-5}, "\E[-5X"}}, // Wrong test if not compared with ncurses behavior
        {"erase_chars(ech)_3", {"\E[%p1%dX", {110}, "\E[110X"}}, // Wrong test if not compared with ncurses behavior
        {"goto_col(hpa)_1", {"\E[%i%p1%dG", {8}, "\E[9G"}},
        {"goto_col(hpa)_2", {"\E[%i%p1%dG", {-3}, "\E[-2G"}}, // Wrong test if not compared with ncurses behavior
        {"goto_col(hpa)_3", {"\E[%i%p1%dG", {10, 20}, "\E[11G"}}, // Wrong test if not compared with ncurses behavior
        // {"insert_chars(ich)_1", {"\E[%p1%d@", {}, ""}}, // Wrong test if not compared with ncurses behavior
        {"insert_chars(ich)_2", {"\E[%p1%d@", {7}, "\E[7@"}},
        {"insert_lines(il)_1", {"\E[%p1%dL", {5}, "\E[5L"}},
        {"insert_lines(il)_2", {"\E[%p1%dL", {-5}, "\E[-5L"}}, // Wrong test if not compared with ncurses behavior
        {"scroll_forward(indn)_1", {"\E[%p1%dS", {10}, "\E[10S"}}, 
        {"scroll_forward(indn)_2", {"\E[%p1%dS", {-10}, "\E[-10S"}}, // Wrong test if not compared with ncurses behavior
        // {"init_color(initc)", {"\E]4;%p1%d;rgb\:%p2%{255}%*%{1000}%/%2.2X/%p3%{255}%*%{1000}%/%2.2X/%p4%{255}%*%{1000}%/%2.2X\E\\", {7,50, 50, 50}, 
        // "\E]4;7;rgb\:"}}
        {"scroll_back(rin)", {"\E[%p1%dT", {4}, "\E[4T"}},
        // {"set_bg_color(setab)", {"", {}, ""}}
        // {"user_string6(u6)", {"\E[%i%d;%dR", {12, 13}, "\E[13;14R"}}
    };

    auto total_no_param_tests = no_param_caps.size();
    auto passed_no_param_tests = 0;
    for (auto& pair: no_param_caps){
        if (evaluateString(pair.second) == pair.second){
            passed_no_param_tests++;
            std::cout << "[PASSED] test for " << pair.first << '\n';
        } else {
            std::cout << "[FAILED] test for " << pair.first << '\n';
        }
    }

    auto total_param_tests = param_caps.size();
    auto passed_param_tests = 0;
    for (auto& [cap_name, vals]: param_caps){
        auto& [test_str, args, exp_res] = vals;
        auto actual_res = evaluateString(test_str, args);

        if (actual_res == exp_res){
            passed_param_tests++;
            std::cout << "[PASSED] test for " << cap_name << '\n';
        } else {
            std::cout << "[FAILED] test for " << cap_name << '\n';
        }
    }
    std::cout << std::string(40, '*') << '\n'
              << '[' << passed_no_param_tests << '/' << total_no_param_tests << "] NO_PARAM TESTS PASSED\n"
              << '[' << passed_param_tests << '/' << total_param_tests << "] PARAM TESTS PASSED\n"
              << std::string(40, '*') << '\n';

    // std::cout << "scrolling backward 4 lines: \E[4T did you see it ?";
    // std::cout << "color will be init'd \E]4;7;rgb\:50/50/50\E\\ did it work\n";
    // std::cout << "scrolling forward 7 lines: \E[7S did you see it ?\n"; 
    // std::cout << "5 lines will be inserted now..\E[5Lwere they inserted?\n"; // this works wierd
    // std::cout << "going to insert some characters..\E[7@did anything change?\n"; // this doesn't work
    // std::cout << "we will go to column 54 \E[196GDid we get there ?\n"; // this depends on size of terminal window
    // std::cout << "some chars will be deleted now \E[10X Did chars delete?\n"; // this doesn't work
    // std::cout << "some lines will be deleted now\n\E[6MDid lines delete?\n"; // this doesn't work
    // std::cout << "some chars will be deleted now\E[10PDid any chars delete?\n"; // this doesn't work
    // std::cout << "the cursor will move up now \E[7A Did it move up?\n";
    // std::cout << "the cursor will move up now \E[-10A Did it move up?\n"; // this doesn't work
    // std::cout << "can you check if the cursor moves to (55, 80) now? \E[55;80H Did it move ?\n"; // this doesn't work
    // std::cout << "can you check if the cursor moves to (-55, -80) now? \E[-55;-80H Did it move ?\n";
    // std::cout << "hi there, the cursor will move to the right now \E[20C Did it move ?\n";
    // std::cout << "hi there, the cursor will move to the right now(maybe left)\E[-4CDid it move ?\n";
    // auto test(){
    //   if (evaluateString("%p1%l%p2%-%d", {42}) == "27") {
    //       std::cout << "test 1 passed";
    //   if (evaluateString("%+%1.1c", {}) == "0") {
    //       std::cout << "test 2 passed";
    //   if (evaluateString("%'b'%s", {}) == "b") {
    //       std::cout << "test 3 passed";
    //   if (evaluateString("%p1%PA%p2%Pz%gz%gA%s", {"1337", 42}) == 'b') {
    //       std::cout << "test 4 passed";
    //   if (evaluateString("%:+d", {}) == "0") {
    //       std::cout << "test 5 passed";
    //   if (evaluateString("%p1%:-5d", {1337}) == "1337") {
    //       std::cout << "test 6 passed";
    //   if (evaluateString("%p2% d", {1337, 42}) == "42") {
    //       std::cout << "test 7 passed";
    //   if (evaluateString("%p2%#o", {1337, 42}) == "052") {
    //       std::cout << "test 8 passed";
    //   if (evaluateString("%p2%3d", {1337, 42}) == " 42") {
    //       std::cout << "test 9 passed";
    //   if (evaluateString("%p1%p2%&%#x", {0x110011,0x010010}) == "0x10010") {
    //       std::cout << "test 10 passed";
    //   if (evaluateString("%p1%p2%|%#x", {0x110011,0x010010}) == "0x110011") {
    //       std::cout << "test 11 passed";
    //   if (evaluateString("%p1%p2%^%#x", {0x110011,0x010010}) == "0x100001") {
    //       std::cout << "test 12 passed";
    //   // unittest { // more tests
    //   //   import std.array : appender;
    //   //   auto a = appender!(char[])();
    //   //   char[] fmt = "%p1%p2%p3%p4%p5%p6%p7%p8%p9" ~
    //   //     "%/%c," ~ // -67890 / 67890 = -1
    //   //     "%/%p1%*%3.3d," ~ // (12345 / -12345) * -42 = 42
    //   //     "%{65536}%m%:+#- o," ~ // 65536 % 1024 = 0
    //   //     "%%%#X," ~
    //   //     "%%%#x,".dup;
    //   //   FmtVar[9] args = [
    //   //     FmtVar(-42),
    //   //     FmtVar(42),
    //   //     FmtVar(1337),
    //   //     FmtVar(-1337),
    //   //     FmtVar(1024),
    //   //     FmtVar(-12345),
    //   //     FmtVar(12345),
    //   //     FmtVar(67890),
    //   //     FmtVar(-67890)];
    //   //   formattedWrite((const(char)[] i) {a.put(i);}, fmt, args);
    //   //   assert(a.data == "-1,042,0,%0XFFFFFFFFFFFFFAC7,%0x539,");
    //   // }
    // }
    // std::cout << "123456\E[2D78\n";
    // std::cout << "check this init string:" << no_param_caps["init_string"] << "cool ?\n";
    // std::cout << "check blank mode" << no_param_caps["blank_mode"] << "this is cool, check with input on\n";
    // std::cout << "a line will be inserted now.." << no_param_caps["insert_line"] << " did you see it ?\n"; // mysterious working
    // std::cout << "the cursor will tab now.." << no_param_caps["tab"] << " did it ?\n"; // doesn't work
    // std::cout << "the cursor will go home now.." << no_param_caps["cursor_home"] << " did it ?\n";
    // std::cout << "the screen will flash now.. " << no_param_caps["flash_screeen"] << " did it ?\n"; // doesn't work
    // std::cout << "hello something " << no_param_caps["clear_to_eol"] << " worked ?\n"; // doesn't work
    // std::cout << "will this clear to end of screen? \E[J cleared ?\n"; // doesn't work
    // std::cout << " hello something delete line cap: aaa\E[M hello otherthing\n";
    // std::cout << "bel cap:" << no_param_caps["bel"] << "hear something ?\n";
    // std::cout << "cr cap:" << no_param_caps["cr"] << "helo\n";
    // std::cout << "back_tab cap:" << no_param_caps["back_tab"] << '\n';
    // std::cout << "this will not print" << no_param_caps["clear"] << "this will print\n";
    // std::cout << "this will make the cursor invisible" << no_param_caps["make_invisible"] << '\n';
    // int n = 10;
    // std::cin >> n;
    // std::cout << "this will make the cursor visible" << no_param_caps["make_visible"] << '\n';


    return 0;
}