#include "FileCollector.h"


void FileCollector::CollectFile(uint32_t fileId, size_t fileSize) {
	File newFile(fileId, fileSize); // add lock
	this->idToFile[fileId] = std::move(newFile); // add std::move
}


// Будем принимать чанк по ссылке, так как хотим только скопировать его часть.
// Сложность такого алгоритма сборки файла O(N). 
void FileCollector::OnNewChunk(uint32_t fileId, size_t pos, Chunk& chunk) {


	if (!idToFile.count(fileId)) {
		std::cerr << "No such file with Id:" << fileId << std::endl;
		return;
	}

	auto& coverMap = idToFile[fileId].GetCoverMap();
	std::vector<uint8_t>& buffer = idToFile[fileId].GetFile();
	auto uncovered = coverMap.AddAndGetUncovered(pos, pos + chunk.size());
	
	for (const auto& interval : uncovered) {
		auto start = interval.first;
		auto end = interval.second;
		size_t offset = start - pos;
		std::copy(chunk.begin() + offset, chunk.begin() + offset + (end - start),
			buffer.begin() + start);
	}
}


std::vector<uint8_t>& FileCollector::GetFile(uint32_t fileId) {
	return idToFile[fileId].GetFile();
}

const std::vector<uint8_t>& FileCollector::GetFileReadOnly(uint32_t fileId){
	return idToFile[fileId].GetFile();
	
}


