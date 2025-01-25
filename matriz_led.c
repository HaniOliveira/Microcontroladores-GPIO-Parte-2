
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

// arquivo .pio
#include "matriz_led.pio.h"

// número de LEDs
#define NUM_PIXELS 25

// pino de saída
#define OUT_PIN 10

#define BUZZER 21

// Prototipagem da função play_tone
void play_tone(uint16_t frequencia, uint16_t duracao);

// Espaço para desenho do frame

// Desenho da explosão - wellington
double desenho_explosao[5][NUM_PIXELS] = {
    {0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0},

    {0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 0.0,
     1.0, 1.0, 1.0, 1.0, 1.0,
     0.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0},

    {0.0, 1.0, 1.0, 1.0, 0.0,
     1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0,
     0.0, 1.0, 1.0, 1.0, 0.0},

    {1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0},

    {0.0, 1.0, 1.0, 1.0, 0.0,
     1.0, 1.0, 1.0, 1.0, 1.0,
     0.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0}
};


double desenho_foguete[5][NUM_PIXELS] = {
    {0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0},

   
};

// Desenho da Estrela
double desenho_estrela[5][NUM_PIXELS] = {
    {0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 1.0, 0.0, 1.0, 0.0,
     1.0, 0.0, 1.0, 0.0, 1.0,
     0.0, 1.0, 0.0, 1.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0},

    
};


double desenho_olho[5][NUM_PIXELS] = {
    // Frame 1: Olho Totalmente Aberto
    {0.0, 0.5, 1.0, 0.5, 0.0,
     0.5, 1.0, 1.0, 1.0, 0.5,
     1.0, 1.0, 0.0, 1.0, 1.0,
     0.5, 1.0, 1.0, 1.0, 0.5,
     0.0, 0.5, 1.0, 0.5, 0.0},

    // Frame 2: Olho Começando a Fechar
    {0.0, 0.0, 1.0, 0.0, 0.0,
     0.5, 1.0, 1.0, 1.0, 0.5,
     1.0, 1.0, 1.0, 1.0, 1.0,
     0.5, 1.0, 1.0, 1.0, 0.5,
     0.0, 0.5, 0.5, 0.5, 0.0},

    // Frame 3: Olho Meio Fechado
    {0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 0.0,
     1.0, 1.0, 1.0, 1.0, 1.0,
     0.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 0.0, 0.5, 0.0, 0.0},

    // Frame 4: Olho Quase Fechado
    {0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     1.0, 1.0, 1.0, 1.0, 1.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0},

    // Frame 5: Olho Totalmente Fechado
    {0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.5, 0.5, 0.5, 0.5, 0.5,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0}
};






// Botão #
double desenho_hashtag[25] = {0.2, 0.2, 0.2, 0.2, 0.2,
                              0.2, 0.2, 0.2, 0.2, 0.2,
                              0.2, 0.2, 0.2, 0.2, 0.2,
                              0.2, 0.2, 0.2, 0.2, 0.2,
                              0.2, 0.2, 0.2, 0.2, 0.2};

// Letra A
double desenho_a[25] = {0.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0};

// Letra B
double desenho_b[25] = {1.0, 1.0, 1.0, 1.0, 1.0,
                        1.0, 1.0, 1.0, 1.0, 1.0,
                        1.0, 1.0, 1.0, 1.0, 1.0,
                        1.0, 1.0, 1.0, 1.0, 1.0,
                        1.0, 1.0, 1.0, 1.0, 1.0};
// Letra C
double desenho_c[25] = {0.8, 0.8, 0.8, 0.8, 0.8,
                        0.8, 0.8, 0.8, 0.8, 0.8,
                        0.8, 0.8, 0.8, 0.8, 0.8,
                        0.8, 0.8, 0.8, 0.8, 0.8,
                        0.8, 0.8, 0.8, 0.8, 0.8};
// Letra D
double desenho_d[25] = {0.5, 0.5, 0.5, 0.5, 0.5,
                        0.5, 0.5, 0.5, 0.5, 0.5,
                        0.5, 0.5, 0.5, 0.5, 0.5,
                        0.5, 0.5, 0.5, 0.5, 0.5,
                        0.5, 0.5, 0.5, 0.5, 0.5};

// Coluna 1 acessa
double desenho_coluna1[25] = {0.2, 0.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.2,
                        0.2, 0.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.2,
                        0.2, 0.0, 0.0, 0.0, 0.0};
