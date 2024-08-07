# Secuenciador MIDI de 4 Pasos y 4 Voces con Arduino Nano

## Descripción del Proyecto

Este proyecto implementa un secuenciador MIDI utilizando un Arduino Nano, permitiendo la creación de secuencias musicales con 4 pasos y 4 voces. Cada voz está controlada por un potenciómetro (A1, A2, A3, A4) y el secuenciador tiene dos modos de operación: monofónico y polifónico. Además, incluye un modo aleatorio para variar las notas en cada paso.

### Características

- **Modos de Operación:**
  - **Monofónico:** En cada paso de la secuencia, se activa una sola voz. El potenciómetro correspondiente al paso actual controla la nota reproducida.
  - **Polifónico:** En cada paso de la secuencia, se reproducen simultáneamente las notas de los 4 potenciómetros.
- **Controles:**
  - **Potenciómetro A0:** Controla la velocidad de la secuencia (de 20 ms a 1000 ms).
  - **Interruptor 11:** Cambia entre modo monofónico y polifónico.
  - **Interruptor 10:** Cambia entre octava baja y octava alta.
  - **Botón D2:** Permite cambiar la escala de notas cuantizadas.
  - **Botón D3:** Activa/desactiva el modo aleatorio. Indicado por un LED en D8.
- **Salidas:**
  - **D6:** Genera un pulso de reloj para sincronizar con otros sintetizadores.

### Diagramas de Conexión

Incluye un diagrama detallado de cómo conectar los componentes al Arduino Nano.

### Video Demostrativo

Puedes ver un video del secuenciador MIDI en acción, mostrando sus diferentes modos de operación y cómo interactuar con él.
Puedes ver el video en https://www.youtube.com/@GCLabChile

## Requisitos

- Arduino Nano
- Potenciómetros (5)
- Interruptores (2)
- Botones (2)
- LEDs (2)
- Conexiones MIDI (opcional)
- Fuente de alimentación
- Sintetizador o DAW

## Instalación y Uso

1. Clona este repositorio: `git clone https://github.com/tu_usuario/nombre_del_proyecto.git`
2. Abre el archivo `.ino` en el Arduino IDE.
3. Sube el código al Arduino Nano.
4. Conecta los componentes según el diagrama de conexión.
5. Conecta a un sintetizador o un DAW mediante MIDI
6. Enciende el sistema y ajusta los controles para crear tus secuencias MIDI.

## Licencia

Este proyecto está licenciado bajo la [Licencia Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)](https://creativecommons.org/licenses/by-nc-sa/4.0/). Esto significa que:

- Puedes utilizar, modificar y compartir este proyecto para fines personales, educativos y creativos.
- Debes dar el crédito adecuado a los autores originales.
- Si modificas o construyes a partir de este proyecto, debes distribuir tus contribuciones bajo la misma licencia que el original.
- No se permite el uso de este proyecto con fines comerciales.

## Contribuciones

Las contribuciones son bienvenidas. Si deseas mejorar este proyecto, por favor realiza un fork del repositorio, realiza tus cambios y envía un pull request.

---

## Créditos

Este proyecto fue desarrollado por Gonzalo Sandoval. Si tienes preguntas o necesitas soporte, no dudes en contactarme.

