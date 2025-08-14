#include "enemy.h"




void generate_kills(Enemy enemy, int amount) {
	for (int i = 0; i < amount; ++i) {
		vector<Item> dropped_items = enemy.kill();
		if (dropped_items.empty()) {
			cout << "No items dropped from " << enemy.get_name() << " on kill " << (i + 1) << "." << endl;
		} else {
			cout << "Items dropped from " << enemy.get_name() << " on kill " << (i + 1) << ":" << endl;
			for (const Item& item : dropped_items) {
				cout << "- " << item.get_name() << " (Chance: " << item.get_chance() << "%)" << endl;
			}
		}
		cout << endl;
	}
}

int generate_kills_untill_all_drops(Enemy enemy) {
	int total_kills = 0;
	vector<Item> all_items = enemy.get_items();
	vector<bool> items_dropped(all_items.size(), false);
	int items_remaining = all_items.size();
	while (items_remaining > 0) {
		total_kills++;
		vector<Item> dropped_items = enemy.kill();
		for (const Item& item : dropped_items) {
			for (size_t i = 0; i < all_items.size(); ++i) {
				if (item.get_name() == all_items[i].get_name() && !items_dropped[i]) {
					items_dropped[i] = true;
					items_remaining--;
				}
			}
		}
	}
	return total_kills;
}

void generate_statistics(Enemy enemy, int amount) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	vector<Item> total_stats;
	int total_kills = 0;
	int kills = 0;
	int max_kills = 0;
	int min_kills = INT_MAX;
	double start_time = clock();
	
	for (int i = 0; i < amount; ++i) {
		if (i % 200 == 0) {
			double elapsed_time = clock() - start_time;
			//clear console
			system("cls");
			cout << "Progress: " << i << "/" << amount << " (" << (elapsed_time / CLOCKS_PER_SEC) << " seconds elapsed)" << endl;
			cout << "ETA: " << (amount - i) * (elapsed_time / CLOCKS_PER_SEC)/i << " seconds" << endl;

		}
		kills = generate_kills_untill_all_drops(enemy);
		bool found = false;
		for (size_t i = 0; i < total_stats.size(); ++i) {
			if (total_stats[i].get_name() == to_string(kills)) {
				total_stats[i] = Item(total_stats[i].get_name(), total_stats[i].get_chance() + 1);
				found = true;
				break;
			}
		}
		if (!found) {
			total_stats.push_back(Item(to_string(kills), 1));
		}
		total_kills += kills;
		if (kills > max_kills) {
			max_kills = kills;
		}
		if (kills < min_kills) {
			min_kills = kills;
		}
	}
	system("cls");
	cout << "Simulation completed!" << endl;
	cout << "time elapsed: " << (clock() - start_time) / CLOCKS_PER_SEC << " seconds" << endl;
	cout << "Total kills: " << total_kills << endl;

	cout << "average amount: " << total_kills / amount << endl;
	cout << "max amount: " << max_kills << endl;
	cout << "min amount: " << min_kills << endl;

	cout << "Do you want to see full statistics? (y/n): ";
	char choice;
	cin >> choice;

	if (choice == 'y' || choice == 'Y') {
		// Sort the statistics by kills
		sort(total_stats.begin(), total_stats.end(), [](const Item& a, const Item& b) {
			return stoi(a.get_name()) < stoi(b.get_name());
		});
		cout << "Full statistics:" << endl;
		for (const Item& item : total_stats) {
			cout << "Kills: " << item.get_name() << ", Amount: " << item.get_chance() << endl;
		}
		int max_count = 0;
		string most_common_kills;
		string median;
		int total_by_far = 0;
		int previus_total_by_far = 0;
		for (const Item& item : total_stats) {
			if (item.get_chance() > max_count) {
				max_count = item.get_chance();
				most_common_kills = item.get_name();
			}
			total_by_far += item.get_chance();
			if (previus_total_by_far < amount / 2 && total_by_far >=amount/2) {
				median = item.get_name();
			}
			previus_total_by_far = total_by_far;
		}
		SetConsoleTextAttribute(hConsole, 15);
		cout << "average amount: " << total_kills / amount << endl;
		cout << "max amount: " << max_kills << endl;
		cout << "min amount: " << min_kills << endl;
		cout << "Most common amount of kills: " << most_common_kills << " (Count: " << max_count << ")" << endl;
		cout << "Median amount of kills: " << median << endl;
		SetConsoleTextAttribute(hConsole, 7);
		cout << "do you want to see how lucky are you? (y/n): ";
		cin >> choice;
		if (choice == 'y' || choice == 'Y') {
			cout << "how many kills did you get? ";
			int your_kills;
			cin >> your_kills;
			total_by_far = 0;
			int critical_point = 0;
			for (const Item& item : total_stats) {
				total_by_far += item.get_chance();
				if (stoi(item.get_name()) < your_kills) {
					critical_point = total_by_far;
				}
			}
			if (critical_point > amount / 2) {
				SetConsoleTextAttribute(hConsole, 12);
			} else {
				SetConsoleTextAttribute(hConsole, 10);
			}
			cout << critical_point << " - people got it in less kills than you" << endl;
			SetConsoleTextAttribute(hConsole, 7);

		} else {
			cout << "Statistics not shown." << endl;
		}


	} else {
		cout << "Statistics not shown." << endl;
	}

}

int main() {
	

	srand(time(0));

	srand(static_cast<unsigned int>(time(0))); // Seed the random number generator
	Enemy glacite_walker("Glacite Walker");
	glacite_walker.add_item(Item("Helmet", 0.5));
	glacite_walker.add_item(Item("Chestplate", 0.5));
	glacite_walker.add_item(Item("Leggings", 0.5));
	glacite_walker.add_item(Item("Boots", 0.5));

	generate_statistics(glacite_walker, 100000);

	return 0;
}