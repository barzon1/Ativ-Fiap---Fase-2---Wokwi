# Sistema de Irrigação Inteligente - FarmTech Solutions (Fase 2)

## 1. Introdução e Objetivo

Este projeto, desenvolvido para a FarmTech Solutions na Fase 2 do desafio, tem como objetivo construir um sistema de irrigação que está utilizando o simulador ESP32 na plataforma Wokwi.com. O foco do sistema é a otimização de recursos hídricos na lavoura de feijão, garantindo que a irrigação seja acionada apenas em condições específicas de umidade, pH e níveis de nutrientes.

## 2. Mapeamento de Sensores e Atuadores

Devido às limitações de sensores agrícolas no Wokwi, foram utilizadas substituições didáticas. O circuito foi montado no ESP32 conforme a seguinte configuração:

| Componente Real (Agro) | Componente Simulado (Wokwi) | Pino (ESP32) | Tipo de Leitura |
| :--- | :--- | :--- | :--- |
| Nível de Nitrogênio (N) | Botão Verde | PIN 15 | Digital (INPUT_PULLUP) |
| Nível de Fósforo (P) | Botão Verde | PIN 14 | Digital (INPUT_PULLUP) |
| Nível de Potássio (K) | Botão Verde | PIN 13 | Digital (INPUT_PULLUP) |
| Sensor de pH (da Terra) | Sensor LDR (Resistor Dependente de Luz) | PIN 36 | Analógica (Simula pH) |
| Sensor de Umidade do Solo | Sensor DHT22 (Umidade do Ar) | PIN 26 | Digital (Leitura de Umidade em %) |
| Bomba D'água | Relé Azul | PIN 25 | Saída Digital (HIGH/LOW) |

Imagem do ESP32:
(https://github.com/barzon1/Ativ-Fiap---Fase-2---Wokwi/blob/main/imagem-wokwi.jpg)

## 3. Lógica de Decisão para a Cultura do Feijão

A cultura do Feijão (variedade escolhida para esta simulação) possui requisitos específicos de solo que determinam a necessidade de irrigação. Os parâmetros e limiares definidos no código C/C++ são:

| Parâmetro | Faixa de Referência | Limiares no Código |
| **Umidade do Solo** | Nível Crítico | `UMIDADE_MINIMA = 60` (Irrigar se $\leq 60\%$) |
| **pH (LDR)** | Ideal/Neutro | $\text{LDR} \geq 400$ e $\text{LDR} \leq 700$ (Valor analógico) |
| **Nutrientes (N, P, K)** | Nível Baixo | Botão **NÃO** Pressionado (Leitura $HIGH$) |

---

### Regra Condicional para Irrigação (Relé LIGA):

A Bomba D'água (Relé Azul) **LIGA** se a umidade do solo estiver **ABAIXO DE 60%** E, ao mesmo tempo, houver alguma desregulação no solo.
O relé **DESLIGA** imediatamente caso a umidade retorne para $\text{Umidade} > 60\%$ (indicando que a água foi reposta) ou se todas as condições de pH e nutrientes forem corrigidas.

---

## 4. Funcionamento do Código

O código utiliza o recurso `INPUT_PULLUP` para simplificar a leitura dos botões (N, P, K). A leitura padrão é $HIGH$ (nível **BAIXO** de nutriente) e se torna $LOW$ quando pressionado (nível **ALTO** de nutriente). A leitura analógica do LDR (pH) e a leitura digital do DHT22 (Umidade) são os dados principais para a lógica de decisão, implementada na função `loop()`.

## 5. Vídeo de Demonstração
[Link para o Vídeo de Demonstração no YouTube]
