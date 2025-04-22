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
        size_t pos, // ������� � �����
        Chunk& chunk);


    // ����� ��������� ���������� �����. 
    // ��������� ���������� � ����������� ����������� ���������.
    std::vector<uint8_t> GetFile(uint32_t fileId); // ��� ���������� ������? 

private: 
    std::unordered_map<uint32_t, std::pair<std::unordered_map<int, Chunk>, size_t>> files; // + convert to std::map
    std::unordered_map<uint32_t, std::mutex*> tablesMx; 
    //std::unordered_map<uint32_t, std::mutex*> chunksMx; 
};
