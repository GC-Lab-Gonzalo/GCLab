/* Licencia de Uso

Este proyecto está licenciado bajo la Licencia Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0). Esto significa que:

- Uso Creativo: Los creadores tienen permiso para utilizar, modificar y compartir este proyecto para fines personales, educativos y creativos.
- Atribución: Si utilizas este proyecto, por favor da el crédito adecuado a los autores originales.
- No Comercial: No se permite el uso de este proyecto con fines comerciales. Esto incluye, pero no se limita a, la venta del proyecto o cualquier modificación del mismo.
- Esta licencia tiene como objetivo fomentar la creatividad y la colaboración entre creadores, al mismo tiempo que protege el trabajo de ser explotado comercialmente sin permiso.

*/

//Librerias utilizadas
#include <MIDI.h>
#include <Bounce2.h>

// Definición de pines
const int potA0 = A0; // Potenciómetro para controlar la velocidad de la secuencia
const int potA1 = A1; // Potenciómetro para la voz 1
const int potA2 = A2; // Potenciómetro para la voz 2
const int potA3 = A3; // Potenciómetro para la voz 3
const int potA4 = A4; // Potenciómetro para la voz 4
const int switchMode = 5; // Interruptor para cambiar entre modo monofónico y polifónico
const int switchOctave = 6; // Interruptor para cambiar entre octavas
const int buttonScale = 2; // Botón para cambiar la escala
const int buttonRandomMode = 10; // Botón para activar/desactivar el modo aleatorio
const int clockOut = 3; // Pin de salida de reloj para sincronización
const int ledPin = 7;  // Pin del LED para la secuencia
const int ledRandom = 8;  // LED para indicar modo aleatorio

// Variables para modos y controles
bool polyphonicMode = false; // Indica si el modo es polifónico o monofónico
bool octaveHigh = true; // Indica si la octava alta está seleccionada
bool randomMode = false; // Indica si el modo aleatorio está activado
int currentStep = 0; // Paso actual en la secuencia
unsigned long lastStepTime = 0; // Tiempo del último paso
int interval = 500; // Intervalo de 500 ms por defecto

// Escalas extendidas
const int JONICA[] = {0, 2, 4, 5, 7, 9, 11, 12, 14, 16, 17, 19, 21, 23, 24, 26, 28, 29, 31, 33, 35};
const int DORICA[] = {0, 2, 3, 5, 7, 9, 10, 12, 14, 15, 17, 19, 21, 22, 24, 26, 27, 29, 31, 33, 34};
const int FRIGIA[] = {0, 1, 3, 5, 7, 8, 10, 12, 13, 15, 17, 19, 20, 22, 24, 25, 27, 29, 31, 32, 34};
const int LIDIA[] = {0, 2, 4, 6, 7, 9, 11, 12, 14, 16, 18, 19, 21, 23, 24, 26, 28, 30, 31, 33, 35};
const int MIXOLIDIA[] = {0, 2, 4, 5, 7, 9, 10, 12, 14, 16, 17, 19, 21, 22, 24, 26, 28, 29, 31, 33, 34};
const int EOLICA[] = {0, 2, 3, 5, 7, 8, 10, 12, 14, 15, 17, 19, 20, 22, 24, 26, 27, 29, 31, 32, 34};
const int LOCRIA[] = {0, 1, 3, 5, 6, 8, 10, 12, 13, 15, 17, 18, 20, 22, 24, 25, 27, 29, 30, 32, 34};
const int PENTATONICA[] = {0, 2, 4, 7, 9, 12, 14, 16, 19, 21, 24, 26, 28, 31, 33};
const int FLAMENCA[] = {0, 1, 4, 5, 7, 8, 10, 12, 13, 16, 17, 19, 20, 22, 24, 25, 28, 29, 31, 32, 34};
const int NEOCLASICA[] = {0, 1, 3, 5, 7, 8, 11, 12, 13, 15, 17, 19, 20, 23, 24, 25, 27, 29, 31, 32, 35};

