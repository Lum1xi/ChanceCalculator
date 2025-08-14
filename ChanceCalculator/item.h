#include "libs.h"

class Item {
	public:
		Item(string name, double chance) : name(name), chance(chance) {}
		string get_name () const {
			return name;
		}
		double get_chance () const {
			return chance;
		}
		bool roll () const {
			double roll = rand() % 10000 / 100.0;
			return roll <= chance;
		}
private:
	string name;
	double chance;

};