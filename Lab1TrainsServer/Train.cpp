//This project files includes
#include "Train.h"

//C++ standart includes
#include <iostream>

namespace deliverCorp
{
	using std::string;
	using std::cout;
	using std::cin;
	using std::abs;

	Train::Train()
	{
		id = Train::nextId;
		++Train::nextId;
		cout << "Filling in a new train-" << id << ":\n";
		cout << "Destination: "; cin >> destination;
		cout << "Number: "; cin >> num;
		cout << "Departure time: "; cin >> departure_time;
		cout << "Seats available: "; cin >> seats_available;
	}

	void get_trains(Train const* const pTrainArray, unsigned const trainsNum, TrainsFilter const& const tf)
	{
		for (Train const* p_to_being_checked_train = pTrainArray + trainsNum - 1; p_to_being_checked_train >= pTrainArray; --p_to_being_checked_train)
		{
			if ((tf.destination_of_interest == "" || p_to_being_checked_train->destination == tf.destination_of_interest) &&
				tf.after_this_time <= p_to_being_checked_train->departure_time &&
				tf.seats_required <= p_to_being_checked_train->seats_available)
			{
				cout << p_to_being_checked_train->id;
			}
		}
	}
}