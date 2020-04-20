#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <cctype>
using namespace std;

//Registration class
class Registration {
private:
	int id_acc, role;
	string username, password;
public:
	//class's default constructor
	Registration() {
		id_acc = 1;
		username = "";
		password = "";
		role = 0;
	}

	vector<Registration> accounts; //vector that contains data about all accounts
	vector<int> accounts_id, accounts_role; //vectors that contain data about IDs and roles of accounts  
	vector<string> accounts_us, accounts_pass; //vectors that contain data about usernames and passwords of accounts

	// overloading operator << 
	friend ostream& operator << (ostream& outf, const Registration& acc) {
		return outf << acc.id_acc << "\t" << acc.username << "\t" << acc.password << "\t" << acc.role << '\n';  // write one string per line, end with a blank line
	}

	//function that handles input errors with displaying of message
	void err_handler() {
		cout << "Error! Follow the manual!" << endl;
		cin.clear();
		cin.ignore(256, '\n');
	}

	//function that constructs table form 
	void table(int chc) {
		if (chc == 1) {
			cout << "--------" << "+---------------" << "+---------" << "------" << "+----" << endl;
			cout << "ID" << "\t|" << "Username" << "\t|" << "Password" << "\t|" << "Role" << endl;
			cout << "--------" << "+---------------" << "+---------" << "------" << "+----" << endl;
		}
		else if (chc == 2) {
			cout << "--------" << "+---------------" << "+---------" << "------" << "+----" << endl;
		}
	}

	//function that handle sign in menu
	int sign_in(int chc) {

		accounts_id.clear();
		accounts_us.clear();
		accounts_pass.clear();
		accounts_role.clear();

		Registration acc;

		int temp_ind = 0;
		int role = 0;
		int id = 0;
		string us_n;
		string pass;

		ifstream file("users.txt");
		if (file.is_open()) {
			while (file >> acc.id_acc >> acc.username >> acc.password >> acc.role) {
				accounts_id.push_back(acc.id_acc);
				accounts_us.push_back(acc.username);
				accounts_pass.push_back(acc.password);
				accounts_role.push_back(acc.role);
			}
		}
		else {
			cout << "Unable to open a file..." << endl;
		}

		cout << "Username: ";
		cin >> us_n;
		while (!count(accounts_us.begin(), accounts_us.end(), us_n)){
			cout << "There isn't similar username in database..." << endl;
			cout << "Enter again: ";
			cin >> us_n;
		}
		cout << "Password: ";
		cin >> pass;

		for (int i = 0; i < accounts_us.size(); i++) {
			if (us_n == accounts_us[i]) {
				temp_ind = i;
			}
		}

		if (us_n == accounts_us[temp_ind] && pass == accounts_pass[temp_ind]) {
			role = accounts_role[temp_ind];
			id = accounts_id[temp_ind];
		}
		else {
			cout << "Your input is incorrect!" << endl;
			cout << "Try again..." << endl;
			sign_in(chc);
		}

		if (chc == 0) {
			return role;
		}
		else if (chc == 1) {
			return id;
		}
	}

	//function that executes the adding of account
	void add_acc(Registration acc) {

		accounts.clear();
		accounts_id.clear();
		accounts_us.clear();

		ifstream file("users.txt");
		if (file.is_open()) {
			while (file >> acc.id_acc >> acc.username >> acc.password >> acc.role) {
				accounts_id.push_back(acc.id_acc);
				accounts_us.push_back(acc.username);
			}
		}

		for (int i = 0; i < accounts_id.size(); i++) {
			if (accounts_id[i] == accounts_id[accounts_id.size() - 1]) {
				acc.id_acc = accounts_id[i] + 1;
			}
		}

		cout << "Username: ";
		cin >> acc.username;
		while (count(accounts_us.begin(), accounts_us.end(), acc.username)) {
			cout << "Similar USERNAME already exists..." << endl;
			cout << "Username: ";
			cin >> acc.username;
		}

		cout << "Password: ";
		cin >> acc.password;

		cout << "App role (admin - 1 / user - 0): ";
		cin >> acc.role;

		while (cin.fail() || !(role == 0 || role == 1)) {
			err_handler();
			cout << "App role (admin - 1 / user - 0): ";
			cin >> acc.role;
		}

		accounts.push_back(acc);

		ofstream file_o;
		file_o.open("users.txt", ios_base::app);
		for (const Registration& acc : accounts) {
			file_o << acc;
		}

		cout << '\n';
		file_o.close();
	}

	//function that executes the deleting of account
	void delete_acc(Registration acc) {

		accounts.clear();
		accounts_id.clear();
		accounts_us.clear();

		ifstream file("users.txt");
		if (file.is_open()) {
			Registration acc;
			while (file >> acc.id_acc >> acc.username >> acc.password >> acc.role) {
				accounts_id.push_back(acc.id_acc);
				accounts_us.push_back(acc.username);
			}
		}
		else {
			cout << "Enable to open a file..." << endl;
		}

		string username_s;
		int temp_ind = 0;
		char chc;

		cout << "Enter the username of account: ";
		cin >> username_s;
		while (!(count(accounts_us.begin(), accounts_us.end(), username_s))) {
			cout << "There isn't person with similar last name..." << endl;
			cout << "Enter again: ";
			cin >> username_s;
		}

		for (int i = 0; i < accounts_us.size(); i++) {
			if (username_s == accounts_us[i]) {
				temp_ind = i;
			}
		}

		cout << "Please, confirm: " << endl;

		if (sign_in(1) == accounts_id[temp_ind]) {
			cout << "You can't delete yourself!" << endl;
			delete_acc(acc);
		}

		cout << '\n';

		if (username_s == accounts_us[temp_ind]) {
			cout << "Do you really want to delete this account? (yes-'y' / no - any letter): ";
			cin >> chc;
			if (chc == 'y') {
				ifstream file("users.txt");
				if (file.is_open()) {
					while (file >> acc.id_acc >> acc.username >> acc.password >> acc.role) {
						accounts.push_back(acc);
						if (username_s == acc.username) {
							cout << '\n';
							cout << "Account's data: " << endl;
							cout << "ID: " << acc.id_acc << endl;
							cout << "Username: " << acc.username << endl;
							cout << "Password: " << acc.password << endl;
							cout << "Role: " << acc.role << endl;
							accounts.pop_back();
						}
						remove("users.txt");
					}
					ofstream n_file;
					n_file.open("users.txt");
					for (const auto& acc : accounts) {
						n_file << acc;
					}
					n_file.close();

					cout << '\n';
					cout << "Account was successfully deleted" << endl;
				}
				else {
					cout << "Unable to open a file..." << endl;
				}
			}
			else {
				_getch;
				cin.ignore(256, '\n');
			}
		}
		else {
			cout << "It isn't account's username..." << endl;
			cout << "Try again..." << endl;
			cout << '\n';
			delete_acc(acc);
		}
	}

