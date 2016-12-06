#define F_CPU 16000000UL         //Says to the compiler which is our clock frequency, permits the delay functions to be very accurate
#include <avr/io.h>              //General definitions of the registers values
#include <util/delay.h>          //This is where the delay functions are located
#include <avr/interrupt.h>



#define FOSC 16000000            // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD -1

#define OFF 0;


char ReceivedChar;
int isReady = 0;
int states = 7;

int main(void)
{

	volatile int Speed = 150;

	//SET UP FOR TIMER2 PWM PINS
	OCR2A = 128;								// sets PWM for 50% duty cycle
	TCCR2A |= (1 << COM2A1) | (1 << COM2B1);	// set non - inverting mode for both lines
	TCCR2A |= (1 << WGM21) | (1 << WGM20);		// set fast PWM Mode
	TCCR2B |= (1 << CS21);						// sets pre-scaler to 8 and starts PWM
	

	//SET UP FOR HALL EFFECT PINS
	//sets PC2 & PC3 & PC0 as inputs for HALL_A, HALL_B, HALL_C
	DDRC &= ~(1 << DDC2) | ~(1 << DDC3) | ~(1 << DDC0);
	//enable pull up resistor on PC2 & PC3 & PC0 for HALL_A, HALL_B, HALL_C
	PORTC |= (1 << PORTC2) | (1 << PORTC3) | (1 << PORTC0);
	
	
	//SET UP FOR ANALOG ADC6 PIN READ IN
	ADMUX = 6;				// use ADC6
	ADMUX |= (1 << REFS0);	// use AVcc as reference voltage
	ADMUX |= (1 << ADLAR);	// 8 bit resolution
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);	 // 128 prescale for 16Mhz
	ADCSRA |= (1 << ADEN);									 // Enable the ADC
	ADCSRA |= (1 << ADIE);									 // Enable Interrupts
	ADCSRA |= (1 << ADSC);									 // Start the ADC conversion
	
	
	//Speed = ADCH;			// read analog pin value


	//SET UP FOR USART SERIAL CONNECTION
	//CODE WILL ECHO SERIAL TO CONSOLE
	
	/*Set baud rate */
	UBRR0H = (MYUBRR >> 8);
	UBRR0L = MYUBRR;
	
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);      // Enable receiver and transmitter
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);    // Set frame: 8data, 1 stp
	
	

	while (1);
	{
		
		while ( !(UCSR0A & (1 << RXC0)) )  // Wait until data is received
		ReceivedChar = UDR0;               // Read the data from the RX buffer
		
		while ( !(UCSR0A & (1 << UDRE0)) ) // Wait until buffer is empty
		UDR0 = ReceivedChar;			   // Send the data to the TX buffer

		if(isReady > 0) {
			
						
			switch (states) {
				
				case 1:
				OCR0B = OFF; //PD5, pin 7
				OCR1A = OFF; //PB1, pin 5
				OCR2A = OFF; //PB3, pin 3
				OCR2B = OFF; //PD3, pin 2
				OCR2A = Speed; //PB2, pin 4
				OCR0A = Speed; //PD6, pin 6 
				break;
	
				case 2:
				OCR0A = OFF; //PD6, pin 6
				OCR2A = OFF; //PB2, pin 4
				OCR2A = OFF; //PB3, pin 3
				OCR2B = OFF; //PD3, pin 2
				OCR1A = Speed; //PB1, pin 5
				OCR0B = Speed; //PD5, pin 7
				break;
				
				case 3:
				OCR0B = OFF; //PD5, pin 7
				OCR2A = OFF; //PB2, pin 4
				OCR2A = OFF; //PB3, pin 3
				OCR2B = OFF; //PD3, pin 2
				OCR1A = Speed; //PB1, pin 5
				OCR0A = Speed; //PD6, pin 6
				break;
				
				case 4:
				OCR0B = OFF; //PD5, pin 7
				OCR0A = OFF; //PD6, pin 6
				OCR1A = OFF; //PB1, pin 5
				OCR2A = OFF; //PB2, pin 4
				OCR2A = Speed; //PB3, pin 3
				OCR2B = Speed; //PD3, pin 2
				break;
				
				case 5:
				OCR0B = OFF; //PD5, pin 7
				OCR0A = OFF; //PD6, pin 6
				OCR1A = OFF; //PB1, pin 5
				OCR2A = OFF; //PB3, pin 3
				OCR2A = Speed; //PB2, pin 4
				OCR2B = Speed; //PD3, pin 2
				break;
				
				case 6:
				OCR0A = OFF; //PD6, pin 6
				OCR1A = OFF; //PB1, pin 5
				OCR2A = OFF; //PB2, pin 4
				OCR2B = OFF; //PD3, pin 2
				OCR2A = Speed; //PB3, pin 3
				OCR0B = Speed; //PD5, pin 7
				break;
				
				case 7:
				turnOff();
				break;
			}
		}
		else { 
			turnOff();
		}
		
	
		//body of while loop
		
	}
}

void turnOff() {

	OCR0B = OFF; //PD5, pin 7
	OCR0A = OFF; //PD6, pin 6
	OCR1A = OFF; //PB1, pin 5
	OCR2A = OFF; //PB2, pin 4
	OCR2A = OFF; //PB3, pin 3
	OCR2B = OFF; //PD3, pin 2
}
