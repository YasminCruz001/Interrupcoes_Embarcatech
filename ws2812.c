#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define LED_VER 13
#define LED_ROXO 11

#define BOTAO_1 5
#define BOTAO_2 6

#define PIXEL_PIN 7
#define TOTAL_PIXELS 25

#define TEMPO_DEBOUNCE 200000 // 200ms de debounce

static volatile uint32_t ultimo_tempo_1 = 0;
static volatile uint32_t ultimo_tempo_2 = 0;
static volatile int numero_atual = 0;

PIO controlador_pio = pio0;
int estado_maquina = 0;
uint32_t buffer_led[TOTAL_PIXELS] = {0};

// Representação numérica na matriz 5x5
int matriz_numerica[10][TOTAL_PIXELS] = {
    {0, 1, 1, 1, 0, // 0
     0, 1, 0, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0},

    {0, 1, 1, 1, 0, // 1
     0, 0, 1, 0, 0,
     0, 0, 1, 0, 0,
     0, 1, 1, 0, 0,
     0, 0, 1, 0, 0},

    {0, 1, 1, 1, 0, // 2
     0, 1, 0, 0, 0,
     0, 1, 1, 1, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0},

    {0, 1, 1, 1, 0, // 3
     0, 0, 0, 1, 0,
     0, 1, 1, 0, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0},

    {0, 1, 0, 0, 0, // 4
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 0, 1, 0},

    {0, 1, 1, 1, 0, // 5
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 0, 0,
     0, 1, 1, 1, 0},

    {0, 1, 1, 1, 0, // 6
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 0, 0,
     0, 1, 1, 1, 0},

    {0, 1, 0, 0, 0, // 7
     0, 0, 0, 1, 0,
     0, 1, 0, 0, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0},

    {0, 1, 1, 1, 0, // 8
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0},

    {0, 1, 1, 1, 0, // 9
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0}
     };

// Enviar cor para os LEDs
static inline void enviar_pixel(uint32_t cor_grb)
{
    pio_sm_put_blocking(controlador_pio, estado_maquina, cor_grb << 8u);
}

// Converter RGB para formato GRB (usado pelos LEDs WS2812)
static inline uint32_t converter_cor(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

// Atualizar LEDs da matriz com os valores armazenados no buffer
void atualizar_leds()
{
    for (int i = 0; i < TOTAL_PIXELS; i++)
    {
        enviar_pixel(buffer_led[i]);
    }
}

// Exibir número na matriz em roxo
void mostrar_numero(int num)
{
    for (int i = 0; i < TOTAL_PIXELS; i++)
    {
        buffer_led[i] = (matriz_numerica[num][i] == 1) ? converter_cor(255, 0, 255) : converter_cor(0, 0, 0);
    }
    atualizar_leds();
}

// Tratador de interrupção dos botões
void interrupcao_gpio(uint gpio, uint32_t eventos)
{
    uint32_t tempo_atual = to_us_since_boot(get_absolute_time());
    // Verifica se o botão 1 foi pressionado e aplica debounce
    if (gpio == BOTAO_1 && tempo_atual - ultimo_tempo_1 > TEMPO_DEBOUNCE)
    {
        ultimo_tempo_1 = tempo_atual;
        numero_atual = (numero_atual + 1) % 10;
    }

    // Verifica se o botão 2 foi pressionado e aplica debounce
    if (gpio == BOTAO_2 && tempo_atual - ultimo_tempo_2 > TEMPO_DEBOUNCE)
    {
            ultimo_tempo_2 = tempo_atual;
            numero_atual = (numero_atual - 1 + 10) % 10;
    }

    // Atualiza a exibição do número na matriz de LEDs
    mostrar_numero(numero_atual);
}

int main()
{
    stdio_init_all();

    // Inicializa os LEDs
    gpio_init(LED_VER);
    gpio_set_dir(LED_VER, GPIO_OUT);

    gpio_init(LED_ROXO);
    gpio_set_dir(LED_ROXO, GPIO_OUT);

    // Configuração dos botões como entrada com pull-up
    gpio_init(BOTAO_1);
    gpio_set_dir(BOTAO_1, GPIO_IN);
    gpio_pull_up(BOTAO_1);

    gpio_init(BOTAO_2);
    gpio_set_dir(BOTAO_2, GPIO_IN);
    gpio_pull_up(BOTAO_2);

    // Adiciona o programa PIO para controlar os LEDs WS2812
    uint deslocamento = pio_add_program(controlador_pio, &ws2812_program);
    ws2812_program_init(controlador_pio, estado_maquina, deslocamento, PIXEL_PIN, 800000, false);

    // Configura interrupções para os botões (detecção de borda de descida)
    gpio_set_irq_enabled_with_callback(BOTAO_1, GPIO_IRQ_EDGE_FALL, true, &interrupcao_gpio);
    gpio_set_irq_enabled_with_callback(BOTAO_2, GPIO_IRQ_EDGE_FALL, true, &interrupcao_gpio);

    // Loop principal piscando o LED vermelho
    while (1)
    {
        gpio_put(LED_VER, true);
        sleep_ms(100);
        gpio_put(LED_VER, false);
        sleep_ms(100);
    }
    return 0;
}
