/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bulk.h
 * Author: boyda
 *
 * Created on 13 ноября 2018 г., 20:36
 */

#pragma once

#include <chrono>
#include <vector>
#include <string>


class Bulk{
  std::chrono::system_clock::time_point firstCommandTime;
  std::vector<std::string> commands;
  bool empty;
  
public:
  
  Bulk(const int maxSize) {
    commands.reserve(maxSize);
    empty = true;
  }
  ~Bulk() = default;
  Bulk(const Bulk&) = delete;
  
  Bulk(Bulk&& other): Bulk(other.commands.capacity()){    
    std::swap(firstCommandTime, other.firstCommandTime);
    std::swap(commands, other.commands);
    std::swap(empty, other.empty);
  }
  
  Bulk& operator=(const Bulk&) = delete;
  Bulk& operator=(Bulk&&) = delete;
  
  void init(const std::chrono::system_clock::time_point& time){
    firstCommandTime = time;
    commands.clear();
  }
  
  inline bool isEmpty(){
    return empty;
  }
  
  void add(std::string&& command){
    commands.emplace_back(std::move(command));
    empty = false;
  }
  
  const std::chrono::system_clock::time_point& getTime() const{
    return firstCommandTime;
  }
  
  const std::vector<std::string>& getCommands() const{
    return commands;
  }
  
  auto getSize(){
    return commands.size();
  }
};
