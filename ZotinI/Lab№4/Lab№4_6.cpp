#include <iostream>
#include <cmath>
#include <fstream>
#include <string>

using std::cout;
using std::cin;
using std::string;

class Date {
protected:
    unsigned day;
    unsigned month;
    unsigned year;

    bool prov(unsigned d, unsigned m, unsigned y) {
        if (d > 31 || m > 12 || y < 1900 || y > 2026 || d == 0 || m == 0) {
            return false;
        }
        return true;
    }

public:
    Date(unsigned d = 1, unsigned m = 1, unsigned y = 2020) {
        if (prov(d, m, y)) {
            day = d;
            month = m;
            year = y;
        }
        else {
            cout << "Warning date \n";
            day = 1;
            month = 1;
            year = 2020;
        }
    }

    bool operator<(const Date& other) const {
        if (year != other.year) {
            return year < other.year;
        }
        if (month != other.month) {
            return month < other.month;
        }
        if (day != other.day) {
            return day < other.day;
        }
        return false;
    }
    bool operator==(const Date& other) const {
        return day == other.day && month == other.month && year == other.year;
    }
    unsigned getday() const {
        return day;
    }
    unsigned getmonth() const {
        return month;
    }
    unsigned getyear() const {
        return year;
    }

    void print_data() const {
        cout << day << '.' << month << '.' << year;
    }
};

class Film : public Date {
private:
    string name;
    string director;
    string composer;
    string screenwriter;
    long long charge;

public:
    Film(string name_ = "undefined", string dir = "undefined", string scr = "undefined",
        string com = "undefined", long long charge_ = 0,
        unsigned d = 1, unsigned m = 1, unsigned y = 2020) : Date(d, m, y) {
        name = name_;
        director = dir;
        screenwriter = scr;
        composer = com;
        charge = charge_;
    }

    Film(const Film& other) : Date(other), name(other.name), director(other.director),
        screenwriter(other.screenwriter), composer(other.composer), charge(other.charge) {
    }

    Film& operator=(const Film& other) {
        if (this != &other) {
            name = other.name;
            director = other.director;
            screenwriter = other.screenwriter;
            composer = other.composer;
            charge = other.charge;
            day = other.day;
            month = other.month;
            year = other.year;
        }
        return *this;
    }

    string getname() const {
        return name;
    }
    string getdir() const {
        return director;
    }
    string getcom() const {
        return composer;
    }
    string getscr() const {
        return screenwriter;
    }
    long long getcharge() const {
        return charge;
    }

    void setname(const string& name_) {
        name = name_;
    }
    void setdir(const string& nd) {
        director = nd;
    }
    void setcom(const string& ncom) {
        composer = ncom;
    }
    void setscr(const string& ns) {
        screenwriter = ns;
    }
    void setcharge(long long nc) {
        charge = nc;
    }
    void setdate(unsigned d, unsigned m, unsigned y) {
        if (prov(d, m, y)) {
            day = d;
            month = m;
            year = y;
        }
    }

    void printfilm() const {
        cout << "Name: " << name << "\nDirector: " << director << "\nComposer: " << composer
            << "\nScreenwriter: " << screenwriter << "\nCharge: " << charge
            << " rub\n" << "Release date: ";
        print_data();
        cout << "\n---------------------\n";
    }

    void savetofile(std::ofstream& file) const {
        file << name << "\n";
        file << director << "\n";
        file << screenwriter << "\n";
        file << composer << "\n";
        file << charge << "\n";
        file << day << " " << month << " " << year << "\n";
    }

    void out_file(std::ifstream& file) {
        getline(file, name);
        getline(file, director);
        getline(file, screenwriter);
        getline(file, composer);
        string ch;
        getline(file, ch);
        charge = stoll(ch);
        string dt;
        getline(file, dt);
        size_t pos = 0;
        size_t prevPos = 0;

        while (pos < dt.length() && dt[pos] != ' ') {
            pos++;
        }
        string dayStr = dt.substr(prevPos, pos - prevPos);
        day = std::stoi(dayStr);

        pos++;
        prevPos = pos;
        while (pos < dt.length() && dt[pos] != ' ') {
            pos++;
        }
        string monthStr = dt.substr(prevPos, pos - prevPos);
        month = std::stoi(monthStr);

        pos++;
        prevPos = pos;
        while (pos < dt.length() && dt[pos] != ' ' && dt[pos] != '\n' && dt[pos] != '\r') {
            pos++;
        }
        string yearStr = dt.substr(prevPos, pos - prevPos);
        year = std::stoi(yearStr);
    }

