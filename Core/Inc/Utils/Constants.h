// ADC
#define ADC1_CHANNEL_SIZE 7
#define ADC2_CHANNEL_SIZE 7
#define ADC3_CHANNEL_SIZE 8
#define ADC_CHANNELS ADC1_CHANNEL_SIZE+ADC2_CHANNEL_SIZE+ADC3_CHANNEL_SIZE  // Total number of ADC channels
#define ADC_VREF 5 // ADC reference voltage

// DAC
#define DAC1_BUFFER_SIZE 1
#define DAC2_BUFFER_SIZE 1

// Digital I/O
#define NUM_DIGITAL_OUTPUTS 8
#define NUM_DIGITAL_INPUTS 3

// RTD Constants
#define MAX_A_PEDAL_POSITION 0.01
#define MIN_BRAKE_PRESSURE 800

// Controller Constants
#define MAX_MONITORS 3

// Torque 
#define TORQUE_LOW_MARGIN 2.0