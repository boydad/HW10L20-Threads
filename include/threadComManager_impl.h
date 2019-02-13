/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   threadComManager_impl.h
 * Author: boyda
 *
 * Created on 11 декабря 2018 г., 11:34
 */

#ifndef THREADCOMMANAGER_IMPL_H
#define THREADCOMMANAGER_IMPL_H

#include "threadComManager.h"
#include "global.h"

ThreadComManager::ThreadComManager(const int bulkSize):
  CommandManager(bulkSize),  
  blockCount(0), commandCount(0), lineCount(0), finish(false),
  loger(&logMutex, &logReady, &finish),
  threadLoger(&Loger::run, &loger)
{}

void ThreadComManager::saveCurrentBulk()
{  
  if(!bulk.isEmpty()){

    {
      std::unique_lock<std::mutex> lock{logMutex};     
      loger.set(&bulk);
      logReady.notify_one();
      while(!loger.isSaved())           
          logReady.wait(lock);
    }
    
    {
      std::lock_guard<std::mutex> lock{bulkQueue};
      bulkBuffer->push(std::move(bulk));
    }
    newBulk.notify_one();  
    
    this->blockCount++;    
  }
}

void ThreadComManager::add(std::string&& command)
{
  lineCount++;
  if(command.compare("{") and command.compare("}"))
    commandCount++;
  CommandManager::add(std::move(command));
}

ThreadComManager::~ThreadComManager()
{  
  this->finalize();
  std::lock_guard<std::mutex> lock{mu_cout}; 
  std::cout << "Main thread(" << std::this_thread::get_id()<< "): "
    << blockCount << " blocks, " << commandCount << " commands, " 
    << lineCount << " lines\n";
}

void ThreadComManager::finalize()
{
   CommandManager::finalize();
   finish = true;
   logReady.notify_all();
   newBulk.notify_all();

  threadLoger.join();
  for(auto& handler: this->handlers)
    handler->finalize();
}

void ThreadComManager::subscribe(const std::shared_ptr<BaseThreadHandler>& hand)
{
  CommandManager::subscribe(hand);    
  hand->init(&finish, &bulkQueue, &newBulk);
  hand->launch();
}
#endif /* THREADCOMMANAGER_IMPL_H */