    bool operator<(const Film& other) const {
        if (name != other.name) {
            return name < other.name;
        }
        return Date::operator<(other);
    }

    bool operator>(const Film& other) const {
        return other < *this;
    }

    bool operator==(const Film& other) const {
        return name == other.name && Date::operator==(other);
    }
};

class Filmoteka {
private:
    Film* lib;
    size_t count;
    size_t capacity;

    void sort() {
        for (size_t i = 0; i < count; ++i) {
            for (size_t j = 0; j < count - i - 1; ++j) {
                if (lib[j] > lib[j + 1]) {
                    Film temp = lib[j];
                    lib[j] = lib[j + 1];
                    lib[j + 1] = temp;
                }
            }
        }
    }

    void insert(const Film& film) {
        size_t pos = 0;
        while (pos < count && lib[pos] < film) {
            pos++;
        }

        for (size_t i = count; i > pos; --i) {
            lib[i] = lib[i - 1];
        }

        lib[pos] = film;
        count++;
    }

    void resize(size_t n) {
        Film* nlib = new Film[n];
        size_t cs;
        if (count < n) {
            cs = count;
        }
        else {
            cs = n;
        }
        for (size_t i = 0; i < cs; ++i) {
            nlib[i] = lib[i];
        }

        delete[] lib;
        lib = nlib;
        capacity = n;
        if (count > capacity) {
            count = capacity;
        }
    }

    bool is_empty() const {
        if (count == 0) {
            cout << "No films in the filmoteka\n";
            return true;
        }
        return false;
    }

public:
    Filmoteka(int cap = 1) : count(0), capacity(0), lib(nullptr) {
        if (cap > 0) {
            capacity = cap;
            lib = new Film[capacity];
        }
    }

    Filmoteka(const Filmoteka& other) : count(other.count), capacity(other.capacity), lib(nullptr) {
        if (capacity > 0 && count > 0) {
            lib = new Film[capacity];
            for (size_t i = 0; i < count; ++i) {
                lib[i] = other.lib[i];
            }
        }
        else if (capacity > 0) {
            lib = new Film[capacity];
        }
    }

    Filmoteka& operator=(const Filmoteka& other) {
        if (this != &other) {
            delete[] lib;
            count = other.count;
            capacity = other.capacity;
            lib = nullptr;
            if (capacity > 0) {
                lib = new Film[capacity];
                if (count > 0) {
                    for (size_t i = 0; i < count; ++i) {
                        lib[i] = other.lib[i];
                    }
                }
            }
        }
        return *this;
    }

    void append() {
        if (count >= capacity) {
            resize(count + 1);
        }
        string nm, dr, cm, sc;
        long long ch;
        unsigned d, m, y;

        cout << "Enter name film: ";
        cin.ignore();
        getline(cin, nm);
        cout << "Enter name of director: ";
        getline(cin, dr);
        cout << "Enter name of composer: ";
        getline(cin, cm);
        cout << "Enter name of screenwriter: ";
        getline(cin, sc);

        do {
            cout << "Enter charge: ";
            if (cin >> ch) {
                if (ch >= 0) {
                    cin.ignore(10000, '\n');
                    break;
                }
                cout << "Please enter a positive number.\n";
                cin.ignore(10000, '\n');
            }
            else {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input. Please enter a number.\n";
            }
        } while (true);

        cout << "Enter release date (day month year): ";
        while (true) {
            if (cin >> d >> m >> y) {
                if (d >= 1 && d <= 31 && m >= 1 && m <= 12 && y >= 1900 && y <= 2026) {
                    cin.ignore(10000, '\n');
                    break;
                }
                cout << "Invalid date! Enter day (1-31), month (1-12), year (1900-2026): ";
            }
            else {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input! Enter three numbers (day month year): ";
            }
        }

        Film f(nm, dr, sc, cm, ch, d, m, y);
        insert(f);
        cout << "Movie added\n";
    }

