#pragma once

#include <string>
#include <vector>
#include "nlohmann/json.hpp"

using namespace nlohmann;

template < typename ValueType,
	detail::enable_if_t <
	!detail::is_basic_json<ValueType>::value,
	int > = 0 >
static void setIfExists(json j, const char* key, ValueType& v) {
	if (j.contains(key) && !j.at(key).is_null())
		j.at(key).get_to(v);
}

#define DISCORD_EVENT_MESSAGE_CREATE "MESSAGE_CREATE"
#define DISCORD_EVENT_PRESENCE_UPDATE "PRESENCE_UPDATE"
#define DISCORD_EVENT_VOICE_STATE_UPDATE "VOICE_STATE_UPDATE"

struct s_user {
	std::string id;
	std::string username;
	std::string discriminator;
	std::string avatar;
	bool bot;
	bool system;
	bool mfa_enabled;
	std::string banner;
	int accent_color{};
	std::string locale;
	bool verified;
	std::string email;
	int flags{};
	int premium_type{};
	int public_flags{};

	static s_user fromJson(json j) {
		s_user user;

		setIfExists(j, "id", user.id);
		setIfExists(j, "username", user.username);
		setIfExists(j, "discriminator", user.discriminator);
		setIfExists(j, "avatar", user.avatar);
		setIfExists(j, "bot", user.bot);
		setIfExists(j, "system", user.system);
		setIfExists(j, "mfa_enabled", user.mfa_enabled);
		setIfExists(j, "banner", user.banner);
		setIfExists(j, "accent_color", user.accent_color);
		setIfExists(j, "locale", user.locale);
		setIfExists(j, "verified", user.verified);
		setIfExists(j, "email", user.email);
		setIfExists(j, "flags", user.flags);
		setIfExists(j, "premium_type", user.premium_type);
		setIfExists(j, "public_flags", user.public_flags);

		return user;
	}
};

struct s_member {
	s_user user;
	std::vector<long long> roles;
	std::string premium_since;
	bool pending;
	std::string nick;
	bool mute;
	std::string	joined_at;
	std::string hoisted_role;
	bool deaf;
	std::string communication_disabled_until;
	std::string avatar;

	static s_member fromJson(json j) {
		s_member member;

		if(j.contains("user"))
			member.user = s_user::fromJson(j.at("user"));
		
		//j.at("roles").get_to(member.roles);
		setIfExists(j, "premium_since", member.premium_since);
		setIfExists(j, "pending", member.pending);
		setIfExists(j, "nick", member.nick);
		setIfExists(j, "mute", member.mute);
		setIfExists(j, "joined_at", member.joined_at);
		setIfExists(j, "hoisted_role", member.hoisted_role);
		setIfExists(j, "deaf", member.deaf);
		setIfExists(j, "communication_disabled_until", member.communication_disabled_until);
		setIfExists(j, "avatar", member.avatar);

		return member;
	}
};

struct s_message_reference {
	std::string channel_id;
	std::string guild_id;
	std::string message_id;

	static s_message_reference fromJson(json j) {
		s_message_reference mess_ref;

		setIfExists(j, "channel_id", mess_ref.channel_id);
		setIfExists(j, "guild_id", mess_ref.guild_id);
		setIfExists(j, "message_id", mess_ref.message_id);

		return mess_ref;
	}
};

struct s_message {
	int type{};
	bool tts;
	std::string timestamp;
	s_message_reference referenced_message;
	bool pinned;
	std::string nonce;
	std::vector<long long> mentions;
	std::vector<long long> mention_roles;
	bool mention_everyone;
	s_member member;
	std::string id;
	int flags{};
	std::string edited_timestamp;
	std::string content;
	std::string channel_id;
	s_user author;
	std::string guild_id;

	static s_message fromJson(json j) {
		s_message message;

		setIfExists(j, "type", message.type);
		setIfExists(j, "tts", message.tts);
		setIfExists(j, "timestamp", message.timestamp);

		if(j.contains("referenced_message") && j.at("referenced_message").is_null())
			message.referenced_message = s_message_reference::fromJson(j.at("referenced_message"));
		
		setIfExists(j, "pinned", message.pinned);
		setIfExists(j, "nonce", message.nonce);
		//setIfExists(j, "mentions", message.mentions);
		//setIfExists(j, "mention_roles", message.mention_roles);
		//setIfExists(j, "mention_everyone", message.mention_everyone);
	
		if(j.contains("member"))
			message.member = s_member::fromJson(j.at("member"));

		setIfExists(j, "id", message.id);
		setIfExists(j, "edited_timestamp", message.edited_timestamp);
		setIfExists(j, "flags", message.flags);
		setIfExists(j, "content", message.content);
		setIfExists(j, "channel_id", message.channel_id);

		if (j.contains("author"))
			message.author = s_user::fromJson(j.at("author"));

		setIfExists(j, "guild_id", message.guild_id);

		return message;
	}
};

