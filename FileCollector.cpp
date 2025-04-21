#include "FileCollector.h"

void FileCollector::CollectFile(uint32_t fileId, size_t fileSize) {

}

void FileCollector::OnNewChunk(uint32_t fileId, size_t pos, std::vector<uint8_t>&& chunk) {

}

std::vector<uint8_t> FileCollector::GetFile(uint32_t fileId)
{
	return std::vector<uint8_t>();
}
