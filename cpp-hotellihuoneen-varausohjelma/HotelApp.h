#pragma once
#include "Hotel.h"
#include <vector>
#include <string>

class HotelApp : public IPrintable
{
private:
	static const std::vector<std::string> menu_options;
	const Hotel hotel;
public:
	static const int print_width;

	HotelApp(const std::string& hotel_name, int num_rooms);
	void print_menu_options() const;
	void get_handle_input() const;

	virtual void print(std::ostream& os) const override;
	~HotelApp() = default;

};

