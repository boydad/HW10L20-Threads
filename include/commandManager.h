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


#include <chrono>
#include <vector>
#include <queue>
#include <string>
#include <memory>

#include "handl.h"
#include "bulk.h"

class CommandManager{
  size_t maxBuffSize;
  size_t numOpenBracket;
  std::vector< std::shared_ptr<IHandler> > handlers;
  Bulk bulk;
  std::shared_ptr<std::queue<Bulk>> bulkBuffer;
  
  void notify();
  void addCustomBulk();
  void delCustomBulk();
  void addInBulk(std::string&& command);
  
  void saveCurrentBulk();
  inline bool isBulkFull();
  

  
public:
  CommandManager(const int bulkSize);
  CommandManager(const CommandManager& other) = delete;
  CommandManager operator=(const CommandManager& other) = delete;
  ~CommandManager();
  
  void add(std::string&& command);  
  void subscribe(const std::shared_ptr<IHandler>& hand);    
};


#include "commandManagar_impl.h"

