#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <set>
#include <deque>

using namespace std;

class BookingManager {
public:

    void Book(long time, const string &hotel_name, int client_id, int room_count) {
        bookings.push_back({time, hotel_name, client_id, room_count});

        unique_clients_per_hotel[hotel_name][client_id]++;
        rooms_booked_per_hotel[hotel_name] += room_count;

        fix_outdated_bookings(time - 86400);
    }

    void fix_outdated_bookings(long until) {
        auto it = bookings.begin();
        for (; it != bookings.end();) {
            Booking booking = *it;

            if (booking.time <= until) {
                auto hotel_name = booking.hotel_name;

                rooms_booked_per_hotel[hotel_name] -= booking.room_count;
                unique_clients_per_hotel[hotel_name][booking.client_id]--;
                if (unique_clients_per_hotel[hotel_name][booking.client_id] == 0) {
                    unique_clients_per_hotel[hotel_name].erase(booking.client_id);
                }

                it = bookings.erase(it);
            } else {
                return;
            }
        }
    }

    int GetClients(const string &hotel_name) const {
        if (unique_clients_per_hotel.count(hotel_name)) {
            return unique_clients_per_hotel.at(hotel_name).size();
        } else {
            return 0;
        }
    }

    long GetRooms(const string &hotel_name) const {
        if (rooms_booked_per_hotel.count(hotel_name)) {
            return rooms_booked_per_hotel.at(hotel_name);
        } else {
            return 0;
        }
    }

    struct Booking {
        long time;
        string hotel_name;
        int client_id;
        int room_count;
    };

private:
    map<string, long> rooms_booked_per_hotel;
    map<string, map<int, int>> unique_clients_per_hotel;
    deque<Booking> bookings;
};

int main() {
    BookingManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;

        if (query_type == "BOOK") {
            long time;
            string hotel_name;
            int client_id;
            int room_count;
            cin >> time >> hotel_name >> client_id >> room_count;
            manager.Book(time, hotel_name, client_id, room_count);
        } else if (query_type == "CLIENTS") {
            string hotel_name;
            cin >> hotel_name;
            cout << manager.GetClients(hotel_name) << "\n";
        } else {
            string hotel_name;
            cin >> hotel_name;
            cout << manager.GetRooms(hotel_name) << "\n";
        }
    }

    return 0;
}