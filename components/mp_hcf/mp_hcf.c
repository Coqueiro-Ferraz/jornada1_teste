#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <stdio.h>

#define MP1 GPIO_NUM_19
#define MP2 GPIO_NUM_21
#define MP3 GPIO_NUM_22
#define MP4 GPIO_NUM_23
#define TMP 10

static const char *TAG = "MP_HCF";

void passo_1()
{
    gpio_set_level(MP1,0);
    gpio_set_level(MP2,1);
    gpio_set_level(MP3,1);
    gpio_set_level(MP4,0);
    vTaskDelay(TMP / portTICK_PERIOD_MS); 
}
void passo_2()
{
    gpio_set_level(MP1,0);
    gpio_set_level(MP2,0);
    gpio_set_level(MP3,1);
    gpio_set_level(MP4,1);
    vTaskDelay(TMP / portTICK_PERIOD_MS); 
}
void passo_3()
{
    gpio_set_level(MP1,1);
    gpio_set_level(MP2,0);
    gpio_set_level(MP3,0);
    gpio_set_level(MP4,1);
    vTaskDelay(TMP / portTICK_PERIOD_MS); 
}
void passo_4()
{
    gpio_set_level(MP1,1);
    gpio_set_level(MP2,1);
    gpio_set_level(MP3,0);
    gpio_set_level(MP4,0);
    vTaskDelay(TMP / portTICK_PERIOD_MS); 
}
void passo_fim()
{
    gpio_set_level(MP1,0);
    gpio_set_level(MP2,0);
    gpio_set_level(MP3,0);
    gpio_set_level(MP4,0);
    vTaskDelay(TMP / portTICK_PERIOD_MS); 
}
void MP_horario(int passos)
{
    ESP_LOGI(TAG, "Rotacionando horário..");
    for (int i = passos * 4; i > 0; i--)
    {
        passo_4();
        passo_3();
        passo_2();
        passo_1();
    }
    passo_fim();
}
void MP_antihora(int passos)
{
    ESP_LOGI(TAG, "Rotacionando anti-horário..");
    for (int i = passos * 4; i > 0; i--)
    {   
        passo_1();
        passo_2();
        passo_3();
        passo_4();
    }
    passo_fim();
}

void rotacionar_MP(int horario, float graus) //64 passos por volta = 5,625 graus por passo
{
    float passos = graus / 2.8125;
    ESP_LOGI(TAG, "Recebido %.1f graus, %d passos", graus, (int)passos);
    horario == 0 ? MP_antihora((int)passos) : MP_horario((int)passos);
    ESP_LOGI(TAG, "Rotação concluída");
}
void MP_init()
{
    gpio_reset_pin(MP1);
    gpio_set_direction(MP1, GPIO_MODE_OUTPUT);
    gpio_set_level(MP1, 0);
    
    gpio_reset_pin(MP2);
    gpio_set_direction(MP2, GPIO_MODE_OUTPUT);
    gpio_set_level(MP2, 0);
    
    gpio_reset_pin(MP3);
    gpio_set_direction(MP3, GPIO_MODE_OUTPUT);
    gpio_set_level(MP3, 0);
    
    gpio_reset_pin(MP4);
    gpio_set_direction(MP4, GPIO_MODE_OUTPUT);
    gpio_set_level(MP4, 0);

    ESP_LOGI(TAG, "MP Configurado modo Driver ULN2003");
}
void MP_init_placa()
{

}


