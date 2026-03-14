#pragma once

#include <concepts>
#include <iostream>
#include <stdexcept>
#include <string>

std::string DefaultCredsReader();

template <class Reader = decltype(&DefaultCredsReader)>
  requires requires(Reader r) {
    { r() } -> std::convertible_to<std::string>;
  }
class ShowCredsAction {
public:
  ShowCredsAction(Reader reader = DefaultCredsReader)
      : reader(std::move(reader)) {}

  int Run() {
    try {
      std::cout << reader() << "\n";
      return 0;
    } catch (const std::exception &e) {
      std::cerr << e.what() << "\n";
      return 1;
    }
  }

private:
  Reader reader;
};

template <class Reader>
ShowCredsAction(Reader) -> ShowCredsAction<Reader>;
