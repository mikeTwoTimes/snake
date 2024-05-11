#pragma once

#include <fstream>

class File_In {
public:
  File_In(const char* file);
  void open(const char* file);
  void reset();
  ~File_In();

  std::ifstream& stream();

  bool error() const;
  
private:
  std::ifstream ins;

  void guard();
  
  File_In(const File_In&) = delete;
  File_In& operator=(const File_In&) = delete;
};

class File_Out {
public:
  File_Out(const char* file);
  void open(const char* file);
  ~File_Out();

  std::ofstream& stream();

  bool error() const;
  
private:
  std::ofstream os;

  void guard();
  
  File_Out(const File_Out&) = delete;
  File_Out& operator=(const File_Out&) = delete;
};
