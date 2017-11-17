CXX      = g++
RM       = rm -rf
CPPFLAGS = -g -std=c++11
LDFLAGS  = -g
LDLIBS   =
OBJDIR   = .build
TARGET   = roost

SRCS += main.cpp
SRCS += roostController.cpp
SRCS += environment.cpp
SRCS += userCommand.cpp
SRCS += userCommandController.cpp

OBJS=$(addprefix $(OBJDIR)/, $(subst .cpp,.o,$(SRCS)))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/%.o: %.cpp $(OBJDIR)
	$(CXX) $(CPPFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJDIR)

distclean: clean
	$(RM) $(TARGET)