struct s_voice_state {
	std::string guild_id;
	std::string channel_id;
	std::string user_id;
	s_member member;
	std::string session_id;
	bool deaf;
	bool mute;
	bool self_deaf;
	bool self_mute;
	bool self_stream;
	bool self_video;
	bool suppress;
	std::string request_to_speak_timestamp;

	static s_voice_state fromJson(json j) {
		s_voice_state vs;

		setIfExists(j, "guild_id", vs.guild_id);
		setIfExists(j, "channel_id", vs.channel_id);
		setIfExists(j, "user_id", vs.user_id);
		
		if(j.contains("member"))
			vs.member = s_member::fromJson(j.at("member"));

		setIfExists(j, "session_id", vs.session_id);
		setIfExists(j, "deaf", vs.deaf);
		setIfExists(j, "mute", vs.mute);
		setIfExists(j, "self_deaf", vs.self_deaf);
		setIfExists(j, "self_mute", vs.self_mute);
		setIfExists(j, "self_stream", vs.self_stream);
		setIfExists(j, "self_video", vs.self_video);
		setIfExists(j, "suppress", vs.suppress);
		setIfExists(j, "request_to_speak_timestamp", vs.request_to_speak_timestamp);

		return vs;
	}
};

struct s_unavailable_guild {
	std::string id;
	std::string name;
	std::string icon;
	std::string splash;
	std::string discovery_splash;
	//s_emoji emojis[];
	//s_feature features[];
	int approximate_member_count{};
	int approximate_presence_count{};
	std::string description;
	//s_stickers stickers[];

	static s_unavailable_guild fromJson(json j) {
		s_unavailable_guild guild;

		setIfExists(j, "id", guild.id);
		setIfExists(j, "name", guild.name);
		setIfExists(j, "icon", guild.icon);
		setIfExists(j, "splash", guild.splash);
		setIfExists(j, "discovery_splash", guild.discovery_splash);
		setIfExists(j, "approximate_member_count", guild.approximate_member_count);
		setIfExists(j, "approximate_presence_count", guild.approximate_presence_count);
		setIfExists(j, "description", guild.description);

		return guild;
	}
};

struct s_role_tag {
	std::string bot_id;
	std::string integration_id;

	static s_role_tag fromJson(json j) {
		s_role_tag result;

		setIfExists(j, "bot_id", result.bot_id);
		setIfExists(j, "integration_id", result.integration_id);

		return result;
	}
};

struct s_role {
	std::string id;
	std::string name;
	int color{};
	bool hoist;
	std::string icon;
	std::string unicode_emoji;
	int position{};
	std::string permissions;
	bool managed;
	bool mentionable;
	std::vector<s_role_tag> tags;

	static s_role fromJson(json j) {
		s_role result;

		setIfExists(j, "id", result.id);
		setIfExists(j, "name", result.name);
		setIfExists(j, "color", result.color);
		setIfExists(j, "hoist", result.hoist);
		setIfExists(j, "icon", result.icon);
		setIfExists(j, "unicode_emoji", result.unicode_emoji);
		setIfExists(j, "position", result.position);
		setIfExists(j, "permissions", result.permissions);
		setIfExists(j, "managed", result.managed);
		setIfExists(j, "mentionable", result.mentionable);

		if(j.contains("tags"))
			for (json elem : j["tags"]) result.tags.push_back(s_role_tag::fromJson(elem));

		return result;
	}
};

struct s_emoji {
	std::string id;
	std::string name;
	std::vector<int> roles;
	s_user user;
	bool require_colons;
	bool managed;
	bool animated;
	bool available;

	static s_emoji fromJson(json j) {
		s_emoji result;

		setIfExists(j, "id", result.id);
		setIfExists(j, "name", result.name);

		if(j.contains("roles"))
			for (int elem : j["roles"]) result.roles.push_back(elem);

		if(j.contains("user"))
			result.user = s_user::fromJson(j["user"]);

		setIfExists(j, "require_colons", result.require_colons);
		setIfExists(j, "managed", result.managed);
		setIfExists(j, "animated", result.animated);
		setIfExists(j, "available", result.available);

		return result;
	}
};

