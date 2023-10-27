/*
    Projeto: Controle do robo com radio futaba
    
    Autor: Eder de Rosso  
    Data da criação: 31/05/17
    Data da 1º revisão: 29/06/17
    Data da 2º revisão: 09/11/17 
    Data da 3º revisão: 19/03/18-> Implementação da função millis na oscilação do led_sig
*/
/****************************************************************************************************************/

//--- Mapeamento de Hardware ---

#define    c_1          2                    //Canal 1 do rádio controle
#define    c_2          3                    //Canal 2 do rádio controle
#define    c_3          4                    //Canal 3 do rádio controle
#define    c_4          5                    //Canal 4 do rádio controle
#define    inv3         6                    //Controle de sentido do motor braços
#define    inv4         7                    //Controle de sentido do motor cabeça
#define    pwm3         8                    //Controle pwm3
#define    pwm4         9                    //Controle pwm4
#define    pwm1         10                   //Controle pwm1
#define    pwm2         11                   //Controle pwm2
#define    led_sig      13                   //LED de indicação de perda de sinal do rádio
#define    inv1         14                   //Controle de sentido do motor1 (A0)
#define    inv2         15                   //Controle de sentido do motor2 (A1)
#define    conf_blue    12                   //confirmação do bluetooth

/****************************************************************************************************************/
// --- Protótipo das funções auxiliares ---


void atuatores();                            //Função de controle dos atuadores do Robô 
void atuadores_bluetooth();                  //Função de controle dos atuadores do robô com APP bluetooth
void sinal_perdido();                        //Função para tratar perda do sinal de rádio
void leitura_canal();                        //Função para leitura das entradas dos canais
void teste_canal();                          //Testa os 4 canais do Turnigy9x (Serial Monitor)
void re();
void direita();
void esquerda();
void frente();
void braco_cima();
void braco_baixo();
void cabeca_direita();
void cabeca_esquerda();
void parado_b_c();

/****************************************************************************************************************/
// --- Variáveis Globais ---

short          change_ch = 0x00;                          //determina qual canal será lido
        
int            canal_01,                                  //armazena a largura do pulso do canal 1
               canal_02,                                  //armazena a largura do pulso do canal 2
               canal_03,                                  //armazena a largura do pulso do canal 3
               canal_04,                                  //armazena a largura do pulso do canal 4
               pwm_valor,                                 //armazena valor de pwm para os motores
               pwm_frente,
               pwm_re,
               pwm_esquerda,                              //armazena valor de pwm da esquerda
               pwm_direita,                               //armazena valor de pwm da direita
               pwm_cab,
               pwm_braco; 
unsigned long  tempo_perdido = 0;
const long     led_intervalo = 300;
int            estado_led = LOW;
char           estado;


/****************************************************************************************************************/
// --- Configurações iniciais ---

void setup()
{ 

    // -- Definição de I/Os --
    pinMode(c_1,  INPUT);                    //Entrada para o canal 1 do rádio
    pinMode(c_2,  INPUT);                    //Entrada para o canal 2 do rádio
    pinMode(c_3,  INPUT);                    //Entrada para o canal 3 do rádio
    pinMode(c_4,  INPUT);                    //Entrada para o canal 4 do rádio

    
    pinMode(pwm1,     OUTPUT);               //saída pwm1 das rodas
    pinMode(pwm2,     OUTPUT);               //saída pwm2 das rodas
    pinMode(pwm3,     OUTPUT);               //saída pwm1 das rodas
    pinMode(pwm4,     OUTPUT);               //saída pwm2 das rodas    
    pinMode(inv1,     OUTPUT);               //saida que inverte o motor das rodas
    pinMode(inv2,     OUTPUT);               //saida que inverte o motor das rodas
    pinMode(inv3,     OUTPUT);               //saida que inverte o motor dos braços
    pinMode(inv4,     OUTPUT);               //saida que inverte o motor da cabeça 
    pinMode(led_sig,  OUTPUT);
    
    
    
     // -- Inicialização de I/Os --
    digitalWrite(pwm1,    LOW);              //inicializa pwm1 em LOW
    digitalWrite(pwm2,    LOW);              //inicializa pwm2 em LOW
    digitalWrite(pwm3,    LOW);              //inicializa pwm3 em LOW
    digitalWrite(pwm4,    LOW);              //inicializa pwm4 em LOW
    digitalWrite(inv1,    LOW);              //inicializa inv1 em LOW
    digitalWrite(inv2,    LOW);              //inicializa inv2 em LOW
    digitalWrite(inv3,    LOW);              //inicializa inv3 em LOW
    digitalWrite(inv4,    LOW);              //inicializa inv4 em LOW
    digitalWrite(led_sig, LOW);              //inicializa led_sig em LOW (desligado)
    
     // -- End I/Os --

  
    Serial.begin(9600);                      //Inicia comunicação Serial com 9600 de baud rate
    
    sinal_perdido();  
   
}

