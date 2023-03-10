#pragma once

#include<QByteArray>
#include <QObject>
#include<QString>
#include<QDateTime>
#include<QJsonValue>
#include<QJsonObject>
#include<set>
class Booking
{
	public:
		Booking(const QDateTime start,const QDateTime finish);
        Booking(QDataStream &buffer, bool with_passcode=false);
        QJsonObject get_jsob(void)const{
            QJsonObject var;
            var.insert("start",m_start.toString());
            var.insert("finish",m_finish.toString());
            var.insert("code_str",code_str_);
            return var;
        }
		std::vector<QDate> get_days(void)const;
		std::vector<int> get_hours(const QDate& day)const;
		bool check_validity(const QDateTime & ref)const;
        QString code_str(void)const{return code_str_;};
		void set_code_str(QString code);
        QByteArray get_passcode(void)const{return m_passcode;}
		bool verify_code_str(QString code_str)const;
		QDateTime start(void)const{return m_start;}
		QDateTime finish(void)const{return m_finish;}
		void set_start(QDateTime start_){m_start=start_;}
		void set_finish(QDateTime finish_){m_finish=finish_;}
		friend bool operator < (const Booking& b1, const Booking& b2)
		{
			return(b1.m_start<b2.m_start&&b1.m_finish<b2.m_start);
		}
		friend bool operator < (const Booking& b1, const QDateTime& b2)
		{
			return(b1.m_finish<b2);
		}
        void serialize(QDataStream &buffer,bool with_passcode) const;
		quint64 calculate_price(quint64 per_hour)const;
        static std::tuple<std::set<Booking>, quint64, QByteArray,quint32> deserialize_state(QByteArray &state);
        static QByteArray serialize_state(const std::set<Booking>& books, const quint64&price_per_hour_, const QByteArray &pay_to_address_hash,const quint32& consindex);
        static Booking get_new_booking_from_metadata(QByteArray& metadata);
        static QByteArray create_new_bookings_metadata(Booking &book);
signals:
		void state_changed(void);
	private:

        QDateTime m_start, m_finish;
		QByteArray m_passcode;   //32 byte
        QString code_str_;
};

