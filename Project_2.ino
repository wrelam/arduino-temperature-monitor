/***************************************************************************//**
    @file   Project_2.ino

    @brief  Sense temperature changes and record them
    @author Walt Elam

    Requires:
        - Adafruit DHT Sensor Library
        - Adafruit Unified Sensor Library

    DHT code based on Adafruit DHT_Unified_Sensor example sketch.
*******************************************************************************/
#include "Debug.h"
#include "Storage.h"
#include "TempMonitor.h"

#define BUTTON_PIN  (2)
#define LED_PIN     (LED_BUILTIN)
#define DHT_PIN     (12)
#define DHT_TYPE    DHT11

/** Seconds between each temperature sample */
#define SAMPLE_TIMEOUT  (10)

TempMonitor tm(DHT_PIN, DHT_TYPE);
// 10 minutes of data collected overall
Storage<float> store(10 * (60 / SAMPLE_TIMEOUT), 0.0F);
bool gReadTemp = false;

/*******************************************************************************
    setupTimer
*//**
    @brief  Setup timer for use as a constant interrupt

    Initializes Timer1 Channel A to interrupt every second.
*******************************************************************************/
void
setupTimer(void)
{
    // Disable interrupts while setting up timer
    noInterrupts();

    // Configure Timer1
    TCCR1A = 0x00;  // 00000000 =   0x00
                    // ||||||| `>   Wave Generation Mode 4 (Bit 0)
                    // |||||| `->   Wave Generation Mode 4 (Bit 1)
                    // ||||| `-->   Port A functionality (unused)
                    // |||| `--->   Port A functionality (unused)
                    // ||| `---->   Port B functionality (unused)
                    // || `----->   Port B functionality (unused)
                    // | `------>   Maintain normal port A functionality
                    //  `------->   Maintain normal port A functionality
    TCCR1B = 0x8C;  // 10001100 =   0x8C
                    // ||||||| `>   Prescaler = 256
                    // |||||| `->   Prescaler = 256
                    // ||||| `-->   Prescaler = 256
                    // |||| `--->   Wave Generation Mode 4 (Bit 2)
                    // ||| `---->   Wave Generation Mode 4 (Bit 3)
                    // || `----->   Reserved
                    // | `------>   Capture edge select on falling edge (unused)
                    //  `------->   Enable capture noise canceler
    OCR1A = 62499;  // Fa = Fclk / 2 * N * (1 + OCR1A)
                    // 1Hz= 16 000 000 / (2 * 256 * (1 + OCR1A))
                    // 1 + OCR1A = 16 000 000 / (2 * 256)
                    // 1 + OCR1A = 16 000 000 / 512
                    // OCR1A = 16 000 000 / 512 - 1
                    // OCR1A = 31250 - 1
                    // OCR1A = 31249 for 1 Hz interrupt
                    // Somehow this is twice as fast, so double the 31250 then
                    // subtract 1 to get 62499... I dunno why it's wrong
    TIMSK1 = 0x02;  // 00000010 = 0x02
                    // ||||||| `>   Overflow Interrupt (disabled)
                    // |||||| `->   Output Compare A Match Interrupt (enabled)
                    // ||||| `-->   Output Compare B Match Interrupt (disabled))
                    // |||| `--->   Output Compare C Match Interrupt (disabled))
                    // ||| `---->   Reserved
                    // || `----->   Input Capture Interrupt (disabled)
                    // | `------>   Reserved
                    //  `------->   Reserved

    // Enable interrupts since all setup has been performed
    interrupts();
}


/*******************************************************************************
    setup
*//**
    @brief  Initializes the serial input interface
*******************************************************************************/
void
setup(void)
{
    pinMode(BUTTON_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    if (!store.init())
    {
        DBG_PRINT("Failed to initialize store\n");
        return;
    }
    setupTimer();
}


/*******************************************************************************
    TIMER1_COMPA_vect
*//**
    @brief  ISR for the Compare A Match Interrupt of Timer1
*******************************************************************************/
ISR(TIMER1_COMPA_vect)
{
    static int counter = 0;

    if (SAMPLE_TIMEOUT == ++counter)
    {
        counter = 0;
        gReadTemp = true;
    }
}


/*******************************************************************************
    loop
*//**
    @brief  Records temperature reading periodically
*******************************************************************************/
void
loop(void)
{
    bool shouldRead = false;
    float tempF = 0.0F;
    static float lastTemp = 0.0F;
    static bool record = false;
    static bool pause = false;
    static size_t recordings = 0;

    // Stop collecting when 10 minutes of data has been collected
    if (recordings == (10 * (60 / SAMPLE_TIMEOUT)))
    {
        return;
    }
    // If we've recorded 5 minutes worth of data
    else if (recordings == (5 * (60 / SAMPLE_TIMEOUT)))
    {
        DBG_PRINT("PLEASE PRESS THE BUTTON\n");
        digitalWrite(LED_PIN, HIGH);

        // Loop until the button is pressed
        while (LOW == digitalRead(BUTTON_PIN))
        {
            // Wait for the button to be pressed
        }
        DBG_PRINT("BUTTON PRESSED, proceeding with recording data\n");
        digitalWrite(LED_PIN, LOW);
    }

    // Read flag, disabling interrupts to avoid shared-data problems
    noInterrupts();
    shouldRead = gReadTemp;
    interrupts();

    if (shouldRead)
    {
        // Reset flag, disabling interrupts to avoid shared-data problems
        noInterrupts();
        gReadTemp = false;
        interrupts();

        tm.update();
        tempF = tm.getTempF();

        DBG_PRINT("Temperature: ");
        DBG_PRINT(tm.getTempC());
        DBG_PRINT(" *C, ");
        DBG_PRINT(tempF);
        DBG_PRINT(" *F\n");

        if (record)
        {
            store.write(tempF);
            recordings++;
        }
        else
        {
            if (lastTemp == 0.0F)
            {
                DBG_PRINT("Storing first temperature\n");
                lastTemp = tempF;
            }
            else
            {
                DBG_PRINT("Checking previous temperature\n");

                /* Wait until temperature stabilizes before recording it */
                if (lastTemp == tempF)
                {
                    DBG_PRINT("Temperature has stabilized!\n");
                    record = true;
                }
                else
                {
                    lastTemp = tempF;
                }
            }
        }
    }
}

