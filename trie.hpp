#ifndef LAK_TRIE_HPP
#define LAK_TRIE_HPP

#include <string>
#include <vector>
#include <optional>
#include <tuple>
#include <assert.h>

namespace lak
{
  template<typename T>
  struct trie
  {
    trie() = default;

    const std::string &key() const { return _key; }

    const std::string &map() const { return _map; }

    const std::vector<trie> &nodes() const { return _nodes; }

    std::optional<T> &value();
    const std::optional<T> &value() const;

    T& operator[](const std::string& key);

    const T& operator[](const std::string& key) const;

    void try_emplace(const std::string &key);

    template<typename ...ARGS>
    void try_emplace(const std::string &key, ARGS &&...args);

    void force_emplace(const std::string &key);

    template<typename ...ARGS>
    void force_emplace(const std::string &key, ARGS &&...args);

  private:

    std::string _key;
    std::optional<T> _value;
    std::string _map = "";
    std::vector<trie> _nodes = {};

    trie(std::string &&k);

    trie(std::string &&k, std::optional<T> &&v);

    trie(std::string &&k, std::string &&m, std::vector<trie> &&n);

    trie(std::string &&k,
         std::optional<T> &&v,
         std::string &&m,
         std::vector<trie> &&n);

    trie *internal_try_emplace(const std::string &key);

    trie *internal_force_emplace(const std::string &key);

    static trie merge(trie &&node1, trie &&node2);

    static std::pair<trie *, std::string>
    find(trie *node, std::string_view key);
  };
}

#include "trie_impl.hpp"

#endif // LAK_TRIE_HPP