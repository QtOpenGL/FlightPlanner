#include "PlanningControlWidget.h"
#include "ui_PlanningControlWidget.h"

#include <QtDebug>

PlanningControlWidget::PlanningControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlanningControlWidget)
{
    ui->setupUi(this);
    this->setPlanningState(FlightPlanner::Stopped);
}

PlanningControlWidget::~PlanningControlWidget()
{
    delete ui;
}

//public slot
void PlanningControlWidget::setPlanningState(FlightPlanner::PlanningStatus status)
{
    this->ui->planButton->setEnabled((status == FlightPlanner::Stopped || status == FlightPlanner::Paused));
    this->ui->pauseButton->setEnabled((status == FlightPlanner::Running));
    this->ui->resetButton->setEnabled((status == FlightPlanner::Paused || status == FlightPlanner::Running));

    if (status == FlightPlanner::Stopped)
    {
        this->ui->currentFitnessDisplay->setValue(0.0);
        this->ui->currentIterationSpinbox->setValue(0);
        this->ui->planProgressBar->setValue(0);
    }
}

void PlanningControlWidget::setPlanningProgress(int iteration, qreal fitness)
{
    this->ui->currentIterationSpinbox->setValue(iteration);
    this->ui->currentFitnessDisplay->setValue(fitness);

    qreal fitnessProgress = 0.0;
    qreal iterationProgress = 0.0;
    if (this->ui->stopAtFitnessCheckBox->isChecked())
    {
        qreal stopAtFitness = this->ui->stopAtFitnessSpinBox->value();
        if (fitness >= stopAtFitness)
            this->planningPauseRequested();
        fitnessProgress = fitness / this->ui->stopAtFitnessSpinBox->value();
    }
    if (this->ui->stopAtIterationCheckBox->isChecked())
    {
        int stopAtIteration = this->ui->stopAtIterationSpinBox->value();
        if (iteration >= stopAtIteration)
            this->planningPauseRequested();
        iterationProgress = (qreal)iteration / (qreal)this->ui->stopAtIterationSpinBox->value();
    }

    qreal progress = qMax<qreal>(fitnessProgress,iterationProgress);
    this->ui->planProgressBar->setValue(progress*100);
}

//public slot
void PlanningControlWidget::setIsPaused()
{
    this->setPlanningState(FlightPlanner::Paused);
}

//public slot
void PlanningControlWidget::setIsStopped()
{
    this->setPlanningState(FlightPlanner::Stopped);
}

//public slot
void PlanningControlWidget::setIsRunning()
{
    this->setPlanningState(FlightPlanner::Running);
}

//private slot
void PlanningControlWidget::on_resetButton_clicked()
{
    this->planningClearRequested();
}

//private slot
void PlanningControlWidget::on_planButton_clicked()
{
    this->planningStartRequested();
}

//private slot
void PlanningControlWidget::on_pauseButton_clicked()
{
    this->planningPauseRequested();
}
