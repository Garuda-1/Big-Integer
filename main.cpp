#include <iostream>
#include "big_integer.h"

using namespace std;

void test_iface(int n) {
    cout << '<';
    for (int i = 0; i < n - 2; i++) {
        cout << '-';
    }
    cout << '>' << endl;
}

void run_testing_fail(string &operand_a, string &operand_b, const string &found, const string &expected) {
    cout << endl;
    cout << "TEST FAILED:" << endl;
    cout << "OPERAND_A: " << endl << operand_a << endl;
    cout << "OPERAND_B: " << endl << operand_b << endl;
    cout << "FOUND: " << endl << found << endl;
    cout << "EXPECTED: " << endl << expected << endl;
}

void run_testing_section_cmp() {
    cout << "TESTING SECTION: cmp" << endl;

    freopen("big_integer_test_cmp.txt", "r", stdin);

    int n;
    cin >> n;
    test_iface(100);

    for (int i = 0; i < n; i++) {
        if ((i * 100 / n) != ((i + 1) * 100 / n)) {
            cout << '|';
        }
        string operand_a, operand_b;
        cin >> operand_a >> operand_b;
        bool l, ng, g, nl, e, ne;
        cin >> l >> ng >> g >> nl >> e >> ne;

        big_integer a(operand_a);
        big_integer b(operand_b);

        bool f;

        f = a < b;
        if (f != l) {
            run_testing_fail(operand_a, operand_b, to_string(f), to_string(l));
            return;
        }
        f = a <= b;
        if (f != ng) {
            run_testing_fail(operand_a, operand_b, to_string(f), to_string(ng));
            return;
        }
        f = a > b;
        if (f != g) {
            run_testing_fail(operand_a, operand_b, to_string(f), to_string(g));
            return;
        }
        f = a >= b;
        if (f != nl) {
            run_testing_fail(operand_a, operand_b, to_string(f), to_string(nl));
            return;
        }
        f = a == b;
        if (f != e) {
            run_testing_fail(operand_a, operand_b, to_string(f), to_string(e));
            return;
        }
        f = a != b;
        if (f != ne) {
            run_testing_fail(operand_a, operand_b, to_string(f), to_string(ne));
            return;
        }
    }
    cout << endl;
    cout << "SECTION TESTS PASSED" << endl;
}

void run_testing_section_arithm(const string &section_name) {
    cout << "TESTING SECTION: " << section_name << endl;

    string file_name = string("big_integer_test_").append(section_name).append(string(".txt"));

    freopen(file_name.c_str(), "r", stdin);

    int n;
    cin >> n;
    test_iface(100);

    for (int i = 0; i < n; i++) {
        if ((i * 100 / n) != ((i + 1) * 100 / n)) {
            cout << '|';
        }
        string operand_a, operand_b, expected;
        cin >> operand_a >> operand_b >> expected;

        big_integer a(operand_a);
        big_integer b(operand_b);
        big_integer c;

        if (section_name == "add") {
            c = a + b;
        }
        if (section_name == "subtract") {
            c = a - b;
        }
        if (section_name == "multiply") {
            c = a * b;
            if (c / a != b) {
                run_testing_fail(operand_a, operand_b, "Div check failure", expected);
                return;
            }
        }
        if (section_name == "divide") {
            c = a / b;
        }
        if (section_name == "mod") {
            c = a % b;
        }

        string found = to_string(c);
        if (found != expected) {
            run_testing_fail(operand_a, operand_b, found, expected);
            return;
        }
    }
    cout << endl;
    cout << "SECTION TESTS PASSED" << endl;
}

void run_testing() {
    run_testing_section_cmp();
    run_testing_section_arithm("add");
    run_testing_section_arithm("subtract");
    run_testing_section_arithm("multiply");
    run_testing_section_arithm("divide");
    run_testing_section_arithm("mod");
}

int main() {
    run_testing();
    return 0;
}
