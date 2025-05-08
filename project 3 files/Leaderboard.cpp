#include "Leaderboard.hpp"
#include <algorithm>
#include <chrono>

using namespace std::chrono;

/**
 * @brief Constructor for RankingResult with top players, cutoffs, and elapsed time.
 *
 * @param top Vector of top-ranked Player objects, in sorted order.
 * @param cutoffs Map of player count thresholds to minimum level cutoffs.
 *   NOTE: This is only ever non-empty for Online::rankIncoming().
 *         This parameter & the corresponding member should be empty
 *         for all Offline algorithms.
 * @param elapsed Time taken to calculate the ranking, in seconds.
*/
RankingResult::RankingResult(const std::vector<Player>& top,
                             const std::unordered_map<size_t, size_t>& cutoffs,
                             double elapsed)
    : top_(top), cutoffs_(cutoffs), elapsed_(elapsed) {}

namespace Offline {

RankingResult heapRank(std::vector<Player>& players) {
    auto start = high_resolution_clock::now();
    
    //Calculate the number of top players to select
    size_t top_count = std::floor(0.1 * players.size());
    std::vector<Player> top;
    
    if (top_count > 0) {
        // Create a min-heap of the first top_count elements
        std::make_heap(players.begin(), players.begin() + top_count, std::greater<Player>());

        // For remaining elements, compare with root of heap
        for (size_t i = top_count; i < players.size(); i++) {
            if (players[i] > players[0]) {
                std::swap(players[0], players[i]);
                std::make_heap(players.begin(), players.begin() + top_count, std::greater<Player>());
            }
        }

        // Extract and sort the top elements
        top.assign(players.begin(), players.begin() + top_count);
        std::sort(top.begin(), top.end());
    }

    auto end = high_resolution_clock::now();
    return RankingResult(top, {}, duration_cast<milliseconds>(end - start).count());
}

/**
 * @brief Partitions players around pivot for quickselect algorithm.
 *
 * @param players Vector to partition.
 * @param left Start index of partition range.
 * @param right End index of partition range.
 * @return Index of pivot after partitioning.
*/
size_t partition(std::vector<Player>& players, size_t left, size_t right) {
    Player pivot = players[right];
    size_t i = left;
    
    for (size_t j = left; j < right; j++) {
        if (players[j] > pivot) {
            std::swap(players[i], players[j]);
            i++;
        }
    }
    std::swap(players[i], players[right]);
    return i;
}

RankingResult quickSelectRank(std::vector<Player>& players) {
    auto start = high_resolution_clock::now();
    
    size_t top_count = std::floor(0.1 * players.size());
    std::vector<Player> top;
    
    if (top_count > 0) {
        size_t left = 0;
        size_t right = players.size() - 1;
        
        //Quickselect algorithm to find the top elements
        while (left <= right) {
            size_t pivot_idx = partition(players, left, right);
            
            if (pivot_idx == top_count - 1) {
                break;
            } else if (pivot_idx < top_count - 1) {
                left = pivot_idx + 1;
            } else {
                right = pivot_idx - 1;
            }
        }
        
        // Extract and sort the top elements
        top.assign(players.begin(), players.begin() + top_count);
        std::sort(top.begin(), top.end());
    }

    auto end = high_resolution_clock::now();
    return RankingResult(top, {}, duration_cast<milliseconds>(end - start).count());
}

} // namespace Offline

namespace Online {

void replaceMin(PlayerIt first, PlayerIt last, Player& target) {
    if (first == last) return;
    
    //Replace the minimum element with the new target
    *first = std::move(target);
    auto heap_size = std::distance(first, last);
    auto current = 0;
    
    while (true) {
        auto left = 2 * current + 1;
        auto right = 2 * current + 2;
        auto smallest = current;
        
        // Find the smallest element among current and its children
        if (left < heap_size && *(first + left) < *(first + smallest)) {
            smallest = left;
        }
        if (right < heap_size && *(first + right) < *(first + smallest)) {
            smallest = right;
        }
        
        if (smallest == current) break;
        
        std::swap(*(first + current), *(first + smallest));
        current = smallest;
    }
}

RankingResult rankIncoming(PlayerStream& stream, const size_t& reporting_interval) {
    auto start = high_resolution_clock::now();
    std::vector<Player> top_players;
    std::unordered_map<size_t, size_t> cutoffs;
    size_t count = 0;
    
    //Process players from the stream
    while (stream.remaining() > 0) {
        Player player = stream.nextPlayer();
        count++;
        //Maintain top players in a min-heap
        if (top_players.size() < reporting_interval) {
            top_players.push_back(std::move(player));
            if (top_players.size() == reporting_interval) {
                std::make_heap(top_players.begin(), top_players.end(), std::greater<Player>());
            }
        } 
        else if (player > top_players.front()) {
            replaceMin(top_players.begin(), top_players.end(), player);
        }
        
        // Record cutoff at intervals and at the end
        if (count % reporting_interval == 0 || stream.remaining() == 0) {
            if (!top_players.empty()) {
                cutoffs[count] = top_players.front().level_;
            }
        }
    }
    
    // Sort the top players in ascending order
    std::sort(top_players.begin(), top_players.end());
    
    auto end = high_resolution_clock::now();
    return RankingResult(top_players, cutoffs, duration_cast<milliseconds>(end - start).count());
}

} // namespace Online