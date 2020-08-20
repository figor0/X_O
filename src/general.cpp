#include <general.h>


const WinCondition::Conditions& WinCondition::conditions()
{
	static const WinCondition::Conditions XOWinCondition
	{{
		{0, 1, 2},
		{3, 4, 5},
		{6, 7, 8},
		{0, 3, 6},
		{1, 4, 7},
		{2, 5, 8},
		{0, 4, 8},
		{2, 4, 6}
	}};
	return XOWinCondition;
}


PlayerRole WinCondition::checkwinner(const WinCondition::PlayArea &states,
									 const WinCondition::Condition& condition)
{
	PlayerRole pre_winner = PlayerRole::NO;
	size_t x_counter = 0;
	size_t o_counter = 0;
	for (const auto& index: condition){
		if ( states.at(index) == FieldState::X){
			x_counter++;
		} else if ( states.at(index) == FieldState::O){
			o_counter++;
		}
	}
	if (x_counter == 3 || o_counter == 3){
		o_counter < x_counter ?
					pre_winner = PlayerRole::X:
					pre_winner = PlayerRole::O;
	}
	return pre_winner;
}

PlayerRole WinCondition::checkPreWinner(
						const WinCondition::PlayArea &states,
						const WinCondition::Condition &condition)
{
	PlayerRole pre_winner = PlayerRole::NO;
	size_t x_counter = 0;
	size_t o_counter = 0;
	for (const auto& index: condition){
		if ( states.at(index) == FieldState::X){
			x_counter++;
			o_counter = 0;
		} else if ( states.at(index) == FieldState::O){
			o_counter++;
			x_counter = 0;
		}
	}
	if (x_counter == 2 || o_counter == 2){
		o_counter < x_counter ?
					pre_winner = PlayerRole::X:
					pre_winner = PlayerRole::O;
	}
	return pre_winner;
}

std::vector<WinCondition::Index> WinCondition::find_empty(const WinCondition::PlayArea &play_area,
														  const WinCondition::Condition &condition)
{
	std::vector<Index> result;
	result.reserve(condition.size());
	for (const auto& index: condition){
		if (play_area.at(index) == FieldState::ZERO){
			result.push_back(index);
		}
	}
	return result;
}



std::vector<WinCondition::Index> WinCondition::find_empty(const WinCondition::PlayArea &play_area)
{
	size_t counter = 0;
	std::vector<WinCondition::Index> result;
	for (const auto& item: play_area){
		if (item == FieldState::ZERO){
			result.push_back(counter);
		}
		counter++;
	}
	return result;
}

std::ostream &operator<<(std::ostream &reader, const FieldState state)
{
	switch(state){
	case FieldState::O:
		reader << 'O';
		break;
	case FieldState::X:
		reader << 'X';
		break;
	case FieldState::ZERO:
		reader << 'Z';
		break;
	}
	return reader;
}

std::ostream &operator<<(std::ostream &reader, const WinCondition::PlayArea &play_area)
{
	int counter = 2;
	for (const auto& item: play_area)
	{
		reader << item;
		if (counter == 0){
			reader << "\n\n";
			counter = 2;
		} else{
			reader << ' ';
			counter--;
		}
	}
	return reader;
}

PlayerRole Field2Role(const FieldState state)
{
	PlayerRole result;
	switch (state) {
	case FieldState::O:
		result = PlayerRole::O;
		break;
	case FieldState::X:
		result = PlayerRole::X;
		break;
	case FieldState::ZERO:
		result = PlayerRole::NO;
		break;
	}
	return result;
}

FieldState Role2Field(const PlayerRole state)
{
	FieldState result;
	switch (state) {
	case PlayerRole::O:
		result = FieldState::O;
		break;
	case PlayerRole::X:
		result = FieldState::X;
		break;
	case PlayerRole::NO:
		result = FieldState::ZERO;
		break;
	}
	return result;
}

PlayerRole WinCondition::checkwinner(const WinCondition::PlayArea &area){
	PlayerRole result = PlayerRole::NO;
	auto& conditions_list = conditions();
	for (const auto& condition: conditions_list){
		auto winner_role = checkwinner(area, condition);
		if ( winner_role != PlayerRole::NO){
			result = winner_role;
			break;
		}
	}
	return result;
}
