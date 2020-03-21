#include <unordered_map>
#include <string>
#include <vector>
#include <optional>
#include <tuple>
#include <assert.h>
#include <iostream>

namespace lak
{
  bool is_prefix(std::string_view str, std::string_view prefix)
  {
    return
      prefix.size() <= str.size() &&
      str.substr(0, prefix.size()) == prefix;
  }

  std::string longest_common_prefix(const std::string &str1,
                                    const std::string &str2)
  {
    return std::string(str1.begin(), std::mismatch(
                         str1.begin(),
                         str1.end(),
                         str2.begin(),
                         str2.end()).first);
  }

  template<typename T>
  struct trie
  {
    std::string key;
    std::optional<T> value;
    std::string map = "";
    std::vector<trie> nodes = {};

    trie() = default;
    trie(std::string &&k)
    : key(std::move(k)),
      value(std::nullopt),
      map(),
      nodes()
    {}
    trie(std::string &&k,
         std::optional<T> &&v)
    : key(std::move(k)),
      value(std::move(v)),
      map(),
      nodes()
    {}
    trie(std::string &&k,
         std::string &&m,
         std::vector<trie> &&n)
    : key(std::move(k)),
      value(std::nullopt),
      map(std::move(m)),
      nodes(std::move(n))
    {}
    trie(std::string &&k,
         std::optional<T> &&v,
         std::string &&m,
         std::vector<trie> &&n)
    : key(std::move(k)),
      value(std::move(v)),
      map(std::move(m)),
      nodes(std::move(n))
    {}

    T& operator[](const std::string& key)
    {
      auto [node, remaining_key] = find(this, key);

      if (remaining_key != node->key)
      {
        node->try_emplace(remaining_key);
      }

      std::tie(node, remaining_key) = find(this, key);

      assert(remaining_key == node->key);
      assert(node->value);

      return *node->value;
    }

    const T& operator[](const std::string& key) const
    {
      auto [node, remaining_key] = find(this, key);

      assert(remaining_key == node->key);
      assert(node->value);

      return *node->value;
    }

    trie *internal_try_emplace(const std::string &key)
    {
      auto [node, remaining_key] = find(this, key);

      if (remaining_key == node->key)
      {
        // exact node already exists.
        return node->value.has_value() ? nullptr : node;
      }

      // merge a newly constructed node with the terminal node that we found.
      *node = merge(std::move(*node), trie(std::string(remaining_key)));

      node = find(node, remaining_key).first;

      assert(!node->value.has_value());

      return node;
    }

    trie *internal_force_emplace(const std::string &key)
    {
      auto [node, remaining_key] = find(this, key);

      if (remaining_key == node->key)
      {
        // exact node already exists.
        if (node->value.has_value()) node->value.reset();
        return node;
      }

      // merge a newly constructed node with the terminal node that we found.
      *node = merge(std::move(*node), trie(std::string(remaining_key)));

      node = find(node, remaining_key).first;

      assert(!node->value.has_value());

      return node;
    }

    template<typename ...ARGS>
    void try_emplace(const std::string &key, ARGS &&...args)
    {
      if (auto node = internal_try_emplace(key); node)
        node->value.emplace(std::forward<ARGS...>(args...));
    }

    void try_emplace(const std::string &key)
    {
      if (auto node = internal_try_emplace(key); node) node->value.emplace();
    }

    template<typename ...ARGS>
    void force_emplace(const std::string &key, ARGS &&...args)
    {
      internal_force_emplace(key)->value.emplace(
        std::forward<ARGS...>(args...));
    }

    void force_emplace(const std::string &key)
    {
      internal_force_emplace(key)->value.emplace();
    }

    static trie merge(trie &&node1, trie &&node2)
    {
      assert(node1.key != node2.key);

      auto common = longest_common_prefix(node1.key, node2.key);

      const auto common_length = common.size();

      if (common_length == node1.key.size())
      {
        // node1.key is a prefix of node2.key.
        node1.map += node2.key.at(common_length);
        node2.key = node2.key.substr(common_length + 1);
        node1.nodes.emplace_back(std::move(node2));
        return node1;
      }
      else if (common_length == node2.key.size())
      {
        // node2.key is a prefix of node1.key.
        node2.map += node1.key.at(common_length);
        node1.key = node1.key.substr(common_length + 1);
        node2.nodes.emplace_back(std::move(node1));
        return node2;
      }
      else
      {
        // node1.key and node2.key are divergent.
        std::string map = std::string()
          + node1.key.at(common_length)
          + node2.key.at(common_length);

        node1.key = node1.key.substr(common_length + 1);
        node2.key = node2.key.substr(common_length + 1);

        std::vector<trie> nodes;
        nodes.emplace_back(std::move(node1));
        nodes.emplace_back(std::move(node2));

        return trie(std::move(common), std::move(map), std::move(nodes));
      }
    }

    static std::pair<trie *, std::string>
    find(trie *node, std::string_view key)
    {
      for (;;)
      {
        if (key.empty() || !is_prefix(key, node->key) || key == node->key)
        {
          return {node, std::string(key)};
        }

        const auto common_size = node->key.size();
        const auto lookup = key.at(common_size);
        const auto pos = node->map.find_first_of(lookup);

        if (pos == std::string::npos)
        {
          return {node, std::string(key)};
        }
        else
        {
          node = node->nodes.data() + pos;
          key = key.substr(common_size + 1);
        }
      }
    }
  };
}

template<typename T>
void print_trie(const lak::trie<T> &trie, std::string key = "");

template<typename T>
void print_trie(const lak::trie<T> &trie, std::string key)
{
  key += "(\"" + trie.key + "\")";
  if (trie.value)
  {
    std::cout << key << ":" << *trie.value << "\n";
  }
  for (size_t i = 0; i < trie.nodes.size(); ++i)
  {
    print_trie(trie.nodes.at(i),
               (key.empty() ? "" : key + "+'") + trie.map.at(i) + "'->");
  }
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

  trie["long long"] = "seventh";
  print_trie(trie);
  std::cout << "\n\n";

  trie.try_emplace("long", "sixth");
  print_trie(trie);
  std::cout << "\n\n";

  trie.force_emplace("long", "eighth");
  print_trie(trie);
  std::cout << "\n\n";

  // print_trie(trie);

  // std::cout << "\n\n";

  return 0;
}