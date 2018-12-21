#pragma once

#include <sstream>
#include "printHandler.h"
#include <BaseThreadHandler.h>

class ThreadSaver: public BaseThreadHandler{
protected:
  virtual void handleBulk(Bulk& bulk) override final
  {  
    std::stringstream name;
    name << "thread(" << std::this_thread::get_id() << ")-" << PrintHandler::genName(bulk);
    std::ofstream log(name.str(), std::ofstream::app);

    log << "bulk: ";
    print(log, bulk);
  }
public:
  virtual ~ThreadSaver(){};
};
