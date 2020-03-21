CXX = g++-8
CXXFLAGS += -no-pie -Wall -Werror -Wfatal-errors -pthread -std=c++17

SRCDIR = examples
BINDIR = bin
OBJDIR = obj

CXXFLAGS += -I./

$(BINDIR)/print_trie.elf: $(OBJDIR)/print_trie.o | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ && chmod +x $@

.PHONY: $(BINDIR)/print_trie.elf

$(OBJDIR)/print_trie.o: $(SRCDIR)/print_trie.cpp trie.hpp trie_impl.hpp Makefile | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(BINDIR):
	mkdir $(BINDIR)

$(OBJDIR):
	mkdir $(OBJDIR)
