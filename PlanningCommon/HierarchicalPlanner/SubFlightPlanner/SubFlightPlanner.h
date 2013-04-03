#ifndef SUBFLIGHTPLANNER_H
#define SUBFLIGHTPLANNER_H

#include <QSharedPointer>
#include <QList>

#include "FlightTaskArea.h"
#include "FlightTasks/FlightTask.h"
#include "Position.h"
#include "UAVOrientation.h"
#include "UAVParameters.h"

#include "PlanningCommon_global.h"

class PLANNINGCOMMONSHARED_EXPORT SubFlightPlanner
{
public:
    SubFlightPlanner(const UAVParameters& uavParams,
                     const QSharedPointer<FlightTask>& task,
                     const QSharedPointer<FlightTaskArea>& area,
                     const Position& startPos,
                     const UAVOrientation& startPose);

    void plan();
    const Wayset &results() const;

private:
    void _greedyPlan();
    const UAVParameters& _uavParams;
    const QSharedPointer<FlightTask>& _task;
    const QSharedPointer<FlightTaskArea>& _area;
    const Position& _startPos;
    const UAVOrientation& _startPose;

    Wayset _results;
};

#endif // SUBFLIGHTPLANNER_H
