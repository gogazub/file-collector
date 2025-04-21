#include<vector>
#include<assert.h>
#include<iostream>
#include"FileCollector.h"

void GetFileTest() {
	FileCollector fileCollector;
	fileCollector.CollectFile(1, 10);
	std::vector<Chunk> chunkArr;
	chunkArr.push_back({ 1,2,3,4 });
	chunkArr.push_back({ 2,3,4,5 });
	/*chunkArr.push_back({ 3,4,5,6 });
	chunkArr.push_back({ 4,5,6,7 });
	chunkArr.push_back({ 5,6,7,8 });
	chunkArr.push_back({ 6,7,8,9 });*/
	//chunkArr.push_back({ 7,8,9,10 });
	
	
	for (int i = 0; i < chunkArr.size(); ++i) {
		fileCollector.OnNewChunk(1, i, chunkArr[i]);
	}
	Chunk v = { 7,8,9,10 };
	fileCollector.OnNewChunk(1, 6, v);
	auto file = fileCollector.GetFile(1);
	
	return;

}
int main() {
	GetFileTest();
}
