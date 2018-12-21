#pragma once

#include <memory>
#include <queue>
#include "bulk.h"

class IHandler{
public:
  virtual ~IHandler() = default;
  
  virtual void handle() = 0;
  virtual void set(const std::shared_ptr<std::queue<Bulk>>& bulkBuffer) = 0;
  virtual void finalize() {};
};