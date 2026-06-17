
// // Definir el pin digital que queremos controlar
// #define LamparaVerde 12 // Puedes cambiar este pin a otro si es necesario
// #define A 4// Soplador
// #define B 5 // Piston
// #define C 2  //ventosas

// #define StartButton 21

// #define PE 18
// Definir el pin digital que queremos controlar
#define LamparaVerde 12// Puedes cambiar este pin a otro si es necesario
#define A 27// Soplador
#define B 33 // Piston
#define C 32  //ventosas

#define StartButton 5

#define PE 18


bool Arranque = false;
bool Arranque2 = false;
bool ParoEmergencia = false;

bool confirmacion1 = false;  // Confirmación para el estado 0
bool confirmacion2 = false;  // Confirmación para el estado 2
bool confirmacion3 = false;  // Confirmación para el estado 2

const int pinPotenciometro = 34;
int entradaAnaloga;
const int pin2Potenciometro = 35;
int entradaAnaloga2;

int estado = 0;
unsigned long TMP_ANTERIOR;
// unsigned long tiempoAnterior = 0;
// unsigned long tiempoAnterior_V = 0;
// unsigned long tiempoAnterior_V2 = 0; 

unsigned long previousPress;
unsigned long previousPressPE;

unsigned long previousMillis1 = 0;  // Almacena el tiempo para el primer intervalo
unsigned long previousMillis2 = 0;  // Almacena el tiempo para el segundo intervalo

//variables de tiempo para los pasos en el switch case

int TMP_PASO10= 300;
int TMP_PASO11= 4000;
int TMP_PASO12= 300;
int TMP_PASO1;
int TMP_PASO19=2000;
int TMP_PASO2;
//int TMP_PASO3 = 2000;
