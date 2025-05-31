#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"
#include "pio_matrix.pio.h"

#define NUM_PIXELS 25
#define PIO_PIN 7                 // Pino de dados da matriz de LEDs
const uint BUTTON_SEQ_1_PIN = 5;  // Botão 0
const uint BUTTON_SEQ_2_PIN = 6;  // Botão 1

// --- Máquina de Estados ---
volatile int modo_programa = 0; // 0=Ocioso, 1=Sequência 1, 2=Sequência 2
volatile int frame_index = 0;   // Controla qual desenho da sequência mostrar

// DESENVOLVEDOR DANIELLI

//  DEBOUNCE DA INTERRUPÇAÕ






//  DESENHOS EXIBIDOS NA MATRIZ

double desenhos_seq1[5][NUM_PIXELS] = {
    {0.5, 0.0, 0.0, 0.0, 0.5,   0.0, 0.5, 0.0, 0.5, 0.0,   0.0, 0.0, 0.5, 0.0, 0.0,   0.0, 0.5, 0.0, 0.5, 0.0,   0.5, 0.0, 0.0, 0.0, 0.5},
    {0.0, 1.0, 1.0, 1.0, 0.0,   1.0, 0.0, 0.0, 0.0, 1.0,   1.0, 0.0, 0.0, 0.0, 1.0,   1.0, 0.0, 0.0, 0.0, 1.0,   0.0, 1.0, 1.0, 1.0, 0.0},
    {0.0, 0.0, 1.0, 0.0, 0.0,   0.0, 1.0, 1.0, 1.0, 0.0,   1.0, 1.0, 1.0, 1.0, 1.0,   0.0, 1.0, 1.0, 1.0, 0.0,   0.0, 0.0, 1.0, 0.0, 0.0},
    {1.0, 1.0, 1.0, 1.0, 1.0,   1.0, 0.0, 0.0, 0.0, 1.0,   1.0, 0.0, 0.0, 0.0, 1.0,   1.0, 0.0, 0.0, 0.0, 1.0,   1.0, 1.0, 1.0, 1.0, 1.0},
    {0.0, 0.0, 0.0, 0.0, 0.0,   0.0, 1.0, 1.0, 1.0, 0.0,   0.0, 1.0, 0.0, 1.0, 0.0,   0.0, 1.0, 1.0, 1.0, 0.0,   0.0, 0.0, 0.0, 0.0, 0.0}
};

double desenhos_seq2[5][NUM_PIXELS] = {
    {0.0, 0.0, 0.0, 0.0, 0.0,   1.0, 1.0, 0.0, 1.0, 1.0,   0.0, 0.0, 0.0, 0.0, 0.0,   0.0, 1.0, 1.0, 1.0, 0.0,   0.0, 1.0, 1.0, 1.0, 0.0},
    {0.0, 0.7, 0.0, 0.7, 0.0,   0.0, 0.7, 0.0, 0.7, 0.0,   0.0, 0.0, 0.0, 0.0, 0.0,   0.0, 0.0, 0.0, 0.0, 0.0,   0.0, 0.7, 0.7, 0.7, 0.0},
    {0.8, 0.8, 0.0, 0.8, 0.8,   0.8, 0.8, 0.0, 0.8, 0.8,   0.0, 0.0, 0.0, 0.0, 0.0,   0.0, 0.8, 0.8, 0.8, 0.0,   0.0, 0.8, 0.0, 0.8, 0.0},
    {0.9, 0.9, 0.0, 0.0, 0.0,   0.9, 0.9, 0.0, 0.9, 0.9,   0.0, 0.0, 0.0, 0.0, 0.0,   0.0, 0.0, 0.0, 0.0, 0.0,   0.0, 0.9, 0.9, 0.9, 0.0},
    {1.0, 1.0, 0.0, 1.0, 1.0,   1.0, 1.0, 0.0, 1.0, 1.0,   0.0, 0.0, 0.0, 0.0, 0.0,   0.0, 1.0, 0.0, 1.0, 0.0,   0.0, 1.0, 1.0, 1.0, 0.0},
};


//FUNCOES DE DEFINIÇÃO RGB

uint32_t cor_rgb(double r, double g, double b) { 
    uint8_t R = (uint8_t)(r * 255.0);
    uint8_t G = (uint8_t)(g * 255.0);
    uint8_t B = (uint8_t)(b * 255.0);
    return ((uint32_t)(G) << 16) | ((uint32_t)(R) << 8) | (uint32_t)(B);
}


//DESENVOLVEDOR GUILHERME

//FUNÇÃO PARA MANIPULAR A MATRIX






//FUNÇÃO LIMPAR A MATRIZ






//FUNÇÃO DE INTERRUPÇÃO





int main() {
    stdio_init_all();
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, PIO_PIN);

    gpio_init(BUTTON_SEQ_1_PIN);
    gpio_set_dir(BUTTON_SEQ_1_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_SEQ_1_PIN);
    gpio_set_irq_enabled_with_callback(BUTTON_SEQ_1_PIN, GPIO_IRQ_EDGE_FALL, true, &button_irq_handler);

    gpio_init(BUTTON_SEQ_2_PIN);
    gpio_set_dir(BUTTON_SEQ_2_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_SEQ_2_PIN);
    gpio_set_irq_enabled_with_callback(BUTTON_SEQ_2_PIN, GPIO_IRQ_EDGE_FALL, true, &button_irq_handler);

    printf("Setup com pinos corretos. Aguardando botoes...\n");

    while (true) {
        if (modo_programa == 1) {
            printf("Mostrando Seq 1, Frame %d\n", frame_index);

            // Sequência 1: Verde -> Azul -> Vermelho
            int seletor_cor = frame_index % 3;

            if (seletor_cor == 0) { // Desenha em Verde
                desenhar_na_matriz(pio, sm, desenhos_seq1[frame_index], 0.0, 1.0, 0.0);
            } else if (seletor_cor == 1) { // Desenha em Azul
                desenhar_na_matriz(pio, sm, desenhos_seq1[frame_index], 0.0, 0.0, 1.0);
            } else { // Desenha em Vermelho
                desenhar_na_matriz(pio, sm, desenhos_seq1[frame_index], 1.0, 0.0, 0.0);
            }

            frame_index++;
            sleep_ms(500);
            if (frame_index >= 5) {
                modo_programa = 0;
            }

        } else if (modo_programa == 2) {
            printf("Mostrando Seq 2, Frame %d\n", frame_index);

            // Sequência 2: Azul -> Verde -> Vermelho
            int seletor_cor = frame_index % 3;
            if (seletor_cor == 0) { // Frame 0, 3 -> Desenha em Azul
                desenhar_na_matriz(pio, sm, desenhos_seq2[frame_index], 0.0, 0.0, 1.0);
            } else if (seletor_cor == 1) { // Frame 1, 4 -> Desenha em Verde
                desenhar_na_matriz(pio, sm, desenhos_seq2[frame_index], 0.0, 1.0, 0.0);
            } else { // Frame 2 -> Desenha em Vermelho
                desenhar_na_matriz(pio, sm, desenhos_seq2[frame_index], 1.0, 0.0, 0.0);
            }

            frame_index++;
            sleep_ms(1000);
            if (frame_index >= 5) {
                modo_programa = 0;
            }
        } else {
            limpar_matriz(pio, sm);
            sleep_ms(20);
        }
    }
}