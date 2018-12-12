#include <stdexcept>
#include <vector>
#include <thread>
#include <string>
#include <string>


#include "threadComManager.h"
#include "threadSaver.h"
#include "printHandler.h"

int main(int argc, char** argv){	
  if(argc != 2)
    throw std::logic_error("Incorrect parameter number");
    
  const int bulkSize = atoi(argv[1]);
  ThreadComManager tmanager(bulkSize);
  
  const int numThreads = 2;
  std::vector<std::thread> threads;
  threads.reserve(numThreads);
  std::mutex textout;
  auto threadSaver = [&textout](std::shared_ptr<ThreadSaver> saver){
    const auto counter = saver->run();   
    
    std::lock_guard<std::mutex> lock{textout};
    std::cout << "File thread(" << std::this_thread::get_id() << "): "
      << counter.first << " blocks, " << counter.second << " commands\n";
  };
  
  for(int n=0; n<numThreads; ++n){
    auto printer = std::make_shared<ThreadSaver>();
    tmanager.subscribe(printer);
    threads.emplace_back(threadSaver, printer);
  }      
  
  for(std::string command; std::getline(std::cin, command); )
    tmanager.add(std::move(command));  
  tmanager.finalize();

  for(auto& th: threads)
    th.join();
  
  return 0;
}
