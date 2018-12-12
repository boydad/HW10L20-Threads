/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   threadSaver_impl.h
 * Author: boyda
 *
 * Created on 11 декабря 2018 г., 12:42
 */

#ifndef THREADSAVER_IMPL_H
#define THREADSAVER_IMPL_H

#include <stdexcept>
#include <sstream>

std::pair<size_t, size_t> ThreadSaver::run()
{  
  if( finish == nullptr or 
      bulkQueue == nullptr or 
      newBulk == nullptr or
      bulkBuffer == nullptr)
        throw std::logic_error("ThreadSaver is not initilized!");
    
  while(!*finish)
  {
    std::unique_lock<std::mutex> lock{*bulkQueue};

    while(bulkBuffer->empty() and !*finish)
      newBulk->wait(lock);

    if(!bulkBuffer->empty()){
      this->handleBulk(lock);
    }
  }
  while(!bulkBuffer->empty())
  {
    bulkQueue->lock();
    if(!bulkBuffer->empty()){
      this->handleBulk(*bulkQueue);
    } else{
      bulkQueue->unlock();
    }
  }
  return std::make_pair(blocksCount, commandsCount);    
}

template <typename T>
void ThreadSaver::handleBulk(T& lock)
{
  Bulk bulk = this->extractBulk();    
  lock.unlock();
  blocksCount++;

  std::stringstream name;
  name << "thread(" << std::this_thread::get_id() << ")-" << this->genName(bulk);
  std::ofstream log(name.str(), std::ofstream::app);

  commandsCount += this->print(log, bulk);
//  this->print(std::cout, bulk);   
}

void ThreadSaver::init(bool& finish, 
                      const std::shared_ptr<std::mutex>& bulkQueue, 
                      const std::shared_ptr<std::condition_variable>& newBulk)
{
  this->finish = &finish;
  this->bulkQueue = bulkQueue;
  this->newBulk = newBulk;
}
  
#endif /* THREADSAVER_IMPL_H */

