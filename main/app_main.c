//////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                       _              //
//               _    _       _      _        _     _   _   _    _   _   _        _   _  _   _          //
//           |  | |  |_| |\| |_| |\ |_|   |\ |_|   |_| |_| | |  |   |_| |_| |\/| |_| |  |_| | |   /|    //    
//         |_|  |_|  |\  | | | | |/ | |   |/ | |   |   |\  |_|  |_| |\  | | |  | | | |_ | | |_|   _|_   //
//                                                                                       /              //
//////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
*   Programa básico para controle da placa durante a Jornada da Programação 1
*   Permite o controle das entradas e saídas digitais, entradas analógicas, display LCD e teclado. 
*   Cada biblioteca pode ser consultada na respectiva pasta em componentes
*   Existem algumas imagens e outros documentos na pasta Recursos
*   O código principal pode ser escrito a partir da linha 86
*/

// Área de inclusão das bibliotecas
//-----------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "ioplaca.h"   // Controles das Entradas e Saídas digitais e do teclado
#include "lcdvia595.h" // Controles do Display LCD
#include "hcf_adc.h"   // Controles do ADC
#include "mp_hcf.h"   // Controles do ADC

//#include "driver/adc.h"

// Área das macros
//-----------------------------------------------------------------------------------------------------------------------

#define CHAVE (entradas>>7)&1

#define LIGAR 'C'==le_teclado()
#define DESLIGAR '0'==le_teclado()
#define AH '7'==le_teclado()
#define HR '8'==le_teclado()

#define LIGA_R1     saidas|0b00000001
#define DESLIGA_R1  saidas&0b11111110

#define LIGA_R2     saidas|0b00000010
#define DESLIGA_R2  saidas&0b11111101  

// Área de declaração de variáveis e protótipos de funções
//-----------------------------------------------------------------------------------------------------------------------

static const char *TAG = "JORNADA_1";
static uint8_t entradas, saidas = 0; //variáveis de controle de entradas e saídas

// Funções e ramos auxiliares
//-----------------------------------------------------------------------------------------------------------------------
/*static void configura_adc()
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
}*/

// Programa Principal
//-----------------------------------------------------------------------------------------------------------------------
uint32_t oldval = 0;
uint32_t valor = 0;
int posicao = 0;

void app_main(void)
{
    /////////////////////////////////////////////////////////////////////////////////////   Programa principal


    // a seguir, apenas informações de console, aquelas notas verdes no início da execução
    ESP_LOGI(TAG, "Iniciando...");
    ESP_LOGI(TAG, "Versão do IDF: %s", esp_get_idf_version());

    /////////////////////////////////////////////////////////////////////////////////////   Inicializações de periféricos (manter assim)
    
    // inicializar os IOs e teclado da placa
    ioinit();      
    entradas = io_le_escreve(saidas); // Limpa as saídas e lê o estado das entradas

    MP_init();

    // inicializar o display LCD 
    lcd595_init();
    lcd595_write(1,1,"   Jornada 1   ");
    lcd595_write(2,1,"Programa Basico");
    
    // Inicializar o componente de leitura de entrada analógica
    esp_err_t init_result = hcf_adc_iniciar();
    if (init_result != ESP_OK) {
        ESP_LOGE("MAIN", "Erro ao inicializar o componente ADC personalizado");
    }

    //delay inicial
    vTaskDelay(1000 / portTICK_PERIOD_MS); 
    lcd595_clear();

    lcd595_write(1,0, " Programa Teste ");
    lcd595_write(2,0, "R1 [ ] - R2 [ ] ");

    /////////////////////////////////////////////////////////////////////////////////////   Periféricos inicializados

   // rotacionar_MP(1, 30);
   // configura_adc();

    /////////////////////////////////////////////////////////////////////////////////////   Início do ramo principal                    
    while (1)                                                                                                                         
    {                                                                                                                                 
        //_______________________________________________________________________________________________________________________________________________________ //
        //-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - -  -  -  -  -  -  -  -  -  -  Escreva seu código aqui!!! //
        //int valor = adc1_get_raw(ADC_CHANNEL_0)*360/4095;
        
        esp_err_t read_result = hcf_adc_ler_3(&valor); 
        valor = valor*270/4095;

        entradas = io_le_escreve (saidas);
        ESP_LOGI(TAG, "Valor ADC:  %"PRIu32" ",valor );

        posicao = valor - oldval;
        ESP_LOGI(TAG, "Result:  %d" ,posicao);
        if(CHAVE) 
        {
            saidas = LIGA_R1;
            lcd595_write(2,3, "[X]");
        }                    
        else
        {
            saidas = DESLIGA_R1;
            lcd595_write(2,3, "[ ]");
        }                                                                                                                                     
        if (LIGAR)
        {
            saidas = LIGA_R2;
            lcd595_write(2,12, "[X]");
        }
        else if (DESLIGAR)
        {
            saidas = DESLIGA_R2;
            lcd595_write(2,12, "[ ]");
        }
        if (posicao > 2)
        {
            rotacionar_MP(1,posicao);
            oldval = valor;
        }
        else if(posicao < -2)
        {
            rotacionar_MP(0,posicao*-1);
            oldval = valor;
        }
        //rotacionar_MP(0,oldval - valor);
        

        
        //-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - -  -  -  -  -  -  -  -  -  -  Escreva seu só até aqui!!! //
        //________________________________________________________________________________________________________________________________________________________//
        vTaskDelay(500 / portTICK_PERIOD_MS);    // delay mínimo obrigatório, se retirar, pode causar reset do ESP
    }
    
    // caso erro no programa, desliga o módulo ADC
 //   hcf_adc_limpar();

    /////////////////////////////////////////////////////////////////////////////////////   Fim do ramo principal
    
}
