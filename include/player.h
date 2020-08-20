#pragma once
#include <array>
#include <memory>
#include <random>
#include <list>
#include <utility>
#include <QObject>
#include <QDebug>
#include <proxy.h>
#include <x_oAI.h>

enum class PlayerRole;

class Player: public std::enable_shared_from_this<Player>
{
public:
	friend class XOProxy;
	virtual ~Player();
	Player();
	bool registration(std::weak_ptr<XOProxy> proxy_ptr, const QString& name);
	void finish(bool winner = false);
	void turn_start_notify();
	void turn_end_notify();
	void start_request();
	QString name() const;
	bool your_turn() const;
	virtual PlayerType type() { return PlayerType::Unknown; }
protected:
	PlayerRole m_player_role;
	XOProxy::id m_id;
	virtual void turn_start(){}
	virtual void turn_end(){}
	long current_id = -1;
	bool m_your_turn = false;
	std::weak_ptr<XOProxy> m_proxy_ptr;
	QString m_name;
};

class HumanPlayer: public Player
{
public:
	HumanPlayer();
	~HumanPlayer() override;
	PlayerType type() override;
private:
	void turn_start() override;
	void turn_end() override;
};

class SimpleAI: public Player
{
public:
	SimpleAI();
	~SimpleAI() override;
	PlayerType type() override;
private:
	void turn_start() override;
	void turn_end() override;
	AI m_ai;
};

class SmartAI: public Player
{
public:
	SmartAI();
	~SmartAI() override;
	PlayerType type() override;
private:
	void turn_start() override;
	void turn_end() override;
	AI m_ai;
};
