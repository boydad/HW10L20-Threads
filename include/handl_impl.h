#pragma once

#include "handl.h"


PrintHandler::PrintHandler(): bulkBuffer(nullptr) {}

void PrintHandler::set(const std::shared_ptr<std::queue<Bulk>>& bulkBuffer){
  this->bulkBuffer = bulkBuffer;
}

void PrintHandler::handle() {
  while(!bulkBuffer->empty()){
    auto& bulk = bulkBuffer->front();      


    const auto time = std::chrono::system_clock::to_time_t(bulk.getTime());
    char name[17];
    strftime(name, 17, "bulk%X.log", localtime(&time));    
    std::ofstream log(name, std::ofstream::app);

    std::cout << "bulk: ";  
    std::string sep{""};  

    for(auto command: bulk.getCommands()){
      std::cout << sep << command;
      log << sep << command;
      sep = ", ";
    }
    log << '\n';
    std::cout << std::endl;

    bulkBuffer->pop();
  }
}
