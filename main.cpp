#include"FileCollector.h"
#include<gtest/gtest.h>
#include"Sender.h"

void RunFileSendingBenchmark() {

		Sender sender;

	std::unordered_map<uint32_t, fileVector> idToFile;

	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<unsigned int> byteDist(0, 255); 

	for (uint32_t fileId = 1; fileId <= 5; ++fileId) {
		size_t fileSize = 1000000 + fileId * 100; 
		fileVector file(fileSize);
		std::generate(file.begin(), file.end(), [&]() { return static_cast<uint8_t>(byteDist(rng)); }); 

		idToFile[fileId] = std::move(file);
	}

	sender.SendManyFiles(std::move(idToFile));
}

int main(int argc, char **argv) {
	
	//testing::InitGoogleTest(&argc, argv);
	//return RUN_ALL_TESTS();
	for (int i = 0; i < 10; ++i) {

		RunFileSendingBenchmark();
		std::cout << std::endl;
	}
	return 0;

}
