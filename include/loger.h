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
#include <iostream>

#include "bulk.h"
#include "printHandler.h"

class Loger{
  std::shared_ptr<std::mutex> mutexLog;
  std::shared_ptr<std::condition_variable> logReady;
  Bulk* bulk;
  bool* finish;
  
  inline void save();  
  
public:
  Loger(const std::shared_ptr<std::mutex>& mutexLog,
        const std::shared_ptr<std::condition_variable>& logReady,
        bool& finish);
  
  inline void set(Bulk* bulk); 
  inline bool isSaved();  
  void run();
  
};

#include "loger_impl.h"

#endif /* LOGER_H */