/****************************************************************************************************************/
//--- Loop Infinito ---

void loop()
{
  
  leitura_canal();
  sinal_perdido();
  atuadores();


//estado = Serial.read();
//atuadores_bluetooth();
     
}


/****************************************************************************************************************/

// --- Desenvolvimento das Funções ---

/****************************************************************************************************************/
// --- Função de Controle dos atuadores do robo com APP bluetooth

void atuadores_bluetooth()
{
    
    if (estado == 'a')
    {
        frente();
    }
    if (estado == 's') 
    {    
       parado();
    }
    if (estado == 'f')
    {
        re();
    }
     if (estado == 's') 
    {    
       parado();
    }
    
    if (estado == 'd')
    {
        direita();
    }
     if (estado == 's') 
    {    
       parado();
    }
     if (estado == 'b')
    {
        esquerda();
    }
     if (estado == 's') 
    {    
       parado();
    }
}


/****************************************************************************************************************/
// --- Função Controle dos Atuadores do Robô  ---

/****************************************************************************************************************/
// --- cabeça e braços  ---
void atuadores()                                        //Função que da movimento aos atuadores do Robo
{

   if (canal_04 > 1200 && canal_04 < 1800)
    {
        if (canal_03 < 1400 )
        {
          braco_cima();
        }
        else if (canal_03 > 1700)
        {
          braco_baixo();
        }
        else parado_b_c();
    }

    else if (canal_03 > 1200 && canal_03 < 1800)
    {
        if (canal_04 >1700)
        {
          cabeca_direita();
        }          
        else if (canal_04 < 1300)
        {
          cabeca_esquerda();
        }
        else parado_b_c();
    }


// --- Deslocamento  ---  
      
    if (canal_01 > 1500 && canal_01 <1600)              //Avalia se o stick da direita esta no centro
    {                                                   //Sim...
        if (canal_02 < 1400)  
        frente();                                      //Avalia se canal_02 é menor que 1400
                                                        //Sim...
                                                        //Função que determina o movimento para frente
        else if  (canal_02 > 1600)                       //Se não... e canal_02 maior que 1600
        {                                               //Sim...
             re();                                      //Função que determina o movimento para traz
        }                                               
        else parado();                                  //se não... Função que mantem o robo parado
        
    }
    else if (canal_02 > 1400 && canal_02 < 1600)        //Avalia se o stick da direita está no centro
    {                                                   //sim...
         if (canal_01 < 1500)
         esquerda();                                    //avalia se o stick esta para esquerda 

         
         else if (canal_01 > 1600)                      //Se não... e canal_01 maior que 1600
         {                                              //Sim...
              direita(); 
                                                        //Função que determina o movimento para direita no seu próprio eixo
         }
         else parado();                                 //Se não... Função que mantem o robo parado
    }
  

}

/****************************************************************************************************************/
// --- Função frente ---

void frente()
{
    pwm_frente = map(canal_02, 1400, 1100, 60, 230);
    digitalWrite(inv1, LOW);
    digitalWrite(inv2, LOW);
    digitalWrite(pwm1, HIGH);
    digitalWrite(pwm2, HIGH);
    pwm_re = LOW;
}

/****************************************************************************************************************/
// --- Função re ---

void re()
{
    pwm_re = map(canal_02,1600, 2000, 60, 245);
    
    digitalWrite(inv1, HIGH);
    digitalWrite(inv2, HIGH);
    digitalWrite(pwm1, HIGH);
    digitalWrite(pwm2, HIGH);
    pwm_frente = LOW; 
  }

/****************************************************************************************************************/
// --- Função direita ---

void direita()
{
    pwm_direita = map(canal_01, 1600, 2000, 50, 245);
    
    digitalWrite(inv1, HIGH);
    digitalWrite(inv2, LOW);
    digitalWrite(pwm1, HIGH);
    digitalWrite(pwm2, HIGH);
    pwm_esquerda = LOW;
}

/****************************************************************************************************************/
// --- Função esquerda ---

void esquerda()
{
    pwm_esquerda = map(canal_01, 1400, 1120, 80, 245);
    
    digitalWrite(inv1, LOW);
    digitalWrite(inv2, HIGH);
    digitalWrite(pwm1, HIGH);
    digitalWrite(pwm2, HIGH);
    pwm_direita = LOW;
}

