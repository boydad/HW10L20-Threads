/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   threadComManager_impl.h
 * Author: boyda
 *
 * Created on 11 декабря 2018 г., 11:34
 */

#ifndef THREADCOMMANAGER_IMPL_H
#define THREADCOMMANAGER_IMPL_H
#include "threadComManager.h"

void ThreadComManager::saveCurrentBulk(){
  std::lock_guard<std::mutex> lock{*bulkQueue};

  if(!bulk.isEmpty()){
    bulkBuffer->push(std::move(bulk));
    this->blockCount++;
  }

  newBulk->notify_one();    
}

#endif /* THREADCOMMANAGER_IMPL_H */

