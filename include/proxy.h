#pragma once
#include <array>
#include <memory>
#include <random>
#include <list>
#include <utility>
#include <QObject>
#include <QDebug>
#include <x_omodel.h>


class Player;

enum class PlayerRole;

enum class FieldState;

enum class ChangeState
{
	SUCSESS,
	FIELD_NOT_EMPTY,
	ACESS_DENIED,
	GAME_STOPED
};

class XOProxy: public QObject
{
	Q_OBJECT
public:
	using Player_ptr = std::shared_ptr<Player>;
	const int players_amount = 2;
	using id = long;
	using PlayerNode = std::pair<id, std::shared_ptr<Player>>;
	enum class GameState{
		Started,
		Stoped
	};
	XOProxy(std::shared_ptr<XOModel> model_ptr,
			QObject* parent = nullptr);
	ChangeState changeRequest(const int index,
								   const FieldState state,
								   const long id);
	Q_INVOKABLE bool clickRequest(int index);
	FieldState getFieldValue(const int index);
	std::array<FieldState, 9> getFieldsImage();
	std::pair<id, PlayerRole> try_registration(std::shared_ptr<Player> player);
	void start();
	void finish(const PlayerRole winner);
	GameState game_state() const;
signals:
	void startNotify();
	void endNotify(const QString winner_name);
	void playerChanged(const QString player_name);
private:
	void refresh();
	void notifynext();
	GameState m_game_state = GameState::Stoped;
	bool m_x_turn = true;
	size_t m_turn = 0;
	std::map<PlayerRole, PlayerNode> m_players;
	std::shared_ptr<XOModel> m_model_ptr;
};
