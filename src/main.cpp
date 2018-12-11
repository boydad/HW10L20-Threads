#include <stdexcept>
#include <vector>
#include <thread>
#include <string>
#include <string>


#include "threadComManager.h"
#include "threadSaver.h"



int main(int argc, char** argv){	
  if(argc != 2)
    throw std::logic_error("Incorrect parameter number");
    
  const int numThreads = 2;
  std::vector<std::thread> threads;
  threads.reserve(numThreads);
  
  
  const int n = atoi(argv[1]);  	
  auto bulkQueue = std::make_shared<std::mutex>();
  auto newBulk = std::make_shared<std::condition_variable>();
  ThreadComManager tmanager(n, bulkQueue, newBulk);
  
  

  bool finish = false;  
  std::mutex textout;
  
  auto threadSaver = [&textout](std::shared_ptr<ThreadSaver> saver){
    const auto counter = saver->run();   
    
    std::lock_guard<std::mutex> lock{textout};
    std::cout << "File thread(" << std::this_thread::get_id() << "): "
      << counter.first << " blocks, " << counter.second << " commands\n";
  };
  
  for(int n=0; n<numThreads; ++n){
    auto printer = std::make_shared<ThreadSaver>(finish, bulkQueue, newBulk);
    tmanager.subscribe(printer);
    threads.emplace_back(threadSaver, printer);
  }    
  
  
  for(std::string command; std::getline(std::cin, command); )
    tmanager.add(std::move(command));  
  tmanager.finalize();
  finish = true;

  for(auto& th: threads)
    th.join();
  
  return 0;
}
