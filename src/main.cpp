#include <string>
#include <stdexcept>

#include "commandManager.h"
#include "handl.h"


#include <string>
int main(int argc, char** argv){
	
  if(argc != 2)
    throw std::logic_error("Incorrect parameters number");
  
	
  int n = atoi(argv[1]);  	
	CommandManager manager(n);   	
  
	auto printer = std::make_shared<PrintHandler>();
	manager.subscribe(printer);
  
  for(std::string command; std::getline(std::cin, command); )
    manager.add(std::move(command));  

	
  return 0;
}
