// water.hh

#ifndef SENSING_H
#define SENSING_H

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
