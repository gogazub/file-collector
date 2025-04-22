#include "FileCollector.h"
#include<iostream> 
#include<thread>
#include<mutex>
#include<future>
std::mutex filesMx; 
std::mutex cerrMx;
std::mutex mutexesMx;


void FileCollector::CollectFile(uint32_t fileId, size_t fileSize) {
	
	std::unordered_map<int, Chunk> chunkTable;

	std::unique_lock<std::mutex> filesLock(filesMx);	// ����� files
	if (files.find(fileId) != files.end()) {
		std::lock_guard<std::mutex> cerrLock(cerrMx);
		std::cerr << "file with ID: " << fileId << " already exists" << std::endl;
		//filesLock.unlock();
		return;
	}

	this->files[fileId] = { chunkTable, fileSize };
	filesLock.unlock();		// ����������� files 

	std::unique_lock<std::mutex> mutexesLock(mutexesMx);	//����� mutexes 
	this->tablesMx[fileId];
	this->fileMx[fileId];
	// ����������� mutexes
	return;
}


// ���������� ���������� ������. �������� ���������� ������ ��� ������� ��������������
void FileCollector::OnNewChunk(uint32_t fileId, size_t pos, Chunk& chunk) {

	std::unique_lock<std::mutex> filesLock(filesMx);	// ����� files

	if (files.find(fileId) == files.end()) {
		//filesLock.unlock();
		std::lock_guard<std::mutex> cerrLock(cerrMx);
		std::cerr << "No such fileId!" << std::endl;

		return;
	} 

	auto* table = &files[fileId].first;	// ��������� �������, �� ������ ������ ����� files[fileId]
	
	filesLock.unlock();// ����������� files

	std::lock_guard<std::mutex> tableLock(tablesMx[fileId]);// ����� table 

	if (table->find(pos) != table->end()) return;  
	(*table)[pos] = chunk; // +optimaze copy  +skip duplicate
}


// ����������� �������� file � ��������� ���������� ��� ����������� ��������� �� UB ��� ������������� ������ (����� ������� ����� �����������������)
void InsertChunk(int i, Chunk chunk, std::vector<uint8_t>& file,std::mutex* fileMx_) {

	// ���� ��� �� ���� ����������� ��������� �������� �� ������ ������, �� ���������� �� UB 
	std::lock_guard<std::mutex> fileLock(*fileMx_);		// lock file to insert
	for (int j = 0; j < chunk.size(); ++j) {
		
		// ��� ���� �������� UB, ���� � ������� ����� ����������
		if (file[i + j] != 0) return; // ���� ��� �������� 0 � ����� ����� n/2, �� O(N^2)
		file[i+j] = chunk[j];
	}
}


void FileFilling(const std::unordered_map<int, Chunk>& table, size_t fileSize, std::vector<uint8_t>& file, std::mutex* fileMx_) {
	//table locked
	
	// 430-450 ms
	//std::vector<std::thread> threads;
	//for(const auto& pair : table) {
	//	const auto i = pair.first;
	//	const auto chunk = pair.second;
	//	
	//	threads.emplace_back([i, chunk, &file, &fileMx]() {
	//		//std::lock_guard<std::mutex> fileLock(fileMx); 
	//		InsertChunk(i, chunk, file);
	//		}
	//	);
	//}
	//for (auto& th : threads) th.join();

	//  250 ms 

	std::mutex fileMx;
	std::vector<std::future<void>> futures;
	
	for (const auto& pair : table) {
		const auto i = pair.first;
		const auto chunk = pair.second;
		
		
		auto fut = std::async(std::launch::async, [i, chunk, &file, &fileMx_]() {
			
			InsertChunk(i, chunk, file, fileMx_);
			});
		futures.push_back(std::move(fut));
	}

	for (auto& f : futures) f.get();
}

// ������� �������������. �������� ���� �� ������������ ���� ��������� ������
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
	std::lock_guard<std::mutex> tableLock(tablesMx[fileId]);
	std::mutex* fileMx_ = &this->fileMx[fileId];
	FileFilling(*table, fileSize, file, fileMx_);
	// unlock table 
	return file;
}