struct s_welcome_screen_channel {
	std::string channel_id;
	std::string description;
	std::string emoji_id;
	std::string emoji_name;

	static s_welcome_screen_channel fromJson(json j) {
		s_welcome_screen_channel result;

		setIfExists(j, "channel_id", result.channel_id);
		setIfExists(j, "description", result.description);
		setIfExists(j, "emoji_id", result.emoji_id);
		setIfExists(j, "emoji_name", result.emoji_name);

		return result;
	}
};

struct s_welcome_screen {
	std::string description;
	std::vector<s_welcome_screen_channel> welcome_channels;

	static s_welcome_screen fromJson(json j) {
		s_welcome_screen result;

		setIfExists(j, "description", result.description);

		if (j.contains("welcome_channels"))
			for (json elem : j["welcome_channels"]) result.welcome_channels.push_back(s_welcome_screen_channel::fromJson(elem));

		return result;
	}
};

struct s_stage_instance {
	std::string id;
	std::string guild_id;
	std::string channel_id;
	std::string topic;
	int privacy_level{};
	bool discoverable_disabled;

	static s_stage_instance fromJson(json j) {
		s_stage_instance result;

		setIfExists(j, "id", result.id);
		setIfExists(j, "guild_id", result.guild_id);
		setIfExists(j, "channel_id", result.channel_id);
		setIfExists(j, "topic", result.topic);
		setIfExists(j, "privacy_level", result.privacy_level);
		setIfExists(j, "discoverable_disabled", result.discoverable_disabled);

		return result;
	}
};

struct s_sticker {
	std::string id;
	std::string pack_id;
	std::string name;
	std::string description;
	std::string tags;
	std::string asset;
	int type{};
	int format_type;
	bool available;
	std::string guild_id;
	s_user user;
	int sort_value{};

	static s_sticker fromJson(json j) {
		s_sticker result;

		setIfExists(j, "id", result.id);
		setIfExists(j, "pack_id", result.pack_id);
		setIfExists(j, "name", result.name);
		setIfExists(j, "description", result.description);
		setIfExists(j, "tags", result.tags);
		setIfExists(j, "asset", result.asset);
		setIfExists(j, "type", result.type);
		setIfExists(j, "format_type", result.format_type);
		setIfExists(j, "available", result.available);
		setIfExists(j, "guild_id", result.guild_id);
		
		if (j.contains("user"))
			result.user = s_user::fromJson(j["user"]);

		setIfExists(j, "sort_value", result.sort_value);

		return result;
	}
};

struct s_guild_scheduled_event_entity_metadata {
	std::string location;

	static s_guild_scheduled_event_entity_metadata fromJson(json j) {
		s_guild_scheduled_event_entity_metadata result;

		setIfExists(j, "location", result.location);

		return result;
	}
};

struct s_guild_scheduled_event {
	std::string id;
	std::string guild_id;
	std::string channel_id;
	std::string creator_id;
	std::string name;
	std::string description;
	std::string scheduled_start_time;
	std::string scheduled_end_time;
	int privacy_level{};
	int status{};
	int entity_type{};
	std::string entity_id{};
	s_guild_scheduled_event_entity_metadata entity_metadata;
	s_user creator;
	int user_count{};

	static s_guild_scheduled_event fromJson(json j) {
		s_guild_scheduled_event result;

		setIfExists(j, "id", result.id);
		setIfExists(j, "guild_id", result.guild_id);
		setIfExists(j, "channel_id", result.channel_id);
		setIfExists(j, "creator_id", result.creator_id);
		setIfExists(j, "name", result.name);
		setIfExists(j, "description", result.description);
		setIfExists(j, "scheduled_start_time", result.scheduled_start_time);
		setIfExists(j, "scheduled_end_time", result.scheduled_end_time);
		setIfExists(j, "privacy_level", result.privacy_level);
		setIfExists(j, "status", result.status);
		setIfExists(j, "entity_type", result.entity_type);
		setIfExists(j, "entity_id", result.entity_id);
		
		if (j.contains("entity_metadata"))
			result.entity_metadata = s_guild_scheduled_event_entity_metadata::fromJson(j["entity_metadata"]);

		if (j.contains("user"))
			result.creator = s_user::fromJson(j["user"]);

		setIfExists(j, "user_count", result.user_count);

		return result;
	}
};

