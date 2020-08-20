#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <x_omodel.h>
#include <x_oAI.h>
#include <stdexcept>

using namespace AI_logic;
using namespace WinCondition;

BOOST_AUTO_TEST_CASE(conditions_check)
{
	const auto& conditions_list = conditions();
	BOOST_CHECK(conditions_list.size() == 8);
	for (const auto& item: conditions_list)
	{
		BOOST_CHECK(item.size() == 3);
	}
}

BOOST_AUTO_TEST_CASE(find_empty_test)
{
	{
		AI_logic::PlayArea area
		{
			FieldState::ZERO, FieldState::ZERO, FieldState::ZERO,
			FieldState::ZERO, FieldState::ZERO, FieldState::ZERO,
			FieldState::ZERO, FieldState::ZERO, FieldState::ZERO
		};
		size_t counter = 9;
		for (auto& item: area){
			item = FieldState::O;
			counter--;
			BOOST_CHECK(find_empty(area).size() == counter);
		}
	}
}

BOOST_AUTO_TEST_CASE( random_test )
{
	AI_logic::PlayArea area
	{
		FieldState::ZERO, FieldState::ZERO, FieldState::ZERO,
		FieldState::ZERO, FieldState::ZERO, FieldState::ZERO,
		FieldState::ZERO, FieldState::ZERO, FieldState::ZERO
	};
	for (int counter = 0; counter < 1000; counter++){
		auto index = random_action(area);
		BOOST_CHECK( index >= 0 && index < 9);
	}
	{
		auto empty_area = area;
		for (size_t counter = 0; counter < area.size(); counter++){
			auto for_change = random_action(empty_area);
			BOOST_CHECK(empty_area.at(for_change) == FieldState::ZERO);
			empty_area[for_change] = FieldState::O;
		}
		BOOST_CHECK_THROW(random_action(empty_area), std::invalid_argument);
	}
}

BOOST_AUTO_TEST_CASE(for_win_test)
{
	using namespace AI_logic;
	AI_logic::PlayArea empty_area
	{
		FieldState::ZERO, FieldState::ZERO, FieldState::ZERO,
		FieldState::ZERO, FieldState::ZERO, FieldState::ZERO,
		FieldState::ZERO, FieldState::ZERO, FieldState::ZERO
	};
	{
		BOOST_CHECK(AI_logic::forWin(empty_area, PlayerRole::X).size() == 0);
		BOOST_CHECK(AI_logic::forWin(empty_area, PlayerRole::O).size() == 0);
	}
	{
		auto thirth_turn = empty_area;
		thirth_turn[4] = FieldState::X;
		thirth_turn[0] = FieldState::X;
		auto win_chanses = forWin(thirth_turn, PlayerRole::X);
		BOOST_CHECK(win_chanses.size() == 1);
		BOOST_CHECK(win_chanses.at(8) == 1);
	}
	{
		auto thirth_turn = empty_area;
		thirth_turn[4] = FieldState::X;
		thirth_turn[0] = FieldState::X;
		thirth_turn[6] = FieldState::X;
		thirth_turn[7] = FieldState::X;
		auto win_chanses = forWin(thirth_turn, PlayerRole::X);
		BOOST_CHECK(win_chanses.size() == 4);
		BOOST_CHECK(win_chanses.at(8) == 2);
	}
	{
		auto thirth_turn = empty_area;
		thirth_turn[4] = FieldState::X;
		thirth_turn[0] = FieldState::X;
		thirth_turn[8] = FieldState::O;
		auto win_chanses = forWin(thirth_turn, PlayerRole::X);
		BOOST_CHECK(win_chanses.size() == 0);
	}
	{
		auto thirth_turn = empty_area;
		thirth_turn[4] = FieldState::X;
		thirth_turn[0] = FieldState::X;
		thirth_turn[6] = FieldState::X;
		thirth_turn[7] = FieldState::X;
		thirth_turn[8] = FieldState::O;
		thirth_turn[1] = FieldState::O;
		thirth_turn[2] = FieldState::O;
		thirth_turn[3] = FieldState::O;
		thirth_turn[5] = FieldState::O;
		auto win_chanses = forWin(thirth_turn, PlayerRole::X);
		BOOST_CHECK(win_chanses.size() == 0);
	}
}

BOOST_AUTO_TEST_CASE(processturn_test)
{
	AI_logic::PlayArea empty_area
	{
		FieldState::ZERO, FieldState::ZERO, FieldState::ZERO,
		FieldState::ZERO, FieldState::ZERO, FieldState::ZERO,
		FieldState::ZERO, FieldState::ZERO, FieldState::ZERO
	};
	{
		auto init_area = empty_area;
		init_area[4] = FieldState::X;
		init_area[0] = FieldState::O;
		auto future_index = futureAnaliz(init_area, PlayerRole::X);
		BOOST_CHECK(future_index.first == true);
		init_area[future_index.second] = FieldState::X;
		auto win_ways = forWin(init_area, PlayerRole::X);
		BOOST_CHECK(win_ways.size() > 0);
	}
}

//BOOST_AUTO_TEST_CASE(ai_test)
//{
//	AI_logic::PlayArea empty_area
//	{
//		FieldState::ZERO, FieldState::ZERO, FieldState::ZERO,
//		FieldState::ZERO, FieldState::ZERO, FieldState::ZERO,
//		FieldState::ZERO, FieldState::ZERO, FieldState::ZERO
//	};
//	{
//		auto play_area = empty_area;
//		AI smart_ai(0);
//		for (size_t i = 0; i < play_area.size() + 2; i++)
//		{
//			if (i % 2 == 0){
//				play_area[smart_ai.choose_field(play_area)] = FieldState::X;
//			} else {
//				play_area[smart_ai.choose_field(play_area)] = FieldState::O;
//			}
//		}
//		BOOST_CHECK(find_empty(play_area).size() == 0);
//		auto& conditions = WinCondition::conditions();
//		for (const auto& condition: conditions){
//			BOOST_CHECK(checkwinner(play_area, condition) == PlayerRole::NO);
//		}
//	}
//}
