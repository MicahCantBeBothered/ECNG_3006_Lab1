#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/times.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"

#define SLOTX 3
#define CYCLEX 5
#define SLOT_T 5000
int Current_Task_Num = 0;
int tps = 0;
int cycle = 0;
int slot = 0;
clock_t now;
clock_t then;
struct tms n;
int Task_iterator = 0;
void LED_SETUP()
{
    int gpio_pins = 2;
    while (gpio_pins <5)
    {
        gpio_init(gpio_pins);
        gpio_set_dir(gpio_pins, GPIO_OUT);
        gpio_put(gpio_pins, 0);
    }
}
void ON_LED (int PIN_NUM)
{
    gpio_set_dir(PIN_NUM, GPIO_OUT);
    gpio_put(PIN_NUM, 1);
}
void OFF_LED (int PIN_NUM)
{
    gpio_set_dir(PIN_NUM, GPIO_OUT);
    gpio_put(PIN_NUM, 0);
}
void Task_1()
{
    ON_LED(2);
    sleep_ms(100);
    OFF_LED(2);
}
void Task_2()
{
    ON_LED(3);
    sleep_ms(100);
    OFF_LED(3);
}
void Task_3()
{
    ON_LED(4);
    sleep_ms(100);
    OFF_LED(4);
}
void Task_Burn()
{
    sleep_ms(100);   
}
void Task_Select(int task_num)
{
    if (task_num == 1)
    {
        Task_1();
        Task_Burn();
        return;
    }
    else if (task_num == 2)
    {
        Task_2();
        Task_Burn();
        return;
    }
    else if (task_num == 3)
    {
        Task_3();
        Task_Burn();
        return;
    }
}
void new_sleep()
{
    busy_wait_ms(200);
}
int64_t alarm_callback(struct repeating_timer *t, int Task_Iter) {
    Task_Iter++;
    if (Task_Iter == 3)
    {
        Task_Iter = 0;
    }
    Task_Select(Task_Iter);
    return 0;
}
void (*ttable[SLOTX][CYCLEX])() = 
{
    {Task_1, Task_2, Task_Burn, Task_Burn, Task_Burn},
    {Task_1, Task_3, Task_Burn, Task_Burn, Task_Burn},
    {Task_Burn, Task_Burn, Task_Burn, Task_Burn, Task_Burn}
};

int main()
{
    stdio_init_all();
    struct repeating_timer timer;
    add_repeating_timer_ms(-300, alarm_callback(&timer, Task_iterator), NULL, false);
    
    int sleep_iter = 0;
    while (sleep_iter < 50)
    {
        new_sleep();
    }
    bool cancelled = cancel_repeating_timer(&timer);
}
