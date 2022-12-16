#pragma once

#include<QByteArray>
#include <QDateTime>
#include <QJsonValue>

class Booking
{

	public:
		Booking(const QDateTime start,const QDateTime finish);
		Booking(const QJsonValue& val);
		bool check_validity(const QDateTime & ref)const;
		void set_code(quint64 code);
		bool verify_code(quint64 code)const;
		bool valid()const{return is_valid;}
		QDateTime get_start(void){return m_start;}
		QDateTime get_finish(void){return m_finish;}
		friend bool operator < (const Booking& b1, const Booking& b2)
		{
			return(b1.m_start<b2.m_start&&b1.m_finish<b2.m_start);
		}
		friend bool operator < (const Booking& b1, const QDateTime& b2)
		{
			return(b1.m_finish<b2);
		}
		QJsonObject get_Json(bool with_passcode) const;
		quint64 calculate_price(quint64 per_hour)const;
	private:
		void check_booking(void);

		QDateTime m_start,m_finish;
		QByteArray m_passcode;   //32 bytes
		bool is_valid;
};

