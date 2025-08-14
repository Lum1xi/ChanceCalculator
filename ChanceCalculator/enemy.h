#include "item.h"


class Enemy {
public:
	Enemy(string name) : name(name) {}
	void add_item(const Item& item) {
		items.push_back(item);
	}
	string get_name() const {
		return name;
	}
	vector<Item> get_items() const {
		return items;
	}
	vector<Item> kill() const {
		vector<Item> dropped_items;
		for (const Item& item : items) {
			if (item.roll()) {
				dropped_items.push_back(item);
			}
		}
		return dropped_items;

	}

private:
		string name;
		vector<Item> items;

};