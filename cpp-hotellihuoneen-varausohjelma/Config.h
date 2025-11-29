#pragma once
#include <string>
#include <unordered_map>

class Config
{
private:
	std::string config_file_name;
	std::unordered_map<std::string, std::string> config_map;
	void load_config();
	void save_config() const;

public:
	Config(const std::string& config_file_name);

	std::string get_value(const std::string& key) const;
	void set_value(const std::string& key, const std::string& value);
};

