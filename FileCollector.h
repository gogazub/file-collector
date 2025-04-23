#pragma once
#include<vector>
#include<unordered_map>
#include<mutex>
#include<map>
#include"File.h"
using Chunk = std::vector<uint8_t>;

class FileCollector
{
public:
    void CollectFile(uint32_t fileId, size_t fileSize);

    void OnNewChunk(
        uint32_t fileId,
        size_t pos, // ������� � �����
        Chunk& chunk);


    // ����� ��������� ���������� �����. 
    // ��������� ���������� � ����������� ����������� ���������.
    std::vector<uint8_t> GetFile(uint32_t fileId); // ��� ���������� ������? 
  
private: 
    std::map<uint32_t, File> idToFile;    
};
