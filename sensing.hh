// water.hh

#ifndef SENSING_H
#define SENSING_H

// DRV8833 class
class DRV8833 {
    public:
        // Singleton functions
        // Get DRV8833 instance
        static DRV8833& instance();

        // Disallow copying
        DRV8833& operator = (const DRV8833&) = delete;
        DRV8833(const DRV8833&) = delete;

        // Disallow moving
        DRV8833& operator = (DRV8833&&) = delete;
        DRV8833(DRV8833&&) = delete;

        // Class functions
        // Step motor steps number of steps (neg is backward)
        void stepSteps(int steps);

        // Driver function to step motor in certain way
        void stepMotor(int step);

        // Step motor forward 1 step
        void stepForwardOne(void);

        // Step motor backward 1 step
        void stepBackwardOne(void);

        // Step motor to position pos
        void stepPosition(unsigned int pos);

        // Step motor to position 0
        void stepZero(void);

        // Step motor to position max_steps
        void stepMax(void);

    private:
        // Singleton variables
        DRV8833();
        ~DRV8833();

        // Class variables
        const uint32_t MAX_STEP = UINT32_MAX / 2;   // FIXME: Calc actual max step
        uint32_t current_pos;                       // Mutex?
        uint32_t step_delay;
        // FIXME: RPM?
};

class Sensing {
    public:
        // Singleton functions
        // Get Sensing instance
        static Sensing& instance();

        // Disallow copying
        Sensing& operator = (const Sensing&) = delete;
        Sensing(const Sensing&) = delete;

        // Disallow moving
        Sensing& operator = (Sensing&&) = delete;
        Sensing(Sensing&&) = delete;

        // Class functions

    private:
        // Singleton variables
        Sensing();
        ~Sensing();

        // Class variables
};

#endif // SENSING_H
