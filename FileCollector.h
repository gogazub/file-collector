#pragma once
#include<vector>
#include<unordered_map>
#include<mutex>

using Chunk = std::vector<uint8_t>;

class FileCollector
{
public:
    void CollectFile(uint32_t fileId, size_t fileSize);


    void OnNewChunk(
        uint32_t fileId,
        size_t pos, // позиция в файле
        Chunk& chunk);


    // Метод получения собранного файла. 
    // Требуется предложить и реализовать оптимальный интерфейс.
    std::vector<uint8_t> GetFile(uint32_t fileId); // Как передавать данные? 
    friend void InsertChunk(int i, Chunk chunk, std::vector<uint8_t>& file, std::mutex& fileMx);
    friend void FileFilling(const std::unordered_map<int, Chunk>& table, size_t fileSize, std::vector<uint8_t>& file, uint32_t fileId);



private: 
    std::unordered_map<uint32_t, std::pair<std::unordered_map<int, Chunk>, size_t>> files; // + convert to std::map
    std::unordered_map<uint32_t, std::mutex> tablesMx; 
    std::unordered_map<uint32_t, std::mutex> fileMx;  // file lock to parallel insert 
    
};
