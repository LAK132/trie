CXX = g++-8
CXXFLAGS += -no-pie -Wall -Werror -Wfatal-errors -pthread -std=c++17

SRCDIR = src
BINDIR = bin
OBJDIR = obj

$(BINDIR)/example.elf: $(OBJDIR)/main.o | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ && chmod +x $@

.PHONY: $(BINDIR)/example.elf

$(OBJDIR)/main.o: $(SRCDIR)/main.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(BINDIR):
	mkdir $(BINDIR)

$(OBJDIR):
	mkdir $(OBJDIR)
