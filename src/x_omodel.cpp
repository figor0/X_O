#include <x_omodel.h>
#include <algorithm>
#include <general.h>

XOModel::XOModel(QObject *parent)
{
	for (auto& item: m_play_fields){
		item = FieldState::ZERO;
	}
}

int XOModel::rowCount(const QModelIndex &parent) const{
	return 9;
}

int XOModel::columnCount(const QModelIndex &parent) const{
	return 1;
}

QVariant XOModel::data(const QModelIndex &index, int role) const
{
	QVariant result;
	if (index.isValid() == true){
		switch (role) {
		case Role::STATE:{
			FieldState state = m_play_fields[index.row()];
			if ( state == FieldState::ZERO){
				result.setValue(QString(""));
			} else if ( state == FieldState::O){
				result.setValue(QString("O"));
			} else if ( state == FieldState::X){
				result.setValue(QString("X"));
			}
			break;
		}
		}
	} else{
		result.setValue(QString());
	}
	return result;
}

void XOModel::refresh(){
	for (auto& item: m_play_fields){
		item = FieldState::ZERO;
	}
	auto first_index = createIndex(0, 0);
	auto last_index = createIndex(rowCount() - 1, 0);
	dataChanged(first_index, last_index);
}

QHash<int, QByteArray> XOModel::roleNames() const{
	return {
		{STATE, "state"}
	};
}

FieldState XOModel::get(const size_t index) const
{
	return m_play_fields.at(index);
}

void XOModel::set(const size_t index, const FieldState &state){
	if (index < m_play_fields.size()){
		m_play_fields[index] = state;
	}
	QModelIndex first_item = createIndex(index, 0);
	emit dataChanged(first_item, first_item);
}

std::array<FieldState, 9> XOModel::getPlay_fields() const
{
	return m_play_fields;
}
