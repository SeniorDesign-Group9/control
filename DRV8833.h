

#ifndef DRV8833_h
#define DRV8833_h
class DRV8833 {
    public:
        DRV8833(
                int number_of_steps, // steps per rotation
                unsigned int motor_pin_a1,
                unsigned int motor_pin_a2,
                unsigned int motor_pin_b1,
                unsigned int motor_pin_b2
        );

        void set_speed(int rpm);

        void step(int steps);

    private:
        void step_motor(int this_step);

        unsigned int step_delay;
        int number_of_steps;

        unsigned int motor_pin_a1;
        unsigned int motor_pin_a2;
        unsigned int motor_pin_b1;
        unsigned int motor_pin_b2;
};
#endif