struct s_channel {
	std::string id;
	int type{};
	std::string guild_id;
	int position{};
	//s_overwrite permission_overwrites;
	std::string name;
	std::string topic;
	bool nsfw{};
	std::string last_message_id;
	int bitrate{};
	int user_limit{};
	int rate_limit_per_user{};
	std::vector<s_user> recipients;
	std::string icon;
	std::string owner_id;
	std::string application_id;
	std::string parent_id;
	std::string last_pin_timestamp;
	std::string rtc_region;
	int video_quality_mode{};
	int message_count{};
	int member_count{};
	//s_thread_metadata thread_metadata;
	//s_thread_member member;
	int default_auto_archive_duration{};
	std::string permissions;

	static s_channel fromJson(json j) {
		s_channel channel;

		setIfExists(j, "id", channel.id);
		setIfExists(j, "type", channel.type);
		setIfExists(j, "guild_id", channel.guild_id);
		setIfExists(j, "position", channel.position);
		setIfExists(j, "name", channel.name);
		setIfExists(j, "topic", channel.topic);
		setIfExists(j, "nsfw", channel.nsfw);
		setIfExists(j, "last_message_id", channel.last_message_id);
		setIfExists(j, "bitrate", channel.bitrate);
		setIfExists(j, "user_limit", channel.user_limit);
		setIfExists(j, "rate_limit_per_user", channel.rate_limit_per_user);

		for (auto elem : j["recipients"]) {
			channel.recipients.push_back(s_user::fromJson(elem));
		}

		setIfExists(j, "icon", channel.icon);
		setIfExists(j, "owner_id", channel.owner_id);
		setIfExists(j, "application_id", channel.application_id);
		setIfExists(j, "parent_id", channel.parent_id);
		setIfExists(j, "last_pin_timestamp", channel.last_pin_timestamp);
		setIfExists(j, "rtc_region", channel.rtc_region);
		setIfExists(j, "video_quality_mode", channel.video_quality_mode);
		setIfExists(j, "message_count", channel.message_count);
		setIfExists(j, "member_count", channel.member_count);
		setIfExists(j, "default_auto_archive_duration", channel.default_auto_archive_duration);
		setIfExists(j, "permissions", channel.permissions);

		return channel;
	}
};

struct s_activity_timestamps {
	int start;
	int end;

	static s_activity_timestamps fromJson(json j) {
		s_activity_timestamps timestamp{};

		setIfExists(j, "start", timestamp.start);
		setIfExists(j, "end", timestamp.end);

		return timestamp;
	}
};

struct s_activity_emoji {
	std::string name;
	std::string id;
	bool animated;

	static s_activity_emoji fromJson(json j) {
		s_activity_emoji emoji;

		setIfExists(j, "name", emoji.name);
		setIfExists(j, "id", emoji.id);
		setIfExists(j, "animated", emoji.animated);

		return emoji;
	}
};

struct s_activity_party {
	std::string id;
	int current_size{};
	int max_size{};

	static s_activity_party fromJson(json j) {
		s_activity_party party;

		setIfExists(j, "id", party.id);
		if (j.contains("size")) {
			party.current_size = j["size"][0];
			party.max_size = j["size"][1];
		}

		return party;
	}
};

struct s_activity_assets {
	std::string large_image;
	std::string large_text;
	std::string small_image;
	std::string small_text;

	static s_activity_assets fromJson(json j) {
		s_activity_assets asset;

		setIfExists(j, "large_image", asset.large_image);
		setIfExists(j, "large_text", asset.large_text);
		setIfExists(j, "small_image", asset.small_image);
		setIfExists(j, "small_text", asset.small_text);

		return asset;
	}
};

struct s_activity_secrets {
	std::string join;
	std::string spectate;
	std::string match;

	static s_activity_secrets fromJson(json j) {
		s_activity_secrets secret;

		setIfExists(j, "join", secret.join);
		setIfExists(j, "spectate", secret.spectate);
		setIfExists(j, "match", secret.match);

		return secret;
	}
};

struct s_activity_button {
	std::string label;
	std::string url;

	static s_activity_button fromJson(json j) {
		s_activity_button button;

		setIfExists(j, "label", button.label);
		setIfExists(j, "url", button.url);

		return button;
	}
};

struct s_activity {
	std::string name;
	int type;
	std::string url;
	int created_at;
	s_activity_timestamps timestamps;
	std::string application_id;
	std::string details;
	std::string state;
	s_activity_emoji emoji;
	s_activity_party party;
	s_activity_assets assets;
	s_activity_secrets secrets;
	bool instance;
	int flags;
	std::vector<s_activity_button> buttons;

