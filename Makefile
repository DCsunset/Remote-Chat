CXX = g++
CXXFLAGS = -O3 -std=c++14 -pthread -Wall -Wextra
LIBS = -l:libboost_program_options.a -l:libboost_filesystem.a -l:libboost_system.a

remote_chat : main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS)

