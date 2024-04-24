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

#include "driver/gpio.h"

#include "ioplaca.h"   // Controles das Entradas e Saídas digitais e do teclado
#include "lcdvia595.h" // Controles do Display LCD
#include "hcf_adc.h"   // Controles do ADC

#include "mp_hcf.h"

// Área das macros
//-----------------------------------------------------------------------------------------------------------------------

#define CHAVE (entradas>>7)&1

#define LIGAR 'C'==le_teclado()
#define HORA '7'==le_teclado()
#define ANTIH '8'==le_teclado()
#define DESLIGAR '0'==le_teclado()

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

// Programa Principal
//-----------------------------------------------------------------------------------------------------------------------

void app_main(void)
{
    /////////////////////////////////////////////////////////////////////////////////////   Programa principal
    vTaskDelay(500 / portTICK_PERIOD_MS); // entrada do monitor

    // a seguir, apenas informações de console, aquelas notas verdes no início da execução
    ESP_LOGI(TAG, "Iniciando...");
    ESP_LOGI(TAG, "Versão do IDF: %s", esp_get_idf_version());

    /////////////////////////////////////////////////////////////////////////////////////   Inicializações de periféricos (manter assim)
    
    // inicializar os IOs e teclado da placa
    ioinit();      
   

    entradas = io_le_escreve (0b00000100); // ativa TRIAC, e desativa todas as outras
    
    printf("Entradas ativas em hexadecimal [binário]: %x [%x %x %x %x %x %x %x %x] \n", entradas,
                                                                                        (entradas>>7)&1,(entradas>>6)&1,(entradas>>5)&1,(entradas>>4)&1,
                                                                                        (entradas>>3)&1,(entradas>>2)&1,(entradas>>1)&1,entradas&1);
   
        
    //entradas = io_le_escreve(saidas); // Limpa as saídas e lê o estado das entradas
    MP_init();


    // inicializar o display LCD 
    lcd595_init();

    lcd595_write(1,1,"   Jornada 1   ");
  //  printf("%x \n", entradas);
   // lcd595_write(2,1,"Programa Basico");
    
    /*// Inicializar o componente de leitura de entrada analógica
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

    rotacionar_MP(1, 270.0);*/

    /////////////////////////////////////////////////////////////////////////////////////   Início do ramo principal                    
    while (1)                                                                                                                         
    {                                                                                                                                 
        //_______________________________________________________________________________________________________________________________________________________ //
        //-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - -  -  -  -  -  -  -  -  -  -  Escreva seu código aqui!!! //
           
        /*entradas = io_le_escreve (saidas);

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
        if(HORA)
        {
            rotacionar_MP(0, 90.0);
        }
        if(ANTIH)
        {
            rotacionar_MP(1, 90.0);
        }*/
        
        //-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - -  -  -  -  -  -  -  -  -  -  Escreva seu só até aqui!!! //
        //________________________________________________________________________________________________________________________________________________________//
        vTaskDelay(10 / portTICK_PERIOD_MS);    // delay mínimo obrigatório, se retirar, pode causar reset do ESP
    }
    
    // caso erro no programa, desliga o módulo ADC
    hcf_adc_limpar();

    /////////////////////////////////////////////////////////////////////////////////////   Fim do ramo principal
    
}
