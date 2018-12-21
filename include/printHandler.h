#pragma once

#include "baseHandler.h"
#include "bulk.h"

class PrintHandler: public BaseHandler{

  PrintHandler(const PrintHandler& other) = delete;
  PrintHandler operator=(const PrintHandler& other) = delete;  

  void handleBulk(Bulk& bulk) override
  {
    const std::string name =  this->genName(bulk);    
    std::ofstream log(name, std::ofstream::app);

    log << "bulk: ";
    print(log, bulk);

    std::cout << "bulk: "; 
    print(std::cout, bulk); 
  }
  
public:
  PrintHandler(): BaseHandler() {};

  static std::string genName(const Bulk& bulk)
  {
    const auto time = std::chrono::system_clock::to_time_t(bulk.getTime());
    char name[17];
    strftime(name, 17, "bulk%X.log", localtime(&time));    
    return std::string(name);
  }
};