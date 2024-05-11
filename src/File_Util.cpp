#include "File_Util.h"

File_In::File_In(const char* file) : ins(file) {}

void File_In::guard() {
  if (this->ins.is_open()) {
    this->ins.close();
  }
}

void File_In::open(const char* file) {
  this->guard();
  this->ins.open(file);
}

void File_In::reset() {
  this->ins.clear();
  this->ins.seekg(0, std::ios::beg);
}

std::ifstream& File_In::stream() { return this->ins; }

File_In::~File_In() { this->guard(); }

bool File_In::error() const {
  return !this->ins;
}

File_Out::File_Out(const char* file) : os(file, std::ios::trunc) {}

void File_Out::guard() {
  if (this->os.is_open()) {
    this->os.close();
  }
}

void File_Out::open(const char* file) {
  this->guard();
  this->os.open(file);
}

std::ofstream& File_Out::stream() { return this->os; }

File_Out::~File_Out() { this->guard(); }

bool File_Out::error() const {
  return !this->os;
}
