#define F_CPU 16000000UL         //Says to the compiler which is our clock frequency, permits the delay functions to be very accurate
#include <avr/io.h>              //General definitions of the registers values
#include <util/delay.h>          //This is where the delay functions are located
#include <avr/interrupt.h>

#define FOSC 16000000            // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD -1
#define OFF 255;

volatile int counts = 0;
volatile int flag = 1;

volatile uint8_t ADCvalue;
volatile long Speed = 255;
uint8_t  states = 7;




///////////////////////////////////////////////////////////////////////////////////////////////

void turnOff() {
	OCR0B = OFF; //PD5, pin 7
	OCR0A = OFF; //PD6, pin 6
	OCR1A = OFF; //PB1, pin 5
	OCR1B = OFF; //PB2, pin 4
	OCR2A = OFF; //PB3, pin 3
	OCR2B = OFF; //PD3, pin 2
}



//ISR(TIMER1_OVF_vect)
//{
//counts++;
//if ( counts > 100 ) {
//
//if(flag == 1) {
//PORTC |= (1 << DDC2);
//flag = 0;
//}
//else if (flag == 0) {
//PORTC &= (0 << DDC2);
//flag = 1;
//}
//counts = 0;
//}
//}


long mapp(uint8_t x, long in_min, long in_max, long out_min, long out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


int main(void)
{
	
	int isReady = 1;

	//SET pin 2-7 as OUTPUT
	DDRB |= (1 << DDB1) | (1 << DDB2) | (1 << DDB3);	// sets port 3,4,5 as
	DDRD |= (1 << DDD3) | (1 << DDD5) | (1 << DDD6);    // pins 2,6,7 as output
	
	//TIMER0
	OCR0A = Speed;
	OCR0B = Speed;
	TCCR0A |= (1 << COM0A1) | (1 << COM0A0)| (1 << COM0B1) | (1 << COM0B0);	// set inverting mode
	TCCR0A |= (1 << WGM01) | (1 << WGM00);		// set fast PWM Mode
	TCCR0B |= (1 << CS01);						// set pre-scaler to 8 and starts PWM
	
	//TIMER1
	OCR1A = Speed;
	OCR1B = Speed;
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << COM1A0) | (1 << COM1B0);	// set non-inverting mode
	TCCR1A |= (1 << WGM10);						// set fast PWM mode 8 bit
	TCCR1B |= (1 << WGM12);						// set fast PWM mode 8 bit
	TCCR1B |= (1 << CS11);						// START the timer with no pre-scaler
	
	//SET UP FOR TIMER2 PWM PINS
	OCR2B = Speed;
	OCR2A = Speed;								// sets PWM for 50% duty cycle
	TCCR2A |= (1 << COM2A1) | (1 << COM2B1) | (1 << COM2A0) | (1 << COM2B0) ;	// set non - inverting mode for both lines
	TCCR2A |= (1 << WGM21) | (1 << WGM20);		// set fast PWM Mode
	TCCR2B |= (1 << CS21);						// sets pre-scaler to 8 and starts PWM
	
	//SET UP FOR HALL EFFECT PINS
	DDRC &= ~(1 << DDC2) | ~(1 << DDC3) | ~(1 << DDC0); //sets PC2 & PC3 & PC0 as inputs for HALL_A, HALL_B, HALL_C
	PORTC |= (1 << PORTC2) | (1 << PORTC3) | (1 << PORTC0); //enable pull up resistor on PC2 & PC3 & PC0 for HALL_A, HALL_B, HALL_C
	
	ADMUX = 6;                // use ADC6
	ADMUX |= (1 << REFS0);    // use AVcc as the reference
	ADMUX |= (1 << ADLAR);    // Right adjust for 8 bit resolution
	ADCSRA |=  (1 << ADPS1) | (1 << ADPS0); // 128 prescale for 16Mhz
	ADCSRA |= (1 << ADATE);   // Set ADC Auto Trigger Enable
	ADCSRB = 0;               // 0 for free running mode
	ADCSRA |= (1 << ADEN);    // Enable the ADC
	ADCSRA |= (1 << ADSC);    // Start the ADC conversion

	turnOff();
	
	//==================================//
	//TEST AREA//
	//
	//DDRC |= (1 << DDC2);
	//TIMSK1 = (1 << TOIE1);
	//sei();
	
	

	while (1) {
		
		//gets state of hall sensors
		states = (PINC & (1 << PC2)) + (((PINC & (1 << PC3))) >> 2 ) + (PINC & (1 << PC0));
		
		//map value for throttle
		ADCvalue = ADCH;
		Speed = mapp(ADCvalue, 43, 215, 255, 0);

		if (Speed > 255)
			Speed = 255;
		else if (Speed < 0)
			Speed = 0;
			
			

		
		
		//if (isReady > 0) {
			
			switch (states) {
				
				case 1 :
				OCR0B = OFF; //PD5, pin 7
				OCR1A = OFF; //PB1, pin 5
				OCR2A = OFF; //PB3, pin 3
				OCR2B = OFF; //PD3, pin 2
				OCR1B = Speed; //PB2, pin 4
				OCR0A = Speed; //PD6, pin 6
				break;
				
				case 2 :
				OCR0A = OFF; //PD6, pin 6
				OCR1B = OFF; //PB2, pin 4
				OCR2A = OFF; //PB3, pin 3
				OCR2B = OFF; //PD3, pin 2
				OCR1A = Speed; //PB1, pin 5
				OCR0B = Speed; //PD5, pin 7
				break;
				
				case 3 :
				OCR0B = OFF; //PD5, pin 7
				OCR1B = OFF; //PB2, pin 4
				OCR2A = OFF; //PB3, pin 3
				OCR2B = OFF; //PD3, pin 2
				OCR1A = Speed; //PB1, pin 5
				OCR0A = Speed; //PD6, pin 6
				break;
				
				case 4 :
				OCR0B = OFF; //PD5, pin 7
				OCR0A = OFF; //PD6, pin 6
				OCR1A = OFF; //PB1, pin 5
				OCR1B = OFF; //PB2, pin 4
				OCR2A = Speed; //PB3, pin 3
				OCR2B = Speed; //PD3, pin 2
				break;
				
				case 5 :
				OCR0B = OFF; //PD5, pin 7
				OCR0A = OFF; //PD6, pin 6
				OCR1A = OFF; //PB1, pin 5
				OCR2A = OFF; //PB3, pin 3
				OCR1B = Speed; //PB2, pin 4
				OCR2B = Speed; //PD3, pin 2
				break;
				
				case 6 :
				OCR0A = OFF; //PD6, pin 6
				OCR1A = OFF; //PB1, pin 5
				OCR1B = OFF; //PB2, pin 4
				OCR2B = OFF; //PD3, pin 2
				OCR2A = Speed; //PB3, pin 3
				OCR0B = Speed; //PD5, pin 7
				break;
				
				default:
				turnOff();
				break;
				
			}
		//}
		//else
			//turnOff();
		//body of while loo
	}
}
