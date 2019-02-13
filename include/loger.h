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
#include <condition_variable>

#include "bulk.h"
#include "printHandler.h"

class Loger{
  std::mutex* mutexLog;
  std::condition_variable* logReady;
  Bulk* bulk;
  bool* finish;
  
  inline void save();  
  
public:
  Loger(std::mutex* mutexLog,
        std::condition_variable* logReady,
        bool* finish);
  
  inline void set(Bulk* bulk); 
  inline bool isSaved();  
  inline void run();
  
};

#include "loger_impl.h"

#endif /* LOGER_H */

