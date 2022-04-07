#pragma once

#include "structs.h"

class discordcache
{
public:
	std::vector<s_user> users;
	std::vector<s_guild> guilds;
	std::vector<s_channel> channels;

	s_guild* get_guild_from_id(const std::string& guild_id) {
		auto const it = std::find_if(guilds.begin(), guilds.end(), [=](s_unavailable_guild g) { return g.id == guild_id; });
		if (it != guilds.end())
			return it._Ptr;
		return nullptr;
	}

	s_user* get_user_from_id(const std::string& user_id) {
		auto const it = std::find_if(users.begin(), users.end(), [=](s_user u) { return u.id == user_id; });
		if (it != users.end())
			return it._Ptr;
		return nullptr;
	}

	s_channel* get_channel_from_id(const std::string& channel_id) {
		auto const it = std::find_if(channels.begin(), channels.end(), [=](s_channel c) { return c.id == channel_id; });
		if (it != channels.end())
			return it._Ptr;
		return nullptr;
	}
};