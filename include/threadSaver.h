/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   threadSaver.h
 * Author: boyda
 *
 * Created on 11 декабря 2018 г., 10:46
 */

#ifndef THREADSAVER_H
#define THREADSAVER_H

#include "printHandler.h"

class ThreadSaver: public PrintHandler{
  size_t blocksCount{0};
  size_t commandsCount{0};
  bool* finish;
  std::shared_ptr<std::mutex> bulkQueue;
  std::shared_ptr<std::condition_variable> newBulk;
  

public:
  ThreadSaver(bool& finish, std::shared_ptr<std::mutex>& bulkQueue,
    std::shared_ptr<std::condition_variable>& newBulk): 
      PrintHandler(), 
      finish(&finish),
      bulkQueue(bulkQueue),
      newBulk(newBulk) {};
      
  ~ThreadSaver(){};
  
//  void init(bool& finish, std::shared_ptr<std::mutex> bulkQueueBusi, 
//      std::shared_ptr<std::condition_variable> newBulk){
//    this->finish = &finish;
//    this->bulkQueueBusi = bulkQueueBusi;
//    this->newBulk = newBulk;
//  }
  
  auto run(){               
    while(!*finish){
      std::unique_lock<std::mutex> lock{*bulkQueue};
      
      while(bulkBuffer->empty() and !*finish)
        newBulk->wait(lock);
      
      if(!bulkBuffer->empty()){
        this->handleBulk(lock);
      }
    }
        
    while(!bulkBuffer->empty()){
      bulkQueue->lock();
      if(!bulkBuffer->empty()){
        this->handleBulk(*bulkQueue);
      } else{
        bulkQueue->unlock();
      }
    }
        
    return std::make_pair(blocksCount, commandsCount);    
  };
  
  
  template <typename T>
  void handleBulk(T& lock){
    Bulk bulk = this->extractBulk();    
    lock.unlock();
    blocksCount++;

    const std::string name =  this->genName(bulk);    
    std::ofstream log(name, std::ofstream::app);

    commandsCount += this->print(log, bulk);
    this->print(std::cout, bulk);   
  }
};


#endif /* THREADSAVER_H */

