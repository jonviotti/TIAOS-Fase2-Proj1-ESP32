# 🍓 Sistema de Irrigação Inteligente para Morango (Wokwi/ESP32)

Este projeto simula um sistema de irrigação automatizado e inteligente, implementado em um ESP32 (Wokwi), focado nas necessidades específicas da cultura do **Morango** (*Fragaria × ananassa*). O objetivo é ligar a bomba d'água (LED azul) **apenas** quando o solo precisa de água e as condições de crescimento (NPK e pH) são ideais para a planta.

## ⚙️ Componentes e Pinos Utilizados

| Componente (Simulado) | Pino ESP32 (GPIO) | Função no Projeto |
| :--- | :--- | :--- |
| **Umidade** (DHT22) | **D23** | Sensor de Umidade e Temperatura. |
| **PH** (LDR) | **D35 (ADC)** | Simula o pH do solo através da leitura analógica. |
| **Bomba d'Água** (LED Azul) | **D18** | Saída para acionar a irrigação. |
| **Nitrogênio (N)** | **D22** (Botão Verde 1) | Simula a presença/ausência do nutriente N (NPK OK = Não pressionado). |
| **Fósforo (P)** | **D21** (Botão Verde 2) | Simula a presença/ausência do nutriente P (NPK OK = Não pressionado). |
| **Potássio (K)** | **D19** (Botão Verde 3) | Simula a presença/ausência do nutriente K (NPK OK = Não pressionado). |
| **Gatilho de Verificação** | **D4** (Botão Vermelho) | Inicia o ciclo de medição e decisão. |

## 📸 Diagrama do Circuito (Wokwi.com)

O diagrama a seguir demonstra as conexões físicas entre o ESP32, os sensores (DHT22 e LDR/pH), a bomba d'água (LED) e os botões NPK/Gatilho.

# 🍓 Sistema de Irrigação Inteligente para Morango (Wokwi/ESP32)

Este projeto simula um sistema de irrigação automatizado e inteligente, implementado em um ESP32 (Wokwi), focado nas necessidades específicas da cultura do **Morango** (*Fragaria × ananassa*). O objetivo é ligar a bomba d'água (LED azul) **apenas** quando o solo precisa de água e as condições de crescimento (NPK e pH) são ideais para a planta.

## ⚙️ Componentes e Pinos Utilizados

| Componente (Simulado) | Pino ESP32 (GPIO) | Função no Projeto |
| :--- | :--- | :--- |
| **Umidade** (DHT22) | **D23** | Sensor de Umidade e Temperatura. |
| **PH** (LDR) | **D35 (ADC)** | Simula o pH do solo através da leitura analógica. |
| **Bomba d'Água** (LED Azul) | **D18** | Saída para acionar a irrigação. |
| **Nitrogênio (N)** | **D22** (Botão Verde 1) | Simula a presença/ausência do nutriente N (NPK OK = Não pressionado). |
| **Fósforo (P)** | **D21** (Botão Verde 2) | Simula a presença/ausência do nutriente P (NPK OK = Não pressionado). |
| **Potássio (K)** | **D19** (Botão Verde 3) | Simula a presença/ausência do nutriente K (NPK OK = Não pressionado). |
| **Gatilho de Verificação** | **D4** (Botão Vermelho) | Inicia o ciclo de medição e decisão. |

## 📸 Diagrama do Circuito (Wokwi.com)

O diagrama a seguir demonstra as conexões físicas entre o ESP32, os sensores (DHT22 e LDR/pH), a bomba d'água (LED) e os botões NPK/Gatilho.

![Diagrama de circuito do projeto de irrigação no Wokwi.com, mostrando as conexões do ESP32 com DHT22, LDR, LED azul e quatro botões.](assets/image_30894b.png)

## 🌱 Especificações da Cultura: Morango

O Morango é uma cultura sensível que requer solo **ácido** e umidade constante, mas sem excesso.

| Parâmetro | Requisito da Cultura | Faixa de Decisão no Código V8 |
| :--- | :--- | :--- |
| **Umidade do Solo** | Manter sempre úmido. | **< 40%** (Precisa Regar) |
| **PH do Solo** | Solo **ácido** (ideal 5.5 a 6.5). | **1024 a 2048** (Escala analógica de 0-4095) |
| **NPK** | Alto requisito de P e K. | **Todos OK** (Nenhum botão de N, P ou K pressionado) |

## 🧠 Lógica de Decisão (C/C++)

A lógica implementada garante que a bomba só seja acionada em condições de **necessidade** (água baixa) **E** **qualidade** (pH e NPK adequados). A rega é um evento de 3 segundos acionado pelo botão D4 (Gatilho).

```cpp
// A bomba SÓ liga se TODAS as condições forem TRUE.

bool precisa_regar = (umidade < UMIDADE_MINIMA); 
bool ph_aceitavel = (leitura_ph >= 1024 && leitura_ph <= 2048); 
bool npk_adequado = (N_OK && P_OK && K_OK); 

if (precisa_regar) {
    if (npk_adequado && ph_aceitavel) {
        // CONDIÇÃO PARA LIGAR A BOMBA: Água baixa E Qualidade OK.
        digitalWrite(PINO_BOMBA, HIGH); 
        // ... rega por 3 segundos
    } else {
        // CONDIÇÃO PARA NÃO REGAR: Umidade baixa, mas NPK ou PH Falhando.
        digitalWrite(PINO_BOMBA, LOW); 
    }
} else {
    // Umidade OK ou Alta.
    digitalWrite(PINO_BOMBA, LOW);
}

## 🌱 Especificações da Cultura: Morango

O Morango é uma cultura sensível que requer solo **ácido** e umidade constante, mas sem excesso.

| Parâmetro | Requisito da Cultura | Faixa de Decisão no Código V8 |
| :--- | :--- | :--- |
| **Umidade do Solo** | Manter sempre úmido. | **< 40%** (Precisa Regar) |
| **PH do Solo** | Solo **ácido** (ideal 5.5 a 6.5). | **1024 a 2048** (Escala analógica de 0-4095) |
| **NPK** | Alto requisito de P e K. | **Todos OK** (Nenhum botão de N, P ou K pressionado) |

## 🧠 Lógica de Decisão (C/C++)

A lógica implementada garante que a bomba só seja acionada em condições de **necessidade** (água baixa) **E** **qualidade** (pH e NPK adequados). A rega é um evento de 3 segundos acionado pelo botão D4 (Gatilho).

```cpp
// A bomba SÓ liga se TODAS as condições forem TRUE.

bool precisa_regar = (umidade < UMIDADE_MINIMA); 
bool ph_aceitavel = (leitura_ph >= 1024 && leitura_ph <= 2048); 
bool npk_adequado = (N_OK && P_OK && K_OK); 

if (precisa_regar) {
    if (npk_adequado && ph_aceitavel) {
        // CONDIÇÃO PARA LIGAR A BOMBA: Água baixa E Qualidade OK.
        digitalWrite(PINO_BOMBA, HIGH); 
        // ... rega por 3 segundos
    } else {
        // CONDIÇÃO PARA NÃO REGAR: Umidade baixa, mas NPK ou PH Falhando.
        digitalWrite(PINO_BOMBA, LOW); 
    }
} else {
    // Umidade OK ou Alta.
    digitalWrite(PINO_BOMBA, LOW);
}