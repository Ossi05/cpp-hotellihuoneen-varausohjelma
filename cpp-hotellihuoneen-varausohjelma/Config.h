#pragma once
#include <string>
#include <unordered_map>

/*
	Manages configuration settings stored in a file.
	format: key=value
*/
class Config
{
private:
	std::string config_file_name;
	std::unordered_map<std::string, std::string> config_map; // Stores key-value pairs in a map during runtime
	void load_config();
	void save_config() const;

public:
	Config(const std::string& config_file_name);

	std::string get_value(const std::string& key) const;
	void set_value(const std::string& key, const std::string& value);
	const std::string& get_file_name() const;
};

