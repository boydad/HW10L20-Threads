/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   threadComManager.h
 * Author: boyda
 *
 * Created on 10 декабря 2018 г., 18:21
 */

#ifndef THREADCOMMANAGER_H
#define THREADCOMMANAGER_H

//#include <vector>
//#include <queue>
//#include <string>
//#include <memory>

#include "commandManager.h"
#include "threadSaver.h"


class ThreadComManager: public CommandManager{
  
  virtual void saveCurrentBulk() override;
  
  std::shared_ptr<std::mutex> bulkQueue;
  std::shared_ptr<std::condition_variable> newBulk;
  size_t blockCount, commandCount, lineCount;
  bool finish;
  
public:
  ThreadComManager( const int bulkSize, 
                    std::shared_ptr<std::mutex>& bulkQueue,
                    std::shared_ptr<std::condition_variable>& newBulk):
      CommandManager(bulkSize), bulkQueue(bulkQueue), newBulk(newBulk),
      blockCount(0), commandCount(0), lineCount(0), finish(false) {};
    
  ThreadComManager(const ThreadComManager& other) = delete;
  ThreadComManager operator=(const ThreadComManager& other) = delete;
  
  virtual void add(std::string&& command) override{
    lineCount++;
    if(command.compare("{") and command.compare("}"))
      commandCount++;
    CommandManager::add(std::move(command));
  }
  
  void subscribe(const std::shared_ptr<ThreadSaver>& hand){
//    hand->init(finish, this->bulkQueue, this->newBulk);
    CommandManager::subscribe(hand);    
  }
  
  virtual ~ThreadComManager(){        
    std::cout << "Main thread(" << std::this_thread::get_id()<< "): " << lineCount << " lines, "
      << commandCount << " commands, " << blockCount << " blocks.\n";
  }

};

#include "threadComManager_impl.h"

#endif /* THREADCOMMANAGER_H */

