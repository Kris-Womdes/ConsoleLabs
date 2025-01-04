#pragma once

#include <string>

using std::string;

namespace deliverCorp
{
	class Train
	{
	public:
		static inline unsigned nextId{};
		unsigned id;
		string destination;
		unsigned num;
		unsigned departure_time;
		unsigned seats_available;
		/*unsigned seats_available_type_A;
		unsigned seats_available_type_B;
		unsigned seats_available_type_C;*/
		Train();
	};

	struct TrainsFilter
	{
		string destination_of_interest = "";
		unsigned after_this_time{ 0 };
		unsigned seats_required{ 0 };
	};

	void get_trains(Train const* const pTrainArray, unsigned const trainsNum, TrainsFilter const& const);
}

