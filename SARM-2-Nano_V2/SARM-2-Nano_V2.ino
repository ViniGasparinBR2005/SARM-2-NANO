//     _____         _____  __  __      ___  
//    / ____|  /\   |  __ \|  \/  |    |__ \ 
//   | (___   /  \  | |__) | \  / |______ ) |
//    \___ \ / /\ \ |  _  /| |\/| |______/ / 
//    ____) / ____ \| | \ \| |  | |     / /_ 
//   |_____/_/    \_\_|  \_\_|  |_|    |____|

// Sarm-2_V2
// Autor : Vinicius Gasparin
// Data  : 28/11/2023
// Colombo - PR

// "Ao infinito e alem!"
// Lightyear, Buzz 

/*  SARM-2_V2 eh um altimetro acionado pela variacao de altitude
  e tem como objetivo ser embarcado em minifoguetes.
  O mesmo salva dados de:
    1_Tempo de execucao  [ms];
    2_Altura             [m ];
    3_Altitude           [m ];
    4_Pressao atmosferica[Pa];
    5_Temperatura        [°C]. 
  Para o funcionamento deste codigo eh necessario:
    A_Um microcontrolador arduino (Recomendase o uso do NANO)
    B_Um sensor BMP-280
    C_Um modulo micro SD (E um cartao micro SD)
    D_Um buzzer
 */
/* # Lista de Conexoes # */
  //BMP-280
    //SCL   -   A05
    //SDA   -   A04
  // SD
    //CS    -   D04
    //MOSI  -   D11
    //CLK   -   D13
    //MISO  -   D12
  // Buzzer
    // +    -   D02*
  // '*' = Alteravel pelo codigo
/* #### Bibliotecas #### */
  #include <Wire.h>           // Inclue a biblioteca da comunicacao I²C
  #include <Adafruit_BMP280.h>// Inclue a biblioteca do sensor BMP-280
  #include <SPI.h>            // Inclue a biblioteca da comunicacao SPI
  #include <SD.h>             // Inclue a biblioteca do modulo SD
/* #### Definicoes ####  */
  #define nml    1013.25      // Nivel do mar local
  #define N      10           // Numero de leituras
  #define tax    1            // Taxa  de  variacao
  #define T      100          // Tempo das leituras [ms]
  #define buzzer 2            // Pino   do   buzzer 
/* #### Definicoes ####  */
  Adafruit_BMP280 bmp;        // Define o objeto bmp
  File myFile;                // Define o objeto myFile
  double H0;                  // Cria  a variavel H0
