#pragma once

#include <QAbstractListModel>
#include <QAbstractItemModel>
#include <QTimer>
#include"Hour_model.hpp"
class Day_box : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QDate day READ day WRITE set_day NOTIFY day_changed)
    Q_PROPERTY(Hour_model* hour_model  READ hour_model WRITE set_hour_model NOTIFY hour_model_changed)

public:
    QDate day(void){return day_m;}
    void set_day(QDate day_){day_m=day_;emit day_changed();}

    Hour_model* hour_model(void)const{return hour_model_m;}
    void set_hour_model(Hour_model* ptr){hour_model_m=ptr;}

    Day_box(QDate day_,Hour_model * hour_model_,QObject *parent):QObject(parent),day_m(day_),hour_model_m(hour_model_){
        hour_model_->setParent(this);

    };

signals:
    void day_changed(void);
    void hour_model_changed(void);

private:
    Hour_model * hour_model_m;
    QDate day_m;
};

class Day_model : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int count READ count WRITE setCount NOTIFY countChanged)

public:
    enum ModelRoles {
        dayRole = Qt::UserRole + 1,
        hour_modelRole,selectedRole
    };
    int count() const;
    explicit Day_model(QObject *parent = nullptr);
    Q_INVOKABLE Day_box* get(int i);
    Q_INVOKABLE void get_new_bookings(void)const;

    void append(Day_box* o);
    void pop_front(void);
    void update_list(void);

    int rowCount(const QModelIndex &p) const;
    QVariant data(const QModelIndex &index, int role) const;

    QHash<int, QByteArray> roleNames() const;



signals:
    void countChanged(int count);
    void new_bookings(std::vector<Booking> nbooks )const;

public slots:
    void setCount(int count);
    void add_booking(const Booking& nbook);


private:
    int m_count;
    QList<Day_box*> m_days;
    QTimer *timer_m;
};

