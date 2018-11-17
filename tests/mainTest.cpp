#include <gtest/gtest.h>
 
#include <math.h>
#include "commandManager.h"
 
 


class TestBulk : public ::testing::Test
{
protected:
	void SetUp(){		  
		const int bulkSize = 3;
		bulk.reset(new CommandManager(bulkSize));
		printer = std::make_shared<PrintHandler>();
		
		bulk->subscribe(printer);		
	}
	void TearDown(){
		
	}
	
	std::unique_ptr<CommandManager> bulk;
	std::shared_ptr<PrintHandler> printer;
};


TEST_F(TestBulk, taskCase1){	
	testing::internal::CaptureStdout();
	
	bulk->add("1");	
	bulk->add("2");	
	bulk->add("3");	
	bulk->add("4");
	bulk->add("5");
		
		auto tmp = bulk.release();
	delete tmp;
	
	const std::string expected("bulk: 1, 2, 3\nbulk: 4, 5\n");
	std::string stdoutput = testing::internal::GetCapturedStdout();
	
	ASSERT_EQ(expected, stdoutput);	
}

TEST_F(TestBulk, taskCase2){
	testing::internal::CaptureStdout();
	
	bulk->add("1");
	bulk->add("2");
	bulk->add("3");
	bulk->add("{");
	bulk->add("4");
	bulk->add("5");
	bulk->add("6");
	bulk->add("7");
	bulk->add("}");
	
	auto tmp = bulk.release();
	delete tmp;
	
	const std::string expected("bulk: 1, 2, 3\nbulk: 4, 5, 6, 7\n");
	std::string stdoutput = testing::internal::GetCapturedStdout();
	
	ASSERT_EQ(expected, stdoutput);	
}

TEST_F(TestBulk, taskCase3){
	testing::internal::CaptureStdout();
	
	bulk->add("{");
	bulk->add("1");
	bulk->add("2");
	bulk->add("{");
	bulk->add("3");	
	bulk->add("4");
	bulk->add("}");
	bulk->add("5");
	bulk->add("6");
	bulk->add("}");
	
	auto tmp = bulk.release();
	delete tmp;
	
	const std::string expected("bulk: 1, 2, 3, 4, 5, 6\n");
	std::string stdoutput = testing::internal::GetCapturedStdout();
	
	ASSERT_EQ(expected, stdoutput);	
}

TEST_F(TestBulk, taskCase4){
	testing::internal::CaptureStdout();
	
	bulk->add("1");
	bulk->add("2");
	bulk->add("3");
	bulk->add("{");	
	bulk->add("4");
	bulk->add("5");
	bulk->add("6");
	
	auto tmp = bulk.release();
	delete tmp;
	
	const std::string expected("bulk: 1, 2, 3\n");
	std::string stdoutput = testing::internal::GetCapturedStdout();
	
	ASSERT_EQ(expected, stdoutput);	
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}