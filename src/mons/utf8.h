/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  utf8.h -- UTF-8 input handling
*/

#ifndef _UTF8_H
#define _UTF8_H

#include <string>
#include <vector>
#include <algorithm>

struct utf8char {
	char chars[4];

	utf8char() noexcept {
		chars[0] = 0;
		chars[1] = 0;
		chars[2] = 0;
		chars[3] = 0;
	}

	bool operator==(const utf8char & other) const noexcept {
		return (chars[0] == other.chars[0]) && (chars[1] == other.chars[1]) &&
			   (chars[2] == other.chars[2]) && (chars[3] == other.chars[3]);
	}

	int length() const noexcept {
		if((chars[0] & 0b1111'0000) == 0b1111'0000) {
			return 4;
		} else if((chars[0] & 0b1110'0000) == 0b1110'0000) {
			return 3;
		} else if((chars[0] & 0b1100'0000) == 0b1100'0000) {
			return 2;
		} else {
			return 1;
		}
	}
};

struct utf8string {
public:
	utf8string() noexcept {}
	utf8string(const char * string) noexcept {
		while(*string) {
			utf8char c;

			c.chars[0] = string[0];
			string++;

			for(int i = 1; i < c.length(); i++) {
				c.chars[i] = string[0];
				string++;
			}

			this->string.push_back(c);
		}
	}

	int length() const noexcept { return string.size(); }

	int raw_length() const noexcept { return string.size() * 4; }

	char * raw_data() noexcept {
		if(string.size() > 0) { return &string[0].chars[0]; }

		return nullptr;
	}

	void add(utf8char c) noexcept { string.push_back(c); }

	void insert(const int index, const utf8char c) noexcept {
		if(index >= (int)string.size()) {
			add(c);
		} else {
			string.insert(string.begin() + index, c);
		}
	}

	void erase(const int index) noexcept { string.erase(string.begin() + index); }

	std::string unpack() const noexcept {
		std::string s;

		for(const auto & c : string) {
			for(int i = 0; i < 4; i++) {
				if(c.chars[i]) { s.push_back(c.chars[i]); }
			}
		}

		return s;
	}

	auto begin() { return string.begin(); }

	auto end() { return string.end(); }

	// NOTE(robin): find faster implementation?
	bool begins_with(utf8string s) const noexcept {
		return std::search(string.begin(), string.end(), s.begin(), s.end()) ==
			   string.begin();
	}

	bool contains(utf8string s) const noexcept {
		return std::search(string.begin(), string.end(), s.begin(), s.end()) !=
			   string.end();
	}

	void clear() noexcept { string.clear(); }

private:
	std::vector<utf8char> string;
};

#endif