	//function for viewing of all accounts
	void view_acc(Registration acc) {
		accounts.clear();
		ifstream file("users.txt");
		if (file.is_open()) {
			while (file >> acc.id_acc >> acc.username >> acc.password >> acc.role) {
				accounts.push_back(acc);
			}
			table(1);
			for (const auto& acc : accounts) {
				cout << acc.id_acc << "\t|" << acc.username << "\t\t|" << acc.password << "\t\t|" << acc.role << '\n';;
				table(2);
			}
		}
		else {
			cout << "Unable to open a file..." << endl;
		}
		file.close();
	}
};

//Admin class
class Admin : public Registration {
private:
	string lst_name, fst_name, mdl_name;
	int id, tab_num, year, month, qnt_work_h, m_per_h;
	float salary;
public:

	//class's default constructor
	Admin() {
		id = 1;
		lst_name = "";
		fst_name = "";
		mdl_name = "";
		tab_num = 0;
		year = 0;
		month = 0;
		qnt_work_h = 0;
		m_per_h = 0;
		salary = 0.0;
	}

	//operator overloading for output
	friend ostream& operator << (ostream& outf, const Admin& empl) {
		return outf << empl.id << '\t' << empl.lst_name << '\t' << empl.fst_name << '\t' << empl.mdl_name << '\t' << empl.tab_num << '\t'
			<< empl.year << '\t' << empl.month << '\t' << empl.qnt_work_h << '\t' << empl.m_per_h << '\t' << empl.salary << '\n';  // write one string per line, end with a blank line
	}

	vector<Admin> employees; //vector that contains data about employees
	vector<Admin> employees_list;
	vector<int> employees_id; //vector that contains data about employees' ID
	vector<int> employees_tb_num; //vector that contains data about employees' table number
	vector<float> employees_slr; //vector that contains data about employees' salary
	vector<string> employees_lst_name; //vector that contains data about employees' last name
	vector<string> employees_fst_name; //vector that contains data about employees' first name

	//function that handles input errors with displaying of message
	void err_handler() {
		cout << "Error! Follow the manual!" << endl;
		cin.clear();
		cin.ignore(256, '\n');
	}

	//function that handles input errors without displaying of message
	void ignore_func() {
		cin.clear();
		cin.ignore(256, '\n');
	}

	//function that checks whether the user want to back to the menu
	void menu_back_frame(char chc) {
		if (chc != 'y') {
			_getch;
			cout << '\n';
		}
		ignore_func();
		cout << '\n';
	}

	//function that constructs table form 
	void table(int chc) {
		if (chc == 1) {
			cout << "--------+" << "---------------" << "+---------------" << "+---------------" << "+---------------" <<
				"+-------" << "+-------" << "+-------" << "+-------" << "+------------|" << endl;
			cout << "ID" << "\t|" << "Last-N" << "\t\t|" << "First-N" << "\t|" << "Mid-N" << "\t\t|" << "Table-N" <<
				"\t|" << "Year" << "\t|" << "Month" << "\t|" << "Q-W-H" << "\t|" << "M-P-H" << "\t|" << "Salary" << "\n";
			cout << "--------+" << "---------------" << "+---------------" << "+---------------" << "+---------------" <<
				"+-------" << "+-------" << "+-------" << "+-------" << "+------------|" << endl;
		}
		else if (chc == 2) {
			cout << "--------+" << "---------------" << "+---------------" << "+---------------" << "+---------------" <<
				"+-------" << "+-------" << "+-------" << "+-------" << "+------------|" << endl;
		}
	}

	//function that executes calculation of salary
	int salary_calc(int time, int money) {
		int salary_start = time * money;
		float salary = (salary_start * 88) / 100;

		return salary;
	}

	//function that executes the adding of data about employees
	void add_data(vector<Admin> employees, vector<int> employees_id, vector<int> employees_tb_num, Admin empl) {

		employees.clear();
		employees_id.clear();
		employees_tb_num.clear();

		ifstream file_i("data.txt");
		if (file_i.is_open()) {
			while (file_i >> empl.id >> empl.lst_name >> empl.fst_name >> empl.mdl_name >>
				empl.tab_num >> empl.year >> empl.month >> empl.qnt_work_h >> empl.m_per_h >> empl.salary) {
				employees_id.push_back(empl.id);
				employees_tb_num.push_back(empl.tab_num);
			}
		}
		else {
			cout << "Unable to open a file..." << endl;
		}
		file_i.close();

		for (int i = 0; i < employees_id.size(); i++) {
			if (employees_id[i] == employees_id[employees_id.size() - 1]) {
				empl.id = employees_id[i] + 1;
			}
		}

		cout << "Enter the last name: ";
		cin >> empl.lst_name;

		cout << "Enter the first name: ";
		cin >> empl.fst_name;

		cout << "Enter the middle name: ";
		cin >> empl.mdl_name;

		cout << "Enter the month of employee's birthday (number): ";
		cin >> empl.month;
		while (cin.fail() || empl.month < 1 || empl.month > 13) {
			err_handler();
			cout << "Enter again: ";
			cin >> empl.month;
		}

		cout << "Enter the year of employee's birthday: ";
		cin >> empl.year;
		while (cin.fail() || empl.year < 1 || empl.month > 2021) {
			err_handler();
			cout << "Enter again: ";
			cin >> empl.year;
		}

		cout << "Enter the table number: ";
		cin >> empl.tab_num;
		while (cin.fail()) {
			err_handler();
			cout << "Enter again: ";
			cin >> empl.tab_num;
		}
		while (count(employees_tb_num.begin(), employees_tb_num.end(), empl.tab_num)) {
			cout << "Similar table number is already used..." << endl;
			cout << "Enter again: ";
			cin >> empl.tab_num;
		}

		cout << "Enter the quantity of work hours: ";
		cin >> empl.qnt_work_h;
		while (cin.fail() || empl.qnt_work_h < 1) {
			err_handler();
			cout << "Enter again: ";
			cin >> empl.qnt_work_h;
		}

		cout << "Enter the amount of money he/she gets per hour: ";
		cin >> empl.m_per_h;
		while (cin.fail() || empl.m_per_h < 1) {
			err_handler();
			cout << "Enter again: ";
			cin >> empl.m_per_h;
		}

		if (empl.qnt_work_h >= 144) {
			empl.salary = salary_calc(empl.qnt_work_h, empl.m_per_h) * 2;
		}
		else {
			empl.salary = salary_calc(empl.qnt_work_h, empl.m_per_h);
		}

		employees.push_back(empl);

		ofstream file;
		file.open("data.txt", ios_base::app);
		file << empl;
		if (true) {
			cout << "Data were successfully added!" << endl;
		}
		else {
			cout << "Data weren't added..." << endl;
		}
		file.close();

		cout << '\n';
	}

