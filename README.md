# LINK DO VIDEO: https://drive.google.com/file/d/1r6vwzNNCo_DsItZiqogJhnkE8Y2GUaPO/view?usp=drive_link

  #                                                                  DESENVOLVIMENTO
# Controle de Matriz de LED com Raspberry Pi Pico

## 1. Visão Geral do Projeto

Neste documento, apresento o codigo que minha equipe e eu desenvolvemos para um controlador de matriz de LED 5x5, utilizando o Raspberry Pi Pico. O objetivo foi criar um sistema interativo onde o usuário pode selecionar diferentes animações visuais através de botões físicos.

O projeto foi um excelente exercício de colaboração, e o resultado é um software robusto e modular.

## 2. Divisão de Tarefas e Contribuições da Equipe

Como líder, estruturei o desenvolvimento de forma a aproveitar os pontos fortes de cada membro da equipe. Abaixo, detalho as responsabilidades e como integrei o trabalho de todos para chegar ao produto final.

### 2.1. Minha Contribuição como Líder do Projeto

Minha função principal foi desenhar a **arquitetura central do software** e garantir que todos os módulos se comunicassem de forma eficiente. Fiquei responsável por:

* **Configuração Inicial e Estrutura do `main`:**
    * Preparei todo o ambiente de inicialização do Pico, configurando os periféricos essenciais como o **PIO (Programmable I/O)** para a comunicação com a matriz de LEDs e os pinos **GPIO** para os botões, já com os resistores de `pull-up` ativados.
    * Desenvolvi o esqueleto da função `main`, implementando o loop `while (true)` que serve como o coração do programa.

* **Lógica Principal e Máquina de Estados:**
    * Criei a **máquina de estados** que gerencia o fluxo do programa, utilizando a variável `modo_programa`. Essa lógica determina se o sistema deve executar uma das sequências de animação ou permanecer em estado ocioso, limpando a matriz para economizar energia.
    * Realizei a **orquestração e integração** final, conectando as funções de desenho do Guilherme com os padrões visuais da Danielli dentro do loop principal, garantindo que as animações fossem executadas corretamente conforme o estado do programa.

### 2.2. Contribuição da Desenvolvedora Danielli

A Danielli ficou encarregada da **parte artística e visual** do projeto. O trabalho dela foi a base para todo o conteúdo que exibimos na matriz.

* **Criação dos Padrões de Animação:** Ela desenhou e implementou as matrizes `desenhos_seq1` e `desenhos_seq2`, que armazenam os 5 frames de cada animação. Cada valor nessas matrizes representa a intensidade de um pixel específico, o que nos deu grande flexibilidade.
* **Função de Suporte a Cores:** Ela também desenvolveu a função utilitária `cor_rgb()`, que foi crucial. Essa função nos permitiu trabalhar com cores de forma intuitiva (usando valores de 0.0 a 1.0) e convertê-las para o formato complexo de 32 bits que a matriz de LEDs WS2812B exige.

### 2.3. Contribuição do Desenvolvedor Guilherme

O Guilherme foi o responsável por fazer a **ponte entre o software e o hardware**, cuidando da manipulação direta da matriz e da interação com o usuário.

* **Funções de Manipulação da Matriz:** Ele escreveu as funções essenciais `desenhar_na_matriz()` e `limpar_matriz()`. A primeira traduz os dados dos desenhos da Danielli em comandos para o PIO, "acendendo" os pixels, enquanto a segunda permite apagar a tela de forma eficiente.
* **Gerenciamento de Interrupções e Debounce:** O Guilherme implementou a rotina de interrupção `button_irq_handler()`, que permite ao nosso sistema responder instantaneamente aos botões sem travar o loop principal. De forma muito inteligente, ele já incluiu um **mecanismo de debounce**, evitando os registros múltiplos que são um problema comum em botões físicos e garantindo uma experiência de uso limpa e sem falhas.
