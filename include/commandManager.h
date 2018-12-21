/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bulk.h
 * Author: boyda
 *
 * Created on 17 сентября 2018 г., 8:44
 */

#pragma once


#include <vector>
#include <memory>
#include <queue>
#include <string>

#include "ihandler.h"
#include "bulk.h"

class CommandManager{
private:
  size_t maxBuffSize;
  size_t numOpenBracket;
  

  inline void notify();
  inline void addCustomBulk();
  inline void delCustomBulk();
  inline void addInBulk(std::string&& command);
  inline bool isBulkFull();    
  
protected:  
  std::vector< std::shared_ptr<IHandler> > handlers;
  Bulk bulk;
  std::shared_ptr<std::queue<Bulk>> bulkBuffer;

  inline void finalize();  
  virtual void saveCurrentBulk();
  

public:
  CommandManager(const int bulkSize);
  CommandManager(const CommandManager& other) = delete;
  CommandManager operator=(const CommandManager& other) = delete;
  virtual ~CommandManager();
  
  inline void add(std::string&& command);  
  inline void subscribe(const std::shared_ptr<IHandler>& hand);    
};

#include "commandManagar_impl.h"

