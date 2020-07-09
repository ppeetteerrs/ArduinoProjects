#include <string.h>
#include "Arduino.h"

// Number of analog input channels
#define N_CHANNELS 5

// Number of short and long moving average counts
#define N_SHORT 2
#define N_LONG 16
#define PERIOD 10
#define THRESHOLD 70

uint8_t input_pins[8] = {A0, A1, A7, A3, A4, A5, A6, A2};
int short_readings[N_CHANNELS][N_SHORT];
int current_short = 0;
int long_readings[N_CHANNELS][N_LONG];
int current_long = 0;
double short_avg = 0;
double long_avg = 0;
bool hit[N_CHANNELS];
unsigned long last_run_time = 0;

void setup() {
    Serial.begin(115200);
    for (int i = 0; i < N_CHANNELS; i++) {
        // Setup input pin
        pinMode(input_pins[i], INPUT_PULLUP);

        // Initialize readings array
        for (int j = 0; j < N_SHORT; j++) {
            short_readings[i][j] = 0;
        }
        for (int j = 0; j < N_LONG; j++) {
            long_readings[i][j] = 0;
        }
    }
}

double arr_average(int* arr, int arr_size) {
    int sum = 0;
    for (int i = 0; i < arr_size; i++) {
        sum += arr[i];
    }
    return (double)sum / arr_size;
}

void loop() {
    // Loop over each channel
    if (millis() > last_run_time + PERIOD) {
        last_run_time += PERIOD;
        for (int channel = 0; channel < N_CHANNELS; channel++) {
            // Update reading arrays
            int value = analogRead(input_pins[channel]);
            short_readings[channel][current_short] = value;
            long_readings[channel][current_long] = value;
            current_short = (current_short + 1) % N_SHORT;
            current_long = (current_long + 1) % N_LONG;

            // Calculate averages
            double short_avg = arr_average(short_readings[channel], N_SHORT);
            double long_avg = arr_average(long_readings[channel], N_LONG);
            double diff = short_avg - long_avg;

            // Serial.print(short_avg);
            // Serial.print(" ");
            // Serial.println(long_avg);

            if (diff > THRESHOLD) {
                if (!hit[channel]) {
                    // Serial.print(channel);
                    // Serial.print(" ");
                    // Serial.println(diff);
                    // Serial.print(millis());
                    // Serial.print(": ");
                    // Serial.print(channel);
                    // Serial.print(" ");
                    Serial.print("Y");
                } else {
                    Serial.print("N");
                }
                hit[channel] = true;
            } else {
                if (!hit[channel]) {
                    Serial.print("N");
                } else {
                    // Serial.print(millis());
                    // Serial.print(": ");
                    // Serial.print(channel);
                    // Serial.print(" ");
                    Serial.print("N");
                }
                hit[channel] = false;
            }
        }
        Serial.println("");
    }
}
