#include"booking.hpp"
#include <QJsonObject>
#include <QDataStream>
#include <QCryptographicHash>
#include<QIODevice>
Booking::Booking(const QDateTime start,const QDateTime finish):is_valid(false),
	m_start(start),m_finish(finish)
{
	check_booking();

};

void Booking::set_code(quint64 code)
{
	QByteArray var;
	auto buffer=QDataStream(&var,QIODevice::WriteOnly | QIODevice::Append);
	buffer.setByteOrder(QDataStream::LittleEndian);
	buffer<<m_start.toMSecsSinceEpoch()<<m_finish.toMSecsSinceEpoch()<<code;
	m_passcode=QCryptographicHash::hash(var,QCryptographicHash::Blake2b_256);
}
bool Booking::verify_code(quint64 code)const
{

	QByteArray var;
	auto buffer=QDataStream(&var,QIODevice::WriteOnly | QIODevice::Append);
	buffer.setByteOrder(QDataStream::LittleEndian);
	buffer<<m_start.toMSecsSinceEpoch()<<m_finish.toMSecsSinceEpoch()<<code;
	auto V_passcode=QCryptographicHash::hash(var,QCryptographicHash::Blake2b_256);
	return(V_passcode==m_passcode);
}

Booking::Booking(const QJsonValue& val)
{
	m_start=QDateTime();
	m_start.setMSecsSinceEpoch((val.toObject()["start"].isNull())?0:val.toObject()["start"].toInteger());
	m_finish=QDateTime();
	m_finish.setMSecsSinceEpoch((val.toObject()["finish"].isNull())?0:val.toObject()["finish"].toInteger());
	check_booking();
	if(!val.toObject()["passcode"].isNull())
		m_passcode=QByteArray::fromHex(val.toObject()["passcode"].toString().toLatin1());
	if(m_passcode.size()!=32)
		is_valid=false;

}
bool Booking::check_validity(const QDateTime & ref)const
{
	if(ref<m_finish)return true;

	return false;
}
QJsonObject Booking::get_Json(bool with_passcode) const
{
	QJsonObject var;
	var.insert("start",QString::number(m_start.toMSecsSinceEpoch()));
	var.insert("finish",QString::number(m_finish.toMSecsSinceEpoch()));
	if(with_passcode)var.insert("passcode",QString(m_passcode.toHex()));
	return var;
}
quint64 Booking::calculate_price(quint64 per_hour)const
{
	const auto msecs=m_start.msecsTo(m_finish)/1000/60/60;
	return per_hour*msecs;
}
void Booking::check_booking(void)
{
	auto cday=QDateTime::currentDateTime();
	auto nweekday=cday.addDays(7);
	if(m_start<m_finish&&m_start>cday&&m_finish<nweekday)is_valid=true;
}