	//function that executes the editting of data about employees
	void edit_data(vector<Admin> employees, vector<int> employees_id, vector<int> employees_tb_num, vector<string> employees_lst_name, Admin empl) {

		employees.clear();
		employees_lst_name.clear();
		employees_tb_num.clear();

		ifstream file_i("data.txt");
		if (file_i.is_open()) {
			while (file_i >> empl.id >> empl.lst_name >> empl.fst_name >> empl.mdl_name >>
				empl.tab_num >> empl.year >> empl.month >> empl.qnt_work_h >> empl.m_per_h >> empl.salary) {
				employees_lst_name.push_back(empl.lst_name);
				employees_tb_num.push_back(empl.tab_num);
			}
		}
		else {
			cout << "Unable to open a file..." << endl;
		}
		file_i.close();

		string lst_name_s;
		int tb_n_s;
		int temp_ind = 0;

		cout << "Enter the last name of employee: ";
		cin >> lst_name_s;
		while (!(count(employees_lst_name.begin(), employees_lst_name.end(), lst_name_s))) {
			cout << "There isn't person with similar last name..." << endl;
			cout << "Enter again: ";
			cin >> lst_name_s;
		}

		cin.ignore(256, '\n');

		cout << "Enter the table number of employee: ";
		cin >> tb_n_s;
		while (cin.fail()) {
			err_handler();
			cout << "Enter again: ";
			cin >> tb_n_s;
		}

		for (int i = 0; i < employees_lst_name.size(); i++) {
			if (lst_name_s == employees_lst_name[i] && tb_n_s == employees_tb_num[i]) {
				temp_ind = i;
			}
		}

		cout << '\n';

		if (lst_name_s == employees_lst_name[temp_ind] && tb_n_s == employees_tb_num[temp_ind]) {
			ifstream file("data.txt");
			if (file.is_open()) {
				while (file >> empl.id >> empl.lst_name >> empl.fst_name >> empl.mdl_name >> empl.tab_num >>
					empl.year >> empl.month >> empl.qnt_work_h >> empl.m_per_h >> empl.salary) {
					employees.push_back(empl);
					if (lst_name_s == empl.lst_name && tb_n_s == empl.tab_num) {
						cout << '\n';
						cout << "Old data: " << endl;
						cout << "ID: " << empl.id << endl;
						cout << "Last name: " << empl.lst_name << endl;
						cout << "First name: " << empl.fst_name << endl;
						cout << "Middle name: " << empl.mdl_name << endl;
						cout << "Month of employee's birthday: " << empl.month << endl;
						cout << "Year of employee's birthday: " << empl.year << endl;
						cout << "Table number: " << empl.tab_num << endl;
						cout << "Quantity of work hours: " << empl.qnt_work_h << endl;
						cout << "Amount of money he/she gets per hour: " << empl.m_per_h << endl;
						cout << "Salary: " << empl.salary << endl;

						int temp_id = empl.id;
						int temp_tbn = empl.tab_num;

						employees.pop_back();

						cout << '\n';
						cout << "New data:" << endl;

						empl.id = temp_id;

						cout << "Enter the last name: ";
						cin >> empl.lst_name;

						cout << "Enter the first name: ";
						cin >> empl.fst_name;

						cout << "Enter the middle name: ";
						cin >> empl.mdl_name;

						cout << "Enter the month of employee's birthday (number): ";
						cin >> empl.month;
						while (cin.fail() || empl.month < 1 || empl.month > 13) {
							err_handler();
							cout << "Enter again: ";
							cin >> empl.month;
						}

						cout << "Enter the year of employee's birthday: ";
						cin >> empl.year;
						while (cin.fail() || empl.year < 1 || empl.month > 2021) {
							err_handler();
							cout << "Enter again: ";
							cin >> empl.year;
						}

						empl.tab_num = temp_tbn;

						cout << "Enter the quantity of work hours: ";
						cin >> empl.qnt_work_h;
						while (cin.fail() || empl.qnt_work_h < 1) {
							err_handler();
							cout << "Enter again: ";
							cin >> empl.qnt_work_h;
						}

						cout << "Enter the amount of money he/she gets per hour: ";
						cin >> empl.m_per_h;
						while (cin.fail() || empl.m_per_h < 1) {
							err_handler();
							cout << "Enter again: ";
							cin >> empl.m_per_h;
						}

						if (empl.qnt_work_h >= 144) {
							empl.salary = salary_calc(empl.qnt_work_h, empl.m_per_h) * 2;
						}
						else {
							empl.salary = salary_calc(empl.qnt_work_h, empl.m_per_h);
						}

						employees.push_back(empl);
					}
					remove("data.txt");
					ofstream n_file;
					n_file.open("data.txt");
					for (const auto& empl : employees) {
						n_file << empl;
					}
					cout << '\n';
					cout << "Data were successfully edited!" << endl;
					n_file.close();
				}
			}
			else {
				cout << "Doesn't enable to open a file..." << endl;
			}
		}
		else {
			cout << "It isn't employee's table number..." << endl;
			cout << "Try again..." << endl;
			cout << '\n';
			edit_data(employees, employees_id, employees_tb_num, employees_lst_name, empl);
		}
	}

	//function that executes the deleting of data about employees
	void delete_data(vector<Admin>& employees, vector<int>& employees_id, vector<int>& employees_tb_num, vector<string>& employees_lst_name, Admin empl) {

		employees.clear();
		employees_lst_name.clear();
		employees_tb_num.clear();

		ifstream file_i("data.txt");
		if (file_i.is_open()) {
			while (file_i >> empl.id >> empl.lst_name >> empl.fst_name >> empl.mdl_name >>
				empl.tab_num >> empl.year >> empl.month >> empl.qnt_work_h >> empl.m_per_h >> empl.salary) {
				employees_lst_name.push_back(empl.lst_name);
				employees_tb_num.push_back(empl.tab_num);
			}
		}
		else {
			cout << "Unable to open a file..." << endl;
		}
		file_i.close();

		string lst_name_s;
		int tb_n_s;
		int temp_ind = 0;
		char chc;

		cout << "Enter the last name of employee: ";
		cin >> lst_name_s;
		while (!(count(employees_lst_name.begin(), employees_lst_name.end(), lst_name_s))) {
			cout << "There isn't person with similar last name..." << endl;
			cout << "Enter again: ";
			cin >> lst_name_s;
		}

		cin.ignore(256, '\n');

		cout << "Enter the table number of employee: ";
		cin >> tb_n_s;
		while (cin.fail()) {
			err_handler();
			cout << "Enter again: ";
			cin >> tb_n_s;
		}

		for (int i = 0; i < employees_lst_name.size(); i++) {
			if (lst_name_s == employees_lst_name[i] && tb_n_s == employees_tb_num[i]) {
				temp_ind = i;
			}
		}
		cout << '\n';

		if (lst_name_s == employees_lst_name[temp_ind] && tb_n_s == employees_tb_num[temp_ind]) {
			cout << "Do you really want to delete data about employee? (yes-'y' / no - any letter): ";
			cin >> chc;
			if (chc == 'y') {
				ifstream file("data.txt");
				if (file.is_open()) {
					while (file >> empl.id >> empl.lst_name >> empl.fst_name >> empl.mdl_name >> empl.tab_num >>
						empl.year >> empl.month >> empl.qnt_work_h >> empl.m_per_h >> empl.salary) {
						employees.push_back(empl);
						if (lst_name_s == empl.lst_name && tb_n_s == empl.tab_num) {
							cout << '\n';
							cout << "Employee's data: " << endl;
							cout << "ID: " << empl.id << endl;
							cout << "Last name: " << empl.lst_name << endl;
							cout << "First name: " << empl.fst_name << endl;
							cout << "Middle name: " << empl.mdl_name << endl;
							cout << "Month of employee's birthday: " << empl.month << endl;
							cout << "Year of employee's birthday: " << empl.year << endl;
							cout << "Table number: " << empl.tab_num << endl;
							cout << "Quantity of work hours: " << empl.qnt_work_h << endl;
							cout << "Amount of money he/she gets per hour: " << empl.m_per_h << endl;
							cout << "Salary: " << empl.salary << endl;
							employees.pop_back();
						}
						remove("data.txt");
					}
					ofstream n_file;
					n_file.open("data.txt");
					for (const auto& empl : employees) {
						n_file << empl;
					}
					n_file.close();
					cout << '\n';
					cout << "Data were successfully deleted" << endl;
				}
				else {
					cout << "Unable to open a file..." << endl;
				}
			}
			else {
				_getch;
				cin.ignore(256, '\n');
			}
		}
		else {
			cout << "It isn't employee's table number..." << endl;
			cout << "Try again..." << endl;
			cout << '\n';
			delete_data(employees, employees_id, employees_tb_num, employees_lst_name, empl);
		}
	}

