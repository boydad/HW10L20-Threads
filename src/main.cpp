#include <string>
#include <stdexcept>
#include <string>

#include <thread>

#include "commandManager.h"
#include "handl.h"


int main(int argc, char** argv){	
  if(argc != 2)
    throw std::logic_error("Incorrect parameters number");
    
  bool finish = false;  
  const int numThreads = 2;
	
  const int n = atoi(argv[1]);    
	CommandManager manager(n);   	
    
  
  std::vector<std::thread> threads;
  threads.reserve(numThreads);
  
  auto bulkQueue = std::make_shared<std::mutex>();
  auto newBulk = std::make_shared<std::condition_variable>();
  ThreadComManager tmanager(n, bulkQueue, newBulk);
  
  auto threadSaver = [](std::shared_ptr<ThreadSaver> saver){
    const auto counter = saver->run();    
    std::cout << "Thread " << std::this_thread::get_id() << ": "
      << counter.first << " blocks, " << counter.second << " commands\n";
  };

  for(int n=0; n<numThreads; ++n){
    auto printer = std::make_shared<ThreadSaver>(finish, bulkQueue, newBulk);
    tmanager.subscribe(printer);
    threads.emplace_back(threadSaver, printer);
  }    
  
  
  for(std::string command; std::getline(std::cin, command); )
    tmanager.add(std::move(command));  
  finish = true;

  for(auto& th: threads)
    th.join();
  
  return 0;
}
