#pragma once
#include<vector>

class FileCollector
{
public:
    void CollectFile(uint32_t fileId, size_t fileSize);


    void OnNewChunk(
        uint32_t fileId,
        size_t pos, // ������� � �����
        std::vector<uint8_t>&& chunk);


    // ����� ��������� ���������� �����. 
    // ��������� ���������� � ����������� ����������� ���������.
    std::vector<uint8_t> GetFile(uint32_t fileId); // ��� ���������� ������? 
};
