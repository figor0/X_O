#pragma once
#include <random>
#include <array>
#include <general.h>
#include <assert.h>
#include <map>

namespace AI_logic {
	using PlayArea = std::array<FieldState, 9>;
	size_t random_action(const PlayArea& play_area);
	size_t smart_action(const PlayArea& play_area);
	size_t current_turn(const PlayArea& play_area);
	size_t find_first(const FieldState field_state,
					  const PlayArea& play_area);
	size_t processTurn2(const PlayArea& play_area);
	size_t processOtherTurn(PlayArea play_area, const PlayerRole role);
	std::map<size_t, int> forWin(const PlayArea& play_area,
								 const PlayerRole player_role);
	std::map<size_t, int>::const_iterator
	find_max_value(const std::map<size_t, int>& index_map);
	std::pair<bool, size_t> futureAnaliz(PlayArea play_area, PlayerRole role);
}

class AI
{
public:
	AI(const int fuckup_chanse);
	size_t choose_field(const AI_logic::PlayArea& play_area);
	int fuckup_chanse() const;
private:
	bool isFuckUp();
	const int m_fuckup_chanse;
};