	static s_activity fromJson(json j) {
		s_activity activity;

		setIfExists(j, "name", activity.name);
		setIfExists(j, "type", activity.type);
		setIfExists(j, "url", activity.url);
		setIfExists(j, "created_at", activity.created_at);
		
		activity.timestamps = s_activity_timestamps::fromJson(j["timestamps"]);

		setIfExists(j, "application_id", activity.application_id);
		setIfExists(j, "details", activity.details);
		setIfExists(j, "state", activity.state);

		activity.emoji = s_activity_emoji::fromJson(j["emoji"]);
		activity.party = s_activity_party::fromJson(j["party"]);
		activity.assets = s_activity_assets::fromJson(j["assets"]);
		activity.secrets = s_activity_secrets::fromJson(j["secrets"]);

		setIfExists(j, "instance", activity.instance);
		setIfExists(j, "flags", activity.flags);

		for (json elem : j["buttons"]) activity.buttons.push_back(s_activity_button::fromJson(elem));

		return activity;
	}
};

struct s_client_status {
	std::string desktop;
	std::string mobile;
	std::string web;

	bool is_desktop;
	bool is_mobile;
	bool is_web;

	static s_client_status fromJson(json j) {
		s_client_status status;

		setIfExists(j, "desktop", status.desktop);
		setIfExists(j, "mobile", status.mobile);
		setIfExists(j, "web", status.web);

		status.is_desktop = !status.desktop.empty();
		status.is_mobile = !status.mobile.empty();
		status.is_web = !status.web.empty();

		return status;
	}
};

namespace gateway_events {
	struct ready {
		int v{};
		std::vector<s_user> users;
		std::vector<s_unavailable_guild> guilds;
		std::string session_id;
		//int shard[2];
		//s_application application;

		static ready fromJson(json j) {
			ready r;
			for (auto& elem : j["users"]) {
				r.users.push_back(s_user::fromJson(elem));
			}
			for (auto& elem : j["guilds"]) {
				r.guilds.push_back(s_unavailable_guild::fromJson(elem));
			}
			setIfExists(j, "session_id", r.session_id);
			return r;
		}
	};

	struct presence {
		s_user user;
		std::string guild_id;
		std::string status;
		std::vector<s_activity> activities;
		s_client_status client_status;

		static presence fromJson(json j) {
			presence presence;

			presence.user = s_user::fromJson(j["user"]);
			setIfExists(j, "guild_id", presence.guild_id);
			setIfExists(j, "status", presence.status);

			for (json elem : j["activities"])
				presence.activities.push_back(s_activity::fromJson(elem));

			presence.client_status = s_client_status::fromJson(j["client_status"]);

			return presence;
		}
	};
}

struct s_guild : s_unavailable_guild {
	std::string icon_hash;
	bool owner{};
	std::string owner_id;
	std::string permissions;
	std::string region;
	std::string afk_channel_id;
	int afk_timeout{};
	bool widget_enabled{};
	std::string widget_channel_id;
	int verification_level{};
	int default_message_notifications{};
	int explicit_content_filter{};
	std::vector<s_role> roles;
	std::vector<s_emoji> emojis;
	std::vector<std::string> features;
	int mfa_level{};
	std::string application_id;
	std::string system_channel_id;
	int system_channel_flags{};
	std::string rules_channel_id;
	std::string joined_at;
	bool large{};
	bool unavailable{};
	int member_count{};
	std::vector<s_voice_state> voice_states;
	std::vector<s_member> members;
	std::vector<s_channel> channels;
	std::vector<s_channel> threads;
	std::vector<gateway_events::presence> presences;
	int max_presences{};
	int max_members{};
	std::string vanity_url_code;
	std::string description;
	std::string banner;
	int premium_tier{};
	int premium_subscription_count{};
	std::string preferred_locale;
	std::string public_updates_channel_id;
	int max_video_channel_users{};
	s_welcome_screen welcome_screen;
	int nsfw_level{};
	std::vector<s_stage_instance> stage_instances;
	std::vector<s_sticker> stickers;
	std::vector<s_guild_scheduled_event> guild_scheduled_events;
	bool premium_progress_bar_enabled{};

