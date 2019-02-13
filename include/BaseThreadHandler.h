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

#pragma once

#include <memory>
#include <mutex>
#include <condition_variable>
#include <thread>

#include "baseHandler.h"
#include "global.h"

class BaseThreadHandler: public BaseHandler{

private:
  size_t blocksCount{0};
  size_t commandsCount{0};  
  bool isInitialized{false};
  
  bool* finish{nullptr};
  std::mutex* bulkQueue{nullptr};
  std::condition_variable* newBulk{nullptr};

  std::unique_ptr<std::thread> thread;
  bool isLaunched{false};
  void loop();

  inline void handleOneBulkFromBuffer(std::unique_lock<std::mutex>& lock);
  inline Bulk extractBulk(std::unique_lock<std::mutex>& lock);
  
public:  
  inline void init(bool* finish, 
            std::mutex* bulkQueue, 
            std::condition_variable* newBulk);   
  inline void launch();
  virtual void finalize() override;
  virtual ~BaseThreadHandler();
};

#include "BaseThreadHandler_impl.h"