	//function that executes the sorting by last name, table num and salary
	void sort_data(vector<Admin>& employees, vector<string>& employees_lst_name, vector<int>& employees_tb_num, vector<float>& employees_slr, Admin empl) {
		int chc;
		string temp;

		employees.clear();
		employees_lst_name.clear();
		employees_tb_num.clear();
		employees_slr.clear();

		ifstream file_i("data.txt");
		if (file_i.is_open()) {
			while (file_i >> empl.id >> empl.lst_name >> empl.fst_name >> empl.mdl_name >>
				empl.tab_num >> empl.year >> empl.month >> empl.qnt_work_h >> empl.m_per_h >> empl.salary) {
				employees.push_back(empl);
				employees_lst_name.push_back(empl.lst_name);
				employees_tb_num.push_back(empl.tab_num);
				employees_slr.push_back(empl.salary);
			}
		}
		else {
			cout << "Unable to open a file..." << endl;
		}
		file_i.close();

		cout << "Which way you want to sort data?" << endl;
		cout << "1 - By a last name" << endl;
		cout << "2 - By a table number" << endl;
		cout << "3 - By salary" << endl;
		cout << "Your choice: ";
		cin >> chc;
		while (cin.fail() || chc < 1 || chc > 3) {
			err_handler();
			cout << "Your choice: ";
			cin >> chc;
		}

		switch (chc) {
		case 1:
			sort(employees_lst_name.begin(), employees_lst_name.end());
			table(1);
			for (int i = 0; i < employees.size(); i++) {
				for (const auto& empl : employees) {
					if (employees_lst_name[i] == empl.lst_name) {
						cout << empl;
						table(2);
					}
				}
			}
			break;
		case 2:
			sort(employees_tb_num.begin(), employees_tb_num.end());
			table(1);
			for (int i = 0; i < employees.size(); i++) {
				for (const auto& empl : employees) {
					if (employees_tb_num[i] == empl.tab_num) {
						cout << empl;
						table(2);
					}
				}
			}
			break;
		case 3:
			sort(employees_slr.begin(), employees_slr.end());
			table(1);
			for (int i = (employees.size() - 1); i >= 0; i--) {
				for (const auto& empl : employees) {
					if (employees_slr[i] == empl.salary) {
						cout << empl;
						table(2);
					}
				}
			}
			break;
		}

	}

	//function that executes the searching by last name, first name and table number
	void search_data(vector<Admin> employees, vector<int> employees_id, vector<string> employees_fst_name, vector<string> employees_lst_name, Admin empl) {

		int chc;
		string lst_name_s, fst_name_s, s_s;
		int i_s = 0;
		int tb_n_s;


		employees_lst_name.clear();
		employees_fst_name.clear();
		employees_id.clear();

		ifstream file_i("data.txt");
		if (file_i.is_open()) {
			while (file_i >> empl.id >> empl.lst_name >> empl.fst_name >> empl.mdl_name >>
				empl.tab_num >> empl.year >> empl.month >> empl.qnt_work_h >> empl.m_per_h >> empl.salary) {
				employees_lst_name.push_back(empl.lst_name);
				employees_fst_name.push_back(empl.fst_name);
				employees_id.push_back(empl.id);
			}
		}
		else {
			cout << "Unable to open a file..." << endl;
		}
		file_i.close();

		cout << "Which method do you want to use for searching data:\n";
		cout << "1 - By last name;" << endl;
		cout << "2 - By first name;" << endl;
		cout << "3 - By table number;" << endl;
		cout << "Enter only integer!" << endl;
		cout << "Your choice: ";
		cin >> chc;
		while (cin.fail() || chc < 1 || chc > 3) {
			err_handler();
			cout << "Your choice: ";
			cin >> chc;
		}

		ifstream file("data.txt");
		switch (chc) {
		case 1:
			cout << "Enter the last name of person: ";
			cin >> lst_name_s;
			while (!(count(employees_lst_name.begin(), employees_lst_name.end(), lst_name_s))) {
				cout << "There isn't person with similar last name..." << endl;
				cout << "Enter again: ";
				cin >> lst_name_s;
			}
			if (file.is_open()) {
				while (file >> empl.id >> empl.lst_name >> empl.fst_name >> empl.mdl_name >> empl.tab_num >>
					empl.year >> empl.month >> empl.qnt_work_h >> empl.m_per_h >> empl.salary) {
					employees.push_back(empl);
					if (lst_name_s == empl.lst_name) {
						s_s = empl.lst_name;
						cout << empl;
					}
				}
				if (lst_name_s != s_s) {
					cout << "There isn't '" << lst_name_s << "' last name" << endl;
				}
			}
			else {
				cout << "Unable to open a file..." << endl;
			}
			break;
		case 2:
			cout << "Enter the first name of person: ";
			cin >> fst_name_s;
			while (!(count(employees_fst_name.begin(), employees_fst_name.end(), fst_name_s))) {
				cout << "There isn't person with similar first name..." << endl;
				cout << "Enter again: ";
				cin >> fst_name_s;
			}
			if (file.is_open()) {
				while (file >> empl.id >> empl.lst_name >> empl.fst_name >> empl.mdl_name >> empl.tab_num >>
					empl.year >> empl.month >> empl.qnt_work_h >> empl.m_per_h >> empl.salary) {
					employees.push_back(empl);
					if (fst_name_s == empl.fst_name) {
						s_s = empl.fst_name;
						cout << empl;
					}
				}
				if (fst_name_s != s_s) {
					cout << "There isn't '" << fst_name_s << "' first name" << endl;
				}
			}
			else {
				cout << "Unable to open a file..." << endl;
			}
			break;
		case 3:
			cout << "Enter the table number of person: ";
			cin >> tb_n_s;
			while (cin.fail()) {
				err_handler();
				cout << "Enter the table number of person: ";
				cin >> tb_n_s;
			}
			while (!(count(employees_tb_num.begin(), employees_tb_num.end(), tb_n_s))) {
				ignore_func();
				cout << "There isn't person with similar table number..." << endl;
				cout << "Enter again: ";
				cin >> tb_n_s;
			}
			if (file.is_open()) {
				while (file >> empl.id >> empl.lst_name >> empl.fst_name >> empl.mdl_name >> empl.tab_num >>
					empl.year >> empl.month >> empl.qnt_work_h >> empl.m_per_h >> empl.salary) {
					employees.push_back(empl);
					if (tb_n_s == empl.tab_num) {
						i_s = empl.tab_num;
						cout << empl;
					}
				}
				if (tb_n_s != i_s) {
					cout << "There isn't '" << tb_n_s << "' last name" << endl;
				}
			}
			else {
				cout << "Unable to open a file..." << endl;
			}
			break;
		}
		file.close();
	}

