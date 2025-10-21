# compiler and args
CC      := gcc
CFLAGS  := -Wall -Wextra -std=c11 -O2 -Iinc
LDFLAGS := -lm

# directory
SRCDIR  := src
INCDIR  := inc
OBJDIR  := obj
BINDIR  := bin

# src files
SRCS := $(wildcard *.c) $(wildcard $(SRCDIR)/*.c)
OBJS := $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(SRCS)))

# target
TARGET := $(BINDIR)/app

.PHONY: all clean

all: $(TARGET)


# linking
$(TARGET): $(OBJS) | $(BINDIR)
	$(CC) $^ -o $@ $(LDFLAGS)

# compiling
$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# create dir automatically
$(OBJDIR) $(BINDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)

# dependent header
-include $(OBJS:.o=.d)

# generate dependence files
$(OBJDIR)/%.d: %.c | $(OBJDIR)
	@$(CC) -MM -MT $(OBJDIR)/$*.o $(CFLAGS) $< > $@
$(OBJDIR)/%.d: $(SRCDIR)/%.c | $(OBJDIR)
	@$(CC) -MM -MT $(OBJDIR)/$*.o $(CFLAGS) $< > $@
