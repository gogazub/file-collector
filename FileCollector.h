#pragma once
#include<unordered_map>
#include<mutex>
#include<random>
#include<algorithm>
#include<future>
#include<iostream> 
#include<thread>
#include<future>
#include"File.h"
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
    std::vector<uint8_t>& GetFile(uint32_t fileId); // Как передавать данные? 
    const std::vector<uint8_t>& GetFileReadOnly(uint32_t fileId);
private: 
    std::map<uint32_t, File> idToFile;    
};