	//function that defines how much a particular employee has earned
	void define_money(vector<int> employees_id, vector<string> employees_fst_name, vector<string> employees_lst_name, vector<int> employees_tb_num, Admin empl) {

		string lst_name_s;
		int tb_n_s, temp_ind = 0;
		float salary_vv;

		employees_lst_name.clear();
		employees_fst_name.clear();
		employees_id.clear();
		employees_tb_num.clear();

		ifstream file_i("data.txt");
		if (file_i.is_open()) {
			while (file_i >> empl.id >> empl.lst_name >> empl.fst_name >> empl.mdl_name >>
				empl.tab_num >> empl.year >> empl.month >> empl.qnt_work_h >> empl.m_per_h >> empl.salary) {
				employees_lst_name.push_back(empl.lst_name);
				employees_fst_name.push_back(empl.fst_name);
				employees_id.push_back(empl.id);
				employees_tb_num.push_back(empl.tab_num);
			}
		}
		else {
			cout << "Unable to open a file..." << endl;
		}
		file_i.close();

		cout << "Enter the last name of employee: ";
		cin >> lst_name_s;
		while (!(count(employees_lst_name.begin(), employees_lst_name.end(), lst_name_s))) {
			cout << '\n';
			cout << "There isn't person with similar last name..." << endl;
			cout << "Enter again: ";
			cin >> lst_name_s;
		}

		ignore_func();

		cout << "Enter the table number of employee: ";
		cin >> tb_n_s;
		while (cin.fail()) {
			cout << '\n';
			err_handler();
			cout << "Enter again: ";
			cin >> tb_n_s;
		}

		ignore_func();

		for (int i = 0; i < employees_lst_name.size(); i++) {
			if (lst_name_s == employees_lst_name[i] && tb_n_s == employees_tb_num[i]) {
				temp_ind = i;
			}
		}

		cout << '\n';

		if (lst_name_s == employees_lst_name[temp_ind] && tb_n_s == employees_tb_num[temp_ind]) {
			ifstream file("data.txt");
			if (file.is_open()) {
				while (file >> empl.id >> empl.lst_name >> empl.fst_name >> empl.mdl_name >> empl.tab_num >>
					empl.year >> empl.month >> empl.qnt_work_h >> empl.m_per_h >> empl.salary) {
					if (lst_name_s == empl.lst_name && tb_n_s == empl.tab_num) {
						cout << "\n";
						cout << "Enter the period of time (in hours): ";
						int period_tm;
						cin >> period_tm;

						cout << '\n';

						if (period_tm >= 144) {
							salary_vv = salary_calc(period_tm, empl.m_per_h) * 2;
							cout << "The salary " << empl.lst_name << ' ' << empl.fst_name << " would earn for " << period_tm << " hours is " << salary_vv << '$' << endl;
						}
						else {
							salary_vv = salary_calc(period_tm, empl.m_per_h);
							cout << "The salary " << empl.lst_name << ' ' << empl.fst_name << " would earn for " << period_tm << " hours is " << salary_vv << '$' << endl;
						}
						break;
					}
				}
			}
			else {
				cout << "Unable to open a file..." << endl;
			}
		}
		else {
			cout << "It isn't employee's table number..." << endl;
			cout << "Try again..." << endl;
			cout << '\n';
			define_money(employees_id, employees_fst_name, employees_lst_name, employees_tb_num, empl);
		}

	}

	//function for viewing of all data
	void view_data(vector<Admin> employees, Admin empl) {

		cout << "\n";
		ifstream file("data.txt");
		if (file.is_open()) {
			table(1);
			while (file >> empl.id >> empl.lst_name >> empl.fst_name >> empl.mdl_name >>
				empl.tab_num >> empl.year >> empl.month >> empl.qnt_work_h >> empl.m_per_h >> empl.salary) {
				cout << empl.id << "\t|" << empl.lst_name << "\t\t|" << empl.fst_name << "\t\t|" << empl.mdl_name << "\t\t|" << empl.tab_num << "\t\t|"
					<< empl.year << "\t|" << empl.month << "\t|" << empl.qnt_work_h << "\t|" << empl.m_per_h << "\t|" << empl.salary << '\n';
				table(2);
			}
		}
		else {
			cout << "Unable to open a file..." << endl;
		}
	}

