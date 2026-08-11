#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <string>
#include <vector>

#include "student.hpp"
#include "trie.hpp"

/**
 * @brief Student Record Management System.
 */
class System {
  /**
   * @brief The storage for student records.
   */
  std::vector<Student *> students;
  Trie<Student *, '0', '9'> sid_index;
  Trie<std::vector<Student *>, 'A', 'Z'> given_name_index;
  Trie<std::vector<Student *>, 'A', 'Z'> family_name_index;
  Trie<std::vector<Student *>, 'A', 'Z'> major_index;

public:
  System();
  ~System();

  /**
   * @brief Insert a student record.
   */
  void insert(const Student &student);

  /**
   * @brief Lookup a student record by SID.
   * @return A pointer to the student if found, otherwise nullptr.
   */
  const Student *lookup_sid(const std::string &sid) const;

  /**
   * @brief Lookup student records by given name.
   * @return A vector of student pointers matching the given name.
   */
  std::vector<const Student *>
  lookup_given_name(const std::string &given_name) const;

  /**
   * @brief Lookup student records by family name.
   * @return A vector of student pointers matching the family name.
   */
  std::vector<const Student *>
  lookup_family_name(const std::string &family_name) const;

  /**
   * @brief Lookup student records by major.
   * @return A vector of student pointers matching the major.
   */
  std::vector<const Student *> lookup_major(const std::string &major) const;

  /**
   * @brief Lookup student records by full name.
   * @return A vector of student pointers matching the full name.
   */
  std::vector<const Student *>
  lookup_full_name(const std::string &given_name,
                   const std::string &family_name) const;

  /**
   * @brief Retrieve all student records.
   * @return A vector of student pointers.
   */
  std::vector<const Student *> lookup_all() const;
};

#endif // SYSTEM_HPP