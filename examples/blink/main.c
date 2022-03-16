#include "unolib.h"
#include "pinMode.h"
#include "digitalWrite.h"
#include "delay.h"
 
int main(void)
{
    /* set LED to output*/
    pinMode(LED_BUILTIN, OUTPUT);

    while(1) {
        /* toggle led on and off */
        digitalWrite(LED_BUILTIN, HIGH);
        delay(2000);
        digitalWrite(LED_BUILTIN, LOW);
        delay(2000);
    }
    return(0); 
}
