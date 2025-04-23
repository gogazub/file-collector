#include"FileCollector.h"
#include<gtest/gtest.h>
#include"Sender.h"

void RunFileSendingBenchmark() {

		Sender sender;

	// Создаём несколько "файлов" с байтами
	std::unordered_map<uint32_t, fileVector> idToFile;

	// Случайный генератор
	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<unsigned int> byteDist(0, 255); // <= заменили uint8_t на unsigned int

	for (uint32_t fileId = 1; fileId <= 5; ++fileId) {
		size_t fileSize = 1000000 + fileId * 100; // размер увеличивается
		fileVector file(fileSize);
		std::generate(file.begin(), file.end(), [&]() { return static_cast<uint8_t>(byteDist(rng)); }); // <= каст к uint8_t

		idToFile[fileId] = std::move(file);
	}

	// Запускаем параллельную отправку файлов
	sender.SendManyFiles(std::move(idToFile));
}

int main(int argc, char **argv) {
	
	//testing::InitGoogleTest(&argc, argv);
	//return RUN_ALL_TESTS();


	RunFileSendingBenchmark();
	return 0;

}
