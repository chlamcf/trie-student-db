#include "system.hpp"
#include <cctype>
#include <vector>

/**
 * @brief Convert a string to uppercase for case-insensitive indexing.
 * @param text The input string to normalize.
 * @return A copy of the input string with alphabetic characters converted to uppercase.
 */
std::string normalize_uppercase(const std::string &text) {
  std::string normalized = text;
  for (int i = 0; i < normalized.size(); ++i) {
    normalized[i] = static_cast<char>(
        std::toupper(static_cast<unsigned char>(normalized[i])));
  }
  return normalized;
}

/**
 * @brief Create a vector of const Student * from a vector of Student *.
 */
std::vector<const Student *>
to_const_students(const std::vector<Student *> &students) {
  std::vector<const Student *> result;
  for (int i = 0; i < students.size(); ++i) {
    result.push_back(students[i]);
  }
  return result;
}

System::System() { }

System::~System() {
    for (int i = 0; i < static_cast<int>(students.size()); i++) {delete students[i];}
}

void System::insert(const Student &student) {
    if (sid_index.search(student.sid) != nullptr) {throw std::runtime_error("This SID already exist.");}
    Student *newstudent = new Student(student);
    students.push_back(newstudent);
    sid_index.insert(newstudent->sid, newstudent);
    given_name_index[normalize_uppercase(newstudent->given_name)].push_back(newstudent);
    family_name_index[normalize_uppercase(newstudent->family_name)].push_back(newstudent);
    major_index[newstudent->major].push_back(newstudent);
}

const Student *System::lookup_sid(const std::string &sid) const {
    Student **found = sid_index.search(sid);
    return found == nullptr ? nullptr : *found;
}

std::vector<const Student *> System::lookup_given_name(const std::string &given_name) const {
    std::vector<Student *> *found = given_name_index.search(normalize_uppercase(given_name));
    return found == nullptr ? std::vector<const Student *>() : to_const_students(*found);
}

std::vector<const Student *> System::lookup_family_name(const std::string &family_name) const {
    std::vector<Student *> *found = family_name_index.search(normalize_uppercase(family_name));
    return found == nullptr ? std::vector<const Student *>() : to_const_students(*found);
}

std::vector<const Student *> System::lookup_major(const std::string &major) const {
    std::vector<Student *> *found = major_index.search(major);
    return found == nullptr ? std::vector<const Student *>() : to_const_students(*found);
}

std::vector<const Student *> System::lookup_full_name(const std::string &given_name, const std::string &family_name) const {
    std::vector<Student *> *given = given_name_index.search(normalize_uppercase(given_name));
    std::vector<Student *> *family = family_name_index.search(normalize_uppercase(family_name));
    if (given == nullptr || family == nullptr) {return {};}
    std::vector<const Student *> result;

    for (int i = 0; i < static_cast<int>(students.size()); i++) {
        Student *candidate = students[i];
        
        bool givenY = false;
        for (int j = 0; j < static_cast<int>(given->size()); j++) {
            if ((*given)[j] == candidate) {
                givenY = true;
                break;
            }
        }
        if (!givenY) {continue;}

        bool familyY = false;
        for (int j = 0; j < static_cast<int>(family->size()); j++) {
            if ((*family)[j] == candidate) {
                familyY = true;
                break;
            }
        }
        if (familyY) {result.push_back(candidate);}
    }
    return result;
}

std::vector<const Student *> System::lookup_all() const {
    return to_const_students(students);
}