	//function that handle admin menu
	void admin_menu() {
		Admin empl;
		Registration acc;

		int chc_i;
		char chc_c;

		ifstream file_i("data.txt");
		if (file_i.is_open()) {
			while (file_i >> empl.id >> empl.lst_name >> empl.fst_name >> empl.mdl_name >>
				empl.tab_num >> empl.year >> empl.month >> empl.qnt_work_h >> empl.m_per_h >> empl.salary) {
				employees.push_back(empl);
				employees_id.push_back(empl.id);
				employees_lst_name.push_back(empl.lst_name);
				employees_fst_name.push_back(empl.fst_name);
				employees_tb_num.push_back(empl.tab_num);
				employees_slr.push_back(empl.salary);
			}
		}
		file_i.close();

		do {
			cout << "\t\t" << "ADMIN MENU" << endl;
			cout << "1 - Add data" << endl;
			cout << "2 - Edit data" << endl;
			cout << "3 - Delete data" << endl;
			cout << "4 - Sort data" << endl;
			cout << "5 - Search employee" << endl;
			cout << "6 - Define employee's salary" << endl;
			cout << "7 - View data" << endl;
			cout << "8 - Add account" << endl;
			cout << "9 - Delete account" << endl;
			cout << "10 - View accounts" << endl;

			cout << '\n';

			cout << "Enter a number: ";
			cin >> chc_i;
			while (cin.fail() || chc_i < 1 || chc_i > 10) {
				err_handler();
				cout << "Enter again: ";
				cin >> chc_i;
			}
			switch (chc_i) {
			case 1:
				cout << '\n';
				cout << "\t\t" << "ADD DATA" << endl;
				do {
					add_data(employees, employees_id, employees_tb_num, empl);
					cout << '\n';
					cout << "Do you want to repeat this procedure?(yes - 'y', no - another letter): ";
					cin >> chc_c;
					menu_back_frame(chc_c);
				} while (chc_c == 'y');
				break;

			case 2:
				cout << '\n';
				cout << "\t\t" << "EDIT DATA" << endl;
				do {
					edit_data(employees, employees_id, employees_tb_num, employees_lst_name, empl);
					cout << '\n';
					cout << "Do you want to repeat this procedure?(yes - 'y', no - another letter): ";
					cin >> chc_c;
					menu_back_frame(chc_c);
				} while (chc_c == 'y');
				break;
			case 3:
				cout << '\n';
				cout << "\t\t" << "DELETE DATA" << endl;
				do {
					delete_data(employees, employees_id, employees_tb_num, employees_lst_name, empl);
					cout << '\n';
					cout << "Do you want to repeat this procedure?(yes - 'y', no - another letter): ";
					cin >> chc_c;
					menu_back_frame(chc_c);
				} while (chc_c == 'y');
				break;
			case 4:
				cout << '\n';
				cout << "\t\t" << "SORT DATA" << endl;
				do {
					sort_data(employees, employees_lst_name, employees_tb_num, employees_slr, empl);
					cout << '\n';
					cout << "Do you want to repeat this procedure?(yes - 'y', no - another letter): ";
					cin >> chc_c;
					menu_back_frame(chc_c);
				} while (chc_c == 'y');
				break;
			case 5:
				cout << '\n';
				cout << "\t\t" << "SEARCH EMPLOYEE" << endl;
				do {
					search_data(employees, employees_id, employees_fst_name, employees_lst_name, empl);
					cout << '\n';
					cout << "Do you want to repeat this procedure?(yes - 'y', no - another letter): ";
					cin >> chc_c;
					menu_back_frame(chc_c);
				} while (chc_c == 'y');
				break;
			case 6:
				cout << '\n';
				cout << "\t\t" << "DEFINE EMPLOYEE'S SALARY" << endl;
				do {
					define_money(employees_id, employees_fst_name, employees_lst_name, employees_tb_num, empl);
					cout << '\n';
					cout << "Do you want to repeat this procedure?(yes - 'y', no - another letter): ";
					cin >> chc_c;
					menu_back_frame(chc_c);
				} while (chc_c == 'y');
				break;
			case 7:
				cout << '\n';
				cout << "\t\t" << "VIEW DATA" << endl;
				do {
					view_data(employees, empl);
					cout << '\n';
					cout << "Do you want to repeat this procedure?(yes - 'y', no - another letter): ";
					cin >> chc_c;
					menu_back_frame(chc_c);
				} while (chc_c == 'y');
				break;

			case 8:
				cout << '\n';
				cout << "\t\t" << "ADD ACCOUNT" << endl;
				do {
					add_acc(acc);
					cout << '\n';
					cout << "Do you want to repeat this procedure?(yes - 'y', no - another letter): ";
					cin >> chc_c;
					menu_back_frame(chc_c);
				} while (chc_c == 'y');
				break;

			case 9:
				cout << '\n';
				cout << "\t\t" << "DELETE ACCOUNT" << endl;
				do {
					delete_acc(acc);
					cout << '\n';
					cout << "Do you want to repeat this procedure?(yes - 'y', no - another letter): ";
					cin >> chc_c;
					menu_back_frame(chc_c);
				} while (chc_c == 'y');
				break;

			case 10:
				cout << '\n';
				cout << "\t\t" << "VIEW ACCOUNTS" << endl;
				do {
					view_acc(acc);
					cout << '\n';
					cout << "Do you want to repeat this procedure?(yes - 'y', no - another letter): ";
					cin >> chc_c;
					menu_back_frame(chc_c);
				} while (chc_c == 'y');
				break;
			}
		} while (!(chc_i < 1) || !(chc_i > 10));
	}
};

//User class
class User {
private:
	string lst_name, fst_name, mdl_name;
	int id, tab_num, year, month, qnt_work_h, m_per_h;
	float salary;
public:
	User() {
		id = 1;
		lst_name = "";
		fst_name = "";
		mdl_name = "";
		tab_num = 0;
		year = 0;
		month = 0;
		qnt_work_h = 0;
		m_per_h = 0;
		salary = 0.0;
	}

	vector<User> employees; //vector that contains data about employees
	vector<User> employees_list;
	vector<int> employees_id; //vector that contains data about employees' ID
	vector<int> employees_tb_num; //vector that contains data about employees' table number
	vector<float> employees_slr; //vector that contains data about employees' salary
	vector<string> employees_lst_name; //vector that contains data about employees' last name
	vector<string> employees_fst_name; //vector that contains data about employees' first name

	//operator overloading for output
	friend ostream& operator << (ostream& outf, const User& empl) {
		return outf << empl.id << '\t' << empl.lst_name << '\t' << empl.fst_name << '\t' << empl.mdl_name << '\t' << empl.tab_num << '\t'
			<< empl.year << '\t' << empl.month << '\t' << empl.qnt_work_h << '\t' << empl.m_per_h << '\t' << empl.salary << '\n';  // write one string per line, end with a blank line
	}

	//function that executes calculation of salary
	int salary_calc(int time, int money) {
		int salary_start = time * money;
		float salary = (salary_start * 88) / 100;

		return salary;
	}

	//function that handles input errors with displaying of message
	void err_handler() {
		cout << "Error! Follow the manual!" << endl;
		cin.clear();
		cin.ignore(256, '\n');
	}

	//function that handles input errors without displaying of message
	void ignore_func() {
		cin.clear();
		cin.ignore(256, '\n');
	}

	//function that checks whether the user want to back to the menu
	void menu_back_frame(char chc) {
		if (chc != 'y') {
			_getch;
			cout << '\n';
		}
		ignore_func();
		cout << '\n';
	}

	//function that constructs table form 
	void table(int chc) {
		if (chc == 1) {
			cout << "--------+" << "---------------" << "+---------------" << "+---------------" << "+---------------" <<
				"+-------" << "+-------" << "+-------" << "+-------" << "+------------|" << endl;
			cout << "ID" << "\t|" << "Last-N" << "\t\t|" << "First-N" << "\t|" << "Mid-N" << "\t\t|" << "Table-N" <<
				"\t|" << "Year" << "\t|" << "Month" << "\t|" << "Q-W-H" << "\t|" << "M-P-H" << "\t|" << "Salary" << "\n";
			cout << "--------+" << "---------------" << "+---------------" << "+---------------" << "+---------------" <<
				"+-------" << "+-------" << "+-------" << "+-------" << "+------------|" << endl;
		}
		else if (chc == 2) {
			cout << "--------+" << "---------------" << "+---------------" << "+---------------" << "+---------------" <<
				"+-------" << "+-------" << "+-------" << "+-------" << "+------------|" << endl;
		}
	}

