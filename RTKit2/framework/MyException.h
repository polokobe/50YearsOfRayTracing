/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */
#pragma once

#include <exception>
#include <string>

namespace RTKit2 {

class MyException : public std::exception {
 public:
  MyException(const std::string& szMessage) : mMessage(szMessage) {}
  MyException(const std::string& szMessage, const char* SDLError) {
    mMessage = szMessage;
    mMessage.append(", SDL Error: ");
    mMessage.append(SDLError);
  }

  virtual const char* what() const noexcept override {
    return mMessage.c_str();
  }

 private:
  std::string mMessage;
};
}  // namespace RTKit2
