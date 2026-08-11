#include <iostream>
#include <limits>
#include <vector>
#include "system.hpp"

using namespace std;

void print_students(const vector<const Student *> &students) {
  if (students.empty()) {
    cout << "No records found." << endl;
    return;
  }

  cout << students.size() << " records found." << endl;
  for (const Student *student : students) {
    cout << *student << endl;
  }
}

int main() {
  System system;

  while (1) {
    cout << "Student Record Management System" << endl
         << "(1) Query Record by SID" << endl
         << "(2) Query Records by Given Name" << endl
         << "(3) Query Records by Family Name" << endl
         << "(4) Query Records by Major" << endl
         << "(5) Query Records by Full Name" << endl
         << "(6) Query all Records" << endl
         << "(7) Insert Record" << endl;

    char choice;
    cin >> choice;

    try {
      switch (choice) {
      case '1': {
        string sid;
        cout << "Enter SID: ";
        cin >> sid;

        const Student *student = system.lookup_sid(sid);
        if (student == nullptr) {
          cout << "No record found." << endl;
        } else {
          cout << *student << endl;
        }
        break;
      }
      case '2': {
        string given_name;
        cout << "Enter Given Name: ";
        cin >> given_name;

        print_students(system.lookup_given_name(given_name));
        break;
      }
      case '3': {
        string family_name;
        cout << "Enter Family Name: ";
        cin >> family_name;

        print_students(system.lookup_family_name(family_name));
        break;
      }
      case '4': {
        string major;
        cout << "Enter Major: ";
        cin >> major;

        print_students(system.lookup_major(major));
        break;
      }
      case '5': {
        string given_name, family_name;
        cout << "Enter Given Name: ";
        cin >> given_name;
        cout << "Enter Family Name: ";
        cin >> family_name;

        print_students(system.lookup_full_name(given_name, family_name));
        break;
      }
      case '6': {
        print_students(system.lookup_all());
        break;
      }
      case '7': {
        Student student;
        cout << "Enter SID: ";
        cin >> student.sid;
        cout << "Enter Given Name: ";
        cin >> student.given_name;
        cout << "Enter Family Name: ";
        cin >> student.family_name;
        cout << "Enter Major: ";
        cin >> student.major;
        cout << "Enter Description: ";
        getline(cin >> std::ws, student.description);

        try {
          system.insert(student);
          cout << "Record inserted." << endl;
        } catch (const exception &error) {
          cout << error.what() << endl;
        }
        break;
      }
      default:
        cout << "Invalid choice." << endl;
        break;
      }
    } catch (const out_of_range &error) {
      cout << error.what() << endl;
    }

    cout << "Press Enter to continue..." << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
  }
}
