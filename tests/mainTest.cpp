#include <gtest/gtest.h>
 
#include <math.h>
#include <thread>
#include "commandManager.h"
#include "threadComManager.h"
 


class TestBulk : public ::testing::Test
{
protected:
	void SetUp(){		  
		const int bulkSize = 3;
    
    finish = false;  
    const int numThreads = 2;

    threads.clear();
    threads.reserve(numThreads);
  
    auto bulkQueue = std::make_shared<std::mutex>();
    auto newBulk = std::make_shared<std::condition_variable>();
    manager.reset(new ThreadComManager(bulkSize));
  
    auto threadSaver = [](std::shared_ptr<ThreadSaver> saver){
      saver->run();
    };

    for(int n=0; n<numThreads; ++n){
      auto printer = std::make_shared<ThreadSaver>();
      manager->subscribe(printer);
      threads.emplace_back(threadSaver, printer);
    }        
    
  }
	void TearDown(){
    
	}
	
  bool finish;
	std::unique_ptr<ThreadComManager> manager;
	std::shared_ptr<PrintHandler> printer;
  std::vector<std::thread> threads;
};


TEST_F(TestBulk, taskCase1){	
	testing::internal::CaptureStdout();
	
	manager->add("1");	
	manager->add("2");	
	manager->add("3");	
	manager->add("4");
	manager->add("5");
  finish = true;
  for(auto& th: threads)
      th.join();
  
		
	auto tmp = manager.release();
	delete tmp;
	
	const std::string expected("bulk: 1, 2, 3\nbulk: 4, 5\n");
	std::string stdoutput = testing::internal::GetCapturedStdout();
	
	ASSERT_EQ(expected, stdoutput);	
}

TEST_F(TestBulk, taskCase2){
	testing::internal::CaptureStdout();
	
	manager->add("1");
	manager->add("2");
	manager->add("3");
	manager->add("{");
	manager->add("4");
	manager->add("5");
	manager->add("6");
	manager->add("7");
	manager->add("}");
	finish = true;
  for(auto& th: threads)
      th.join();
  
	auto tmp = manager.release();
	delete tmp;
	
	const std::string expected("bulk: 1, 2, 3\nbulk: 4, 5, 6, 7\n");
	std::string stdoutput = testing::internal::GetCapturedStdout();
	
	ASSERT_EQ(expected, stdoutput);	
}

TEST_F(TestBulk, taskCase3){
	testing::internal::CaptureStdout();
	
	manager->add("{");
	manager->add("1");
	manager->add("2");
	manager->add("{");
	manager->add("3");	
	manager->add("4");
	manager->add("}");
	manager->add("5");
	manager->add("6");
	manager->add("}");
  finish = true;
  for(auto& th: threads)
      th.join();
	
	auto tmp = manager.release();
	delete tmp;
	
	const std::string expected("bulk: 1, 2, 3, 4, 5, 6\n");
	std::string stdoutput = testing::internal::GetCapturedStdout();
	
	ASSERT_EQ(expected, stdoutput);	
}

TEST_F(TestBulk, taskCase4){
	testing::internal::CaptureStdout();
	
	manager->add("1");
	manager->add("2");
	manager->add("3");
	manager->add("{");	
	manager->add("4");
	manager->add("5");
	manager->add("6");
  finish = true;
  for(auto& th: threads)
      th.join();
	
	auto tmp = manager.release();
	delete tmp;
	
	const std::string expected("bulk: 1, 2, 3\n");
	std::string stdoutput = testing::internal::GetCapturedStdout();
	
	ASSERT_EQ(expected, stdoutput);	
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}