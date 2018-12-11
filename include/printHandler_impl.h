#pragma once

PrintHandler::PrintHandler(): bulkBuffer(nullptr) {}

void PrintHandler::set(const std::shared_ptr<std::queue<Bulk>>& bulkBuffer){
  this->bulkBuffer = bulkBuffer;
}

void PrintHandler::handle() {
  while(!bulkBuffer->empty()){
    Bulk bulk = this->extractBulk();    
    const std::string name =  this->genName(bulk);    
    std::ofstream log(name, std::ofstream::app);
    
    this->print(log, bulk);
    this->print(std::cout, bulk);    
  }
}



std::string PrintHandler::genName(const Bulk& bulk){
  const auto time = std::chrono::system_clock::to_time_t(bulk.getTime());
  char name[17];
  strftime(name, 17, "bulk%X.log", localtime(&time));    
  return std::string(name);
}

inline size_t PrintHandler::print(std::ostream& stream, Bulk& bulk){   
  size_t count = 0;
  std::string sep{""};  
  
  stream << "bulk(" << std::this_thread::get_id() << "): "; 
  for(auto& command: bulk.getCommands()){    
    stream << sep << command;
    sep = ", ";
    count++;
  }
  stream << '\n';
  
  return count;
}