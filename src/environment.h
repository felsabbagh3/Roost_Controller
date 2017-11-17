#pragma once

#include <random>
#include <string>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>

/// \brief Represents an air conditioner or heater in the environment
class unit_t
{
public:
        /// \brief A config of the unit
        struct config_t
        {
                double dutyCycle;   ///< Duty cycle
                double efficiency;  ///< Efficiency
                double equilibrium; ///< Equilibrium point
        };

        /// \brief The state of the unit
        struct state_t
        {
                bool on;           ///< Whether or not the unit is on
                int timeOn;        ///< How long the unit has been on  (-1 if off)
                int timeOff;       ///< How long the unit has been off (-1 if on)
                double energyUsed; ///< Amount of energy used so far
        };

        ~unit_t ();
        /// \param config_ Description of the unit
        explicit unit_t (std::string name_, config_t config_);

        /// \brief Update the unit
        /// \param timestep_ Timestep
        /// \return Updated state
        state_t update (int timestep_);
        /// \brief Turn the unit on or off
        /// \param on_ Whether or not the unit should be on
        void setOn (bool on_);
        /// \brief Compute the temperature delta
        /// \param temperature_ Current temperature
        /// \param area_ Area being affected
        /// \param timestep_
        double computeTemperature (double temperature_, double area_, int timestep_) const;

private:
        void error (std::string const &message_);
        void status (std::string const &message_);

        config_t const m_config; ///< Description of the unit
        std::string const m_name;          ///< Name of the unit

        state_t m_state; ///< Current state of the unit
};

/// \brief The environment class is responsible for simulating the "environment". At a minimum,
/// the "environment" includes the air conditioning unit, the heater unit, the current time,
/// and the internal temperature. You're free to extend it to include *more* than this, if it
/// allows you to implement more advanced or interesting features.
///
/// Keep in mind that once Roost is an actual product, values like temperature will be coming
/// from sensors, and therefore the reported values might be imperfect. It may be a good idea
/// to simulate that.
class environment_t
{
public:
        /// \brief A config of the environment
        struct config_t
        {
                unit_t::config_t ac;     ///< AC config
                unit_t::config_t heater; ///< Heater config
                double area;             ///< Area of building
                double initTemp;         ///< Initial temperature of environment
        };

        /// \brief Represents the state of the environment
        struct state_t
        {
                unit_t::state_t ac;     ///< AC state
                unit_t::state_t heater; ///< Heater state
                int time;               ///< Current time in seconds
                double temperature;     ///< Current temperature
        };

        ~environment_t ();
        /// \param config_ Environment config
        explicit environment_t (config_t const &config_);

        /// \brief Update the environment by simulating forward for timestep_ seconds
        /// \param timestep_ Number of seconds to simulate forward in time
        /// \return State of the environment after the time has elapsed
        state_t update (int timestep_);

        /// \brief Turn on/off the air conditioner
        /// \param on_ Whether or not to turn on the AC
        void enableAC (bool on_);
        /// \brief Turn on/off the heater
        /// \param off_ Whether or not to turn on the heater
        void enableHeater (bool on_);

private:
        config_t const m_config; ///< Description of environment

        unit_t m_ac;                                ///< Air conditioner
        unit_t m_heater;                            ///< Heater
        state_t m_state;                            ///< Current state
        std::default_random_engine m_rng;           ///< Random number generator
        std::normal_distribution<> m_tempNoiseDist; ///< Noise distribution for temperature
};
