/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ihandle.h
 * Author: boyda
 *
 * Created on 11 декабря 2018 г., 10:43
 */

#ifndef IHANDLE_H
#define IHANDLE_H

#include "bulk.h"

class IHandler{
public:
  virtual ~IHandler() = default;
  
  virtual void handle() = 0;
  virtual void set(const std::shared_ptr<std::queue<Bulk>>& bulkBuffer) = 0;
};


#endif /* IHANDLE_H */

