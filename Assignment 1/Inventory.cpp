//  Author: Shokhsanam Mubashirova
//  Course: CSCI-335
//  Instructor: Prof. Justin Tojeira
//  Date: 02/26/2025
//  Assignment: Project 1
//  Description: This program implements Inventory class

#include "Inventory.hpp"
#include <stdexcept> // for std::out_of_range

/**
    *@brief Default constructor with optional parameters
    *@param items: 2D vector of items to initialize the inventory grid
    *@param equipped: pointer to the item to initialize the equipped item
    *@return inventory object
*/
Inventory::Inventory(
    const std::vector<std::vector<Item>>& items,
    Item* equipped)
    : inventory_grid_{items}, equipped_{equipped ? new Item(*equipped) : nullptr}, weight_{0.0f}, item_count_{0} {

    // Calculate the initial weight and item_count excluding NONE types
    for (const auto& row : inventory_grid_) {
        for (const auto& item : row) {
            if (item.type_ != NONE) {
                weight_ += item.weight_;
                ++item_count_;
            }
        }
    }
}

/**
    *@brief Get the equipped item
    *@return pointer to the equipped item
*/
Item* Inventory::getEquipped() const {
    return equipped_; // Return the equipped item
}

/**
    *@brief equip an item
    *@param itemtoEquip: pointer to the item to equip
    *@post eqipped item is set to the item to equip
*/
void Inventory::equip(Item* itemToEquip) {
    if (equipped_) {
        delete equipped_; // Delete the currently equipped item if it exists
    }
    equipped_ = itemToEquip ? new Item(*itemToEquip) : nullptr; // Equip the new item
}

/**
    *@brief discards the equipped item
    *@post the equipped item is set to nullptr
*/
void Inventory::discardEquipped() {
    if (equipped_) {
        delete equipped_; // Delete the currently equipped item
        equipped_ = nullptr; // Set equipped item to nullptr
    }
}

/**
    * @brief Gets all the items in the inventory grid.
    * @return std::vector<std::vector<Item>> A 2D vector of items in the inventory grid.
*/
std::vector<std::vector<Item>> Inventory::getItems() const {
    return inventory_grid_; // Return the inventory grid
}

/**
    * @brief Gets the total weight of items in the inventory. 
    * @return float The total weight of items.
*/
float Inventory::getWeight() const {
    return weight_; // Return the total weight of items
}

/**
    * @brief Gets the total count of non-NONE items.
    * @return size_t The count of non-NONE items.
*/
size_t Inventory::getCount() const {
    return item_count_; // Return the count of non-NONE items
}

/**
    * @brief Retrieves an item at a specified row and column. 
    * @param row The row index.
    * @param col The column index.
    * @return Item The item at the specified position.
    * @throws std::out_of_range if the row or column is out of range.
*/
Item Inventory::at(const size_t& row, const size_t& col) const {
    if (row >= inventory_grid_.size() || col >= inventory_grid_[row].size()) {
        throw std::out_of_range("Row or column out of range"); // Throw exception if out of range
    }
    return inventory_grid_[row][col]; // Return the item at the specified position
}

/**
    * @brief Stores an item in the inventory grid.
    * @param row The row index.
    * @param col The column index.
    * @param pickup The item to store.
    * @return bool True if the item was successfully stored, false if the cell is already occupied.
    * @throws std::out_of_range if the row or column is out of range.
    * @post The item is stored in the specified cell if the cell is empty.
*/
bool Inventory::store(const size_t& row, const size_t& col, const Item& pickup) {
    if (row >= inventory_grid_.size() || col >= inventory_grid_[row].size()) {
        throw std::out_of_range("Row or column out of range"); // Throw exception if out of range
    }

    if (inventory_grid_[row][col].type_ != NONE) {
        return false; // Return false if the cell is already occupied
    }

    inventory_grid_[row][col] = pickup; // Store the item in the specified cell
    weight_ += pickup.weight_; // Add the item's weight to the total weight
    ++item_count_; // Increment the item count
    return true; // Return true indicating successful storage
}

/**
    * @brief Copy constructor (deep copy).
    * @param rhs The Inventory object to copy from.
*/
Inventory::Inventory(const Inventory& rhs)
    : inventory_grid_(rhs.inventory_grid_), equipped_(rhs.equipped_ ? new Item(*rhs.equipped_) : nullptr),
      weight_(rhs.weight_), item_count_(rhs.item_count_) {}

/**
 * @brief Move constructor. 
 * @param rhs The Inventory object to move from.
 * @post The rhs object is left in a valid but empty state.
*/
Inventory::Inventory(Inventory&& rhs)
    : inventory_grid_(std::move(rhs.inventory_grid_)),
      equipped_(rhs.equipped_), weight_(rhs.weight_), item_count_(rhs.item_count_) {
    // Reset rhs to a valid but empty state
    rhs.equipped_ = nullptr;
    rhs.weight_ = 0;
    rhs.item_count_ = 0;
}

/**
    * @brief Copy assignment operator.
    * @param rhs The Inventory object to copy from.
    * @return Inventory& A reference to the assigned Inventory object.
    * @post The current object is updated to be a copy of rhs.
*/
Inventory& Inventory::operator=(const Inventory& rhs) {
    if (this != &rhs) {
        inventory_grid_ = rhs.inventory_grid_;
        if (equipped_) {
            delete equipped_; // Delete the currently equipped item if it exists
        }
        equipped_ = rhs.equipped_ ? new Item(*rhs.equipped_) : nullptr; // Deep copy the equipped item
        weight_ = rhs.weight_;
        item_count_ = rhs.item_count_;
    }
    return *this;
}

/**
    * @brief Move assignment operator.
    * @param rhs The Inventory object to move from.
    * @return Inventory& A reference to the assigned Inventory object.
    * @post The current object is updated to be a moved version of rhs, and rhs is left in a valid but empty state.
*/
Inventory& Inventory::operator=(Inventory&& rhs) {
    if (this != &rhs) {
        inventory_grid_ = std::move(rhs.inventory_grid_);
        if (equipped_) {
            delete equipped_; // Delete the currently equipped item if it exists
        }
        equipped_ = rhs.equipped_; // Move the equipped item
        weight_ = rhs.weight_;
        item_count_ = rhs.item_count_;

        // Reset rhs to a valid but empty state
        rhs.equipped_ = nullptr;
        rhs.weight_ = 0;
        rhs.item_count_ = 0;
    }
    return *this;
}

/**
    * @brief Destructor. 
    * @post The equipped item is deleted if it exists.
*/
Inventory::~Inventory() {
    if (equipped_) {
        delete equipped_; // Delete the equipped item if it exists
    }
}