    void rewrite() {
        if (is_empty())
            return;

        string nm;
        unsigned ny;

        cout << "Enter name of film for searching: \n";
        cin.ignore();
        getline(cin, nm);
        cout << "Enter year: ";
        do {
            if (cin >> ny) {
                if (ny >= 1900 && ny <= 2026) {
                    cin.ignore(10000, '\n');
                    break;
                }
                cout << "Enter number (1900 - 2026): ";
            }
            else {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input! Enter a positive number: ";
            }
        } while (true);
        bool found = false;
        for (size_t k = 0; k < count; ++k) {
            if (lib[k].getname() == nm && lib[k].getyear() == ny) {
                found = true;
                cout << "\nFound film:\n";
                lib[k].printfilm();
                int a;
                cout << "\nWhat do you want to change?: \n1. Name \n2. Director \n3. Composer \n4. Screenwriter \n5. Charge \n6. Release date \n7. Exit\n";

                do {
                    if (cin >> a) {
                        string value;
                        unsigned nd, nmonth, newyear;
                        long long ncharge;
                        Film newfilm = lib[k];

                        switch (a) {
                        case 1:
                            cout << "Enter name film: ";
                            cin.ignore();
                            getline(cin, value);
                            newfilm.setname(value);
                            for (size_t i = k; i < count - 1; ++i) {
                                lib[i] = lib[i + 1];
                            }
                            count--;
                            insert(newfilm);
                            cout << "Name updated\n";
                        case 2:
                            cout << "Enter name of director: ";
                            cin.ignore();
                            getline(cin, value);
                            lib[k].setdir(value);
                            cout << "Director updated\n";
                            return;
                        case 3:
                            cout << "Enter name of composer: ";
                            cin.ignore();
                            getline(cin, value);
                            lib[k].setcom(value);
                            cout << "Composer updated\n";
                            return;
                        case 4:
                            cout << "Enter name of screenwriter: ";
                            cin.ignore();
                            getline(cin, value);
                            lib[k].setscr(value);
                            cout << "Screenwriter updated\n";
                            return;
                        case 5:
                            do {
                                cout << "Enter charge: ";
                                if (cin >> ncharge) {
                                    if (ncharge >= 0) {
                                        cin.ignore(10000, '\n');
                                        break;
                                    }
                                    cout << "Please enter a positive number.\n";
                                    cin.ignore(10000, '\n');
                                }
                                else {
                                    cin.clear();
                                    cin.ignore(10000, '\n');
                                    cout << "Invalid input. Please enter a number.\n";
                                }
                            } while (true);
                            lib[k].setcharge(ncharge);
                            cout << "Charge updated\n";
                            return;
                        case 6:
                            cout << "Enter release date (day month year): ";
                            while (true) {
                                if (cin >> nd >> nmonth >> newyear) {
                                    if (nd >= 1 && nd <= 31 && nmonth >= 1 && nmonth <= 12 && newyear >= 1900 && newyear <= 2026) {
                                        cin.ignore(10000, '\n');
                                        break;
                                    }
                                    cout << "Invalid date! Enter day (1-31), month (1-12), year (1900-2026): ";
                                }
                                else {
                                    cin.clear();
                                    cin.ignore(10000, '\n');
                                    cout << "Invalid input! Enter three numbers (day month year): ";
                                }
                            }
                            newfilm.setdate(nd, nmonth, newyear);
                            for (size_t i = k; i < count - 1; ++i) {
                                lib[i] = lib[i + 1];
                            }
                            count--;
                            insert(newfilm);
                            cout << "Date updated\n";
                            return;
                        case 7:
                            return;
                        default:
                            cout << "Invalid choice. Try again: ";
                        }
                    }
                    else {
                        cin.clear();
                        cin.ignore(10000, '\n');
                        cout << "Invalid input. Try again: ";
                    }
                } while (true);
            }
        }
        if (!found) {
            cout << "Film not found!\n";
        }
    }

