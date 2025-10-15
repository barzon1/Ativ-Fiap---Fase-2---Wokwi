#include <DHT.h> // Inclui a biblioteca para o DHT22

// --- Definição dos Pinos ---
// Entradas (Sensores/Simulações)
const int PIN_N = 13;   // Nitrogênio (N) - Botão
const int PIN_P = 14;   // Fósforo (P) - Botão
const int PIN_K = 27;   // Potássio (K) - Botão
const int PIN_LDR = 36; // pH - LDR (A0 no Wokwi é D36 no ESP32)
const int PIN_DHT = 26; // Umidade do Solo - DHT22

// Saída (Atuador)
const int PIN_RELE = 25; // Bomba d'água - Relé (LOW liga, HIGH desliga)

// --- Configuração do DHT ---
#define DHTTYPE DHT22 
DHT dht(PIN_DHT, DHTTYPE);

const int UMIDADE_MINIMA = 60; // Irrigar se a Umidade (%) estiver ABAIXO deste valor.

// Mapeamento do pH ideal do feijão (6.0 - 6.8) para o valor analógico do LDR 
const int PH_MINIMO_IDEAL = 400; // Valor Analógico que simula um pH levemente ácido (aprox. 5.5).
const int PH_MAXIMO_IDEAL = 700; // Valor Analógico que simula um pH neutro/levemente básico (aprox. 9.5).

// --- Setup: Configuração Inicial ---
void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(PIN_N, INPUT_PULLUP);
  pinMode(PIN_P, INPUT_PULLUP);
  pinMode(PIN_K, INPUT_PULLUP);

  // Configura o relé como SAÍDA e o mantém DESLIGADO inicialmente.
  pinMode(PIN_RELE, OUTPUT);
  digitalWrite(PIN_RELE, HIGH);

  Serial.println("Sistema de Irrigacao Inteligente FarmTech Solutions (Feijao) - INICIADO!");
}

// --- Loop: Onde a Mágica Acontece ---
void loop() {
  // 1. Leitura dos Sensores
  // A leitura do botão deve ser invertida (!) pois usamos PULLUP.
  bool nivel_N = !digitalRead(PIN_N); 
  bool nivel_P = !digitalRead(PIN_P); 
  bool nivel_K = !digitalRead(PIN_K); 

  int ph_analog = analogRead(PIN_LDR); 

  float umidade = dht.readHumidity(); 

  // Tratamento de Erro de Leitura do DHT
  if (isnan(umidade)) {
    Serial.println("! ERRO: Falha na leitura do sensor DHT22. Tentando novamente...");
    delay(2000);
    return; 
  }

  // 2. Lógica de Decisão para Feijão
  bool ligar_irrigacao = false;

  // Condição 1: A umidade está baixa? 
  bool umidade_baixa = (umidade < UMIDADE_MINIMA);

  // Condição 2: O pH está na faixa ideal (6.0 a 6.8) para o feijão?
  bool ph_na_faixa = (ph_analog >= PH_MINIMO_IDEAL && ph_analog <= PH_MAXIMO_IDEAL);

  // Condição 3: Nutrientes N e P estão disponíveis? 
  bool nutrientes_ok = (nivel_N && nivel_P);

  // DECISÃO FINAL:
  if (umidade_baixa && ph_na_faixa && nutrientes_ok) {
    ligar_irrigacao = true;
  }

  // 3. Acionamento do Relé
  if (ligar_irrigacao) {
    digitalWrite(PIN_RELE, LOW); // LOW = Relé LIGADO 
    Serial.println("\n*** IRRIGAÇÃO LIGADA! ***");
    Serial.println("Motivo: Umidade Baixa, pH e N/P na faixa ideal.");
  } else {
    digitalWrite(PIN_RELE, HIGH); // HIGH = Relé DESLIGADO 

   
    if (!umidade_baixa) {
      Serial.println("IRRIGAÇÃO DESLIGADA: Umidade do Solo OK (Acima de 60%).");
    } else if (umidade_baixa && !ph_na_faixa) {
      Serial.println("IRRIGAÇÃO DESLIGADA: Umidade Baixa, mas pH FORA da Faixa Ideal (Risco de baixa produtividade).");
    } else if (umidade_baixa && !nutrientes_ok) {
      Serial.println("IRRIGAÇÃO DESLIGADA: Umidade Baixa, mas N ou P não aplicados/disponíveis (Risco de deficiência).");
    } else {
      Serial.println("IRRIGAÇÃO DESLIGADA: Condições gerais não atendidas.");
    }
  }

  // 4. Exibição de Dados 
  Serial.println("\n--- Status Atual dos Sensores (Cultura: Feijão) ---");
  Serial.print("N (Nitrogenio): "); Serial.println(nivel_N ? "ALTO (Aplicado)" : "BAIXO");
  Serial.print("P (Fosforo): "); Serial.println(nivel_P ? "ALTO (Aplicado)" : "BAIXO");
  Serial.print("K (Potassio): "); Serial.println(nivel_K ? "ALTO (Aplicado)" : "BAIXO"); 
  Serial.print("Umidade do Solo (DHT%): "); Serial.print(umidade); Serial.print(" % (Minimo: "); Serial.print(UMIDADE_MINIMA); Serial.println("%)");
  Serial.print("pH (LDR Valor): "); Serial.print(ph_analog); Serial.print(" (Ideal: "); Serial.print(PH_MINIMO_IDEAL); Serial.print(" - "); Serial.print(PH_MAXIMO_IDEAL); Serial.println(")");
  Serial.println("-------------------------------------------------");

  delay(5000); 
}
