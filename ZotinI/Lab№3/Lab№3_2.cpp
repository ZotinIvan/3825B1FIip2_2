#include <iostream>
#include <cmath>
#include <fstream>
#include <string>

using std::cout;
using std::cin;
using std::ostream;




class Tab {
private:
	size_t count;
	double a;
	double b;
	double* res;
	double* points;
	bool wastabulation;
	enum FunctionType { SIN, COS, EXP, LOG, SQRT } tfunc;
	bool truecount(size_t c) const {
		return c >= 1 && c <= 20;
	}

	double calc(double x) const {
		switch (tfunc) {
		case SIN: return std::sin(x);
		case COS: return std::cos(x);
		case EXP: return std::exp(x);
		case LOG: return std::log(x);
		case SQRT: return std::sqrt(x);
		default: return 0.0;
		}
	}
public:
	Tab(size_t c = 5, double a1 = 10.0, double b1 = 20.0) {
		if (truecount(c)) {
			count = c;
		}
		else {
			count = 5;
		}
		res = new double[count];
		points = new double[count];
		wastabulation = false;
		tfunc = SIN;
		if (a1 < b1) {
			a = a1;
			b = b1;
		}
		else {
			b = a1;
			a = b1;
		}

	}

	Tab(const Tab& other) : count(other.count), a(other.a), b(other.b), tfunc(other.tfunc) {
		res = new double[count];
		points = new double[count];
		for (size_t i = 0; i < count; ++i) {
			res[i] = other.res[i];
			points[i] = other.points[i];
		}
	}

	Tab& operator=(const Tab& other) {
		if (this != &other) {
			delete[] res;
			delete[] points;

			count = other.count;
			res = new double[count];
			points = new double[count];
			a = other.a;
			b = other.b;
			tfunc = other.tfunc;
			for (size_t i = 0; i < count; ++i) {
				res[i] = other.res[i];
				points[i] = other.points[i];
			}
		}
		return *this;
	}

	void set_func() {
		int ch;
		do {
			cout << "\n--- Select function ---\n";
			cout << "1. sin(x)\n";
			cout << "2. cos(x)\n";
			cout << "3. exp(x)\n";
			cout << "4. log(x) (natural logarithm)\n";
			cout << "5. sqrt(x)\n";

			cin >> ch;
			if (cin.fail()) {
				cin.clear();
				cin.ignore(100000, '\n');
			}

			switch (ch) {
			case 1:
				tfunc = SIN;
				break;
			case 2:
				tfunc = COS;
				break;
			case 3:
				tfunc = EXP;
				break;
			case 4:
				tfunc = LOG;
				break;
			case 5:
				tfunc = SQRT;
				break;
			default:
				cout << "Invalid choice.\n";
				break;
			}
		} while (ch < 1 || ch > 5);
		cout << "The operation is completed\n";
	}

	void set_count() {
		cout << "Enter the number of tabulation points: " << '\n';
		size_t c;
		do {
			cin >> c;
			if (cin.fail()) {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Input error! Enter a number (1-20): ";
			}
		} while (!truecount(c));
		count = c;
		double* npoints = new double[c];
		double* nres = new double[c];

		delete[] points;
		delete[] res;

		points = npoints;
		res = nres;
		cout << "The operation is completed\n";
	}

	void print_count() const {
		cout << "Current number of points: " << count << '\n';
	}

	void set_otr() {
		double a1, b1;
		cout << "Enter start of tabulation: " << '\n';
		cin >> a1;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(10000, '\n');
			cout << "Input error.\n";
			return;
		}

