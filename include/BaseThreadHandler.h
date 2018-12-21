/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   threadSaver.h
 * Author: boyda
 *
 * Created on 11 декабря 2018 г., 10:46
 */

#ifndef THREADSAVER_H
#define THREADSAVER_H

#include <memory>
#include <mutex>
#include <condition_variable>
#include <thread>

#include "baseHandler.h"
#include "global.h"

class BaseThreadHandler: public BaseHandler{

private:
  size_t blocksCount{0};
  size_t commandsCount{0};
  std::shared_ptr<bool> finish{nullptr};
  bool isInitialized{false};
  std::shared_ptr<std::mutex> bulkQueue{nullptr};
  std::shared_ptr<std::condition_variable> newBulk{nullptr};

  std::unique_ptr<std::thread> thread;
  bool isLaunched{false};
  void loop();

  void handleOneBulkFromBuffer(std::unique_lock<std::mutex>& lock);
  Bulk extractBulk(std::unique_lock<std::mutex>& lock);
  // virtual void handleBulk(Bulk& bulk){std::cout << "TTTTTTT";};
  
public:  
  void init(std::shared_ptr<bool>& finish, 
            const std::shared_ptr<std::mutex>& bulkQueue, 
            const std::shared_ptr<std::condition_variable>& newBulk);
   
  void launch();
  void finalize();
  virtual ~BaseThreadHandler();
};


#include <sstream>
#include "printHandler.h"

class ThreadSaver: public BaseThreadHandler{
protected:
  virtual void handleBulk(Bulk& bulk) override final
  {  
    std::stringstream name;
    name << "thread(" << std::this_thread::get_id() << ")-" << PrintHandler::genName(bulk);
    std::ofstream log(name.str(), std::ofstream::app);

    log << "bulk: ";
    print(log, bulk);
  //  this->print(std::cout, bulk);   
  }
public:
  virtual ~ThreadSaver(){};
};



#include "BaseThreadHandler_impl.h"
#endif /* THREADSAVER_H */

