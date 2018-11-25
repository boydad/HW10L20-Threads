/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bulk.h
 * Author: boyda
 *
 * Created on 17 сентября 2018 г., 8:44
 */

#pragma once


#include <chrono>
#include <vector>
#include <queue>
#include <string>
#include <memory>

#include "handl.h"
#include "bulk.h"

class CommandManager{
protected:
  size_t maxBuffSize;
  size_t numOpenBracket;
  std::vector< std::shared_ptr<IHandler> > handlers;
  Bulk bulk;
  std::shared_ptr<std::queue<Bulk>> bulkBuffer;
  
  void notify();
  void addCustomBulk();
  void delCustomBulk();
  void addInBulk(std::string&& command);
  
  virtual void saveCurrentBulk();
  inline bool isBulkFull();
  

  
public:
  CommandManager(const int bulkSize);
  CommandManager(const CommandManager& other) = delete;
  CommandManager operator=(const CommandManager& other) = delete;
  virtual ~CommandManager();
  
  void add(std::string&& command);  
  void subscribe(const std::shared_ptr<IHandler>& hand);    
};

class ThreadComManager: public CommandManager{
  
  void saveCurrentBulk() override{
    std::lock_guard<std::mutex> lock{*bulkQueue};
    
    if(!bulk.isEmpty()){
      bulkBuffer->push(std::move(bulk));
      this->blockCount++;
    }
    
    newBulk->notify_all();    
  }
  
  std::shared_ptr<std::mutex> bulkQueue;
  std::shared_ptr<std::condition_variable> newBulk;
  size_t blockCount, commandCount, lineCount;
  
public:
  ThreadComManager(const int bulkSize, 
    std::shared_ptr<std::mutex>& bulkQueue,
    std::shared_ptr<std::condition_variable>& newBulk):
      CommandManager(bulkSize), bulkQueue(bulkQueue), newBulk(newBulk),
      blockCount(0), commandCount(0), lineCount(0) {};
    
    ThreadComManager(const ThreadComManager& other) = delete;
    ThreadComManager operator=(const ThreadComManager& other) = delete;
      
  
};


#include "commandManagar_impl.h"

