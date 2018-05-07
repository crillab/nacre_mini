EXEC      ?= nacre_mini

CSRCS      = $(shell find . -name *.cc)

OBJDIR     = objs
COBJS      = $(CSRCS:./%.cc=$(OBJDIR)/%.o)
PCOBJS     = $(addsuffix p,  $(COBJS))
RCOBJS     = $(addsuffix r,  $(COBJS))

DEP 	  := $(COBJS:%.o=%.d)

CFLAGS     = `xml2-config --cflags` -I../XCSP3-CPP-Parser-master/include $(addprefix -I, $(shell find . -type d))
LDFLAGS    = `xml2-config --libs` -L../XCSP3-CPP-Parser-master/lib/ -lparserxcsp3core -lm

.PHONY: s p install
s:      $(EXEC)
p:      $(EXEC)_profile
r:      $(EXEC)_release
install: lib $(EXEC)_release

$(EXEC): CXX = g++ -std=c++14 -Wall -Wfatal-errors -O3 -g
$(EXEC)_profile: CXX = g++ -std=c++14 -Wall -Wfatal-errors -O2 -g -pg -fprofile-arcs -ftest-coverage
$(EXEC)_release: CXX = g++ -std=c++14 -O3 -DNDEBUG

## Clean rule
.PHONY: clean fclean
clean:
	rm -rf $(OBJDIR)
	rm -f $(EXEC)*
	rm -f *grind.out.*
	find . -name "*~" -delete
	find . -name ".#*" -delete
	find . -name "#*" -delete

fclean: clean
	@make -C ../XCSP3-CPP-Parser-master/samples/ clean

## Lib maker
.PHONY: lib
lib:
	@make -C ../XCSP3-CPP-Parser-master/samples/ clean
	@make -C ../XCSP3-CPP-Parser-master/samples/ lib -j

# Link the executable
$(EXEC) : $(COBJS)
	@echo "Linking: $@"
	@$(CXX) $(COBJS) $(LDFLAGS) -o $@

$(EXEC)_profile : $(PCOBJS)
	@echo "Linking: $@"
	@$(CXX) $(PCOBJS) $(LDFLAGS) -o $@

$(EXEC)_release : $(RCOBJS)
	@echo "Linking: $@"
	@$(CXX) $(RCOBJS) $(LDFLAGS) -o $@

# Add dependency files, if they exist
-include $(DEP)


# Source file rules
$(OBJDIR)/%.o $(OBJDIR)/%.op $(OBJDIR)/%.or: ./%.cc ./%.hh
	@mkdir -p $(@D)
	@echo "Compiling: $<"
	@$(CXX) -c $(CFLAGS) -MT $@ -MMD -MP -o $@ $<
