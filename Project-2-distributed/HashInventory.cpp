#include "HashInventory.hpp"

/**
 * @brief Default constructor for the Inventory template class.
 *
 * Initializes an empty inventory with no items, no equipped item,
 * and zero total weight.
 *
 * @tparam Comparator The comparison class for querying items
 */
template <class Comparator>
Inventory<Comparator, std::unordered_set<Item>>::Inventory()
    : equipped_(nullptr), weight_(0.0f) {}
    // your code here


/**
 * @brief Retrieves the value stored in `equipped_`
 * @return The Item pointer stored in `equipped_`
 */
template <class Comparator>
Item* Inventory<Comparator, std::unordered_set<Item>>::getEquipped() const
{
    // your code here
    return equipped_;
}

/**
 * @brief Equips a new item.
 * @param itemToEquip A pointer to the item to equip.
 * @post Updates `equipped` to the specified item
 * without deallocating the original.
 */
template <class Comparator>
void Inventory<Comparator, std::unordered_set<Item>>::equip(Item* itemToEquip)
{
    // your code here
    equipped_ = itemToEquip;
}

/**
 * @brief Discards the currently equipped item.
 * @post Deallocates the item pointed to by `equipped`
 * and sets `equipped` to nullptr, if `equipped` is not nullptr already.
 */
template <class Comparator>
void Inventory<Comparator, std::unordered_set<Item>>::discardEquipped()
{
    // your code here
    delete equipped_;
    equipped_ = nullptr;
}

/**
 * @brief Retrieves the value stored in `weight_`
 * @return The float value stored in `weight_`
 */
template <class Comparator>
float Inventory<Comparator, std::unordered_set<Item>>::getWeight() const
{
    // your code here
    return weight_;
}

/**
 * @brief Retrieves the size of items_
 */
template <class Comparator>
size_t
Inventory<Comparator, std::unordered_set<Item>>::size() const
{
    // your code here
    return items_.size();
}

/**
 * @brief Retrieves a copy of the container holding inventory items.
 *
 * @return Container of items in the inventory
 */
template <class Comparator>
std::unordered_set<Item>
Inventory<Comparator, std::unordered_set<Item>>::getItems() const
{
    // your code here
    return items_;
}

/**
 * @brief Attempts to add a new item to the inventory.
 *
 * @param target Item to be added to the inventory
 * @return true if the item was successfully added, false if an item
 *         with the same name already exists
 * @post Updates the weight_ member to reflect the new Item pickup
 */
template <class Comparator>
bool Inventory<Comparator, std::unordered_set<Item>>::pickup(const Item& target)
{
    // your code here
    if (contains(target.name_)) return false;
    items_.insert(target);
    weight_ += target.weight_;
    return true;
}

/**
 * @brief Attempts to remove an item from the inventory by name.
 *
 * @param name Name of the item to be removed
 * @return true if the item was successfully removed, false if the
 *         item was not found in the inventory
 * @post Updates the weight_ member to reflect removing the Item
 */
template <class Comparator>
bool Inventory<Comparator, std::unordered_set<Item>>::discard(
    const std::string& itemName)
{
    // your code here
    Item temp(itemName, 0.0f, ItemType::NONE);
    auto it = items_.find(temp);
    if (it == items_.end()) return false;
    weight_ -= it->weight_;
    items_.erase(it);
    return true;
}

/**
 * @brief Checks if an item with the given name exists in the inventory.
 *
 * @param name Name of the item to search for
 * @return true if the item exists in the inventory, false otherwise
 */
template <class Comparator>
bool Inventory<Comparator, std::unordered_set<Item>>::contains(
    const std::string& itemName) const
{
    // your code here
    Item temp(itemName, 0.0f, ItemType::NONE);
    return items_.find(temp) != items_.end();
}

/**
 * @brief Queries the inventory for items within a specified range.
 *
 * Returns a set of items that fall between the start and end items
 * according to the specified Comparator (inclusive on both ends)
 *
 * @param start An Item whose compared property is the lower bound of the query range
 * @param end An Item whose compared property is the upper bound of the query range
 * @return std::unordered_set of items within the specified range
 *
 * @note Returns an empty set if the end item is less than the start item
 * @example To select all Items with weights 0.4 to 10.9, we'd setup the class & parameters as such:
 *  - this Inventory object is of type Inventory<CompareItemWeight>
 *  - start = Item("some_name", 0.4, ItemType::None)
 *  - end = = Item("some_other_name", 10.9, ItemType::None)
 *
 */
template <class Comparator>
std::unordered_set<Item>
Inventory<Comparator, std::unordered_set<Item>>::query(const Item& start,
    const Item& end) const
{
    // your code here
    std::unordered_set<Item> result;
    if (Comparator::lessThan(end, start)) return result;
    
    for (const auto& item : items_) {
        if (Comparator::leq(start, item) && Comparator::leq(item, end)) {
            result.insert(item);
        }
    }
    return result;
}

/**
 * @brief Destructor for the Inventory class.
 * @post Deallocates any dynamically allocated resources.
 */
template <class Comparator>
Inventory<Comparator, std::unordered_set<Item>>::~Inventory()
{
    // your code here
    discardEquipped();
}