// Lista de escalas y longitud de cada una
const int* scales[] = {JONICA, DORICA, FRIGIA, LIDIA, MIXOLIDIA, EOLICA, LOCRIA, PENTATONICA, FLAMENCA, NEOCLASICA};
const int scaleLengths[] = {21, 20, 20, 21, 20, 20, 20, 15, 21, 21};
int currentScale = 0; // Índice de la escala actual

// Instancia de MIDI
MIDI_CREATE_DEFAULT_INSTANCE();

// Variables para la gestión de notas MIDI
int lastNotePlayed[4] = {0, 0, 0, 0}; // Guardar las últimas notas reproducidas por cada canal

// Variables para el estado de los botones
Bounce buttonScaleDebouncer = Bounce();
Bounce buttonRandomModeDebouncer = Bounce();

void setup() {
  // Configuración de pines
  pinMode(switchMode, INPUT_PULLUP); // Configurar switchMode como entrada con resistencia pull-up
  pinMode(switchOctave, INPUT_PULLUP); // Configurar switchOctave como entrada con resistencia pull-up
  pinMode(buttonScale, INPUT_PULLUP); // Configurar buttonScale como entrada con resistencia pull-up
  pinMode(buttonRandomMode, INPUT_PULLUP); // Configurar buttonRandomMode como entrada con resistencia pull-up
  pinMode(clockOut, OUTPUT); // Configurar clockOut como salida
  pinMode(ledPin, OUTPUT); // Configurar ledPin como salida
  pinMode(ledRandom, OUTPUT); // Configurar ledRandom como salida

  // Inicialización de MIDI
  MIDI.begin(MIDI_CHANNEL_OMNI); // Iniciar la comunicación MIDI en todos los canales
  MIDI.turnThruOff(); // Desactivar el MIDI Thru

  // Configuración de los debouncers
  buttonScaleDebouncer.attach(buttonScale); // Asociar el debouncer con el pin buttonScale
  buttonScaleDebouncer.interval(20); // Establecer el intervalo de debounce a 20 ms

  buttonRandomModeDebouncer.attach(buttonRandomMode); // Asociar el debouncer con el pin buttonRandomMode
  buttonRandomModeDebouncer.interval(20); // Establecer el intervalo de debounce a 20 ms
}

void loop() {
  // Leer el estado de los interruptores y botones
  polyphonicMode = digitalRead(switchMode) == LOW; // Leer el estado del interruptor de modo
  octaveHigh = digitalRead(switchOctave) == LOW; // Leer el estado del interruptor de octava

  // Verificar el estado de los botones con debounce
  buttonScaleDebouncer.update(); // Actualizar el estado del debouncer para el botón de escala
  buttonRandomModeDebouncer.update(); // Actualizar el estado del debouncer para el botón de modo aleatorio

  // Cambiar la escala si se presiona el botón
  if (buttonScaleDebouncer.fell()) {
    currentScale = (currentScale + 1) % (sizeof(scales) / sizeof(scales[0])); // Cambiar a la siguiente escala
  }

  // Activar/desactivar el modo aleatorio
  if (buttonRandomModeDebouncer.fell()) {
    randomMode = !randomMode; // Alternar el estado del modo aleatorio
    digitalWrite(ledRandom, randomMode ? HIGH : LOW); // Encender o apagar el LED de modo aleatorio
    stopAllNotes(); // Limpiar notas al activar/desactivar el modo aleatorio
  }

  // Leer el potenciómetro A0 para ajustar la velocidad
  int speedValue = analogRead(potA0); // Leer el valor del potenciómetro A0
  interval = map(speedValue, 0, 1023, 20, 1000); // Mapear el valor a un intervalo entre 20 ms y 1000 ms

  // Modo secuencial o aleatorio
  unsigned long currentTime = millis(); // Obtener el tiempo actual en milisegundos
  if (currentTime - lastStepTime >= interval) {
    lastStepTime = currentTime; // Actualizar el tiempo del último paso

    currentStep = (currentStep + 1) % 4; // Avanzar al siguiente paso en la secuencia

    // Control del LED según el paso actual
    if (currentStep == 0 || currentStep == 2) {
      digitalWrite(ledPin, HIGH); // Encender LED en los pasos 0 y 2
    } else {
      digitalWrite(ledPin, LOW); // Apagar LED en los pasos 1 y 3
    }

    // Enviar pulso de reloj
    digitalWrite(clockOut, HIGH); // Encender la salida del reloj
    delay(1); // Esperar 1 ms
    digitalWrite(clockOut, LOW); // Apagar la salida del reloj

    // Reproducir notas según el modo
    if (randomMode) {
      // Modo aleatorio
      if (polyphonicMode) {
        // Modo polifónico: reproducir todas las voces al mismo tiempo
        playRandomNotes(1, 4);
      } else {
        // Modo monofónico: reproducir una voz a la vez
        playRandomNotes(currentStep + 1, currentStep + 1);
      }
    } else {
      // Modo normal (cuantizado)
      if (polyphonicMode) {
        // Modo polifónico: reproducir todas las voces al mismo tiempo
        playNoteFromPot(potA1, 1);
        playNoteFromPot(potA2, 2);
        playNoteFromPot(potA3, 3);
        playNoteFromPot(potA4, 4);
      } else {
        // Modo monofónico: reproducir una voz a la vez
        int potPin = analogPinForStep(currentStep);
        playNoteFromPot(potPin, currentStep + 1);
      }
    }
  }
}

