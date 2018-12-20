/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   loger.h
 * Author: boyda
 *
 * Created on 12 декабря 2018 г., 7:40
 */

#ifndef LOGER_H
#define LOGER_H

#include <mutex>
#include <memory>
#include <stdexcept>

#include "bulk.h"
#include "printHandler.h"

class Loger{
  std::shared_ptr<std::mutex> mutexLog;
  std::shared_ptr<std::condition_variable> logReady;
  Bulk* bulk;
  bool* finish;
  
  inline void save(){
    if(bulk != nullptr){
      PrintHandler::print(std::cout, *bulk);  
      bulk = nullptr;
    }          
  }
  
public:
  Loger(const std::shared_ptr<std::mutex>& mutexLog,
        const std::shared_ptr<std::condition_variable>& logReady,
        bool& finish):
  mutexLog(mutexLog), logReady(logReady), bulk(nullptr), finish(&finish)
  {};
  
  inline void set(Bulk* bulk){
    this->bulk = bulk;
  }
  
  inline bool isSaved(){
    return bulk == nullptr;
  }
  
  void run(){        
    while(!*finish){
      std::unique_lock<std::mutex> lock{*mutexLog};
      
      while(bulk == nullptr and !*finish)
        logReady->wait(lock);
      
      save();
      logReady->notify_one();
    }
    save();        
    logReady->notify_one();
  }
  
  
};

#endif /* LOGER_H */