	static s_guild fromJson(json j) {
		s_guild result;

		// unavailable guild

		setIfExists(j, "id", result.id);
		setIfExists(j, "name", result.name);
		setIfExists(j, "icon", result.icon);
		setIfExists(j, "splash", result.splash);
		setIfExists(j, "discovery_splash", result.discovery_splash);
		setIfExists(j, "approximate_member_count", result.approximate_member_count);
		setIfExists(j, "approximate_presence_count", result.approximate_presence_count);
		setIfExists(j, "description", result.description);

		// available guild

		setIfExists(j, "icon_hash", result.icon_hash);
		setIfExists(j, "owner", result.owner);
		setIfExists(j, "owner_id", result.owner_id);
		setIfExists(j, "permissions", result.permissions);
		setIfExists(j, "region", result.region);
		setIfExists(j, "afk_channel_id", result.afk_channel_id);
		setIfExists(j, "afk_timeout", result.afk_timeout);
		setIfExists(j, "widget_enabled", result.widget_enabled);
		setIfExists(j, "widget_channel_id", result.widget_channel_id);
		setIfExists(j, "verification_level", result.verification_level);
		setIfExists(j, "default_message_notifications", result.default_message_notifications);
		setIfExists(j, "explicit_content_filter", result.explicit_content_filter);

		if (j.contains("roles"))
			for (json elem : j["roles"]) result.roles.push_back(s_role::fromJson(elem));

		if (j.contains("emojis"))
			for (json elem : j["emojis"]) result.emojis.push_back(s_emoji::fromJson(elem));

		if (j.contains("features"))
			for (std::string str : j["features"]) result.features.push_back(str);

		setIfExists(j, "mfa_level", result.mfa_level);
		setIfExists(j, "application_id", result.application_id);
		setIfExists(j, "system_channel_id", result.system_channel_id);
		setIfExists(j, "system_channel_flags", result.system_channel_flags);
		setIfExists(j, "rules_channel_id", result.rules_channel_id);
		setIfExists(j, "joined_at", result.joined_at);
		setIfExists(j, "large", result.large);
		setIfExists(j, "unavailable", result.unavailable);
		setIfExists(j, "member_count", result.member_count);

		if (j.contains("voice_states"))
			for (json elem : j["voice_states"]) result.voice_states.push_back(s_voice_state::fromJson(elem));

		if (j.contains("members"))
			for (json elem : j["members"]) result.members.push_back(s_member::fromJson(elem));

		if (j.contains("channels"))
			for (json elem : j["channels"]) result.channels.push_back(s_channel::fromJson(elem));

		if (j.contains("threads"))
			for (json elem : j["threads"]) result.threads.push_back(s_channel::fromJson(elem));

		if (j.contains("presences"))
			for (json elem : j["presences"]) result.presences.push_back(gateway_events::presence::fromJson(elem));

		setIfExists(j, "max_presences", result.max_presences);
		setIfExists(j, "max_members", result.max_members);
		setIfExists(j, "vanity_url_code", result.vanity_url_code);
		setIfExists(j, "description", result.description);
		setIfExists(j, "banner", result.banner);
		setIfExists(j, "premium_subscription_count", result.premium_subscription_count);
		setIfExists(j, "preferred_locale", result.preferred_locale);
		setIfExists(j, "public_updates_channel_id", result.public_updates_channel_id);
		setIfExists(j, "max_video_channel_users", result.max_video_channel_users);

		if (j.contains("welcome_screen"))
			result.welcome_screen = s_welcome_screen::fromJson(j["welcome_screen"]);

		setIfExists(j, "nsfw_level", result.approximate_presence_count);

		if (j.contains("stage_instances"))
			for (json elem : j["stage_instances"]) result.stage_instances.push_back(s_stage_instance::fromJson(elem));

		if (j.contains("stickers"))
			for (json elem : j["stickers"]) result.stickers.push_back(s_sticker::fromJson(elem));

		if (j.contains("guild_scheduled_events"))
			for (json elem : j["guild_scheduled_events"]) result.guild_scheduled_events.push_back(s_guild_scheduled_event::fromJson(elem));

		setIfExists(j, "premium_progress_bar_enabled", result.approximate_presence_count);

		return result;
	}

	static s_guild fromUnavailableGuild(s_unavailable_guild ug) {
		s_guild guild;

		guild.id = ug.id;
		guild.name = ug.name;
		guild.icon = ug.icon;
		guild.splash = ug.splash;
		guild.discovery_splash = ug.discovery_splash;
		guild.approximate_member_count = ug.approximate_member_count;
		guild.approximate_presence_count = ug.approximate_presence_count;
		guild.description = ug.description;

		return guild;
	}
};