void playNoteFromPot(int potPin, int channel) {
  int potValue = analogRead(potPin); // Leer el valor del potenciómetro

  // Verificar si el potenciómetro está en un umbral cercano a 0 para no reproducir ninguna nota
  if (potValue <= 10) {
    stopNote(channel); // Detener la nota en el canal correspondiente
    return; // Salir de la función
  }

  // Detener la nota anterior si está sonando
  stopNote(channel);

  int note;
  // Modo cuantizado: seleccionar una nota de la escala
  int index = map(potValue, 0, 1023, 0, scaleLengths[currentScale] - 1);
  note = (octaveHigh ? 48 : 36) + scales[currentScale][index];

  // Enviar mensaje de nota MIDI
  MIDI.sendNoteOn(note, 127, channel);
  lastNotePlayed[channel - 1] = note; // Guardar la nota reproducida para enviar NoteOff más tarde
}

void stopAllNotes() {
  // Enviar NoteOff para todas las notas previamente reproducidas
  for (int i = 0; i < 4; i++) {
    if (lastNotePlayed[i] != 0) {
      MIDI.sendNoteOff(lastNotePlayed[i], 0, i + 1);
      lastNotePlayed[i] = 0; // Reiniciar la última nota reproducida
    }
  }
}

void stopNote(int channel) {
  // Detener la nota en un canal específico
  if (lastNotePlayed[channel - 1] != 0) {
    MIDI.sendNoteOff(lastNotePlayed[channel - 1], 0, channel);
    lastNotePlayed[channel - 1] = 0; // Reiniciar la última nota reproducida
  }
}

int analogPinForStep(int step) {
  // Devolver el pin analógico correspondiente al paso actual
  switch (step) {
    case 0: return potA1;
    case 1: return potA2;
    case 2: return potA3;
    case 3: return potA4;
    default: return potA1;
  }
}

void playRandomNotes(int startChannel, int endChannel) {
  // Generar notas aleatorias para cada voz dentro de la escala seleccionada
  for (int i = startChannel - 1; i < endChannel; i++) {
    // Detener la nota anterior si está sonando
    stopNote(i + 1);

    int randomIndex = random(scaleLengths[currentScale]);
    int note = (octaveHigh ? 48 : 36) + scales[currentScale][randomIndex];
    MIDI.sendNoteOn(note, 127, i + 1);
    lastNotePlayed[i] = note; // Guardar la nota reproducida
  }
}
