#include "FileCollector.h"
#include<iostream> 
#include<thread>
#include<mutex>

std::mutex filesMx;
std::mutex cerrMx;
std::mutex mutexesMx;
int MAX_THREADS = 12;


void FileCollector::CollectFile(uint32_t fileId, size_t fileSize) {
	
	std::unordered_map<int, Chunk> chunkTable;

	// ����� files
	std::unique_lock<std::mutex> filesLock(filesMx);
	if (files.find(fileId) != files.end()) {
		std::lock_guard<std::mutex> cerrLock(cerrMx);
		std::cerr << "file with ID: " << fileId << " already exists" << std::endl;
		//filesLock.unlock();
		return;
	}

	this->files[fileId] = { chunkTable, fileSize };
	// ����������� files 
	filesLock.unlock();
	//����� mutexes 
	std::unique_lock<std::mutex> mutexesLock(mutexesMx);
	std::mutex* mtx = new std::mutex();
	
	this->tablesMx[fileId] = mtx;
	// ����������� mutexes
	return;
}

void FileCollector::OnNewChunk(uint32_t fileId, size_t pos, Chunk& chunk) {

	// ����� files
	std::unique_lock<std::mutex> filesLock(filesMx);
	if (files.find(fileId) == files.end()) {
		std::lock_guard<std::mutex> cerrLock(cerrMx);
		std::cerr << "No such fileId!" << std::endl;
		return;
	} 
	
	// ��������� �������, �� ������ ������ ����� files[fileId]
	auto* table = &files[fileId].first;
	// ����������� files
	filesLock.unlock();
	// ����� table 

	if (table->find(pos) != table->end()) return; // skip dublicate 
	(*table)[pos] = chunk; // +optimaze copy 
	// ����������� table 
}



void InsertChunk(int i, Chunk chunk, std::vector<uint8_t>& file) {

	for (int j = 0; j < chunk.size(); ++j) {
		//lock file �� fileId
		if (file[i + j] != 0) return; // ���� ��� �������� 0 � ����� ����� n/2, �� O(N^2)
		file[i+j] = chunk[j];
	}
}

void FileFilling(const std::unordered_map<int, Chunk>& table, size_t fileSize, std::vector<uint8_t>& file) {
	//table locked
	std::mutex fileMx;
	std::vector<std::thread> threads;
	for(const auto& pair : table) {
		const auto i = pair.first;
		const auto chunk = pair.second;
		
		threads.emplace_back([i, chunk, &file, &fileMx]() {
			//std::lock_guard<std::mutex> fileLock(fileMx); 
			InsertChunk(i, chunk, file);
			}
		);
	}
	for (auto& th : threads) th.join();
}

std::vector<uint8_t> FileCollector::GetFile(uint32_t fileId) {
	
	// lock files
	std::unique_lock<std::mutex> filesLock(filesMx);
	if (files.find(fileId) == files.end()) {
		// filesLock.unlock()
		std::lock_guard<std::mutex> cerrLock(cerrMx);
		std::cerr << "No such file with fileId = " << fileId << '!' << std::endl;
		return std::vector<uint8_t>();
	}
	//filesLock.lock()
	auto* table = &files[fileId].first;
	size_t fileSize = files[fileId].second;

	//unlock files
	filesLock.unlock();

	std::vector<uint8_t> file(fileSize);
	//lock table (������ ��������� ����� �� ����� ����� + ��������� ������� �� OnNewChunk)
	std::lock_guard<std::mutex> tableLock(*tablesMx[fileId]); 
	FileFilling(*table, fileSize, file);
	// unlock table 
	return file;
}
