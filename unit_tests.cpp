#pragma once
#include <gtest/gtest.h>
#include "FileCollector.h"
#include <vector>



TEST(FileCollectorTest, CollectFileCreatesFileEntry) {
    FileCollector collector;

    uint32_t fileId = 1;
    size_t fileSize = 100;

    collector.CollectFile(fileId, fileSize);

    // Получаем файл и проверяем его размер
    std::vector<uint8_t> file = collector.GetFile(fileId);
    EXPECT_EQ(file.size(), fileSize);
}

TEST(FileCollectorTest, OnNewChunkAddsChunksCorrectly) {
    FileCollector collector;

    uint32_t fileId = 1;
    size_t fileSize = 100;
    collector.CollectFile(fileId, fileSize);

    // Создаём чанк
    Chunk chunk1(10, 1);  // Чанк из 10 элементов, заполненный единицами
    collector.OnNewChunk(fileId, 0, chunk1); // Добавляем на позицию 0

    // Получаем файл и проверяем его содержимое
    std::vector<uint8_t> file = collector.GetFile(fileId);
    for (size_t i = 0; i < 10; ++i) {
        EXPECT_EQ(file[i], 1);
    }

    // Добавляем второй чанк
    Chunk chunk2(10, 2);
    collector.OnNewChunk(fileId, 10, chunk2);

    // Проверяем, что второй чанк добавился корректно
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
    collector.OnNewChunk(fileId, 0, chunk1);  // Добавляем чанк

    // Пытаемся добавить тот же чанк ещё раз, он не должен повторяться
    collector.OnNewChunk(fileId, 0, chunk1);

    std::vector<uint8_t> file = collector.GetFile(fileId);
    // Проверяем, что чанк был добавлен только один раз
    for (size_t i = 0; i < 10; ++i) {
        EXPECT_EQ(file[i], 1);
    }
}

TEST(FileCollectorTest, GetFileHandlesNonExistentFile) {
    FileCollector collector;

    uint32_t nonExistentFileId = 999;
    std::vector<uint8_t> file = collector.GetFile(nonExistentFileId);
    // Проверяем, что возвращён пустой вектор
    EXPECT_TRUE(file.empty());
}

TEST(FileCollectorTest, LargeDataTest) {
    FileCollector collector;

    uint32_t fileId = 1;
    size_t fileSize = 1000000;  // Один миллион байт
    collector.CollectFile(fileId, fileSize);

    // Добавляем данные в чанк
    Chunk chunk(1000, 1); // Чанк размером 1000 байт, заполненный единицами
    for (size_t i = 0; i < fileSize / chunk.size(); ++i) {
        collector.OnNewChunk(fileId, i * chunk.size(), chunk);
    }

    // Получаем файл и проверяем его размер
    std::vector<uint8_t> file = collector.GetFile(fileId);
    EXPECT_EQ(file.size(), fileSize);

    // Проверяем, что все данные в файле равны 1 (наш чанк состоит из единиц)
    for (size_t i = 0; i < file.size(); ++i) {
        EXPECT_EQ(file[i], 1);
    }
}
