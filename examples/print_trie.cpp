#include "trie.hpp"

#include <iostream>

template<typename T>
void print_trie(const lak::trie<T> &trie, std::string key = "");

template<typename T>
void print_trie(const lak::trie<T> &trie, std::string key)
{
#if 0
  key += "(\"" + trie.key() + "\")";

  if (trie.value()) std::cout << key << ":" << *trie.value() << "\n";

  for (size_t i = 0; i < trie.nodes().size(); ++i)
  {
    print_trie(trie.nodes().at(i),
               (key.empty() ? "'" : key + "+'") + trie.map().at(i) + "'->");
  }
#else
  key += trie.key();

  if (trie.value()) std::cout << key << ":" << *trie.value() << "\n";

  for (size_t i = 0; i < trie.nodes().size(); ++i)
  {
    print_trie(trie.nodes().at(i), key + trie.map().at(i));
  }
#endif
}

int main()
{
  lak::trie<std::string> trie;
  std::cout << "\n\n";

  trie.try_emplace("", "first");
  trie.try_emplace("i", "second");
  print_trie(trie);
  std::cout << "\n\n";

  trie.try_emplace("int", "third");
  print_trie(trie);
  std::cout << "\n\n";

  trie.try_emplace("intint", "fourth");
  print_trie(trie);
  std::cout << "\n\n";

  trie.try_emplace("l", "fifth");
  print_trie(trie);
  std::cout << "\n\n";

  trie["long long"].value() = "seventh";
  print_trie(trie);
  std::cout << "\n\n";

  trie.try_emplace("long", "sixth");
  print_trie(trie);
  std::cout << "\n\n";

  trie.force_emplace("long", "eighth");
  print_trie(trie);
  std::cout << "\n\n";

  return 0;
}