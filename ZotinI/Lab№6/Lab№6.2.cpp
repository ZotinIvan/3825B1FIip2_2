#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>

using std::cout;
using std::cin;
using std::string;

class Gamer {
protected:
    string a;
    int n;

    bool isnum(const string& s) {
        if (s.empty()) 
            return false;
        for (char c : s) {
            if (c < '0' || c > '9') 
                return false;
        }
        return true;
    }

    void set_n() {
        while (true) {
            cout << "Enter the length of the number: ";
            if (cin >> n && n > 0) {
                break;
            }
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Enter a positive number\n";
        }
        cin.ignore(10000, '\n');
    }

public:
    Gamer() {
        set_n();
        a = "0";
    }

    void set_str() {
        cout << "Enter number: \n";
        bool fl = false;
        while (true) {
            getline(cin, a);
            if (!isnum(a)) {
                cout << "Enter only digits.\n";
                continue;
            }
            if (a.length() != n) {
                cout << "Number must be " << n << " digits long.\n";
                continue;
            }

            if (a[0] - '0' == 0) {
                cout << "The number does not start with 0\n";
                continue;
            }
            fl = false;
            for (size_t i = 0; i < n - 1; i++) {
                for (size_t j = i+1; j < n; j++) {
                    if (a[i] == a[j]) {
                        fl = true;
                        break;
                    }
                }
                if (fl) {
                    break;
                }
            }
            if (fl) {
                cout << "The numbers should not be repeated\n";
                continue;
            }
            break;
        }

    }
};

class Computer {
protected:
    int* num;
    size_t bulls, cows;
    size_t m;
public:
    Computer(size_t n = 1): m(n), bulls(0), cows(0) {
        num = new int[m];
    }

    Computer(const Computer& other) : m(other.m), bulls(other.bulls), cows(other.cows) {
        num = new int[m];
        for (size_t i = 0; i < m; i++) {
            num[i] = other.num[i];
        }
    }
    Computer& operator=(const Computer& other) {
        if (this != &other) {
            delete[] num;
            m = other.m;
            num = new int[m];
            for (size_t i = 0; i < m; i++) {
                num[i] = other.num[i];
            }
            bulls = other.bulls;
            cows = other.cows;
        }
        return *this;
    }

    void generate(size_t n) {
        int a;
        num[0] = rand() % 9 + 1;
        for (size_t i = 1; i < n; i++) {
            a = rand() % 10;
            for (size_t j = 0; j < i; j++) {
                if (num[j] == a) {
                    i--;
                    break;
                }
                if (j == i - 1) {
                    num[i] = a;
                }
            }
        }
    }

    bool attempt(const string& a, size_t n) {
        bulls = 0;
        cows = 0;
        for (size_t i = 0; i < n; i++) {
            if ((a[i] - '0') == num[i]) {
                bulls++;
            }
        }

        for (size_t i = 0; i < n; i++) {
            if ((a[i] - '0') != num[i]) {
                for (size_t j = 0; j < n; j++) {
                    if (i != j && (a[i] - '0') == num[j]) {
                        cows++;
                        break;
                    }
                }
            }
        }
        cout << "Bulls: " << bulls << ", Cows: " << cows << '\n';
        return bulls == n;
    }

    ~Computer() {
        delete[] num;
    }
 
};


class Game: public Gamer, public Computer {
public:
    Game(): Gamer(), Computer(n) {
        string fl;
        do {
            generate(n);
            while (true) {
                cout << "--------------------\n";
                set_str();
                if (attempt(a, n)) {
                    cout << "The number is guessed!\n";
                    break;
                }
            }
            cout << "--------------------\n";
            cout << "Play again? If you want, then enter '1'\n";
            getline(cin, fl);
            if (fl == "1") {
                bulls = 0;
                set_n();
            }
            else {
                break;
            }
        } while (true);
    }
};


int main() {
    srand(time(nullptr));
    cout << "---BULLS and COWS---\n";
    cout << "--------------------\n";
    Game game;
    cout << "Exit...\n";
    return 0;
}