/****************************************************************************************************************/
// --- Função de movimento dos braços ---

void braco_cima()
{
    pwm_braco = map(canal_03, 1600, 2000, 50, 245);
    digitalWrite(inv3, LOW);
    digitalWrite(pwm3, HIGH);
}

/****************************************************************************************************************/
// --- Função de movimento dos braços ---

void braco_baixo()
{
    pwm_braco = map(canal_03, 1600, 2000, 50, 245);
    digitalWrite(inv3, HIGH);
    digitalWrite(pwm3, HIGH);
}


/****************************************************************************************************************/
// --- Função de movimento da cabeça ---

void cabeca_direita()
{
    pwm_cab = map(canal_04, 1600, 2000, 50, 245);
    digitalWrite(inv4, LOW);
    digitalWrite(pwm4, HIGH);
}

/****************************************************************************************************************/
// --- Função de movimento da cabeça ---

void cabeca_esquerda()
{
    pwm_cab = map(canal_04, 1600, 2000, 50, 245);
    digitalWrite(inv4, HIGH);
    digitalWrite(pwm4, HIGH);
}


/****************************************************************************************************************/
// --- Função que mantem o robo parado ---

void parado()
{
    
    digitalWrite(pwm1,  LOW);                         //desliga pwm1
    digitalWrite(pwm2,  LOW);                         //desliga pwm2
    digitalWrite(inv1,  LOW);
    digitalWrite(inv2,  LOW);
     
}

/****************************************************************************************************************/
// --- Função que mantem a cabeça e os braços do robo parado ---

void parado_b_c()
{
    
    digitalWrite(inv3, LOW);
    digitalWrite(inv4, LOW);
    digitalWrite(pwm3, LOW);
    digitalWrite(pwm4, LOW);
    
}

/****************************************************************************************************************/
// --- Função para tratar perda do sinal de rádio ---

void sinal_perdido()                                          //trata perda do sinal de rádio
{
    digitalWrite(led_sig, LOW);                               // Desliga o led sempre a caso o sinal volte
    while(canal_02 < 300)                                     //loop enquanto canal_02 não for detectado
    {
      parado();
      parado_b_c();
      canal_01 = 0;
      canal_02 = 0;
      canal_03 = 0;
      canal_04 = 0;
      unsigned long tempo_atual = millis();                   // tempo_atual assume o valor de millis
      if (tempo_atual - tempo_perdido >= led_intervalo)       // Se tempo_atual menos tempo_perdido for maior ou igual ao de led_intervalo
      {
        estado_led = !estado_led;                             // Faz estado_led assumir o estado inverso
        tempo_perdido = tempo_atual;                          // Iguala tempo_perdido ao tempo_atual
        digitalWrite(led_sig, estado_led);                    // Escreve no led_sig o estado_led
      }

      leitura_canal();          
    
    }
      
}

/****************************************************************************************************************/
// --- Função para Leitura dos Canais do Rádio ---

void leitura_canal()                                      //Faz a leitura dos 8 canais do rádio
{

      canal_01 = pulseIn(c_1, HIGH, 50000);               //Lê pulso em nível alto do canal 1 e armazena em canal_01
      canal_02 = pulseIn(c_2, HIGH, 50000);               //Lê pulso em nível alto do canal 2 e armazena em canal_02
      canal_03 = pulseIn(c_3, HIGH, 50000);               //Lê pulso em nível alto do canal 3 e armazena em canal_03
      canal_04 = pulseIn(c_4, HIGH, 50000);               //Lê pulso em nível alto do canal 4 e armazena em canal_04
     
      

}

/****************************************************************************************************************/
//--- Testa os canais e imprime no serial monitor ---

void teste_canal()                                         
{                                                         //(comentar esta função e só chamar quando necessário)

      Serial.println("=================");                //imprime
      Serial.print("= Canal 01:  ");                      //imprime Canal 01
      Serial.println(canal_01);                           //imprime valor do pulso do canal 1 em us
      Serial.print("= Canal 02:  ");                      //imprime Canal 02
      Serial.println(canal_02);                           //imprime valor do pulso do canal 2 em us
      Serial.print("= Canal 03:  ");                      //imprime Canal 03
      Serial.println(canal_03);                           //imprime valor do pulso do canal 3 em us
      Serial.print("= Canal 04:  ");                      //imprime Canal 04
      Serial.println(canal_04);                           //imprime valor do pulso do canal 4 em us
      Serial.println("=================");                //imprime
}
/****************************************************************************************************************/