void error  (int erro){
  /*Esta funcao tem como objetivo informar possiveis erros no altimetro
  por meio de aviso sonoro ou mensagem plotada no monitor serial*/
  if     (erro == 1) {/*Erro do sensor BMP-280*/
    Serial.println(F("ERRO [BMP280]")); // Informa o erro no terminal
    while(1) {                          // Entra em um loop infinito
      if(bmp.begin(0x76))               // Testa novamente o sensor
        break;                          // Interrompe o loop
      tone(buzzer, 500);                // Inicia o buzzer
      delay(200);                       // Aguarda 200ms
      noTone(buzzer);                   // Desativa o buzzer
      delay(1000);}                     // Aguarda 1s
  }
  else if(erro == 2) {/* Erro  do  modulo  SD */
    Serial.println(F("ERRO [  SD  ]")); // Informa o erro no terminal
    while(1) {                          // Entra em um loop infinito
      if(SD.begin(4))                   // Testa novamente o modulo
        break;                          // Interrompe o loop
      tone(buzzer, 500);                // Toca o Buzzer a 500hz
      delay(200);                       // Aguarda 200ms
      noTone(buzzer);                   // Destiva o Buzzer
      delay(200);                       // Aguarda 200ms
      tone(buzzer, 500);                // Toca o Buzzer a 500hz
      delay(200);                       // Aguarda 200ms
      noTone(buzzer);                   // Destiva o Buzzer
      delay(1000);}                     // Aguarda 1s
  }
  else if(erro == 3) {/* Erro do arquivo .DAT */
    Serial.println(F("ERRO [ .DAT ]")); // Informa o erro no terminal
    tone(buzzer, 500);                  // Toca o Buzzer a 500hz
    delay(200);                         // Aguarda 200ms
    noTone(buzzer);                     // Destiva o Buzzer
    delay(200);                         // Aguarda 200ms
    tone(buzzer, 500);                  // Toca o Buzzer a 500hz
    delay(200);                         // Aguarda 200ms
    noTone(buzzer);                     // Destiva o Buzzer
    delay(200);                         // Aguarda 200ms
    tone(buzzer, 500);                  // Toca o Buzzer a 500hz
    delay(200);                         // Aguarda 200ms
    noTone(buzzer);                     // Destiva o Buzzer
    delay(1000);}                       // Aguarda 1s
}
void coleta () {
  /*Esta funcao tem como objetivo salvar os dados de voo*/
  double time, altu, alti, pres, temp;      // Cria as variaveis de dados
  time = millis          (   )     ;        // Guarda o tempo
  altu = bmp.readAltitude(nml) - H0;        // Guarda a altura
  alti = bmp.readAltitude(nml)     ;        // Guarda a altitude
  pres = bmp.readPressure(   )     ;        // Guarda a pressao atmosferica
  temp = bmp.readTemperature()     ;        // Guarda a temperatura
  myFile = SD.open("dados.dat", FILE_WRITE);// Abre o arquivo .DAT
  myFile.print(time);                       // Salva o tempo
  myFile.print("  ");                       // "  "
  myFile.print(altu);                       // Salva a altura
  myFile.print("  ");                       // "  "
  myFile.print(alti);                       // Salva a altitude
  myFile.print("  ");                       // "  "
  myFile.print(pres);                       // Salva a pressao atmosferica
  myFile.print("  ");                       // "  "
  myFile.print(temp);                       // Salva a temperatura
  myFile.println(  );                       // Encerra a linha
  myFile.close  (  );                       // Fecha o arquivo .DAT
}
void altura () {
  /*Esta funcao tem como objetivo definir H0 (Altitude inicial)
  para o calculo de altura durante o voo*/
  int i;                                  // Cria o indice
  float soma = 0;                         // Cria a soma e define como 0
  for(i = 0; i < N; i++) {                // Faz N leituras
    soma = soma + bmp.readAltitude(nml);  // Acrescenta a altitude em soma
    delay(T);}                            // Aguarda T
  H0 = soma / N;                          // Divide soma por N
}
bool sit_voo(double h1, double h2, float Tv) {
  /*Esta funcao tem como objetivo detectar a variacao de altitude, ela retorna:
  0 se estavel, 1 se varia*/
  double dh;                 // Variavel que quarda a variacao de altitude
  dh = h2 - h1;              // Calcula a variacao de altitude
  if (abs(dh) >= abs(Tv))  { // Se |dh| eh maior que tax
    return 1;}               // Retorna 1
  else{                      // Se |dh| eh menor que tax
    return 0;}               // Retorna 0
}
void pre_voo() {
  /*Esta funcao tem como objetivo guardar alti de antes da decolagem, e salvalas ao inicio do voo*/
  float time[N], altu[N], alti[N], pres[N], temp[N];// Cria as variaveis de dados
  float ti, t;                                      // Cria as variaveis de tempo
  int i;                                            // Cria o indice i
  for(i = 0; i < N; i++) {                          // Percorre todas as posicoes de alti[]
    alti[i] = bmp.readAltitude(nml);}               // Inicias as posicoes de alti[]
  while(sit_voo(alti[0], alti[N-1], tax) == 0) {    // Aguarda o inicio do voo
    ti = millis();                                  // Guarda o tempo inicial
    for(i = N-1; i > 0; i--) {                      // Percorre as posicoes de forma reversa
      time[i] = time[i-1];                          // Move os dados de temp[]
      altu[i] = altu[i-1];                          // Move os dados de altu[]
      alti[i] = alti[i-1];                          // Move os dados de alti[]
      pres[i] = pres[i-1];                          // Move os dados de pres[]
      temp[i] = temp[i-1];}                         // Move os dados de temp[]
    time[0] = millis          (   )     ;           // Guarda o   tempo     na posicao 0
    altu[0] = bmp.readAltitude(nml) - H0;           // Guarda a   altura    na posicao 0
    alti[0] = bmp.readAltitude(nml)     ;           // Guarda a  altitude   na posicao 0
    pres[0] = bmp.readPressure(   )     ;           // Guarda a   pressao   na posicao 0
    temp[0] = bmp.readTemperature()     ;           // Guarda a temperatura na posicao 0
    t = millis() - ti;                              // Calcula o tempo de execucao
    tone(buzzer, 900);                              // Ativa o buzzer
    delay(80);                                      // Aguarda 80ms
    noTone(buzzer);                                 // Desativa o buzzer
    delay(20);                                      // Aguarda 20ms
    delay(T - t);}                                  // Aguarda T - tempo de execucao
  for(i = N - 1; i >= 0; i--) {                     // Percorre todas as posicoes
    myFile = SD.open("dados.dat", FILE_WRITE);      // Abre o arquivo .DAT
    myFile.print  (time[i]);                        // Imprime o tempo
    myFile.print  ("  ")   ;                        // "  "
    myFile.print  (altu[i]);                        // Imprime a altura
    myFile.print  ("  ")   ;                        // "  "
    myFile.print  (alti[i]);                        // Imprime a altitude
    myFile.print  ("  ")   ;                        // "  "
    myFile.print  (pres[i]);                        // Imprime a pressao atmosferica
    myFile.print  ("  ")   ;                        // "  "
    myFile.println(temp[i]);                        // Imprime a temperatura
    myFile.close  (       );                        // Fecha o arquivo .DAT
  }
}
void dur_voo() {
  /*Esta fucao tem como objetivo coletar dados durante o voo e analizar seu termino*/
  float ti, t;                                // Cria as variaveis de tempo
  double ALTI[N];                             // Cria um vetor para a altitude
  int i, I = 0;                               // Cria os indices
  for(i = 0; i < N; i++)                      // Preenche o vetor
    ALTI[i] = bmp.readAltitude(nml);          // Define a posicao i do vetor como a altura
  while(I < (N + 20)) {                       // Aguarda N leituras seguidas sem variacao
    ti = millis();                            // Guarda o tempo inicial
    coleta();                                 // Coleta os dados
    for(i = N-1; i > 0; i--)                  // Le o vetor de forma reversa
      ALTI[i] = ALTI[i-1];                    // Passa os dados uma posicao para tras no vetor
      ALTI[0] = bmp.readAltitude(nml);        // Define a posicao 0 do vetor como a altitude
    if(sit_voo(ALTI[0], ALTI[N-1], tax) == 0) // Analiza a situacao do voo
      I++;                                    // Se estavel acressenta 1 a I
    else                                      // Se nao
      I = 0;                                  // Define I como 0
    t = millis() - ti;                        // Calcula o tempo de execucao
    delay(T - t);                             // Aguard T ms
  }
}
void pos_voo() {
  /*Esta funcao tem como objetivo soar um buzzer apos o pouso*/
  while(1) {
      tone(buzzer, 900 );               // Inicia o buzzer
      delay(250);                       // Aguarda 250ms
      tone(buzzer, 500 );               // Inicia o buzzer
      delay(250);                       // Aguarda 1s
      tone(buzzer, 1000);               // Inicia o buzzer
      delay(500);}                      // Aguarda 1s
}
void setup  () {
  /* =================== Inicializacoes =================== */
  pinMode(buzzer , OUTPUT);             // Define buzzer como OUTPUT
  Serial.begin(9600);                   // Inicia a comunicacao Serial
  if(!bmp.begin(0x76))                  // Inicia o sensor [BMP-280]
    error(1);                           // Erro 1
  if(!SD.begin(4))                      // Inicia o modulo SD
    error(2);                           // Erro 2
  /* ================== Plotagem inicial ================== */
  myFile = SD.open("dados.dat", FILE_WRITE);
  if (!myFile)
    error(3);
  myFile.println("");
  myFile.println("#==================== SARM-2-Nano_V2 ====================#");
  altura();
  myFile.print  ("#                      H0 = ");
  myFile.print  (H0     );
  myFile.println(".                      #"    );
  myFile.println("#time [ms] ; altu [m] ; alti [m] ; pres [Pa] ; temp [°C]:#");
  myFile.close  ();
  /* ================== Leituras pre_voo ================== */
  pre_voo();
  /* ================== Leituras dur_voo ================== */
  tone(buzzer, 900 );
  dur_voo();
  noTone(buzzer);
  /* ==================  Alarme pos_voo  ================== */
  pos_voo();
}