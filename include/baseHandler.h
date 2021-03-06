#pragma once

#include <fstream>
#include <queue>

#include "ihandler.h"
#include "bulk.h"

class BaseHandler: public IHandler{  
protected:
  std::shared_ptr<std::queue<Bulk>> bulkBuffer;
  
  virtual void handleBulk(Bulk& bulk) = 0;
  inline Bulk extractBulk();
  
public:
  BaseHandler();
  virtual ~BaseHandler() = default;
  BaseHandler(const BaseHandler& other) = delete;
    
  inline void set(const std::shared_ptr<std::queue<Bulk>>& bulkBuffer) override;  
  void handle() override;
};

#include "baseHandler_impl.h"