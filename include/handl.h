/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IHandler.h
 * Author: boyda
 *
 * Created on 17 сентября 2018 г., 8:46
 */

#ifndef IHANDLER_H
#define IHANDLER_H


#include <chrono>
#include <ctime>
#include <fstream>
#include <string>
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "bulk.h"

class IHandler{
public:
  virtual ~IHandler() = default;
  
  virtual void handle() = 0;
  virtual void set(const std::shared_ptr<std::queue<Bulk>>& bulkBuffer) = 0;
};


class PrintHandler: public IHandler{
  
protected:
  std::shared_ptr<std::queue<Bulk>> bulkBuffer;
  
  inline size_t print(std::ostream& stream, Bulk& bulk);
  
  //why can not be
  //Bulk&& extractBulk(){...
  auto extractBulk(){
    Bulk bulk = std::move(bulkBuffer->front());      
    bulkBuffer->pop();
    return std::move(bulk);
  }
  std::string genName(const Bulk& bulk);
  
public:
  PrintHandler();
  virtual ~PrintHandler() = default;
  PrintHandler(const PrintHandler& other) = delete;
  PrintHandler operator=(const PrintHandler& other) = delete;
  
  void set(const std::shared_ptr<std::queue<Bulk>>& bulkBuffer) override;  
  void handle() override;

};

class ThreadSaver: public PrintHandler{
  size_t blocksCount{0};
  size_t commandsCount{0};
  bool& finish;
  std::shared_ptr<std::mutex> bulkQueueBusi;
  std::shared_ptr<std::condition_variable> newBulk;
  

public:
  ThreadSaver(bool& finish, std::shared_ptr<std::mutex>& bulkQueue,
    std::shared_ptr<std::condition_variable>& newBulk): 
      PrintHandler(), 
      finish(finish),
      bulkQueueBusi(bulkQueue),
      newBulk(newBulk) {};
      
  ~ThreadSaver(){};
  
  auto run(){
    while(!finish){
      std::unique_lock<std::mutex> lock{*bulkQueueBusi};
      
      while(bulkBuffer->empty() and !finish)
        newBulk->wait(lock);
      
      if(!bulkBuffer->empty()){
        Bulk bulk = this->extractBulk();    
        lock.unlock();
        blocksCount++;
        
        const std::string name =  this->genName(bulk);    
        std::ofstream log(name, std::ofstream::app);

        commandsCount += this->print(log, bulk);
        this->print(std::cout, bulk);    
      }
    }
    return std::make_pair(this->blocksCount, this->commandsCount);
    
  };
  
};

#include "handl_impl.h"

#endif /* IHANDLER_H */

