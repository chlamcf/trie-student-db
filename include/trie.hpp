#ifndef _TRIE_HPP
#define _TRIE_HPP

#include <functional>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>

/**
 * @brief The trie.
 * @tparam V The value type stored in the trie.
 * @tparam BeginChar The inclusive lower bound of supported key characters.
 * @tparam EndChar The inclusive upper bound of supported key characters.
 */
template <typename V, char BeginChar, char EndChar> class Trie final {
private:
  static_assert(BeginChar <= EndChar, "BeginChar must be <= EndChar");
  static const int BranchingFactor = EndChar - BeginChar + 1;

  /**
   * @brief The node in the trie.
   */
  struct Node {
    /**
     * @brief The key string corresponding to @ref value.
     */
    std::string key;
    /**
     * @brief The value corresponding to @ref key.
     */
    V value;
    /**
     * @brief The children of the node in the trie.
     */
    Trie<V, BeginChar, EndChar> children[BranchingFactor];

    Node(const std::string &key, const V &value) : key(key), value(value) { }

    Node(const Node &other) : key(other.key), value(other.value) {
        for (int i = 0; i < BranchingFactor; i++) {children[i] = other.children[i];}
    }

    Node &operator=(const Node &other) {
        if (this != &other) {
            key = other.key;
            value = other.value;
            for (int i = 0; i < BranchingFactor; i++) {children[i] = other.children[i];}
        }
        return *this;
    }
  };

  /** @brief The root node pointer; nullptr means that the trie is empty. */
  Node *root;

  /**
   * @brief Converts a character into a child index.
   * @throws std::out_of_range if the character is outside [BeginChar, EndChar].
   */
  int idx_of(char c) const {
    if (c < BeginChar || c > EndChar) {
      std::stringstream ss;
      ss << "character '" << c << "' is outside trie range [" << BeginChar
         << ", " << EndChar << "]";
      throw std::out_of_range(ss.str());
    }
    return c - BeginChar;
  }

public:
  /**
   * @brief Constructs an empty trie.
   */
  Trie() {
    root = nullptr;
  }

  /**
   * @brief Constructs a deep copy of the trie.
   */
  Trie(const Trie &other) {
    if (other.root != nullptr) {
        root = new Node(*other.root);
    } else {
        root = nullptr;
    }
  }

  /**
   * @brief Assigns by deep-copying another trie.
   */
  Trie &operator=(const Trie &other) {
    if (this != &other) {
        delete root;
        root = other.root == nullptr ? nullptr : new Node(*other.root);
    }
    return *this;
  }

  /**
   * @brief Destructs the trie and all its nodes.
   */
  ~Trie() {
    delete root;
  }

  /**
   * @brief Searches for a key-value pair by key.
   * @param key The key to locate.
   * @return A pointer to the value if exists, otherwise nullptr.
   * @throws std::out_of_range for out-of-range key characters.
   */
  V *search(const std::string &key) const {
    for (int i = 0; i < static_cast<int>(key.size()); i++) {idx_of(key[i]);}
    const Trie *curr = this;
    for (int search_depth = 0;; search_depth++) {
        if (curr->root == nullptr) {return nullptr;}
        if (curr->root->key == key) {return &curr->root->value;}
        if (search_depth == static_cast<int>(key.size())) {return nullptr;}
        curr = &curr->root->children[curr->idx_of(key[search_depth])];
    }
  }

  /**
   * @brief Inserts or overwrites a key-value pair by the key.
   * @param key The key to insert or overwrite.
   * @param val The value to insert or overwrite.
   * @throws std::out_of_range for out-of-range key characters.
   */
  void insert(const std::string &key, const V &val) {
    for (int i = 0; i < static_cast<int>(key.size()); i++) {idx_of(key[i]);}
    Trie *curr = this;
    for (int insert_depth = 0;; insert_depth++) {
        if (curr->root == nullptr) {
            curr->root = new Node(key, val);
            return;
        }
        if (curr->root->key == key) {
            curr->root->value = val;
            return;
        }
        if (insert_depth == static_cast<int>(key.size())) {break;}
        curr = &curr->root->children[curr->idx_of(key[insert_depth])];
    }
    std::string displaced_key = curr->root->key;
    V displaced_value = curr->root->value;
    curr->root->key = key;
    curr->root->value = val;
    insert(displaced_key, displaced_value);
  }

  /**
   * @brief Removes a key-value pair by the key if exists.
   * @param key The key to remove.
   * @throws std::out_of_range for out-of-range key characters.
   */
  void remove(const std::string &key) {
    for (int i = 0; i < static_cast<int>(key.size()); i++) {idx_of(key[i]);}
    Trie *target = nullptr;
    Trie *curr = this;
    for (int remove_depth = 0;; remove_depth++) {
        if (curr->root == nullptr) {return;}
        if (curr->root->key == key) {
            target = curr;
            break;
        }
        if (remove_depth == static_cast<int>(key.size())) {return;}
        curr = &curr->root->children[curr->idx_of(key[remove_depth])];
    }
    if (target == nullptr) return;
    
    bool childrenY = false;
    for (int i = 0; i < BranchingFactor; i++) {
        if (target->root->children[i].root != nullptr) {
            childrenY = true;
            break;
        }
    }

    if (!childrenY) {
        delete target->root;
        target->root = nullptr;
    } else {
        Trie *leaf = target;
        while (true) {
            bool childfound = false;
            for (int i = 0; i < BranchingFactor; i++) {
                if (leaf->root->children[i].root != nullptr) {
                    leaf = &leaf->root->children[i];
                    childfound = true;
                    break;
                }
            }
            if (!childfound) {break;}
        }

        target->root->key = leaf->root->key;
        target->root->value = leaf->root->value;
        delete leaf->root;
        leaf->root = nullptr;
    }
  }

  /**
   * @brief Looks up a key and inserts a default value if it is missing.
   */
  V &operator[](const std::string &key) {
    V *found = search(key);
    if (found == nullptr) {
        insert(key, V{});
        found = search(key);
    }
    return *found;
  }

  /**
   * @brief Looks up a key or throws a std::out_of_range if it is missing.
   * @throws std::out_of_range if the key is missing or if the key contains out-of-range characters.
   */
  const V &operator[](const std::string &key) const {
    V *found = search(key);
    if (found == nullptr) {throw std::out_of_range("This key does not exist.");}
    return *found;
  }

  /**
   * @brief Traverses all key-value pairs in the trie.
   * @param v The function to call for each key-value pair.
   */
  void traverse(std::function<void(const std::string &, const V &)> v) const {
    if (root == nullptr) {return;}
    v(root->key, root->value);
    for (int i = 0; i < BranchingFactor; i++) {
      const Trie &child = root->children[i];
      child.traverse(v);
    }
  }

  /**
   * @brief Traverses all entries and allows value mutation.
   */
  void traverse(std::function<void(const std::string &, V &)> v) {
    if (root == nullptr) {return;}
    v(root->key, root->value);
    for (int i = 0; i < BranchingFactor; i++) {root->children[i].traverse(v);}
  }

  /**
   * @brief Merges another trie into this trie.
   */
  Trie &operator+=(const Trie &rhs) {
    rhs.traverse([&](const std::string &key, const V &value) {insert(key, value);});
    return *this;
  }

  /**
   * @brief Merges two tries and returns the result as a new trie.
   */
  friend Trie operator+(Trie lhs, const Trie &rhs) {
    lhs += rhs;
    return lhs;
  }

  /**
   * @brief Compares key-value content for equality.
   */
  friend bool operator==(const Trie &lhs, const Trie &rhs) {
    bool trie_equal = true;

    lhs.traverse([&](const std::string &key, const V &value) {
        V *other = rhs.search(key);
        if (other == nullptr || *other != value) {trie_equal = false;}
    });

    rhs.traverse([&](const std::string &key, const V &value) {
        V *other = lhs.search(key);
        if (other == nullptr || *other != value) {trie_equal = false;}
    });

    return trie_equal;
  }

  /**
   * @brief Compares key-value content for inequality.
   */
  friend bool operator!=(const Trie &lhs, const Trie &rhs) {
    return !(lhs == rhs);
  }

  /**
   * @brief Prints all key-value entries to a stream.
   */
  friend std::ostream &operator<<(std::ostream &os, const Trie &trie) {
    trie.traverse([&](const std::string &key, const V &value) {os << key << " => " << value << '\n';});
    return os;
  }
};

#endif // _TRIE_HPP
