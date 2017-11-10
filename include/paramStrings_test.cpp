#include <iostream>
#include <map>
#include "paramStrings.cpp"

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

    std::map<std::string, std::tuple<std::string, std::vector<int>, std::string>> param_caps{
        {"cursor_left(cub)", {"\E[%p1%dD", {5}, "\E[5D"}},
        {"cursor_down(cud)", {"\E[%p1%dB", {12}, "\E[12B"}}
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
    std::cout << '[' << passed_no_param_tests << '/' << total_no_param_tests << "] NO_PARAM TESTS PASSED\n";

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
    std::cout << '[' << passed_param_tests << '/' << total_param_tests << "] PARAM TESTS PASSED\n";


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