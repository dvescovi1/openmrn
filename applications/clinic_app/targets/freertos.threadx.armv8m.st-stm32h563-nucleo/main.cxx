#include "openlcb/DefaultNode.hxx"
#include "utils/Blinker.hxx"
#include "hardware.hxx"

extern void hw_init();

int main()
{
    hw_init();
    Blinker blinker(LED_GREEN_Pin());
    while (1) {
        blinker.poll();
    }
    return 0;
}
