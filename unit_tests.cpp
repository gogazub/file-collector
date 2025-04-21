#pragma once
#include <gtest/gtest.h>
#include "FileCollector.h"
#include <vector>



TEST(FileCollectorTest, CollectFileCreatesFileEntry) {
    FileCollector collector;

    uint32_t fileId = 1;
    size_t fileSize = 100;

    collector.CollectFile(fileId, fileSize);

    // �������� ���� � ��������� ��� ������
    std::vector<uint8_t> file = collector.GetFile(fileId);
    EXPECT_EQ(file.size(), fileSize);
}

TEST(FileCollectorTest, OnNewChunkAddsChunksCorrectly) {
    FileCollector collector;

    uint32_t fileId = 1;
    size_t fileSize = 100;
    collector.CollectFile(fileId, fileSize);

    // ������ ����
    Chunk chunk1(10, 1);  // ���� �� 10 ���������, ����������� ���������
    collector.OnNewChunk(fileId, 0, chunk1); // ��������� �� ������� 0

    // �������� ���� � ��������� ��� ����������
    std::vector<uint8_t> file = collector.GetFile(fileId);
    for (size_t i = 0; i < 10; ++i) {
        EXPECT_EQ(file[i], 1);
    }

    // ��������� ������ ����
    Chunk chunk2(10, 2);
    collector.OnNewChunk(fileId, 10, chunk2);

    // ���������, ��� ������ ���� ��������� ���������
    file = collector.GetFile(fileId);
    for (size_t i = 10; i < 20; ++i) {
        EXPECT_EQ(file[i], 2);
    }
}

TEST(FileCollectorTest, OnNewChunkHandlesDuplicateChunks) {
    FileCollector collector;

    uint32_t fileId = 1;
    size_t fileSize = 100;
    collector.CollectFile(fileId, fileSize);

    Chunk chunk1(10, 1);
    collector.OnNewChunk(fileId, 0, chunk1);  // ��������� ����

    // �������� �������� ��� �� ���� ��� ���, �� �� ������ �����������
    collector.OnNewChunk(fileId, 0, chunk1);

    std::vector<uint8_t> file = collector.GetFile(fileId);
    // ���������, ��� ���� ��� �������� ������ ���� ���
    for (size_t i = 0; i < 10; ++i) {
        EXPECT_EQ(file[i], 1);
    }
}

TEST(FileCollectorTest, GetFileHandlesNonExistentFile) {
    FileCollector collector;

    uint32_t nonExistentFileId = 999;
    std::vector<uint8_t> file = collector.GetFile(nonExistentFileId);
    // ���������, ��� ��������� ������ ������
    EXPECT_TRUE(file.empty());
}

TEST(FileCollectorTest, LargeDataTest) {
    FileCollector collector;

    uint32_t fileId = 1;
    size_t fileSize = 1000000;  // ���� ������� ����
    collector.CollectFile(fileId, fileSize);

    // ��������� ������ � ����
    Chunk chunk(1000, 1); // ���� �������� 1000 ����, ����������� ���������
    for (size_t i = 0; i < fileSize / chunk.size(); ++i) {
        collector.OnNewChunk(fileId, i * chunk.size(), chunk);
    }

    // �������� ���� � ��������� ��� ������
    std::vector<uint8_t> file = collector.GetFile(fileId);
    EXPECT_EQ(file.size(), fileSize);

    // ���������, ��� ��� ������ � ����� ����� 1 (��� ���� ������� �� ������)
    for (size_t i = 0; i < file.size(); ++i) {
        EXPECT_EQ(file[i], 1);
    }
}
