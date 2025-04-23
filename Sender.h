#pragma once
#include"FileCollector.h"
using fileVector = std::vector<uint8_t>;
class Sender {
public:
	void SendFile(fileVector file, uint32_t fileId);
	void SendManyFiles(std::unordered_map<uint32_t, fileVector> idToFile);
	std::vector<std::pair<size_t, Chunk>> SplitIntoChunks(const fileVector& file, size_t chunkCount, bool sameSize);
private:
	FileCollector collector;
	std::mutex coutMx;
};