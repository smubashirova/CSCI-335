//  Author: Shokhsanam Mubashirova
//  Course: CSCI-335
//  Instructor: Prof. Justin Tojeira
//  Date: 02/26/2025
//  Assignment: Project 1
//  Description: This program implements Inventory class

#include "Player.hpp"

/**
    * @brief Constructs a new Player object.
    * @param name A const reference to the player's name.
    * @param inventory A const reference to the player's inventory.
*/
Player::Player(const std::string& name, const Inventory& inventory)
    : name_(name), inventory_(inventory) {} // Initialize name and inventory

/**
    * @brief Gets the player's name.
    * @return std::string The player's name.
*/
std::string Player::getName() const {
    return name_; // Return the player's name
}

/**
    * @brief Gets a reference to the player's inventory.
    * @return Inventory& A reference to the player's inventory.
*/
Inventory& Player::getInventoryRef() {
    return inventory_; // Return a reference to the player's inventory
}

/**
    * @brief Copy constructor.
    * @param rhs The Player object to copy from.
*/
Player::Player(const Player& rhs)
    : name_(rhs.name_), inventory_(rhs.inventory_) {} // Copy name and inventory from rhs

/**
    * @brief Move constructor.
    * @param rhs The Player object to move from.
    * @post The rhs object is left in a valid but empty state.
*/
Player::Player(Player&& rhs)
    : name_(std::move(rhs.name_)), inventory_(std::move(rhs.inventory_)) {
    // Reset rhs to a valid but empty state
    rhs.name_ = ""; // Clear the name of rhs
}

/**
    * @brief Copy assignment operator.
    * @param rhs The Player object to copy from.
    * @return Player& A reference to the assigned Player object.
    * @post The current object is updated to be a copy of rhs.
*/
Player& Player::operator=(const Player& rhs) {
    if (this != &rhs) {
        name_ = rhs.name_; // Copy the name from rhs
        inventory_ = rhs.inventory_; // Copy the inventory from rhs
    }
    return *this; // Return *this to allow chained assignment
}

/**
    * @brief Move assignment operator.
    * @param rhs The Player object to move from.
    * @return Player& A reference to the assigned Player object.
    * @post The current object is updated to be a moved version of rhs, and rhs is left in a valid but empty state.
*/
Player& Player::operator=(Player&& rhs) {
    if (this != &rhs) {
        name_ = std::move(rhs.name_); // Move the name from rhs
        inventory_ = std::move(rhs.inventory_); // Move the inventory from rhs

        // Reset rhs to a valid but empty state
        rhs.name_ = ""; // Clear the name of rhs
    }
    return *this; // Return *this to allow chained assignment
}
