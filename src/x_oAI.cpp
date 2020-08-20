#include <x_oAI.h>
#include <map>
#include <QDebug>
#include <exception>
#include <algorithm>

AI::AI(const int fuckup_chanse): // between 0 and 100 percent
	m_fuckup_chanse(fuckup_chanse)
{
	assert((fuckup_chanse >= 0 == true) && (fuckup_chanse <= 100 == true));
}

size_t AI::choose_field(const AI_logic::PlayArea &play_area)
{
	std::cout << play_area << std::endl;
	size_t result;
	if (isFuckUp()){
		result = AI_logic::random_action(play_area);
	} else{
		result = AI_logic::smart_action(play_area);
	}
	return result;
}

size_t AI_logic::random_action(const AI_logic::PlayArea &play_area)
{
	size_t available = current_turn(play_area);
	int result = -1;
	if (available > 0 && available < 10){
		size_t counter = rand()%(play_area.size() + 1 - available);
		for (size_t i = 0; i < play_area.size(); i++)
		{
			if (play_area.at(i) == FieldState::ZERO){
				if (counter == 0){
					result = i;
				}
				counter--;
			}
		}
	}
	if (result < 0 || result >= 9){
		throw std::invalid_argument("");
	}
	return result;
}

size_t AI_logic::smart_action(const AI_logic::PlayArea &play_area)
{
	size_t result;
	size_t turn = current_turn(play_area);
	qDebug() << "Turn: " << current_turn(play_area);
	if ( turn == 1){
		result = 4;
	} else if ( turn == 2){
		result = processTurn2(play_area);
	} else if ( turn%2 == 1){
		result = processOtherTurn(play_area, PlayerRole::X);
	} else {
		result = processOtherTurn(play_area, PlayerRole::O);
	}
	return result;
}

size_t AI_logic::current_turn(const AI_logic::PlayArea &play_area)
{
	size_t turn_number = 10;
	for (const auto& item: play_area){
		if (item == FieldState::ZERO){
			turn_number--;
		}
	}
	return turn_number;
}

size_t AI_logic::find_first(const FieldState field_state, const AI_logic::PlayArea &play_area)
{
	size_t result = 0;
	for (const auto& item: play_area){
		if (item == field_state){
			break;
		}
		result++;
	}
	return result;
}

size_t AI_logic::processTurn2(const AI_logic::PlayArea &area){
	size_t first_x_position = find_first(FieldState::X, area);
	size_t result;
	if (first_x_position == 4){
		result = 0;
	} else{
		result = 4;
	}
	return result;
}

std::map<size_t, int>::const_iterator find_max_value(const std::map<size_t, int>& index_map)
{
	std::map<size_t, int>::const_iterator result_it = index_map.end();
	int max_counter = 0;
	for (auto map_it = index_map.begin(); map_it != index_map.end(); map_it++)
	{
		if (map_it->second > max_counter){
			result_it = map_it;
		}
	}
	return result_it;
}

size_t AI_logic::processOtherTurn(AI_logic::PlayArea play_area, const PlayerRole role)
{
	PlayerRole your_role = role;
	PlayerRole enemy_role;
	your_role == PlayerRole::X ?
				enemy_role = PlayerRole::O :
				enemy_role = PlayerRole::X;
	auto wincheck = forWin(play_area, your_role);
	auto max_value_it = find_max_value(wincheck);
	if ( max_value_it != wincheck.end() ){
		qDebug() << "Exist win path";
		return max_value_it->first;
	}
	auto fallcheck = forWin(play_area, enemy_role);
	auto enemy_max_value_it = find_max_value(fallcheck);
	if ( enemy_max_value_it != fallcheck.end() ){
		qDebug() << "Exist fall path";
		return enemy_max_value_it->first;
	}
	auto win_way = futureAnaliz(play_area, your_role);
	if ( win_way.first == true)
	{
		qDebug() << "Exist pre win path";
		return win_way.second;
	}
	return random_action(play_area);
}


std::map<size_t, int> AI_logic::forWin(const AI_logic::PlayArea &play_area,
										 const PlayerRole play_role){
	std::map<size_t, int> result;
	if (play_role != PlayerRole::NO){
		for (const auto& condition: WinCondition::conditions())
		{
			if (WinCondition::checkPreWinner(play_area, condition) == play_role){
				auto empties = WinCondition::find_empty(play_area, condition);
				if (empties.size() != 0){
					if (result.find(empties.at(0)) == result.end())
						result[empties.at(0)] = 0;
					result[empties.at(0)]++;
				}
			}
		}
	}
	return result;
}

bool AI::isFuckUp()
{
	return m_fuckup_chanse > rand()%100;
}

int AI::fuckup_chanse() const
{
	return m_fuckup_chanse;
}

std::pair<bool, size_t> AI_logic::futureAnaliz(AI_logic::PlayArea play_area, PlayerRole role){
	FieldState state;
	role == PlayerRole::X ?
			state = FieldState::X:
			state = FieldState::O;
	std::map<size_t, int> future;
	size_t counter = 0;
	for (auto& item: play_area){
		if (item == FieldState::ZERO){
			role == PlayerRole::X ?
					item = FieldState::X:
					item = FieldState::O;
			auto prewin = forWin(play_area, role);
			future[counter] = prewin.size();
			item = FieldState::ZERO;
		}
		counter++;
	}
	std::pair<bool, size_t> result {false, 0};
	auto max_future_value = find_max_value(future);
	if (max_future_value != future.end()){
		result.first = true;
		result.second= max_future_value->first;
	}
	return result;
}

std::map<size_t, int>::const_iterator
AI_logic::find_max_value(const std::map<size_t, int> &index_map)
{
	auto result_it = index_map.end();
	int max = 0;
	for (auto it = index_map.begin();
		 it != index_map.end();
		 it++)
	{
		if (it->second > max){
			max = it->second;
			result_it = it;
		}
	}
	return result_it;
}
