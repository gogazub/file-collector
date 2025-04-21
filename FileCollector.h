#pragma once
#include<vector>

class FileCollector
{
public:
    void CollectFile(uint32_t fileId, size_t fileSize);


    void OnNewChunk(
        uint32_t fileId,
        size_t pos, // позиция в файле
        std::vector<uint8_t>&& chunk);


    // Метод получения собранного файла. 
    // Требуется предложить и реализовать оптимальный интерфейс.
    std::vector<uint8_t> GetFile(uint32_t fileId); // Как передавать данные? 
};