		cout << "Enter end of tabulation: " << '\n';
		cin >> b1;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(10000, '\n');
			cout << "Input error.\n";
			return;
		}

		if (a1 < b1) {
			a = a1;
			b = b1;
		}
		else {
			b = a1;
			a = b1;
		}
		cout << "The operation is completed\n";
	}

	void print_otr() const {
		cout << "Start of tabulation: " << a << '\n';
		cout << "End of tabulation: " << b << '\n';
	}

	void tabulation() {
		bool d = false;
		double st;
		if (count != 1) {
			st = (b - a) / (count - 1);
		}
		else {
			st = 0;
		}
		double cur = a;
		for (size_t i = 0; i < count; ++i) {
			if (tfunc == LOG && cur <= 0) {
				cout << "Logarithm of non-positive number at x = " << cur << '\n';
				d = true;
				break;
			}
			if (tfunc == SQRT && cur < 0) {
				cout << "Square root of negative number at x = " << cur << '\n';
				d = true;
				break;
			}
			res[i] = calc(cur);
			points[i] = cur;
			cur += st;
		}
		if (!d) {
			cout << "Tabulation is done" << '\n';
			wastabulation = true;
		}
		else {
			cout << "Tabulation is failed" << '\n';
		}

	}

	void print_tabulation() const {
		if (res == nullptr || !wastabulation) {
			cout << "There are no results" << '\n';
		}
		else {
			cout << "------------Results------------" << '\n';
			cout << "x              |          f(x) " << '\n';
			cout << "-------------------------------" << '\n';

			for (size_t i = 0; i < count; ++i) {
				cout.precision(6);
				cout << std::fixed;
				cout.width(15); cout << std::left << points[i] << " | ";
				cout.width(15); cout << std::left << res[i] << '\n';
			}
			cout << "-------------------------------" << '\n';
		}
	}

	void savetofile(const std::string& name, bool append = true) {
		if (res == nullptr || points == nullptr || !wastabulation) {
			cout << "No results to save.\n";
			return;
		}
		std::ofstream file;
		if (append) {
			file.open(name, std::ios::app);
		}
		else {
			file.open(name);
		}

		if (file.is_open()) {
			if (append) {
				file << "\n========================================\n";
				file << "New tabulation results:\n";
				file << "========================================\n";
			}

			file << "------------Results------------\n";
			file << "x              |          f(x) \n";
			file << "-------------------------------\n";

			for (size_t i = 0; i < count; ++i) {
				file.precision(6);
				file << std::fixed;
				file.width(15); file << std::left << points[i] << " | ";
				file.width(15); file << std::left << res[i] << '\n';
			}
			file << "-------------------------------\n";
			file.close();
			if (append) {
				cout << "Results appended to " << name << '\n';
			}
			else {
				cout << "Results saved to " << name << "\n";
			}
		}
		else {
			cout << "File opening error" << '\n';
		}
	}

	~Tab() {
		delete[] res;
		delete[] points;
	}
};


int main() {
	size_t s;
	Tab t;
	do {
		cout << "---Function tabulator---" << '\n';
		cout << "1. set the current function" << '\n';
		cout << "2. set the current number of tabulation points" << '\n';
		cout << "3. find out the current number of tabulation points" << '\n';
		cout << "4. set a tabulation segment" << '\n';
		cout << "5. find out the tabulation segment" << '\n';
		cout << "6. perform tabulation of the function" << '\n';
		cout << "7. output tabulation results" << '\n';
		cout << "8. save tabulation results to a file" << '\n';
		cout << "0. exit" << '\n';

		cin >> s;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(10000, '\n');
			cout << "Invalid input. Please enter a number.\n";
			continue;
		}

		switch (s) {
		case 1:
			t.set_func();
			break;
		case 2:
			t.set_count();
			break;
		case 3:
			t.print_count();
			break;
		case 4:
			t.set_otr();
			break;
		case 5:
			t.print_otr();
			break;
		case 6:
			t.tabulation();
			break;
		case 7:
			t.print_tabulation();
			break;
		case 8:
			cout << "Choose whether you want to rewrite(0) or add (1) to the file." << '\n';
			size_t k;
			do {
				cin >> k;
				if (cin.fail()) {
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Invalid input. Please enter a number.\n";
					continue;
				}
			} while (k != 1 && k != 0);
			if (k == 0) {
				t.savetofile("Tab.txt", false);
			}
			else {
				t.savetofile("Tab.txt", true);
			}
			break;
		case 0:
			break;
		default:
			cout << "Invalid choice. Try again.\n";
			break;
		}
		cout << '\n';
	} while (s != 0);

	return 0;
}