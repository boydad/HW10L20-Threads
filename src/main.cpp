#include <stdexcept>
#include <vector>
#include <thread>
#include <string>
#include <string>


#include "threadComManager.h"
#include "BaseThreadHandler.h"

std::mutex mu_cout;

int main(int argc, char** argv){    
  if(argc != 2)
    throw std::logic_error("Incorrect parameter number");
    
  const int bulkSize = atoi(argv[1]);
  ThreadComManager tmanager(bulkSize);

  const int numThreads = 2;
  
  for(int n=0; n<numThreads; ++n){
    auto saver = std::make_shared<ThreadSaver>();
    tmanager.subscribe(saver);
  }      
  
  for(std::string command; std::getline(std::cin, command); )
    tmanager.add(std::move(command));  
  // tmanager.finalize();

  return 0;
} 