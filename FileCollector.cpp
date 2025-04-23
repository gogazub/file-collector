#include "FileCollector.h"
#include<iostream> 
#include<thread>
#include<mutex>
#include<future>

void FileCollector::CollectFile(uint32_t fileId, size_t fileSize) {
	File newFile(fileId, fileSize); // add lock
	this->idToFile[fileId] = newFile; // add std::move
}

void FileCollector::OnNewChunk(uint32_t fileId, size_t pos, Chunk& chunk) {

	auto coverMap = idToFile[fileId].GetCoverMap();
	std::vector<uint8_t>& buffer = idToFile[fileId].GetFile();
	auto uncovered = coverMap.AddAndGetUncovered(pos, pos + chunk.size());
	
	for (const auto& pair : uncovered) {
		auto start = pair.first;
		auto end = pair.second;
		size_t offset = start - pos;
		std::copy(chunk.begin() + offset, chunk.begin() + offset + (end - start),
			buffer.begin() + start);
	}
}


// Сделать многопоточным. Написать тест на параллельный сбор множества файлов
std::vector<uint8_t> FileCollector::GetFile(uint32_t fileId) {
	return idToFile[fileId].GetFile();
}


