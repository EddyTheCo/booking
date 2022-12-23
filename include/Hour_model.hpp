#pragma once

#include <QAbstractListModel>
#include <QAbstractItemModel>
#include"booking.hpp"
class Hour_box : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString hour READ hour WRITE set_hour NOTIFY hour_changed)
    Q_PROPERTY(bool booked READ booked WRITE set_booked NOTIFY booked_changed)
    Q_PROPERTY(bool selected READ selected WRITE set_selected NOTIFY selected_changed)
public:
    Hour_box(QString hour_,bool booked_,bool selected_,QObject *parent):QObject(parent),hour_m(hour_),
        booked_m(booked_),selected_m(selected_){};
    QString hour() const{return hour_m;};
    bool booked() const{return booked_m;}
    bool selected() const{return selected_m;};

    void set_hour(QString hour_){if(hour_!=hour_m){hour_m=hour_;emit hour_changed(hour_m);}};
    void set_booked(bool booked_){if(booked_!=booked_m){booked_m=booked_;emit booked_changed(booked_m);}};
    void set_selected(bool selected_){if(selected_!=selected_m){selected_m=selected_;emit selected_changed(selected_m);}};
signals:
void hour_changed(QString hour);
void booked_changed(bool booked);
void selected_changed(bool selected);

private:
    QString hour_m;
    bool booked_m,selected_m;
};

class Hour_model : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int count READ count CONSTANT)


public:
    enum ModelRoles {
        hourRole = Qt::UserRole + 1,
        bookedRole,selectedRole
    };
    int count() const;
    explicit Hour_model(QObject *parent = nullptr);
    Q_INVOKABLE Hour_box* get(int i);
    Q_INVOKABLE bool setProperty(int i, QString role, const QVariant value);
    void add_booked_hours(const std::vector<int>& bhours);
    std::vector<Booking> get_bookings_from_selected(QDate day);


    int rowCount(const QModelIndex &p) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;
    QHash<int, QByteArray> roleNames() const;



private:
    int m_count;
    QList<Hour_box*> m_hours;

};

