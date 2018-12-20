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

#include <memory>
#include <mutex>
#include <condition_variable>

#include "commandManager.h"
#include "threadSaver.h"
#include "loger.h"

class ThreadComManager: public CommandManager{
private:
  std::shared_ptr<std::mutex> bulkQueue;
  std::shared_ptr<std::condition_variable> newBulk;
  size_t blockCount, commandCount, lineCount;
  bool finish;
  
  std::shared_ptr<std::mutex> logMutex;
  std::shared_ptr<std::condition_variable> logReady;
  Loger loger;
  std::thread threadLoger;
  

  virtual void saveCurrentBulk() override;  

public:
  ThreadComManager(const int bulkSize):
    CommandManager(bulkSize),  
    bulkQueue(std::make_shared<std::mutex>()), 
    newBulk(std::make_shared<std::condition_variable>()),
    blockCount(0), commandCount(0), lineCount(0), finish(false),
    logMutex(std::make_shared<std::mutex>()), 
    logReady(std::make_shared<std::condition_variable>()),
    loger(logMutex, logReady, finish),
    threadLoger(&Loger::run, &loger)
  {};    
  ThreadComManager(const ThreadComManager& other) = delete;
  ThreadComManager operator=(const ThreadComManager& other) = delete;
  virtual ~ThreadComManager();
  
  void add(std::string&& command);  
  void finalize();
  void subscribe(const std::shared_ptr<ThreadSaver>& hand);
};

#include "threadComManager_impl.h"

#endif /* THREADCOMMANAGER_H */