	//function that executes the sorting by last name, table num and salary
	void sort_data(vector<User>& employees, vector<string>& employees_lst_name, vector<int>& employees_tb_num, vector<float>& employees_slr, User empl) {
		int chc;
		string temp;

		employees.clear();
		employees_lst_name.clear();
		employees_tb_num.clear();
		employees_slr.clear();

		ifstream file_i("data.txt");
		if (file_i.is_open()) {
			while (file_i >> empl.id >> empl.lst_name >> empl.fst_name >> empl.mdl_name >>
				empl.tab_num >> empl.year >> empl.month >> empl.qnt_work_h >> empl.m_per_h >> empl.salary) {
				employees.push_back(empl);
				employees_lst_name.push_back(empl.lst_name);
				employees_tb_num.push_back(empl.tab_num);
				employees_slr.push_back(empl.salary);
			}
		}
		else {
			cout << "Unable to open a file..." << endl;
		}
		file_i.close();

		cout << "Which way you want to sort data?" << endl;
		cout << "1 - By a last name" << endl;
		cout << "2 - By a table number" << endl;
		cout << "3 - By salary" << endl;
		cout << "Your choice: ";
		cin >> chc;
		while (cin.fail() || chc < 1 || chc > 3) {
			err_handler();
			cout << "Your choice: ";
			cin >> chc;
		}

		switch (chc) {
		case 1:
			sort(employees_lst_name.begin(), employees_lst_name.end());
			table(1);
			for (int i = 0; i < employees.size(); i++) {
				for (const auto& empl : employees) {
					if (employees_lst_name[i] == empl.lst_name) {
						cout << empl;
						table(2);
					}
				}
			}
			break;
		case 2:
			sort(employees_tb_num.begin(), employees_tb_num.end());
			table(1);
			for (int i = 0; i < employees.size(); i++) {
				for (const auto& empl : employees) {
					if (employees_tb_num[i] == empl.tab_num) {
						cout << empl;
						table(2);
					}
				}
			}
			break;
		case 3:
			sort(employees_slr.begin(), employees_slr.end());
			table(1);
			for (int i = (employees.size() - 1); i >= 0; i--) {
				for (const auto& empl : employees) {
					if (employees_slr[i] == empl.salary) {
						cout << empl;
						table(2);
					}
				}
			}
			break;
		}

	}

	//function that executes the searching by last name, first name and table number
	void search_data(vector<User> employees, vector<int> employees_id, vector<string> employees_fst_name, vector<string> employees_lst_name, User empl) {

		int chc;
		string lst_name_s, fst_name_s, s_s;
		int i_s = 0;
		int tb_n_s;


		employees_lst_name.clear();
		employees_fst_name.clear();
		employees_id.clear();

		ifstream file_i("data.txt");
		if (file_i.is_open()) {
			while (file_i >> empl.id >> empl.lst_name >> empl.fst_name >> empl.mdl_name >>
				empl.tab_num >> empl.year >> empl.month >> empl.qnt_work_h >> empl.m_per_h >> empl.salary) {
				employees_lst_name.push_back(empl.lst_name);
				employees_fst_name.push_back(empl.fst_name);
				employees_id.push_back(empl.id);
			}
		}
		else {
			cout << "Unable to open a file..." << endl;
		}
		file_i.close();

		cout << "Which method do you want to use for searching data:\n";
		cout << "1 - By last name;" << endl;
		cout << "2 - By first name;" << endl;
		cout << "3 - By table number;" << endl;
		cout << "Enter only integer!" << endl;
		cout << "Your choice: ";
		cin >> chc;
		while (cin.fail() || chc < 1 || chc > 3) {
			err_handler();
			cout << "Your choice: ";
			cin >> chc;
		}

		ifstream file("data.txt");
		switch (chc) {
		case 1:
			cout << "Enter the last name of person: ";
			cin >> lst_name_s;
			while (!(count(employees_lst_name.begin(), employees_lst_name.end(), lst_name_s))) {
				cout << "There isn't person with similar last name..." << endl;
				cout << "Enter again: ";
				cin >> lst_name_s;
			}
			if (file.is_open()) {
				while (file >> empl.id >> empl.lst_name >> empl.fst_name >> empl.mdl_name >> empl.tab_num >>
					empl.year >> empl.month >> empl.qnt_work_h >> empl.m_per_h >> empl.salary) {
					employees.push_back(empl);
					if (lst_name_s == empl.lst_name) {
						s_s = empl.lst_name;
						cout << empl;
					}
				}
				if (lst_name_s != s_s) {
					cout << "There isn't '" << lst_name_s << "' last name" << endl;
				}
			}
			else {
				cout << "Unable to open a file..." << endl;
			}
			break;
		case 2:
			cout << "Enter the first name of person: ";
			cin >> fst_name_s;
			while (!(count(employees_fst_name.begin(), employees_fst_name.end(), fst_name_s))) {
				cout << "There isn't person with similar first name..." << endl;
				cout << "Enter again: ";
				cin >> fst_name_s;
			}
			if (file.is_open()) {
				while (file >> empl.id >> empl.lst_name >> empl.fst_name >> empl.mdl_name >> empl.tab_num >>
					empl.year >> empl.month >> empl.qnt_work_h >> empl.m_per_h >> empl.salary) {
					employees.push_back(empl);
					if (fst_name_s == empl.fst_name) {
						s_s = empl.fst_name;
						cout << empl;
					}
				}
				if (fst_name_s != s_s) {
					cout << "There isn't '" << fst_name_s << "' first name" << endl;
				}
			}
			else {
				cout << "Unable to open a file..." << endl;
			}
			break;
		case 3:
			cout << "Enter the table number of person: ";
			cin >> tb_n_s;
			while (cin.fail()) {
				err_handler();
				cout << "Enter the table number of person: ";
				cin >> tb_n_s;
			}
			while (!(count(employees_tb_num.begin(), employees_tb_num.end(), tb_n_s))) {
				ignore_func();
				cout << "There isn't person with similar table number..." << endl;
				cout << "Enter again: ";
				cin >> tb_n_s;
			}
			if (file.is_open()) {
				while (file >> empl.id >> empl.lst_name >> empl.fst_name >> empl.mdl_name >> empl.tab_num >>
					empl.year >> empl.month >> empl.qnt_work_h >> empl.m_per_h >> empl.salary) {
					employees.push_back(empl);
					if (tb_n_s == empl.tab_num) {
						i_s = empl.tab_num;
						cout << empl;
					}
				}
				if (tb_n_s != i_s) {
					cout << "There isn't '" << tb_n_s << "' last name" << endl;
				}
			}
			else {
				cout << "Unable to open a file..." << endl;
			}
			break;
		}
		file.close();
	}

