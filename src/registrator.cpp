#include <registrator.h>


int TypeToInt(const PlayerType type)
{
	int result;
	switch (type) {
	case PlayerType::Human:
		result = 0;
		break;
	case PlayerType::SimpleAI:
		result = 1;
		break;
	case PlayerType::SmartAI:
		result = 2;
		break;
	default:
		result = -1;
	}
	return result;
}

PlayerType IntToType(const int value)
{
	PlayerType result;
	switch (value) {
	case 0:
		result = PlayerType::Human;
		break;
	case 1:
		result = PlayerType::SimpleAI;
		break;
	case 2:
		result = PlayerType::SmartAI;
		break;
	default:
		result = PlayerType::Unknown;
	}
	return result;
}

std::shared_ptr<Player> createPlayer(PlayerType type)
{
	std::shared_ptr<Player> result;
	switch (type) {
	case PlayerType::Human:
		result = std::make_shared<HumanPlayer>();
		break;
	case PlayerType::SimpleAI:
		result = std::make_shared<SimpleAI>();
		break;
	case PlayerType::SmartAI:
		result = std::make_shared<SmartAI>();
		break;
	default:
		std::cerr << "Unknown player int type" << '\n';
	}
	return result;
}

Registrator::Registrator(Registrator::Proxy_ptr proxy_ptr, QObject *parent): QObject(parent),
	m_proxy_ptr(proxy_ptr)
{}

bool Registrator::registration(const int first_player, const int second_player)
{
	refresh();
	bool result = false;
	PlayerType player1 = IntToType(first_player);
	PlayerType player2 = IntToType(second_player);
	if (player1 != PlayerType::Unknown && player2 != PlayerType::Unknown){
		auto player1_ptr = createPlayer(IntToType(first_player));
		auto player2_ptr = createPlayer(IntToType(second_player));
		if (player1_ptr->registration(m_proxy_ptr, "Player1") == true &&
			player2_ptr->registration(m_proxy_ptr, "Player2") == true){
			m_players[0] = player1_ptr;
			m_players[1] = player2_ptr;
			result = true;
		}
	}
	return result;
}

void Registrator::refresh(){
	for (auto& item: m_players){
		item.reset();
		item.reset();
	}
}
