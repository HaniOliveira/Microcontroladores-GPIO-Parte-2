
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

// Espaço para desenho do frame


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

                break;

            case '4':

                break;

            case '5':

                break;

            case '6':

                break;

            case '7':

                break;

            case '8':

                break;

            case '9':

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
