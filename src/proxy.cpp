#include <proxy.h>
#include <player.h>
#include <general.h>
#include <QModelIndex>

XOProxy::XOProxy(std::shared_ptr<XOModel> model_ptr,
				 QObject *parent): QObject(parent),
	m_game_state(GameState::Stoped),
	m_model_ptr(model_ptr)
{}

ChangeState XOProxy::changeRequest(const int index,
								   const FieldState state,
								   const long id)
{
	ChangeState change_result = ChangeState::GAME_STOPED;
	qDebug() << "Proxy change request";
	std::cout << state << std::endl;
	if ( game_state() == GameState::Started){
		change_result = ChangeState::ACESS_DENIED;
		PlayerRole current_role = Field2Role(state);
		auto current_player = m_players[current_role];
		if (current_player.first == id){
			if (m_model_ptr->get(index) == FieldState::ZERO)
			{
				qDebug() << "Zero field";
				m_model_ptr->set(index, state);
				auto winner = WinCondition::checkwinner(m_model_ptr->getPlay_fields());
				if (winner == PlayerRole::NO){
					notifynext();
					change_result = ChangeState::SUCSESS;
				} else {
					finish(winner);
				}
			} else {
				change_result = ChangeState::FIELD_NOT_EMPTY;
			}
		}
	}
	return change_result;
}

bool XOProxy::clickRequest(int index)
{
	bool result = false;
	Player_ptr current_player_ptr;
	for (const auto& player: m_players){
		auto player_ptr = player.second.second;
		if ( player_ptr->your_turn() == true && player_ptr->type() == PlayerType::Human){
			current_player_ptr = player.second.second;
			result = true;
			break;
		}
	}
	result = changeRequest(index,
				  Role2Field(current_player_ptr->m_player_role),
				  current_player_ptr->m_id) == ChangeState::SUCSESS;
	return result;
}

FieldState XOProxy::getFieldValue(const int index)
{
	return m_model_ptr->get(index);
}

std::array<FieldState, 9> XOProxy::getFieldsImage(){
	return m_model_ptr->getPlay_fields();
}

void XOProxy::start()
{
	if (m_players.size() == 2 && game_state() == GameState::Stoped)
	{
		qDebug() << "Game started";
		m_model_ptr->flush();
		emit startNotify();
		m_game_state = GameState::Started;
		notifynext();
	} else {
		qDebug() << "Game not started, size = " << m_players.size();
	}
}

void XOProxy::finish(const PlayerRole winner = PlayerRole::NO)
{
	if (game_state() == GameState::Started)
	{
		if (winner != PlayerRole::NO){
			for (auto& player: m_players){
				if (player.first == winner){
					player.second.second->finish(true);
					emit endNotify(player.second.second->name());
				} else {
					player.second.second->finish(false);
				}
			}
		} else {
			emit endNotify("");
			for (auto& player: m_players){
				player.second.second->finish(false);
			}
		}
		m_game_state = GameState::Stoped;
	}
	refresh();
}

void XOProxy::notifynext()
{
	m_turn++;
	m_x_turn = !m_x_turn;
	qDebug() << "Turn: " << m_turn;
	if (m_turn <= 9 && m_turn >= 0){
		auto& x_player = m_players[PlayerRole::X].second;
		auto& o_player = m_players[PlayerRole::O].second;
		if (m_x_turn == false){
			o_player->turn_end_notify();
			x_player->turn_start_notify();
			emit playerChanged(x_player->name());
		} else{
			x_player->turn_end_notify();
			o_player->turn_start_notify();
			emit playerChanged(o_player->name());
		}
	} else {
		finish(PlayerRole::NO);
	}
}

XOProxy::GameState XOProxy::game_state() const
{
	return m_game_state;
}

void XOProxy::refresh(){
	m_model_ptr->refresh();
	m_players.clear();
	m_x_turn = true;
	m_turn = 0;
	m_game_state = GameState::Stoped;
}

PlayerRole generateRole()
{
	PlayerRole result;
	switch (rand()%2) {
	case 0: result = PlayerRole::X;
	case 1: result = PlayerRole::O;
	}
	return result;
}
std::pair<XOProxy::id, PlayerRole> XOProxy::try_registration(XOProxy::Player_ptr player_ptr)
{
	std::pair<XOProxy::id, PlayerRole> result {0, PlayerRole::NO};
	if ( game_state() == GameState::Stoped && m_players.size() < 2){
		result.first = rand();
		PlayerRole generated_role;
		if (m_players.size() == 0){
			generated_role = generateRole();
		} else{
			if (m_players.count(PlayerRole::X) == 0){
				generated_role = PlayerRole::X;
			} else{
				generated_role = PlayerRole::O;
			}
		}
		m_players[generated_role].second = player_ptr;
		m_players[generated_role].first = result.first;
		result.second = generated_role;
	}
	return result;
}

