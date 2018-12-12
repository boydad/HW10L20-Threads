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

#include <memory>
#include <mutex>

#include "printHandler.h"

class ThreadSaver: public PrintHandler{
private:
  size_t blocksCount{0};
  size_t commandsCount{0};
  bool* finish{nullptr};
  std::shared_ptr<std::mutex> bulkQueue{nullptr};
  std::shared_ptr<std::condition_variable> newBulk{nullptr};
  
public:
  ThreadSaver(): PrintHandler()
  {};      
  ~ThreadSaver()
  {};
  
  void init(bool& finish, 
            const std::shared_ptr<std::mutex>& bulkQueue, 
            const std::shared_ptr<std::condition_variable>& newBulk);
  std::pair<size_t, size_t> run();    
  template <typename T>   void handleBulk(T& lock);
};

#include "threadSaver_impl.h"
#endif /* THREADSAVER_H */

