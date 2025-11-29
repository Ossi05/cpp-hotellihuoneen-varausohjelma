#pragma once
#include <stdexcept>
#include <string>

class RoomNotFoundException : public std::runtime_error {
public:
	explicit RoomNotFoundException(const std::string& msg = "Huonetta ei löytynyt")
		: std::runtime_error(msg) {}
	virtual ~RoomNotFoundException() = default;
};

class MenuOptionNotFoundException : public std::runtime_error {
public:
	explicit MenuOptionNotFoundException(const std::string& msg = "Valittua valikkovaihtoehtoa ei löytynyt")
		: std::runtime_error(msg) {}
	virtual ~MenuOptionNotFoundException() = default;
};

class ReservationNotFoundException : public std::runtime_error {
public:
	explicit ReservationNotFoundException(const std::string& msg = "Varausta ei löytynyt")
		: std::runtime_error(msg) {}
	virtual ~ReservationNotFoundException() = default;
};

class ReservationIdAlreadyExistsException : public std::runtime_error {
public:
	explicit ReservationIdAlreadyExistsException(const std::string& msg = "Varauksen ID on jo olemassa")
		: std::runtime_error(msg) {}
	virtual ~ReservationIdAlreadyExistsException() = default;
};

class InvalidCSVFormatException : public std::runtime_error {
public:
	explicit InvalidCSVFormatException(const std::string& msg = "Virheellinen CSV-muoto")
		: std::runtime_error(msg) {}
	virtual ~InvalidCSVFormatException() = default;
};

class RoomNotAvailableException : public std::runtime_error {
public:
	explicit RoomNotAvailableException(const std::string& msg = "Huone ei ole vapaana")
		: std::runtime_error(msg) {}
	virtual ~RoomNotAvailableException() = default;
};

class FileAccessException : public std::runtime_error {
public:
	explicit FileAccessException(const std::string& msg = "Tiedoston avaaminen epäonnistui")
		: std::runtime_error(msg) {}
	virtual ~FileAccessException() = default;
};

class ConfigKeyNotFoundException : public std::runtime_error {
public:
	explicit ConfigKeyNotFoundException(const std::string& msg = "Config avainta ei löytynyt")
		: std::runtime_error(msg) {}
	virtual ~ConfigKeyNotFoundException() = default;
};