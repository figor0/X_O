#include <array>
#include <player.h>
#include <memory>
#include <proxy.h>
#include <QObject>
#include <iostream>
#include <general.h>

int TypeToInt(const PlayerType type);

PlayerType IntToType(const int value);

std::shared_ptr<Player> createPlayer(PlayerType type);

class Registrator: public QObject
{
	Q_OBJECT
public:
	using Players = std::array<std::shared_ptr<Player>, 2>;
	using Proxy_ptr = std::shared_ptr<XOProxy>;
	Registrator(Proxy_ptr proxy_ptr,
				QObject* parent = nullptr);
	Q_INVOKABLE bool registration(const int first_player,
					  const int second_player);
	Q_INVOKABLE void refresh();
private:
	Players m_players;
	Proxy_ptr m_proxy_ptr;
};
