#pragma once

// Player, Actor ±¸Á¶Ã¼

struct __cppobj Player : Actor, IPlayer {
	unsigned int m_characterId;
	std::basic_string<char, std::char_traits<char>, std::allocator<char> > m_playerName;
	std::basic_string<char, std::char_traits<char>, std::allocator<char> > m_teamName;
	unsigned __int8 m_avatarIndex;
	unsigned int m_colors[4];
	std::map<IItem*, ItemAndCount, std::less<IItem*>, std::allocator<std::pair<IItem* const, ItemAndCount> > > m_inventory;
	std::set<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > m_pickups;
	std::map<IItem*, float, std::less<IItem*>, std::allocator<std::pair<IItem* const, float> > > m_cooldowns;
	std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, unsigned int, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::basic_string<char, std::char_traits<char>, std::allocator<char> > const, unsigned int> > > m_circuitInputs;
	std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::vector<bool, std::allocator<bool> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::basic_string<char, std::char_traits<char>, std::allocator<char> > const, std::vector<bool, std::allocator<bool> > > > > m_circuitOutputs;
	bool m_admin;
	bool m_pvpEnabled;
	bool m_pvpDesired;
	float m_pvpChangeTimer;
	int m_pvpChangeReportedTimer;
	bool m_changingServerRegion;
	std::basic_string<char, std::char_traits<char>, std::allocator<char> > m_currentRegion;
	std::basic_string<char, std::char_traits<char>, std::allocator<char> > m_changeRegionDestination;
	std::set<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > m_aiZones;
	int m_mana;
	float m_manaRegenTimer;
	float m_healthRegenCooldown;
	float m_healthRegenTimer;
	int m_countdown;
	Vector3 m_remoteLookPosition;
	Rotation m_remoteLookRotation;
	IItem* m_equipped[10];
	unsigned int m_currentSlot;
	std::map<IQuest*, PlayerQuestState, std::less<IQuest*>, std::allocator<std::pair<IQuest* const, PlayerQuestState> > > m_questStates;
	IQuest* m_currentQuest;
	float m_walkingSpeed;
	float m_jumpSpeed;
	float m_jumpHoldTime;
	ActorRef<NPC> m_currentNPC;
	std::basic_string<char, std::char_traits<char>, std::allocator<char> > m_currentNPCState;
	ILocalPlayer* m_localPlayer;
	WriteStream* m_eventsToSend;
	bool m_itemsUpdated;
	float m_itemSyncTimer;
	unsigned int m_chatMessageCounter;
	float m_chatFloodDecayTimer;
	IItem* m_lastHitByItem;
	float m_lastHitItemTimeLeft;
	float m_circuitStateCooldownTimer;
};

struct __cppobj Actor : IActor {
	unsigned int m_refs;
	unsigned int m_id;
	IUE4Actor* m_target;
	TimerSet* m_timers;
	std::basic_string<char, std::char_traits<char>, std::allocator<char> > m_blueprintName;
	ActorRef<IActor> m_owner;
	int m_health;
	std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, bool, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::basic_string<char, std::char_traits<char>, std::allocator<char> > const, bool> > > m_states;
	float m_forwardMovementFraction;
	float m_strafeMovementFraction;
	Vector3 m_remotePosition;
	Vector3 m_remoteVelocity;
	Rotation m_remoteRotation;
	float m_remoteLocationBlendFactor;
	Spawner* m_spawner;
};