// Coluna 2 acessa
double desenho_coluna2[25] = {0.0, 0.4, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.4, 0.0,
                        0.0, 0.4, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.4, 0.0,
                        0.0, 0.4, 0.0, 0.0, 0.0};
// Coluna 3 acessa
double desenho_coluna3[25] = {0.0, 0.0, 0.6, 0.0, 0.0,
                        0.0, 0.0, 0.6, 0.0, 0.0,
                        0.0, 0.0, 0.6, 0.0, 0.0,
                        0.0, 0.0, 0.6, 0.0, 0.0,
                        0.0, 0.0, 0.6, 0.0, 0.0};
// Coluna 4 acessa
double desenho_coluna4[25] = {0.0, 0.0, 0.0, 0.8, 0.0,
                        0.0, 0.8, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.8, 0.0,
                        0.0, 0.8, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.8, 0.0};
// Coluna 5 acessa
double desenho_coluna5[25] = {0.0, 0.0, 0.0, 0.0, 1.0,
                        1.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 1.0,
                        1.0, 0.0, 0.0, 0.0, 0.0,
                       0.0, 0.0, 0.0, 0.0, 1.0};

double frames_maca[7][NUM_PIXELS] = {
    {
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0
    },
    {
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0
    },
    {
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0
    },
    {
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0
    },
    {
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    0.0, 1.0, 1.0, 1.0, 0.0
    },
    {
    0.0, 0.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    0.0, 1.0, 1.0, 1.0, 0.0
    },
    {
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0
    }
};
// Desenho para o número 9 (carinha piscando)
double desenho_nove[5][NUM_PIXELS] = {
{0.0, 0.0, 1.0, 0.0, 0.0,
 0.0, 1.0, 0.0, 1.0, 0.0,  // Olhos abertos
 0.0, 0.0, 1.0, 0.0, 0.0,
 0.0, 1.0, 0.0, 1.0, 0.0,
 1.0, 0.0, 0.0, 0.0, 1.0},

{0.0, 0.0, 1.0, 0.0, 0.0,
 0.0, 1.0, 0.0, 1.0, 0.0, // Olhos semicerrados
 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 1.0, 0.0, 1.0, 0.0,
 1.0, 0.0, 0.0, 0.0, 1.0},

{0.0, 0.0, 1.0, 0.0, 0.0,
 0.0, 0.0, 0.0, 0.0, 0.0, // Olhos fechados
 0.0, 0.0, 1.0, 0.0, 0.0,
 0.0, 1.0, 0.0, 1.0, 0.0,
 1.0, 0.0, 0.0, 0.0, 1.0},

{0.0, 0.0, 1.0, 0.0, 0.0,
 0.0, 1.0, 0.0, 1.0, 0.0, // Olhos semicerrados
 0.0, 0.0, 0.0, 0.0, 0.0,
 0.0, 1.0, 0.0, 1.0, 0.0,
 1.0, 0.0, 0.0, 0.0, 1.0},

{0.0, 0.0, 1.0, 0.0, 0.0,
 0.0, 1.0, 0.0, 1.0, 0.0,  // Olhos abertos
 0.0, 0.0, 1.0, 0.0, 0.0,
 0.0, 1.0, 0.0, 1.0, 0.0,
 1.0, 0.0, 0.0, 0.0, 1.0}

};

// imprimir valor binário
void imprimir_binario(int num)
{
    int i;
    for (i = 31; i >= 0; i--)
    {
        (num & (1 << i)) ? printf("1") : printf("0");
    }
}

// rotina da interrupção
static void gpio_irq_handler(uint gpio, uint32_t events)
{
    printf("Interrupção ocorreu no pino %d, no evento %d\n", gpio, events);
    printf("HABILITANDO O MODO GRAVAÇÃO");
    reset_usb_boot(0, 0); // habilita o modo de gravação do microcontrolador
}

