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

#include "bulk.h"

class IHandler{
public:
  virtual ~IHandler() = default;
  
  virtual void handle() = 0;
  virtual void set(const std::shared_ptr<std::queue<Bulk>>& bulkBuffer) = 0;
};


class PrintHandler: public IHandler{
  
  std::shared_ptr<std::queue<Bulk>> bulkBuffer;
  
public:
  PrintHandler();
  virtual ~PrintHandler() = default;
  PrintHandler(const PrintHandler& other) = delete;
  PrintHandler operator=(const PrintHandler& other) = delete;
  
  void set(const std::shared_ptr<std::queue<Bulk>>& bulkBuffer) override;  
  void handle() override;

};


#include "handl_impl.h"

#endif /* IHANDLER_H */