	//function that defines how much a particular employee has earned
	void define_money(vector<int> employees_id, vector<string> employees_fst_name, vector<string> employees_lst_name, vector<int> employees_tb_num, User empl) {

		string lst_name_s;
		int tb_n_s, temp_ind = 0;
		float salary_vv;

		employees_lst_name.clear();
		employees_fst_name.clear();
		employees_id.clear();
		employees_tb_num.clear();

		ifstream file_i("data.txt");
		if (file_i.is_open()) {
			while (file_i >> empl.id >> empl.lst_name >> empl.fst_name >> empl.mdl_name >>
				empl.tab_num >> empl.year >> empl.month >> empl.qnt_work_h >> empl.m_per_h >> empl.salary) {
				employees_lst_name.push_back(empl.lst_name);
				employees_fst_name.push_back(empl.fst_name);
				employees_id.push_back(empl.id);
				employees_tb_num.push_back(empl.tab_num);
			}
		}
		else {
			cout << "Unable to open a file..." << endl;
		}
		file_i.close();

		cout << "Enter the last name of employee: ";
		cin >> lst_name_s;
		while (!(count(employees_lst_name.begin(), employees_lst_name.end(), lst_name_s))) {
			cout << '\n';
			cout << "There isn't person with similar last name..." << endl;
			cout << "Enter again: ";
			cin >> lst_name_s;
		}

		ignore_func();

		cout << "Enter the table number of employee: ";
		cin >> tb_n_s;
		while (cin.fail()) {
			cout << '\n';
			err_handler();
			cout << "Enter again: ";
			cin >> tb_n_s;
		}

		ignore_func();

		for (int i = 0; i < employees_lst_name.size(); i++) {
			if (lst_name_s == employees_lst_name[i] && tb_n_s == employees_tb_num[i]) {
				temp_ind = i;
			}
		}

		cout << '\n';

		if (lst_name_s == employees_lst_name[temp_ind] && tb_n_s == employees_tb_num[temp_ind]) {
			ifstream file("data.txt");
			if (file.is_open()) {
				while (file >> empl.id >> empl.lst_name >> empl.fst_name >> empl.mdl_name >> empl.tab_num >>
					empl.year >> empl.month >> empl.qnt_work_h >> empl.m_per_h >> empl.salary) {
					if (lst_name_s == empl.lst_name && tb_n_s == empl.tab_num) {
						cout << "\n";
						cout << "Enter the period of time (in hours): ";
						int period_tm;
						cin >> period_tm;

						cout << '\n';

						if (period_tm >= 144) {
							salary_vv = salary_calc(period_tm, empl.m_per_h) * 2;
							cout << "The salary " << empl.lst_name << ' ' << empl.fst_name << " would earn for " << period_tm << " hours is " << salary_vv << '$' << endl;
						}
						else {
							salary_vv = salary_calc(period_tm, empl.m_per_h);
							cout << "The salary " << empl.lst_name << ' ' << empl.fst_name << " would earn for " << period_tm << " hours is " << salary_vv << '$' << endl;
						}
						break;
					}
				}
			}
			else {
				cout << "Unable to open a file..." << endl;
			}
		}
		else {
			cout << "It isn't employee's table number..." << endl;
			cout << "Try again..." << endl;
			cout << '\n';
			define_money(employees_id, employees_fst_name, employees_lst_name, employees_tb_num, empl);
		}

	}

	//function for viewing of all data
	void view_data(vector<User> employees, User empl) {

		cout << "\n";
		ifstream file("data.txt");
		if (file.is_open()) {
			table(1);
			while (file >> empl.id >> empl.lst_name >> empl.fst_name >> empl.mdl_name >>
				empl.tab_num >> empl.year >> empl.month >> empl.qnt_work_h >> empl.m_per_h >> empl.salary) {
				cout << empl.id << "\t|" << empl.lst_name << "\t\t|" << empl.fst_name << "\t\t|" << empl.mdl_name << "\t\t|" << empl.tab_num << "\t\t|"
					<< empl.year << "\t|" << empl.month << "\t|" << empl.qnt_work_h << "\t|" << empl.m_per_h << "\t|" << empl.salary << '\n';
				table(2);
			}
		}
		else {
			cout << "Unable to open a file..." << endl;
		}
	}

	//function that handle admin menu
	void user_menu() {
		User empl;

		int chc_i;
		char chc_c;

		ifstream file_i("data.txt");
		if (file_i.is_open()) {
			while (file_i >> empl.id >> empl.lst_name >> empl.fst_name >> empl.mdl_name >>
				empl.tab_num >> empl.year >> empl.month >> empl.qnt_work_h >> empl.m_per_h >> empl.salary) {
				employees.push_back(empl);
				employees_id.push_back(empl.id);
				employees_lst_name.push_back(empl.lst_name);
				employees_fst_name.push_back(empl.fst_name);
				employees_tb_num.push_back(empl.tab_num);
				employees_slr.push_back(empl.salary);
			}
		}
		file_i.close();

		do {
			cout << "\t\t" << "USER MENU" << endl;
			cout << "1 - Sort data" << endl;
			cout << "2 - Search employee" << endl;
			cout << "3 - Define employee's salary" << endl;
			cout << "4 - View data" << endl;

			cout << '\n';

			cout << "Enter a number: ";
			cin >> chc_i;
			while (cin.fail() || chc_i < 1 || chc_i > 4) {
				err_handler();
				cout << "Enter again: ";
				cin >> chc_i;
			}
			switch (chc_i) {
			case 1:
				cout << '\n';
				cout << "\t\t" << "SORT DATA" << endl;
				do {
					sort_data(employees, employees_lst_name, employees_tb_num, employees_slr, empl);
					cout << '\n';
					cout << "Do you want to repeat this procedure?(yes - 'y', no - another letter): ";
					cin >> chc_c;
					menu_back_frame(chc_c);
				} while (chc_c == 'y');
				break;
			case 2:
				cout << '\n';
				cout << "\t\t" << "SEARCH EMPLOYEE" << endl;
				do {
					search_data(employees, employees_id, employees_fst_name, employees_lst_name, empl);
					cout << '\n';
					cout << "Do you want to repeat this procedure?(yes - 'y', no - another letter): ";
					cin >> chc_c;
					menu_back_frame(chc_c);
				} while (chc_c == 'y');
				break;
			case 3:
				cout << '\n';
				cout << "\t\t" << "DEFINE EMPLOYEE'S SALARY" << endl;
				do {
					define_money(employees_id, employees_fst_name, employees_lst_name, employees_tb_num, empl);
					cout << '\n';
					cout << "Do you want to repeat this procedure?(yes - 'y', no - another letter): ";
					cin >> chc_c;
					menu_back_frame(chc_c);
				} while (chc_c == 'y');
				break;
			case 4:
				cout << '\n';
				cout << "\t\t" << "VIEW DATA" << endl;
				do {
					view_data(employees, empl);
					cout << '\n';
					cout << "Do you want to repeat this procedure?(yes - 'y', no - another letter): ";
					cin >> chc_c;
					menu_back_frame(chc_c);
				} while (chc_c == 'y');
				break;
			}
		} while (!(chc_i < 1) || !(chc_i > 4));
	}
};

//Authorization class
class Authorization : public Admin, public User {

public:
	Authorization() {}


	void err_handler() {
		cin.clear();
		cin.ignore(256, '\n');
		cout << "Follow the manual!" << endl;
	}

	void start_up() {
		Authorization us;
		int chc;

		cout << "\tWELCOME TO THE PROGRAM" << endl;

		int r = sign_in(0);

		switch (r) {
		case 0:
			user_menu();
		case 1:
			admin_menu();
		}
	}
};

// Main function
int main() {
	Authorization auz;
	auz.start_up();
	return 0;
}
