#pragma once
#include<vector>
#include<map>
class IntervalCoverMap {
public:
    // ��������� ����� �������� [start, end) � ���������� ������ ���������� �����������
    std::vector<std::pair<size_t, size_t>> AddAndGetUncovered(size_t start, size_t end);

private:
    // std::map: key � ����� ������� ���������, value � ������
    std::map<size_t, size_t> covered;
};
