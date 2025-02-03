# Exibição de Números com LEDs WS2812 e Controle por Botões
Unidade 4 - Capítulo 4

Este projeto utiliza uma matriz de LEDs WS2812 5x5 para exibir números de 0 a 9, controlados por dois botões conectados a um microcontrolador RP2040 na placa BitDogLab. Além disso, um LED RGB é integrado ao sistema, com o componente vermelho piscando continuamente a uma frequência de 5 Hz. O código é escrito em C, utilizando o Pico SDK, e a simulação é realizada com a extensão Wokwi Simulator, integrada ao VS Code.
O objetivo deste projeto é demonstrar como utilizar interrupções para capturar eventos de botões, aplicar técnicas de debouncing via software para leituras precisas e integrar o controle de LEDs endereçáveis WS2812 com um microcontrolador.

## Funcionalidades

- **LED Vermelho**: Pisca a uma taxa de 5 Hz.
- **Botão A**: Incrementa o número exibido na matriz de LEDs (0-9).
- **Botão B**: Decrementa o número exibido na matriz de LEDs (0-9).
- **Matriz de LEDs WS2812**: Exibe números de 0 a 9 em formato digital.
- **Debouncing de Botões**: Interrupções com tratamento de debouncing para evitar múltiplos disparos.

## Requisitos

- **Hardware**:
  - Computador pessoal
  - Raspberry Pi Pico W (RP2040)
  - Cabo micro-USB para USB-A

- **Software**:
- VS Code instalado
  - Extensões:
   - C/C++
   - Pico SDK
- Pico SDK configurado
- Wokwi integrado ao VS Code (para simulação)
- Git instalado

## Configuração Inicial
1. Clone o repositório para sua máquina local:
   ```bash
   git clone <https://github.com/YasminCruz001/Interrupcoes_Embarcatech.git>
2. Abra o projeto no VS Code.

## Vídeo demonstrativo

[![Vídeo de Demonstração](https://img.shields.io/badge/Assistir-Vídeo-blue)](https://drive.google.com/file/d/1F8rPZ8euf3PFMoI5qfjCSZo_gJHYV6XR/view?usp=sharing)
