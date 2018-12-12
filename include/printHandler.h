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
#include <iostream>
#include <queue>

#include "ihandler.h"
#include "bulk.h"

class PrintHandler: public IHandler{
  
protected:
  std::shared_ptr<std::queue<Bulk>> bulkBuffer;
  
  Bulk extractBulk();
  std::string genName(const Bulk& bulk);
  
public:
  PrintHandler();
  virtual ~PrintHandler() = default;
  PrintHandler(const PrintHandler& other) = delete;
  PrintHandler operator=(const PrintHandler& other) = delete;
  
  static size_t print(std::ostream& stream, const Bulk& bulk);
  void set(const std::shared_ptr<std::queue<Bulk>>& bulkBuffer) override;  
  void handle() override;
};

#include "printHandler_impl.h"

#endif /* IHANDLER_H */

