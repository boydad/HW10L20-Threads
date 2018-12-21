/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
 
/* 
 * File:   loger_impl.h
 * Author: boyda
 *
 * Created on 12 декабря 2018 г., 11:01
 */
 
#pragma once
#include <memory>
#include "bulk.h"

#include <loger.h>

Loger::Loger( const std::shared_ptr<std::mutex>& mutexLog,
              const std::shared_ptr<std::condition_variable>& logReady,
              std::shared_ptr<bool>& finish):
mutexLog(mutexLog), logReady(logReady), bulk(nullptr), finish(finish)
{}
 
inline void Loger::set(Bulk* bulk)
{
  this->bulk = bulk;
}
 
inline bool Loger::isSaved()
{
  return bulk == nullptr;
}
 
void Loger::run()
{        
  while(!*finish){
    std::unique_lock<std::mutex> lock{*mutexLog};
 
    while(this->isSaved() and !*finish)
      logReady->wait(lock);
    save();
    logReady->notify_one();      
  }
  save();     
  logReady->notify_one();    
  
}
  
void Loger::save(){  
  if(bulk != nullptr){
    std::cout << "bulk: ";
    print(std::cout, *bulk);  
    bulk = nullptr;
  }          
  
}