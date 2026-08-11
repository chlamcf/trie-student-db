#include "student.hpp"

std::ostream &operator<<(std::ostream &os, const Student &student) {
    os << student.family_name << ", " << student.given_name << " (" << student.sid << ") "
       << student.major << ": " << student.description;
    return os;
}