#pragma once
#include <QAbstractListModel>
#include <array>
#include <memory>
#include <random>
#include <list>
#include <utility>
#include <QObject>
#include <QDebug>
#include <general.h>

enum class FieldState;

class XOModel: public QAbstractListModel
{
	Q_OBJECT
public:
	enum Role{
		STATE = Qt::UserRole + 1
	};
	XOModel(QObject *parent = nullptr);
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	void refresh();
	QHash<int, QByteArray> roleNames() const;
	FieldState get(const size_t index) const;
	void set(const size_t index,
			 const FieldState& state);
	std::array<FieldState, 9> getPlay_fields() const;
	void flush(){
		for (auto& item: m_play_fields){
			item = FieldState::ZERO;
		}
		dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, 0));
	}
private:
	std::array<FieldState, 9> m_play_fields;
};

