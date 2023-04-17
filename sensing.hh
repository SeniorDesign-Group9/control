// water.hh

#ifndef SENSING_H
#define SENSING_H

#include <array>
#include <ctime>
#include <queue>
#include <deque>
#include "DRV8833.hh"

#define POSITIONS       500

template <typename T, int MaxLen, typename Container=std::deque<T>>
class FixedQueue : public std::queue<T, Container> {
public:
    void push(const T& value) {
        if (this->size() == MaxLen) {
           this->c.pop_front();
        }
        std::queue<T, Container>::push(value);
    }
};

typedef struct result {
    std::time_t time;
    std::array<uint16_t, POSITIONS> vis_results;
    std::array<uint16_t, POSITIONS> nir_results;
} result_t;


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
        // Performs ADC conversion and pushes a result_t onto the FixedQueue results
        void getResult(DRV8833& motor);

        inline float resultRawToFloat(uint16_t raw_result);

        // Set lamp on or off
        void lampSet(bool on);

        // Toggle lamp
        void lampToggle(void);

        result_t queuePeek(void);

    private:
        // Singleton variables
        Sensing();
        ~Sensing();

        // Class variables
        const uint16_t ONE_VOLT = static_cast<uint16_t>(static_cast<float>(UINT16_MAX) / static_cast<float>(3.3));
        unsigned int lampGpio;
        bool lampOn;

        FixedQueue<result_t, 4> results;
};



#endif // SENSING_H
