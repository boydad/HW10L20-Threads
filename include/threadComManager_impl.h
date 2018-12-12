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

  ThreadComManager::ThreadComManager(const int bulkSize):
    CommandManager(bulkSize),  
    bulkQueue(std::make_shared<std::mutex>()), 
    newBulk(std::make_shared<std::condition_variable>()),
    blockCount(0), commandCount(0), lineCount(0), finish(false),
    logMutex(std::make_shared<std::mutex>()), 
    logReady(std::make_shared<std::condition_variable>()),
    loger(logMutex, logReady, finish),
    threadLoger(&Loger::run, &loger)
  {}

void ThreadComManager::saveCurrentBulk()
{  
  if(!bulk.isEmpty()){
    loger.set(&bulk);
    logReady->notify_one();
    while(!loger.isSaved()){
      std::unique_lock<std::mutex> lock{*logMutex};     
      logReady->wait(lock);
    }
    {
      std::lock_guard<std::mutex> lock{*bulkQueue};
      bulkBuffer->push(std::move(bulk));
    }
    newBulk->notify_one();  
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
  threadLoger.join();
  std::cout << "Main thread(" << std::this_thread::get_id()<< "): "
    << blockCount << " blocks, " << commandCount << " commands, " 
    << lineCount << " lines\n";    
}

void ThreadComManager::finalize()
{
   CommandManager::finalize();
   finish = true;
   newBulk->notify_all();
   logReady->notify_all();
}

void ThreadComManager::subscribe(const std::shared_ptr<ThreadSaver>& hand)
{
  hand->init(finish, bulkQueue, newBulk);
  CommandManager::subscribe(hand);    
}
#endif /* THREADCOMMANAGER_IMPL_H */

