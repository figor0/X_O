#include <player.h>
#include <general.h>
#include <wait.h>
#include <unistd.h>

Player::~Player(){}

Player::Player()
{}

bool Player::registration(std::weak_ptr<XOProxy> proxy_ptr,
						  const QString& name)
{
	bool success = false;
	m_proxy_ptr = proxy_ptr;
	auto proxy_shared_ptr = m_proxy_ptr.lock();
	auto geted_data = proxy_shared_ptr->try_registration(shared_from_this());
	if (geted_data.second != PlayerRole::NO){
		m_id = geted_data.first;
		m_player_role = geted_data.second;
		m_name = name;
		success = true;
		start_request();
	}
	return success;
}

void Player::finish(bool winner)
{
	m_proxy_ptr.reset();
	if (winner == true){
		qDebug() << "EEEEEEEE, i am winner";
	}
	m_your_turn = false;
}

void Player::turn_start_notify()
{
	m_your_turn = true;
	turn_start();
}

void Player::turn_end_notify()
{
	m_your_turn = false;
	turn_end();
}

void Player::start_request()
{
	m_proxy_ptr.lock()->start();
}

QString Player::name() const
{
	return m_name;
}

bool Player::your_turn() const
{
    return m_your_turn;
}

HumanPlayer::HumanPlayer()
{}

HumanPlayer::~HumanPlayer()
{}

PlayerType HumanPlayer::type()
{
	return PlayerType::Human;
}

void HumanPlayer::turn_start()
{
	qDebug() << "User player turn start. Name = " << name();
}

void HumanPlayer::turn_end()
{
	qDebug() << "User player turn end. Name = " << name();
}

SimpleAI::SimpleAI(): m_ai(100)
{}

SimpleAI::~SimpleAI(){}

PlayerType SimpleAI::type()
{
	return PlayerType::SimpleAI;
}

void SimpleAI::turn_start()
{
	qDebug() << "SimpleAI player turn start. Name = " << name();
	auto proxy_prt = m_proxy_ptr.lock();
	auto area = proxy_prt->getFieldsImage();
	proxy_prt->changeRequest(m_ai.choose_field(area),
							 Role2Field(m_player_role),
							 m_id);
}

void SimpleAI::turn_end()
{
	qDebug() << "SimpleAI player turn end. Name = " << name();
}

SmartAI::SmartAI(): m_ai(0)
{}

SmartAI::~SmartAI()
{}

PlayerType SmartAI::type()
{
	return PlayerType::SmartAI;
}

void SmartAI::turn_start()
{
	qDebug() << "SmartAI player turn start. Name = " << name();
	auto proxy_prt = m_proxy_ptr.lock();
	auto area = proxy_prt->getFieldsImage();
	auto index = m_ai.choose_field(area);
	qDebug() << index;
	proxy_prt->changeRequest(index,
							 Role2Field(m_player_role),
							 m_id);

}

void SmartAI::turn_end()
{
	qDebug() << "SmartAI player turn end. Name = " << name();
}