// rotina para definição da intensidade de cores do led
uint32_t matrix_rgb(double b, double r, double g)
{
    unsigned char R, G, B;
    R = r * 255;
    G = g * 255;
    B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

// rotina para acionar a matrix de leds e fazer as animações

void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{

    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {

        valor_led = matrix_rgb((b > 0.0 ? desenho[24 - i] : 0.0), (r > 0.0 ? desenho[24 - i] : 0.0), (g > 0.0 ? desenho[24 - i] : 0.0));
        pio_sm_put_blocking(pio, sm, valor_led);
    }

    imprimir_binario(valor_led);
}


void desenho_pio_foguete(double desenho_foguete[5][NUM_PIXELS], uint32_t valor_led, PIO pio, uint sm, double r, double g, double b) {
    for (int frame = 0; frame < 5; frame++) {
        for (int i = 0; i < NUM_PIXELS; i++) {
            valor_led = matrix_rgb(desenho_foguete[frame][i], r, g);  // Atualiza a cor conforme o quadro
            pio_sm_put_blocking(pio, sm, valor_led);
        }
        sleep_ms(500);  // Pausa para criar o efeito de animação
    }
}


void desenho_pio_olho(double desenho_olho[5][NUM_PIXELS], uint32_t valor_led, PIO pio, uint sm, double r, double g, double b) {
    for (int frame = 0; frame < 5; frame++) { // Itera pelos 5 frames
        for (int i = 0; i < NUM_PIXELS; i++) {
            valor_led = matrix_rgb(desenho_olho[frame][i], r, g, b);  // Define a cor do LED
            pio_sm_put_blocking(pio, sm, valor_led);
        }
        sleep_ms(200);  // Pausa para criar o efeito de animação
    }
}



// Desenho coluna 1
void desenho_pio_coluna1(double *desenho_coluna1, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {
        
        valor_led = matrix_rgb(0.0, desenho_coluna1[24 - i], 0.0);  // Cor vermelho (r=0, g=1, b=0)
        pio_sm_put_blocking(pio, sm, valor_led);
    }


    imprimir_binario(valor_led);
}

// Desenho coluna 2
void desenho_pio_coluna2(double *desenho_coluna2, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {
        valor_led = matrix_rgb(0.0, desenho_coluna2[24 - i], 0.0);  // Cor vermelho (r=0, g=1, b=0)
        pio_sm_put_blocking(pio, sm, valor_led);
    }

    imprimir_binario(valor_led);
}

// Desenho coluna 3
void desenho_pio_coluna3(double *desenho_coluna3, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {
        valor_led = matrix_rgb(0.0, desenho_coluna3[24 - i], 0.0);  // Cor vermelho (r=0, g=1, b=0)
        pio_sm_put_blocking(pio, sm, valor_led);
    }

    imprimir_binario(valor_led);
}

// Desenho coluna 4
void desenho_pio_coluna4(double *desenho_coluna4, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {
        valor_led = matrix_rgb(0.0, desenho_coluna4[24 - i], 0.0);  // Cor vermelho (r=0, g=1, b=0)
        pio_sm_put_blocking(pio, sm, valor_led);
    }

    imprimir_binario(valor_led);
}

// Desenho coluna 5
void desenho_pio_coluna5(double *desenho_coluna5, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {
        valor_led = matrix_rgb(0.0, desenho_coluna5[24 - i], 0.0);  // Cor vermelho (r=0, g=1, b=0)
        pio_sm_put_blocking(pio, sm, valor_led);
    }

    imprimir_binario(valor_led);
}

// Desenho da Estrela com a cor azul
// void desenho_pio_foguete(double desenho_foguete[5][NUM_PIXELS], uint32_t valor_led, PIO pio, uint sm, double r, double g, double b) {
//     for (int frame = 0; frame < 5; frame++) {
//         for (int i = 0; i < NUM_PIXELS; i++) {
//             valor_led = matrix_rgb(desenho_foguete[frame][i], r, g);  // Atualiza a cor conforme o quadro
//             pio_sm_put_blocking(pio, sm, valor_led);
//         }
//         sleep_ms(200);  // Pausa para criar o efeito de animação
//     }
// }

void desenho_pio_estrela(double desenho_estrela[5][NUM_PIXELS], uint32_t valor_led, PIO pio, uint sm, double r, double g, double b) {
    for (int frame = 0; frame < 5; frame++) {
        for (int i = 0; i < NUM_PIXELS; i++) {
            valor_led = matrix_rgb(desenho_estrela[frame][i], r, g);  // Atualiza a cor conforme o quadro
            pio_sm_put_blocking(pio, sm, valor_led);
        }
        sleep_ms(200);  // Pausa para criar o efeito de animação
    }
}

// Função para fazer a animação da explosão - wellington
void desenho_pio_explosao(double desenho_explosao[5][NUM_PIXELS], uint32_t valor_led, PIO pio, uint sm) {
    // Definindo as cores: Vermelho, Verde e Azul
    double cores[3][3] = {
        {1.0, 0.0, 0.0}, // Vermelho
        {0.0, 1.0, 0.0}, // Verde
        {0.0, 0.0, 1.0}  // Azul
    };

    // Apagar todos os LEDs antes de iniciar a animação
    for (int i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgb(0.0, 0.0, 0.0); // Todos os LEDs apagados
        pio_sm_put_blocking(pio, sm, valor_led);
    }
    sleep_ms(500); // Pausa para mostrar que os LEDs estão apagados
     // Tocar o buzzer com um som grave por 1 segundo

    play_tone(200, 1000);  // Toca um tom de 200 Hz por 1000 ms
    // Loop para cada frame da explosão
    for (int frame = 0; frame < 5; frame++) {
        // Alterna entre as cores
        for (int cor = 0; cor < 3; cor++) {
            for (int i = 0; i < NUM_PIXELS; i++) {
                // Define a cor do LED com base no frame e na cor atual
                valor_led = matrix_rgb(desenho_explosao[frame][i] * cores[cor][0], 
                                       desenho_explosao[frame][i] * cores[cor][1], 
                                       desenho_explosao[frame][i] * cores[cor][2]);
                pio_sm_put_blocking(pio, sm, valor_led);
            }
            sleep_ms(200);  // Pausa para criar o efeito de animação
        }
    }

    // Apagar todos os LEDs após a animação
    for (int i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgb(0.0, 0.0, 0.0); // Todos os LEDs apagados
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}

void animacao_pio_maca(double frames_maca[7][NUM_PIXELS], uint32_t valor_led, PIO pio, uint sm, double r, double g, double b) {

    for (int16_t i = 0; i < 7; i++)
    {
        for (int16_t j = 0; j < NUM_PIXELS; j++)
        {

            if (i >= 3 && i <= 5 && j == 17 || i >= 3 && i <= 5 && j == 21) {

                valor_led = matrix_rgb(0.0, 0.0, (g > 0.0 ? frames_maca[i][24 - j] : 0.0));
                pio_sm_put_blocking(pio, sm, valor_led);

            } else {

                valor_led = matrix_rgb(0.0, (r > 0.0 ? frames_maca[i][24 - j] : 0.0), 0.0);
                pio_sm_put_blocking(pio, sm, valor_led);

            }
        }
        sleep_ms(117);
    }
}


uint8_t columns[4] = {1, 2, 3, 4};
uint8_t rows[4] = {5, 6, 7, 8};

char KEY_MAPS[16] = {
    '1', '2', '3', 'A',
    '4', '5', '6', 'B',
    '7', '8', '9', 'C',
    '*', '0', '#', 'D'};

void init_keypad()
{
    for (uint8_t i = 0; i < 4; i++)
    {
        gpio_init(columns[i]);
        gpio_set_dir(columns[i], GPIO_OUT);
        gpio_put(columns[i], 1);

        gpio_init(rows[i]);
        gpio_set_dir(rows[i], GPIO_IN);
        gpio_pull_up(rows[i]);
    }
}

char get_key()
{
    for (uint8_t col = 0; col < 4; col++)
    {
        gpio_put(columns[col], 0);
        for (uint8_t row = 0; row < 4; row++)
        {
            if (gpio_get(rows[row]) == 0)
            {
                sleep_ms(50);
                if (gpio_get(rows[row]) == 0)
                {
                    gpio_put(columns[col], 1);
                    return KEY_MAPS[row * 4 + col];
                }
            }
        }
        gpio_put(columns[col], 1);
    }
    return '\0';
}

/*Função para usar o buzzer*/
void play_tone(uint16_t frequencia, uint16_t duracao)
{

    gpio_init(BUZZER);
    gpio_set_dir(BUZZER, GPIO_OUT);

    // Calcula o período do sinal em microssegundos
    uint32_t periodo = 1000000 / frequencia; // Período em microssegundos
    uint32_t metade_perodo = periodo / 2;    // Metade do período

    for (uint16_t i = 0; i < (duracao * 1000) / periodo; i++)
    {                            // Duração em milissegundos
        gpio_put(BUZZER, 1);     // Liga o buzzer
        sleep_us(metade_perodo); // Espera metade do período
        gpio_put(BUZZER, 0);     // Desliga o buzzer
        sleep_us(metade_perodo); // Espera metade do período
    }
}

void desenho_pio_nove(double desenho_nove[5][NUM_PIXELS], uint32_t valor_led, PIO pio, uint sm, double r, double g, double b) {
    for (int frame = 0; frame < 5; frame++){
        for (int i = 0; i < NUM_PIXELS; i++) {
            valor_led = matrix_rgb(desenho_nove[frame][i]*b, desenho_nove[frame][i] *r, desenho_nove[frame][i]*g);
            pio_sm_put_blocking(pio, sm, valor_led);
        }
        sleep_ms(200);
    }

    // Apaga os LEDs após a animação
    for (int i = 0; i < NUM_PIXELS; i++) {
        pio_sm_put_blocking(pio, sm, matrix_rgb(0, 0, 0)); // Cor preta (todos os LEDs apagados)
    }
}

int main()
{
    stdio_init_all();

    init_keypad();
    PIO pio = pio0;
    bool ok;
    uint16_t i;
    uint32_t valor_led;
    double r = 0.0, b = 0.0, g = 0.0;
    // coloca a frequência de clock para 128 MHz, facilitando a divisão pelo clock
    ok = set_sys_clock_khz(128000, false);

    printf("Sistema inicializado.\n");

    // configurações da PIO
    uint offset = pio_add_program(pio, &matriz_led_program);
    uint sm = pio_claim_unused_sm(pio, true);
    matriz_led_program_init(pio, sm, offset, OUT_PIN);

    while (1)
    {
        char key = get_key();
        if (key != '\0')
        {
            printf("Tecla pressionada: %c\n", key);

            switch (key)
            {
            case '1':
                    desenho_pio_foguete(desenho_foguete, valor_led, pio, sm, r, g, b);
                break;

            case '2':
                     desenho_pio_estrela(desenho_estrela, valor_led, pio, sm, r, g, b);
                break;

            case '3':

                     desenho_pio_coluna1(desenho_coluna1, valor_led, pio, sm, r, g, b);
                     sleep_ms(200);
                     desenho_pio_coluna2(desenho_coluna2, valor_led, pio, sm, r, g, b);
                     sleep_ms(200);
                     desenho_pio_coluna3(desenho_coluna3, valor_led, pio, sm, r, g, b);
                     sleep_ms(200);
                     desenho_pio_coluna4(desenho_coluna4, valor_led, pio, sm, r, g, b);
                     sleep_ms(200);
                     desenho_pio_coluna5(desenho_coluna5, valor_led, pio, sm, r, g, b);
                     sleep_ms(200);
                     desenho_pio(desenho_a, valor_led, pio, sm, 0.0, 0.0, 0.0);

                break;

            case '4':
                animacao_pio_maca(frames_maca, valor_led, pio, sm, 1.0, 1.0, 0.0);
                break;

            case '5':
                 desenho_pio_olho(desenho_olho, valor_led, pio, sm, 0.0, 0.0, 1.0);  // Cor azul para o olho
                break;

            case '6':

                break;

            case '7':
                desenho_pio_explosao(desenho_explosao, valor_led, pio, sm);
                break;

            case '8':

                break;

            case '9':
                desenho_pio_nove(desenho_nove, valor_led, pio, sm, 1.0, 1.0, 0.0 ); // Amarelo
                break;

            case '#':
                desenho_pio(desenho_hashtag, valor_led, pio, sm, 1.0, 1.0, 1.0);

                break;

            case '*':

                break;

            case 'A':
                desenho_pio(desenho_a, valor_led, pio, sm, 0.0, 0.0, 0.0);
                break;

            case 'B':
                desenho_pio(desenho_b, valor_led, pio, sm, 0.0, 0.0, 1.0);
                break;

            case 'C':
                desenho_pio(desenho_c, valor_led, pio, sm, 0.8, 0.0, 0.0);
                break;

            case 'D':
                desenho_pio(desenho_d, valor_led, pio, sm, 0.0, 0.5, 0.0);
                break;

            // Adicione mais casos para outras teclas se necessário
            default:

                break;
            }
        }
        sleep_ms(100);
    }

    return 0;
}
