CXXFLAGS += -std=c++11 -Wall -Wextra -pedantic
CXXFLAGS += -I ../include

SOURCES = main.test.cpp \
          bpstd/optional.test.cpp

OBJECTS = $(SOURCES:.cpp=.o)

all: unit_tests

unit_tests: $(OBJECTS) ../include/bpstd/optional.hpp catch.hpp
	@echo "[CXXLD] $@"
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.cpp ../include/bpstd/optional.hpp catch.hpp
	@echo "[CXX] $@"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -fr unit_tests $(OBJECTS)
