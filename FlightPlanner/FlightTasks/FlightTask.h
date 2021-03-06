#ifndef FLIGHTTASK_H
#define FLIGHTTASK_H

#include <QtGlobal>
#include <QObject>
#include <QPolygonF>
#include <QList>
#include <QString>
#include <QWeakPointer>

#include "Position.h"
#include "TimingConstraint.h"
#include "UAVParameters.h"
#include "Serializable.h"

class FlightTask : public QObject, public Serializable
{
    Q_OBJECT
public:
    FlightTask();
    virtual ~FlightTask();

    //for de-serializing
    FlightTask(QDataStream& stream);

    //pure-virtual from Serializable
    virtual void serialize(QDataStream& stream) const;

    virtual bool shortnessRewardApplies() const;

    virtual QString taskType() const=0;

    virtual qreal calculateFlightPerformance(const QList<Position>& positions,
                                             const QPolygonF& geoPoly,
                                             const UAVParameters& uavParams)=0;

    virtual qreal priority() const;

    virtual qreal maxTaskPerformance() const;

    const QList<TimingConstraint>& timingConstraints() const;
    void setTimingConstraints(const QList<TimingConstraint>& nConstraints);

    void addTimingConstraint(const TimingConstraint& nConstraint);
    void removeTimingConstraint(const TimingConstraint& nConstraint);
    void removeTimingConstraintAt(int index);

    const QString& taskName() const;
    void setTaskName(const QString& nName);

    const QList<QWeakPointer<FlightTask> >& dependencyConstraints() const;
    void setDependencyConstraints(const QList<QWeakPointer<FlightTask> >& nConstraints);
    void addDependencyContraint(const QSharedPointer<FlightTask>& other);

    quint64 uuid() const;
    void resolveDependencies();

    static QHash<quint64, QWeakPointer<FlightTask> > _uuidToWeakTask;

private slots:
    void handleDependencyDeleted();

signals:
    void flightTaskChanged();

protected:
    static qreal normal(qreal x, qreal stdDev, qreal scaleFactor=1000.0);

private:
    QList<TimingConstraint> _timingConstraints;
    QString _taskName;
    QList<QWeakPointer<FlightTask> > _dependencyConstraints;
    quint64 _uuid;

    QSet<quint64> _unresolvedDependencies;
};

#endif // FLIGHTTASK_H