    void searchFilm() {
        if (is_empty())
            return;

        string nm;
        unsigned ny;

        cout << "Enter film name to search: ";
        cin.ignore();
        getline(cin, nm);
        cout << "Enter release year: ";
        do {
            if (cin >> ny) {
                if (ny >= 1900 && ny <= 2026) {
                    break;
                }
                cout << "Enter number (1900 - 2026): ";
            }
            else {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input! Enter a positive number: ";
            }
        } while (true);

        for (size_t i = 0; i < count; ++i) {
            if (lib[i].getname() == nm && lib[i].getyear() == ny) {
                cout << "Found film:\n";
                lib[i].printfilm();
                return;
            }
        }
        cout << "Film not found\n";
    }

    void directorfilms() {
        if (is_empty())
            return;

        string dir;
        cout << "Enter name of director: ";
        cin.ignore();
        getline(cin, dir);

        bool fl = false;
        cout << "\n--- Films by " << dir << " ---\n";
        for (size_t i = 0; i < count; ++i) {
            if (lib[i].getdir() == dir) {
                lib[i].printfilm();
                fl = true;
            }
        }
        if (!fl) {
            cout << "No films found by this director\n";
        }
    }

    void films_of_year() {
        if (is_empty())
            return;

        unsigned y;
        cout << "Enter year: \n";
        do {
            if (cin >> y) {
                if (y >= 1900 && y <= 2026) {
                    break;
                }
                cout << "Enter number (1900 - 2026): ";
            }
            else {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input! Enter a positive number: ";
            }
        } while (true);

        bool fl = false;
        cout << "\n--- Films from " << y << " ---\n";
        for (size_t i = 0; i < count; ++i) {
            if (lib[i].getyear() == y) {
                lib[i].printfilm();
                fl = true;
            }
        }
        if (!fl) {
            cout << "No films found for this year\n";
        }
    }

    void maxcharge() {
        if (is_empty())
            return;

        size_t n;
        cout << "Enter number of films to display (max " << count << "): ";

        do {
            if (cin >> n) {
                if (n >= 1) {
                    break;
                }
                cout << "Invalid input! Enter a positive number: ";
            }
            else {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input! Enter a positive number: ";
            }
        } while (true);

        if (n > count) {
            cout << "There are only " << count << " films in the filmoteka\n";
            n = count;
        }

        Film* temp = new Film[count];
        for (size_t i = 0; i < count; ++i) {
            temp[i] = lib[i];
        }

        for (size_t i = 0; i < count; ++i) {
            for (size_t j = 0; j < count - i - 1; ++j) {
                if (temp[j].getcharge() < temp[j + 1].getcharge()) {
                    Film t = temp[j];
                    temp[j] = temp[j + 1];
                    temp[j + 1] = t;
                }
            }
        }

        cout << "\n-------Top films-------\n";
        for (size_t k = 0; k < n; ++k) {
            temp[k].printfilm();
        }
        delete[] temp;
    }

    void maxcharge_in_year() {
        if (is_empty())
            return;

        unsigned y;
        cout << "Enter year: \n";
        do {
            if (cin >> y) {
                if (y >= 1900 && y <= 2026) {
                    break;
                }
                cout << "Enter number (1900 - 2026): ";
            }
            else {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input! Enter a positive number: ";
            }
        } while (true);

        Film* temp = new Film[count];
        size_t amount = 0;
        for (size_t i = 0; i < count; ++i) {
            if (lib[i].getyear() == y) {
                temp[amount++] = lib[i];
            }
        }

        if (amount == 0) {
            cout << "There are no films this year\n";
        }
        else {
            size_t n;
            cout << "Enter number of films to display (max " << amount << "): ";

            do {
                if (cin >> n) {
                    if (n >= 1) {
                        break;
                    }
                    cout << "Invalid input! Enter a positive number: ";
                }
                else {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input! Enter a positive number: ";
                }
            } while (true);

            if (n > amount) {
                cout << "There are only " << amount << " films in the filmoteka\n";
                n = amount;
            }


            for (size_t i = 0; i < amount; ++i) {
                for (size_t j = 0; j < amount - i - 1; ++j) {
                    if (temp[j].getcharge() < temp[j + 1].getcharge()) {
                        Film t = temp[j];
                        temp[j] = temp[j + 1];
                        temp[j + 1] = t;
                    }
                }
            }
            cout << "\n-------Top films in " << y << "------ - \n";
            for (size_t k = 0; k < n; ++k) {
                temp[k].printfilm();
            }
        }
        delete[] temp;
    }

