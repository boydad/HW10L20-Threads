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

#include <fstream>
//#include <string>
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>

//#include "bulk.h"

#include "ihandler.h"

class PrintHandler: public IHandler{
  
protected:
  std::shared_ptr<std::queue<Bulk>> bulkBuffer;
  
  inline size_t print(std::ostream& stream, Bulk& bulk);
  
  //why can not be
  //Bulk&& extractBulk(){...
  auto extractBulk(){
    Bulk bulk = std::move(bulkBuffer->front());      
    bulkBuffer->pop();
    return std::move(bulk);
  }
  std::string genName(const Bulk& bulk);
  
public:
  PrintHandler();
  virtual ~PrintHandler() = default;
  PrintHandler(const PrintHandler& other) = delete;
  PrintHandler operator=(const PrintHandler& other) = delete;
  
  void set(const std::shared_ptr<std::queue<Bulk>>& bulkBuffer) override;  
  void handle() override;

};

#include "printHandler_impl.h"

#endif /* IHANDLER_H */

