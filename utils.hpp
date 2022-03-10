#pragma once

#include <stdexcept>

class NotImplemented : public std::logic_error {
 public:
  NotImplemented() : std::logic_error("Function not yet implemented"){};
};

#define todo (throw NotImplemented())