    void print_count() const {
        cout << "Number of films in the filmoteka: " << count << '\n';
    }

    void remove_film() {
        if (is_empty())
            return;

        string nm;
        unsigned ny;

        cout << "Enter name of film to remove: \n";
        cin.ignore();
        getline(cin, nm);
        cout << "Enter year: ";

        do {
            if (cin >> ny) {
                if (ny >= 1900 && ny <= 2026) {
                    break;
                }
                cout << "Enter number (1900 - 2026): ";
            }
            else {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input! Enter a positive number: ";
            }
        } while (true);

        bool fl = false;
        for (size_t i = 0; i < count; ++i) {
            if (lib[i].getname() == nm && lib[i].getyear() == ny) {
                for (size_t j = i; j < count - 1; ++j) {
                    lib[j] = lib[j + 1];
                }
                count--;
                fl = true;
                cout << "Film removed successfully\n";
                break;
            }
        }
        if (!fl) {
            cout << "Film not found\n";
        }
    }

    void write_in_file(const string& name) const {
        if (is_empty())
            return;

        std::ofstream file(name);
        if (file.is_open()) {
            file << count << "\n";
            for (size_t i = 0; i < count; ++i) {
                lib[i].savetofile(file);
            }
            cout << "Filmoteka successfully saved to file: " << name << "\n";
            cout << "Saved " << count << " films.\n";
            file.close();
        }
        else {
            cout << "Cannot open file\n";
        }
    }

    void read_file(const string& name) {
        std::ifstream file(name);

        if (file.is_open()) {
            if (lib != nullptr) {
                delete[] lib;
                lib = nullptr;
            }
            count = 0;
            capacity = 0;

            size_t n;
            file >> n;
            file.ignore();

            if (n > 0) {
                resize(n);
                for (size_t i = 0; i < n && i < capacity; ++i) {
                    Film t;
                    t.out_file(file);
                    lib[count] = t;
                    count++;
                }
            }
            file.close();
            sort();
            cout << "Filmoteka successfully loaded from file: " << name << "\n";
            cout << "Loaded " << count << " films.\n";
        }
        else {
            cout << "Cannot open file\n";
        }
    }

    ~Filmoteka() {
        if (lib != nullptr) {
            delete[] lib;
        }
    }
};

int main() {
    int s;
    Filmoteka f;
    string fname;

    do {
        cout << "---Filmoteka---\n";
        cout << "1. add a movie\n";
        cout << "2. change the data\n";
        cout << "3. find the movie by name and year\n";
        cout << "4. give out all the films of the specified director\n";
        cout << "5. give out all the films released in the selected year\n";
        cout << "6. give out the set number of films with the highest fees\n";
        cout << "7. give out a set number of films with the highest collections in the selected year\n";
        cout << "8. find out the current number of films\n";
        cout << "9. delete the film\n";
        cout << "10. save the movie library to a file\n";
        cout << "11. read the film library from the file\n";
        cout << "0. exit\n";
        cout << "Choice: ";

        cin >> s;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (s) {
        case 1:
            f.append();
            break;
        case 2:
            f.rewrite();
            break;
        case 3:
            f.searchFilm();
            break;
        case 4:
            f.directorfilms();
            break;
        case 5:
            f.films_of_year();
            break;
        case 6:
            f.maxcharge();
            break;
        case 7:
            f.maxcharge_in_year();
            break;
        case 8:
            f.print_count();
            break;
        case 9:
            f.remove_film();
            break;
        case 10:
            cout << "Enter the file name to save: ";
            cin >> fname;
            f.write_in_file(fname);
            break;
        case 11:
            cout << "Enter the file name to load: ";
            cin >> fname;
            f.read_file(fname);
            break;
        case 0:
            break;
        default:
            cout << "Input error. Try again\n";
            break;
        }
    } while (s != 0);

    return 0;
}
