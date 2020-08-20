#pragma once
#include <array>
#include <vector>
#include <iostream>


enum class PlayerRole{
	X,
	O,
	NO
};

enum class FieldState{
	ZERO,
	X,
	O
};

enum class PlayerType{
	Human,
	SimpleAI,
	SmartAI,
	Unknown
};


PlayerRole Field2Role(const FieldState state);

FieldState Role2Field(const PlayerRole state);

namespace WinCondition
{
	using Index = size_t;
	using Condition = std::array<Index, 3>;
	using Conditions = std::array<Condition, 8>;
	using PlayArea = std::array<FieldState, 9>;
	PlayerRole checkwinner(const PlayArea& states,
					   const WinCondition::Condition& condition);
	const Conditions& conditions();
	PlayerRole checkwinner(const PlayArea& area);
	PlayerRole checkPreWinner(const PlayArea& states,
							  const WinCondition::Condition& condition);
	std::vector<Index> find_empty(const PlayArea& play_area,
								  const Condition& condition);
	std::vector<Index> find_empty(const PlayArea& play_area);
}

std::ostream& operator<<(std::ostream& reader, const FieldState state);
std::ostream& operator<<(std::ostream& reader,
						 const WinCondition::PlayArea& play_area  );
