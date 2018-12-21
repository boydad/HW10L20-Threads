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
#include <thread>

#include "commandManager.h"
#include "BaseThreadHandler.h"
#include "loger.h"

class ThreadComManager: public CommandManager{
private:
  std::shared_ptr<std::mutex> bulkQueue;
  std::shared_ptr<std::condition_variable> newBulk;
  size_t blockCount, commandCount, lineCount;
  std::shared_ptr<bool> finish;
  
  std::shared_ptr<std::mutex> logMutex;
  std::shared_ptr<std::condition_variable> logReady;
  Loger loger;
  std::thread threadLoger;
  

  virtual void saveCurrentBulk() override;  

public:
  ThreadComManager(const int bulkSize);   
  ThreadComManager(const ThreadComManager& other) = delete;
  ThreadComManager operator=(const ThreadComManager& other) = delete;
  virtual ~ThreadComManager();
  
  inline void add(std::string&& command);  
  inline void finalize();
  inline void subscribe(const std::shared_ptr<BaseThreadHandler>& hand);
};

#include "threadComManager_impl.h"

#endif /* THREADCOMMANAGER_H */

