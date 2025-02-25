# LED Matrix Control com Raspberry Pi Pico

## Descrição do Projeto

Este projeto utiliza o Raspberry Pi Pico para controlar uma matriz de LEDs que desenha padrões ou formas gráficas. Os padrões são ativados através de comandos enviados por um teclado matricial. Este projeto demonstra como integrar hardware e software para criar uma experiência interativa com LEDs e microcontroladores.

## Funcionalidades

- Controle de uma matriz de LEDs para criar desenhos e padrões.
- Integração com um teclado matricial para selecionar diferentes desenhos ou padrões.
- Possibilidade de personalizar os padrões de luzes via código.
- Interface responsiva e de fácil uso para interação.

## Hardware Utilizado

- **Raspberry Pi Pico**: Microcontrolador responsável pelo processamento dos sinais e controle da matriz de LEDs.
- **Matriz de LEDs**: Responsável por exibir os desenhos e padrões.
- **Teclado Matricial (4x4)**: Utilizado para selecionar os desenhos.


## Software Utilizado

- **Wokwi**: Simulador utilizado para desenvolver e testar o projeto antes da implementação em hardware real.

## Configuração e Instalação

1. **Configuração do Ambiente**:
   - Certifique-se de que o Raspberry Pi Pico esteja configurado para rodar no VSCODE.
   

3. **Carregar o Código**:
   - Clone este repositório:
     ```bash
     git clone https://github.com/HaniOliveira/Microcontroladores-GPIO-Parte-2.git
     ```
   
   - Conecte o Raspberry Pi Pico ao computador e envie o código.

4. **Executar o Projeto**:
   - Energize o circuito.
   - Use o teclado matricial para interagir e visualizar os padrões na matriz de LEDs.

## Exemplo de Uso

- Pressione as teclas '1', '2','3','4','7', '9' '*'para os desenhos
-Pressione a tecla  'A' para apagar todos os leds
-Pressione a tecla 'B' para ligar todos os leds em azul
-Pressione a tecla 'D' para ligar todos os leds em verde
-Pressione a tecla '#' para ligar todos os leds em branco
-Pressione a tecla 'C' para ligar todos os leds em vermelho
- Outros padrões podem ser configurados diretamente no código.

## Estrutura do Repositório

```plaintext
/
|-- matriz_led.c         # Arquivo principal do projeto.
|-- README.md        # Documentação do projeto.
|-- schematics/      # Esquemas elétricos do circuito.
|-- patterns/        # Padrões de LEDs em formato de código.
```

## Licença

link do vídeo do projeto https://www.canva.com/design/DAGdVi8wF8A/hZvalySC6r90VJV9hBtqVA/watch?utm_content=DAGdVi8wF8A&utm_campaign=designshare&utm_medium=link2&utm_source=uniquelinks&utlId=he4e0ad8e11


