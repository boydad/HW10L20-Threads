#pragma once

BaseHandler::BaseHandler(): bulkBuffer(nullptr) 
{}

void BaseHandler::set(const std::shared_ptr<std::queue<Bulk>>& bulkBuffer)
{
  this->bulkBuffer = bulkBuffer;
}

void BaseHandler::handle()
{
  while(!bulkBuffer->empty()){
    Bulk bulk = this->extractBulk();    
    this->handleBulk(bulk);
  }
}

Bulk BaseHandler::extractBulk()
{
  Bulk bulk = std::move(bulkBuffer->front());      
  bulkBuffer->pop();
  return bulk;
}