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

#pragma once

#include <stdexcept>
#include <memory>
#include <thread>

#include "bulk.h"

void BaseThreadHandler::init(std::shared_ptr<bool>& finish, 
                      const std::shared_ptr<std::mutex>& bulkQueue, 
                      const std::shared_ptr<std::condition_variable>& newBulk)
{
  this->finish = finish;
  this->bulkQueue = bulkQueue;
  this->newBulk = newBulk;

  this->isInitialized = true;
}
  
void BaseThreadHandler::launch()
{
  thread = std::make_unique<std::thread>(&BaseThreadHandler::loop, this);
  isLaunched = true;
}  

void BaseThreadHandler::loop()
{  
  if(!isInitialized)
    throw std::logic_error("BaseThreadHandler: run before initialization!");
    
  while(!*finish){
    std::unique_lock<std::mutex> lock{*bulkQueue};

    while(bulkBuffer->empty() and !*finish)
      newBulk->wait(lock);
    
    this->handleOneBulkFromBuffer(lock);
  }

  while(!bulkBuffer->empty()){
    std::unique_lock<std::mutex> lock{*bulkQueue};    
    this->handleOneBulkFromBuffer(lock);
  }

  std::lock_guard<std::mutex> lock{mu_cout};
  std::cout << "File thread(" << std::this_thread::get_id() << "): "
    << blocksCount << " blocks, " << commandsCount << " commands\n";
}

void BaseThreadHandler::handleOneBulkFromBuffer(std::unique_lock<std::mutex>& lock)
{
  if(!bulkBuffer->empty()){
    auto bulk = this->extractBulk(lock);
    commandsCount += bulk.getSize();
    this->handleBulk(bulk);
  }
}

Bulk BaseThreadHandler::extractBulk(std::unique_lock<std::mutex>& lock)
{
  Bulk bulk = BaseHandler::extractBulk();    
  lock.unlock();
  blocksCount++;
  return bulk;
}

void BaseThreadHandler::finalize()
{
  if(isLaunched)
    thread->join();
  isLaunched = false;
}

BaseThreadHandler::~BaseThreadHandler()
{  
}

