#include <iostream>

#include "SimulationMonitor.h"
#include "SimulationChangerImpl.h"

namespace
{
    auto const TimestepsForMonitor = 1000;

    auto const InitDuration = 400;
    auto const StepDuration = 100;
    auto const RetreatDuration = 400;

    auto const RetreatStartFactor = 0.35;
    auto const RetreatEndFactor = 0.6;
    auto const EmergencyRetreatStartFactor = 0.25;
    auto const MaxRetreats = 5;
}

SimulationChangerImpl::~SimulationChangerImpl()
{
    deactivate();
}

void SimulationChangerImpl::init(SimulationMonitor * monitor, NumberGenerator* numberGenerator)
{
    if (!_monitorConnections.empty()) {
        deactivate();
    }

    _numberGenerator = numberGenerator;
    _monitor = monitor;
    for (auto const& connection : _monitorConnections) {
        disconnect(connection);
    }
    _monitorConnections.emplace_back(
        connect(_monitor, &SimulationMonitor::dataReadyToRetrieve, this, &SimulationChangerImpl::monitorDataAvailable));
}

void SimulationChangerImpl::notifyNextTimestep()
{
    ++_timestepsSinceBeginning;

    if (0 == (_timestepsSinceBeginning % TimestepsForMonitor)) {
        _monitorDataRequired = true;
        _monitor->requireData();
    }

}

void SimulationChangerImpl::activate(SimulationParameters const & currentParameters)
{
    _state = State::Init;
    _parameters = currentParameters;
    _initialParameters = currentParameters;

    _numRetreats = 0;

    _timestepsSinceBeginning = 0;
    _measurementsSinceBeginning = 0;
    _measurementsOfCurrentEpoch = 0;
    _measurementsOfCurrentRetreat = 0;

    std::cerr << "[parameter changer] activated" << std::endl;
}

void SimulationChangerImpl::deactivate()
{
    if (State::Deactivated == _state) {
        return;
    }

    _state = State::Deactivated;
    std::cerr << "[parameter changer] deactivated" << std::endl;
}

SimulationParameters const & SimulationChangerImpl::retrieveSimulationParameters()
{
    return _parameters;
}

void SimulationChangerImpl::monitorDataAvailable()
{
    if (!_monitorDataRequired) {
        return;
    }
    _monitorDataRequired = false;

    ++_measurementsSinceBeginning;

    auto data = _monitor->retrieveData();
    auto activeClusters = data.numClustersWithTokens;

    if (State::Init == _state) {
        if (InitDuration == _measurementsSinceBeginning) {
            _activeClustersReference = activeClusters;
            std::cerr << "[parameter changer] measurement finished: " << *_activeClustersReference << " active clusters"
                << std::endl;

            _state = State::FindEpochTarget;
            std::cerr << "[parameter changer] find epoch target" << std::endl;
        }
    }
    else if (State::FindEpochTarget == _state) {
        _calculator = SimulationParametersCalculator::createWithRandomTarget(_parameters, _numberGenerator);
        _measurementsOfCurrentEpoch = _measurementsSinceBeginning;
        _numRetreats = 0;

        _state = State::Epoch;
        std::cerr << "[parameter changer] start epoch" << std::endl;
    }
    else if (State::Epoch == _state) {
        if (activeClusters < *_activeClustersReference * RetreatStartFactor) {
            std::cerr << "[parameter changer] critical number of " << activeClusters << " active clusters reached" << std::endl;

            ++_numRetreats;
            _state = State::Retreat;
            std::cerr << "[parameter changer] start retreat" << std::endl;

            while (!_calculator->isSourceReached()) {
                _calculator->getPrevious();
            }
            _parameters = _calculator->getSource();
            Q_EMIT simulationParametersChanged();

            _measurementsOfCurrentRetreat = _measurementsSinceBeginning;
        }
        else if (0 == ((_measurementsSinceBeginning - _measurementsOfCurrentEpoch) % StepDuration)) {
            _parameters = _calculator->getNext();
            Q_EMIT simulationParametersChanged();
            std::cerr << "[parameter changer] epoch step" << std::endl;

            if (_calculator->isTargetReached()) {
                _state = State::FindEpochTarget;
                std::cerr << "[parameter changer] end epoch" << std::endl;
            }
        }
    }
    else if (State::Retreat == _state) {
        if (activeClusters < *_activeClustersReference * EmergencyRetreatStartFactor) {
            std::cerr << "[parameter changer] very critical number of " << activeClusters << " active clusters reached" << std::endl;

            _state = State::EmergencyRetreat;
            std::cerr << "[parameter changer] start emergency retreat" << std::endl;

            _parameters = _initialParameters;
            Q_EMIT simulationParametersChanged();
        }
        if (activeClusters > *_activeClustersReference * RetreatEndFactor
            || _measurementsOfCurrentRetreat + RetreatDuration < _measurementsSinceBeginning) {
            std::cerr << "[parameter changer] end retreat" << std::endl;

            if (_numRetreats == MaxRetreats) {
                _state = State::FindEpochTarget;
                std::cerr << "[parameter changer] find epoch target" << std::endl;
            }
            else {
                std::cerr << "[parameter changer] restart epoch" << std::endl;
                _state = State::Epoch;
            }
        }
    }
    else if (State::EmergencyRetreat == _state) {
        if (activeClusters > *_activeClustersReference * RetreatEndFactor) {
            std::cerr << "[parameter changer] end emergency retreat" << std::endl;

            _state = State::FindEpochTarget;
            std::cerr << "[parameter changer] find epoch target" << std::endl;
        }
    }
}
