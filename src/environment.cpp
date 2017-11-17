#include "environment.h"

using namespace std;

namespace
{
unit_t::state_t const DEFAULT_UNIT_STATE = { false, -1, 60, 0 };
environment_t::state_t const DEFAULT_ENV_STATE = { DEFAULT_UNIT_STATE, DEFAULT_UNIT_STATE, 0,
                                                   -1 };
}

unit_t::~unit_t ()
{
}

unit_t::unit_t (string name_, config_t config_)
        : m_config (config_)
        , m_name (name_)
        , m_state (DEFAULT_UNIT_STATE)
{
}

unit_t::state_t unit_t::update (int timestep_)
{
        if (m_state.on)
        {
                m_state.timeOn += timestep_;
                m_state.energyUsed += timestep_ / m_config.efficiency;
        }
        else
        {
                m_state.timeOff += timestep_;
        }

        if (m_state.timeOn > 60 * m_config.dutyCycle)
                error ("Unit overheated!");

        return m_state;
}

void unit_t::setOn (bool on_)
{
        if (on_ == m_state.on)
                return;

        if (on_)
        {
                if (m_state.timeOff < (1 - m_config.dutyCycle) * 60)
                        error ("Unit failure! Not allowed enough time to rest");

                m_state.timeOff = -1;
                m_state.timeOn = 0;
                m_state.energyUsed += 100 / m_config.efficiency;
                // status ("Turning on");
        }
        else
        {
                m_state.timeOff = 0;
                m_state.timeOn = -1;
                // status ("Turning off");
        }

        m_state.on = on_;
}

void unit_t::error (string const &message_)
{
        ostringstream ss;
        ss << m_name << ": " << message_ << endl;

        throw runtime_error (ss.str ());
}

void unit_t::status (string const &message_)
{
        cout << m_name << ": " << message_ << endl;
}

double unit_t::computeTemperature (double temperature_, double area_, int timestep_) const
{
        double const tempGrad = (m_config.equilibrium - temperature_);
        double const tempDelta = m_config.efficiency * timestep_ * tempGrad / area_;

        return temperature_ + tempDelta;
}


//////////////////////////////////////////////////////////////////////////
environment_t::~environment_t ()
{
}

environment_t::environment_t (config_t const &config_)
        : m_config (config_)
        , m_ac ("AC", config_.ac)
        , m_heater ("HEATER", config_.heater)
        , m_state (DEFAULT_ENV_STATE)
        , m_rng (random_device () ())
        , m_tempNoiseDist (0, 0.5)
{
        m_state.temperature = m_config.initTemp;
}

environment_t::state_t environment_t::update (int timestep_)
{
        m_state.ac = m_ac.update (timestep_);
        m_state.heater = m_heater.update (timestep_);

        // We should never have both units on at the same time
        if (m_state.ac.on && m_state.heater.on)
                throw runtime_error ("You tripped the breaker! Be sure the AC and heater are "
                                     "never on simultaneously.");

        m_state.time += timestep_;

        if (m_state.ac.on)
        {
                m_state.temperature = m_ac.computeTemperature (
                    m_state.temperature, m_config.area, timestep_);
        }
        else if (m_state.heater.on)
        {
                m_state.temperature = m_heater.computeTemperature (
                    m_state.temperature, m_config.area, timestep_);
        }

        environment_t::state_t noisyState = m_state;
        noisyState.temperature += m_tempNoiseDist (m_rng);

        return noisyState;
}

void environment_t::enableAC (bool on_)
{
        m_ac.setOn (on_);
}

void environment_t::enableHeater (bool on_)
{
        m_heater.setOn (on_);
}
