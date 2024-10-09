//Micah Mitchell - 816035305
//
// The transition from one state to another requires the same conditions to be met, so it possible to make the program check for the satisfying conditions
// and then alternate the device state when the conditions are met. 
//

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"

volatile bool timer_fired = false;
bool Device_State = true; // True = ON, False = OFF
char Prev_char = 'b';
void setupLED()         // LED fuctions to show Device states
{
	gpio_init(28);
	gpio_set_dir(28, GPIO_OUT);
}

void LED_ON()
{
	gpio_put(28 , 1);
}
void LED_OFF()
{
	gpio_put(28, 0);
}


bool Char_Compare(char Cur_Char, char New_Char) 
{
	if (Cur_Char == New_Char)
	{
		//printf("Same Char!\n"); Used to test logic
		return true;
	}
	if (Cur_Char != New_Char)
	{
		//printf("Different Char!\n"); Used to test logic
		return false;
	}
}

bool Device_State_Manager(bool Cur_State) // Function designed to alternate device states, only to be called when the device needs to change states.
{
    bool New_State;
	if (Cur_State == true)
    {
        LED_OFF();
        New_State = !Cur_State;
    }
    else
    {
        LED_ON();
        New_State = !Cur_State;
    } 
    return New_State;
}

int64_t repeating_timer_callback(struct repeating_timer *t)// I dont know how this works, i found it in the pico libracy doc
{
    timer_fired = true;
    char Current_Char = getchar();
    if (timer_fired == false && Char_Compare(Current_Char, Prev_char) == true)
    {
        Device_State = (Device_State_Manager(Device_State));
        printf("Device State is changed\n");
    }
    else if (Char_Compare(Current_Char, Prev_char) == false)
    {
        Device_State = (Device_State_Manager(Device_State));
        printf("Device State is changed\n");
        printf("Device State is changed\n");
    }
    else 
    {
        printf("Device state is unchanged\n");
    }
    Prev_char = Current_Char;
    timer_fired = false;
    return 0;
}

int main()
{
    LED_ON();
    struct repeating_timer timer;
    add_repeating_timer_ms(500, repeating_timer_callback(&timer), NULL, false);// I dont know how this works, i found it in the example from the SDK doc
    sleep_ms(5000);
        /*char Current_Char = getchar(); 
        if (timer_fired == false && Char_Compare(Current_Char, Prev_char) == true)
        {
            Device_State = (Device_State_Manager(Device_State));
            printf("Device State is changed\n");
        }
        else if (Char_Compare(Current_Char, Prev_char) == false)
        {
            Device_State = (Device_State_Manager(Device_State));
            printf("Device State is changed\n");
            printf("Device State is changed\n");
        }
        else 
        {
            printf("Device state is unchanged\n");
        }
        char Prev_char = Current_Char;
        timer_fired = false;*/
    

	//char Current_Char = getchar();
	//char Prev_Char = 'b';
	//printf("Bool output is %d", Char_Compare(Prev_Char, Current_Char));
	
}