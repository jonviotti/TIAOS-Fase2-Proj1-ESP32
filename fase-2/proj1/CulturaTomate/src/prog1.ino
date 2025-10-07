// =========================================================
// CÓDIGO V8: LÓGICA COMPLETA E PH LIDO VIA MONITOR SERIAL
// - Permite enviar o valor do pH (0 a 4095) pelo terminal para teste.
// =========================================================
#include <DHT.h>

// =========================================================
// VARIÁVEIS GLOBAIS
// =========================================================
float last_umidade = 50.0;
int leitura_ph = 2000; // Valor inicial seguro para o PH

// =========================================================
// DEFINIÇÕES DE PINOS
// =========================================================
// PINOS DE NPK (Condição OK = HIGH/Nao pressionado)
const int PINO_N = 22; 
const int PINO_P = 21; 
const int PINO_K = 19; 
const int PINO_NPK_OK_GATILHO = 4; // Usado APENAS para iniciar a checagem

const int PINO_PH = 35; // Mantido, mas nao usado para leitura no V8
#define DHTTYPE DHT22
const int PINO_UMIDADE = 23; 
const int PINO_BOMBA = 18; // LED Azul

DHT dht(PINO_UMIDADE, DHTTYPE);

// =========================================================
// CONSTANTES DE DECISÃO
// =========================================================
const int UMIDADE_MINIMA = 35; 
const int UMIDADE_MAXIMA = 70; 
const int VALOR_IDEAL_PH = 2048;
const int MARGEM_PH = 500;       // Margem de erro, totalizando 1548 a 2548

// =========================================================
// FUNÇÃO SETUP
// =========================================================
void setup() {
  Serial.begin(115200);
  dht.begin();
  
  // Configuração dos Pinos de Entrada (Input Pullup para botões)
  pinMode(PINO_NPK_OK_GATILHO, INPUT_PULLUP);
  pinMode(PINO_N, INPUT_PULLUP);
  pinMode(PINO_P, INPUT_PULLUP);
  pinMode(PINO_K, INPUT_PULLUP);
  
  // Configuração do Pino de Saída (Bomba)
  pinMode(PINO_BOMBA, OUTPUT);
  digitalWrite(PINO_BOMBA, LOW); 
  
  Serial.println("--- SISTEMA DE IRRIGAÇÃO INICIALIZADO (V8 - PH VIA SERIAL) ---");
  Serial.print("Faixa PH Aceitavel: "); 
  Serial.print(VALOR_IDEAL_PH - MARGEM_PH); 
  Serial.print(" a "); 
  Serial.println(VALOR_IDEAL_PH + MARGEM_PH);
  Serial.println("Envie um numero (0-4095) pelo terminal para definir o valor de PH.");
}

// =========================================================
// FUNÇÃO LOOP
// =========================================================
void loop() {
  
  // LEITURA DO PH VIA SERIAL: Se dados forem enviados, atualiza o PH.
  if (Serial.available() > 0) {
    int valor_lido = Serial.parseInt();
    if (valor_lido >= 0 && valor_lido <= 4095) {
      leitura_ph = valor_lido;
      Serial.print("\n[INFO] PH atualizado via Serial para: ");
      Serial.println(leitura_ph);
    }
    // Limpa o buffer serial para nao ler o mesmo dado repetidamente
    while (Serial.available() > 0) { Serial.read(); }
  }

  
  // O LOOP INTEIRO SÓ É EXECUTADO QUANDO O BOTÃO GATILHO É PRESSIONADO
  if (digitalRead(PINO_NPK_OK_GATILHO) == LOW) {
      
      delay(50); // Debounce
      if (digitalRead(PINO_NPK_OK_GATILHO) == HIGH) return; // Se soltou, sai

      Serial.println("\n--- INICIANDO VERIFICACAO ---");
      
      // 1. LEITURA E PROCESSAMENTO DOS DADOS
      // ---------------------------------------------------------
      
      // Leitura de NPK: OK se N, P e K NAO estiverem pressionados (HIGH)
      bool npk_adequado = (digitalRead(PINO_N) == HIGH && 
                           digitalRead(PINO_P) == HIGH && 
                           digitalRead(PINO_K) == HIGH);
      
      // Leitura de Umidade (Com tratamento de falha NaN)
      float current_umidade = dht.readHumidity(); 
      float umidade; 
      if (isnan(current_umidade)) {
          umidade = last_umidade; 
          Serial.println("!! Falha na leitura do DHT (NaN). Usando o valor anterior."); 
      } else {
          umidade = current_umidade;
          last_umidade = current_umidade; // Atualiza o valor de segurança
      }
      
      // A leitura_ph (PH) ja foi atualizada no inicio do loop via Serial

      
      // 2. AVALIAÇÃO DAS REGRAS
      // ---------------------------------------------------------
      bool precisa_regar = (umidade < UMIDADE_MINIMA);
      bool ph_aceitavel = (leitura_ph >= (VALOR_IDEAL_PH - MARGEM_PH) && 
                           leitura_ph <= (VALOR_IDEAL_PH + MARGEM_PH));

      // 3. LÓGICA DE DECISÃO FINAL
      // ---------------------------------------------------------

      digitalWrite(PINO_BOMBA, LOW); // Assume que a bomba está desligada por padrão

      if (precisa_regar) {
          if (npk_adequado && ph_aceitavel) {
              digitalWrite(PINO_BOMBA, HIGH); 
              Serial.println(">> REGANDO: Umidade Baixa, NPK e PH OK. <<");
              delay(3000); // Rega por 3 segundos
              digitalWrite(PINO_BOMBA, LOW);
              Serial.println(">> FIM DA REGAGEM.");
          } else {
              Serial.println(">> ALERTA: Umidade Baixa, MAS FALHA em NPK ou PH. Nao regando. <<");
          }
      } 
      else if (umidade > UMIDADE_MAXIMA) {
          Serial.println(">> IRRIGAÇÃO SUSPENSA: Umidade Alta. <<");
      }
      else {
          Serial.println(">> ESPERANDO. Umidade OK, Bomba Desligada. <<");
      }

      // 4. IMPRESSÃO DE STATUS DETALHADA
      // ---------------------------------------------------------
      Serial.println("-- DIAGNÓSTICO --");
      Serial.print("1. Umidade ("); Serial.print(umidade, 1); Serial.print("%) < "); Serial.print(UMIDADE_MINIMA); Serial.print("%: "); Serial.println(precisa_regar ? "OK" : "FALHA");
      Serial.print("2. NPK Adequado (N/P/K): "); Serial.println(npk_adequado ? "OK" : "FALHA");
      Serial.print("3. PH ("); Serial.print(leitura_ph); Serial.print(") 1548-2548: "); Serial.println(ph_aceitavel ? "OK" : "FALHA");
      Serial.println("----------------------------------------");

      // Espera o botão ser solto antes de continuar
      while (digitalRead(PINO_NPK_OK_GATILHO) == LOW) { delay(100); }
  }
}