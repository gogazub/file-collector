#include <map>
#include <vector>
#include <utility>
#include"IntervalCoverMap.h"
 

std::vector<std::pair<size_t, size_t>> IntervalCoverMap::AddAndGetUncovered(size_t start, size_t end) {
        std::vector<std::pair<size_t, size_t>> uncovered;

        if (start >= end) return uncovered;

        auto it = covered.lower_bound(start);
        if (it != covered.begin()) --it;

        // Найдём все покрытые интервалы, которые пересекаются с [start, end)
        std::vector<std::map<size_t, size_t>::iterator> toErase;

        size_t newStart = start;
        size_t newEnd = end;

        while (it != covered.end() && it->first < end) {
            size_t curStart = it->first;
            size_t curEnd = it->second;

            if (curEnd < start) {
                ++it;
                continue;
            }

            // Сохраняем новые границы покрытия
            newStart = std::min(newStart, curStart);
            newEnd = std::max(newEnd, curEnd);

            // Найдём непокрытую часть перед текущим покрытым интервалом
            if (start < curStart) {
                uncovered.emplace_back(start, std::min(curStart, end));
            }

            // Сдвигаем начало интервала дальше текущего покрытого
            start = std::max(start, curEnd);

            toErase.push_back(it);
            ++it;
        }

        // Ещё осталась непокрытая часть в конце
        if (start < end) {
            uncovered.emplace_back(start, end);
        }

        // Удалим старые покрытые и добавим объединённый
        for (auto& e : toErase) covered.erase(e);
        covered[newStart] = newEnd;

        return uncovered;
    }


