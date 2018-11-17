#pragma once

#include "commandManager.h"


CommandManager::CommandManager(const int bulkSize): 
    maxBuffSize(bulkSize), 
    numOpenBracket(0), bulk(bulkSize), bulkBuffer(new std::queue<Bulk>()) {}

CommandManager::~CommandManager(){
  if(numOpenBracket == 0)
    this->saveCurrentBulk();
}

void CommandManager::add(std::string&& command) { 
  if(!command.compare("{")){
      this->addCustomBulk();
  } else if(!command.compare("}")){
      this->delCustomBulk();      
  } else{
      this->addInBulk(std::move(command));       
  }    
}

void CommandManager::addCustomBulk(){
  if(numOpenBracket == 0)
    this->saveCurrentBulk();
  ++numOpenBracket;
}

void CommandManager::delCustomBulk(){
  if(numOpenBracket == 1)
    this->saveCurrentBulk();
  --numOpenBracket;  
}

void CommandManager::addInBulk(std::string&& command){
  if(bulk.isEmpty())
    bulk.init(std::chrono::system_clock::now());
  
  bulk.add(std::move(command));
  
  if(this->isBulkFull())
    this->saveCurrentBulk();     
}

void CommandManager::saveCurrentBulk(){    
  if(!bulk.isEmpty())
    bulkBuffer->push(std::move(bulk));

  this->notify();
}

void CommandManager::notify(){
  for(auto handler: handlers)
    handler->handle();
}

bool CommandManager::isBulkFull(){
  return bulk.getSize() == maxBuffSize and numOpenBracket == 0;
}

void CommandManager::subscribe(const std::shared_ptr<IHandler>& handler){
  handler->set(bulkBuffer);
  handlers.emplace_back(handler);
}
