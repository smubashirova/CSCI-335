#include "PlayerStream.hpp"
#include <stdexcept>

/**
 * @brief Constructs a VectorPlayerStream from an existing vector of Players.
 *
 * @param players The vector of Player objects to be streamed.
 * @post The stream is initialized to return players in their original order,
 *       starting from the first element.
*/
VectorPlayerStream::VectorPlayerStream(const std::vector<Player>& players) 
    : players_(players), current_(0) {}

/**
 * @brief Retrieves the next Player in the stream.
 *
 * @return The next Player object in the sequence.
 * @post The stream position advances to the next player.
 * @throws std::runtime_error if called when no more players are remaining.
*/    
Player VectorPlayerStream::nextPlayer() {
    if (current_ >= players_.size()) {
        throw std::runtime_error("No more players in stream");
    }
    return players_[current_++];
}

/**
 * @brief Returns the number of players remaining in the stream.
 *
 * @return The count of players left to be read from the stream.
 * @note Returns 0 when all players have been read.
*/
size_t VectorPlayerStream::remaining() const {
    return players_.size() - current_;
}