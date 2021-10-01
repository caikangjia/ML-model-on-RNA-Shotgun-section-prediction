CXX = g++
CXXFLAGS = -Wall -Werror -pedantic --std=c++11 -g


all: test

test: BinarySearchTree_compile_check.exe \
		BinarySearchTree_tests.exe \
		BinarySearchTree_public_test.exe \
		Map_compile_check.exe Map_public_test.exe main.exe

	./BinarySearchTree_tests.exe
	./BinarySearchTree_public_test.exe

	./Map_public_test.exe

	./main.exe Sample_Section_I test.csv --debug > Sample_Section_I.out.txt

	./main.exe Sample_Section_II test.csv > Sample_Section_II.out.txt

	./main.exe Sample_Section_III.csv test.csv > Sample_Section_III.out.txt

	./main.exe Sample_Section_IV.csv test.csv > Sample_Section_IV.out.txt

main.exe: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $@

BinarySearchTree_tests.exe: BinarySearchTree_tests.cpp unit_test_framework.cpp BinarySearchTree.h
	$(CXX) $(CXXFLAGS) $< unit_test_framework.cpp -o $@

%_public_test.exe: %_public_test.cpp unit_test_framework.cpp  %.h
	$(CXX) $(CXXFLAGS) $< unit_test_framework.cpp -o $@

%_compile_check.exe: %_compile_check.cpp %.h
	$(CXX) $(CXXFLAGS) $< -o $@

# disable built-in rules
.SUFFIXES:

# these targets do not create any files
.PHONY: clean
clean :
	rm -vrf *.o *.exe *.gch *.dSYM *.stackdump *.out.txt
