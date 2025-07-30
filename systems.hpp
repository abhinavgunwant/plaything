#ifndef PLAYTHING_SYSTEMS
#define PLAYTHING_SYSTEMS

#include <vector>

#define SULFUR_TIME 1
#define WOOD_TIME 1.25
#define METAL_TIME 3
#define HQM_TIME 5

using namespace std;

class FurnaceSystem {
    private:
        vector<uint32_t> ids;

    public:
        /**
         * The furnace burns:
         * - Sulfur in 1s
         * - Wood in 1.25s
         * - Metal in 3s
         * - HQM in 5s
         */
        void run(float frameTime);
        void addFurnaceId(uint32_t id);
        void removeFurnaceId(uint32_t id);
};

#endif

