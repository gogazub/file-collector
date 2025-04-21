#include "FileCollector.h"
#include<iostream> 


void FileCollector::CollectFile(uint32_t fileId, size_t fileSize) {
	std::unordered_map<int, Chunk> chunkTable;
	this->files[fileId] = { chunkTable, fileSize };
}
void FileCollector::OnNewChunk(uint32_t fileId, size_t pos, Chunk& chunk) {

	if (files.find(fileId) == files.end()) {
		std::cerr << "No such fileId!" << std::endl;
		return;
	}
	
	auto* table = &files[fileId].first;

	if (table->find(pos) != table->end()) return; // skip dublicate 
	(*table)[pos] = chunk; // +optimaze copy 
}

void InsertChunk(int i, int& curPos, Chunk chunk, std::vector<uint8_t>& file) {
	for (int j = (curPos - i); j < chunk.size(); ++j) {
		file[curPos++] = chunk[j];
	}
}

void FileFilling(const std::unordered_map<int, Chunk>& table, size_t fileSize, std::vector<uint8_t>& file) {
	int curPos = 0;
	for (int i = 0; (i<fileSize) && curPos<fileSize; ++i) {
		if (table.find(i) == table.end()) continue;

		Chunk chunk = table.at(i);
		if (curPos < i) curPos = i;
	
		InsertChunk(i,curPos, chunk, file);
	}
}

std::vector<uint8_t> FileCollector::GetFile(uint32_t fileId){
	if (files.find(fileId) == files.end()) {
		std::cerr << "No such file with fileId = " << fileId << '!' << std::endl;
		return std::vector<uint8_t>();
	}

	auto* table = &files[fileId].first;
	size_t fileSize = files[fileId].second;
	
	std::vector<uint8_t> file(fileSize);
	
	/*for (int i = 0; i < fileSize; ++i) {
		auto* chank = &table[i];
	}*/
	FileFilling(*table, fileSize, file);
	return file;
}




/*
1 2 3 4 
  2 3 4 5
    3 4 5 6
O(NM) -> O(N^2)

1 2 3 4 
        5 
		  6



*/