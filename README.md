# Control de un LED por Wi-Fi con ESP32

## Descripción del proyecto

Este proyecto permite encender y apagar un LED desde el navegador de un teléfono o computador mediante una conexión Wi-Fi local. El ESP32 funciona como punto de acceso inalámbrico y como servidor web, por lo que no se necesita un router ni conexión a Internet para controlar el circuito.

La interfaz web presenta los controles **Encender** y **Apagar** y muestra el estado del LED.

## Objetivo

Implementar un sistema básico de control inalámbrico que conecte una interfaz web con una salida digital del ESP32, integrando conceptos de electrónica, programación embebida y comunicación HTTP.

## Materiales

- 1 placa ESP32 (por ejemplo, ESP32 DevKit con módulo ESP32-WROOM-32).
- 1 LED.
- 1 resistencia limitadora de 220 Ω.
- 1 protoboard.
- Cables Dupont.
- Cable USB de datos.
- Computador con Arduino IDE.
- Teléfono o computador con conexión Wi-Fi.

## Conexiones del circuito

Para el montaje de referencia con ESP32 clásico:

| Componente | Conexión |
|---|---|
| GPIO23 del ESP32 | A una terminal de la resistencia de 220 Ω |
| Resistencia de 220 Ω | Su otra terminal al ánodo (pata larga) del LED |
| Cátodo del LED (pata corta/lado plano) | GND del ESP32 |

El LED y la resistencia deben quedar **en serie**:

```text
GPIO23 ─── Resistencia 220 Ω ─── Ánodo LED |>| Cátodo ─── GND
```

> **Importante:** La asignación GPIO23 corresponde al ESP32 clásico indicado en el montaje de referencia. En otras variantes de ESP32 se debe comprobar el pinout y ajustar el programa. No conectes el LED directamente al GPIO ni apliques 5 V a un pin GPIO.

## Requisitos de software

- Arduino IDE 2.x.
- Paquete de tarjetas **esp32 by Espressif Systems**.
- Biblioteca `WiFi.h` y servidor web `WebServer.h`, incluidas con el paquete Arduino-ESP32.

Para instalar el soporte de ESP32, agrega esta URL en las preferencias de Arduino IDE, en el campo de URLs adicionales del Gestor de tarjetas:

`https://espressif.github.io/arduino-esp32/package_esp32_index.json`

Luego instala **esp32 by Espressif Systems**, selecciona la placa que corresponda (por ejemplo, **ESP32 Dev Module**) y el puerto serial correcto.

## Configuración y uso

1. Abre el sketch del proyecto en Arduino IDE.
2. Verifica que el pin configurado en el programa coincida con el cableado del LED.
3. Conecta el ESP32 mediante un cable USB de datos.
4. Selecciona la placa y el puerto correspondientes.
5. Compila y carga el programa.
6. Abre el Monitor serie a **115200 baudios** para consultar el nombre de la red y la dirección IP que informa el ESP32.
7. Desde el teléfono, conéctate a la red Wi-Fi creada por el ESP32.
8. Si el teléfono indica que la red no tiene Internet, permanece conectado: es el comportamiento esperado de una red local.
9. Abre en el navegador la dirección IP indicada por el Monitor serie. En la configuración de referencia suele ser `http://192.168.4.1/`.
10. Pulsa **Encender** y **Apagar** y comprueba que el LED y el estado mostrado en la página cambien correctamente.

Los datos de red (SSID y contraseña) deben consultarse en las constantes configuradas en el sketch. No se deben asumir credenciales distintas de las que tenga el programa cargado.

## ¿Cómo funciona?

1. El ESP32 configura su interfaz inalámbrica en modo punto de acceso (AP).
2. El teléfono se conecta directamente a la red creada por el ESP32.
3. El navegador solicita la página web al servidor HTTP del microcontrolador.
4. Al pulsar un botón, el navegador envía una solicitud HTTP a la ruta correspondiente.
5. El ESP32 ejecuta la función asociada y cambia el nivel lógico del GPIO conectado al LED.
6. El servidor responde y la página se actualiza para mostrar el estado actual.

En el programa de referencia, la ruta principal atiende una solicitud `GET`, mientras que las acciones de encendido y apagado se reciben mediante solicitudes `POST`.

## Respuestas a las preguntas de la guía

### 1. ¿Por qué funciona sin Internet?

Porque el ESP32 crea su propia red Wi-Fi en modo punto de acceso. El teléfono se comunica directamente con el microcontrolador dentro de esa red local y accede a su servidor web mediante una dirección IP privada. La comunicación entre ambos dispositivos no necesita salir a Internet ni pasar por un router externo.

### 2. ¿Qué hace `handleClient()`?

`servidor.handleClient()` revisa y atiende las solicitudes HTTP que llegan al servidor web. Se ejecuta repetidamente dentro de `loop()`. Cuando recibe una petición dirigida a una ruta registrada, permite que se ejecute la función asociada; por ejemplo, la función que enciende o apaga el LED. Sin atender las solicitudes, los botones de la página no podrían controlar el GPIO.

### 3. ¿Qué riesgo hay en conectar el LED sin resistencia?

Sin una resistencia limitadora, la corriente que circula por el LED y el GPIO podría ser excesiva. Esto puede dañar el LED y también el pin o el propio ESP32. La resistencia en serie limita la corriente a un valor adecuado para el circuito. En el montaje de referencia se utiliza una resistencia de 220 Ω.

### 4. ¿Qué diferencia existe entre el estado físico y la variable `ledEncendido`?

El estado físico es la condición real del LED: si está iluminado o apagado. La variable `ledEncendido` es un dato lógico almacenado en el programa (`true` o `false)) que representa el estado que el software espera haber establecido. El programa actualiza esa variable cuando ejecuta las órdenes de encendido o apagado y la utiliza para mostrar el estado en la página. Si hay un problema eléctrico, de cableado o de conexión, la variable podría indicar “encendido” aunque el LED físicamente no ilumine; por eso es importante comprobar ambos estados.

## Evidencias

Añade las fotografías y los videos de tu práctica en una carpeta llamada `evidencias/`. Después, ajusta los nombres de archivo de esta sección para que coincidan exactamente con los archivos que subas al repositorio.

### Fotografías

- **Montaje del circuito:** `evidencias/montaje.jpg`
- **Conexiones del ESP32 y LED:** `evidencias/conexiones.jpg`
- **Interfaz de control en el navegador:** `evidencias/interfaz-web.png`

### Videos

- **Demostración de encendido y apagado:** `evidencias/prueba-control.mp4`

Puedes insertar una imagen directamente en este README con la sintaxis:

```md
![Montaje del circuito](evidencias/montaje.jpg)
```

Para enlazar un video almacenado en el repositorio:

```md
[Ver demostración de funcionamiento](evidencias/prueba-control.mp4)
```

> Si tus archivos tienen otros nombres o extensiones, reemplaza las rutas de ejemplo anteriores. GitHub no reproduce todos los formatos de video de la misma manera en todas las vistas; un enlace al archivo es una alternativa sencilla.

## Pruebas de funcionamiento

| Prueba | Acción | Resultado esperado |
|---|---|---|
| Inicio | Reiniciar el ESP32 | El LED inicia apagado |
| Red Wi-Fi | Buscar redes desde el teléfono | Aparece el punto de acceso del ESP32 |
| Página web | Abrir la IP local en el navegador | Se muestra la interfaz de control |
| Encendido | Pulsar **Encender** | El LED ilumina y la página indica encendido |
| Apagado | Pulsar **Apagar** | El LED se apaga y la página indica apagado |
| Repetición | Alternar varias veces | El sistema responde a las órdenes |

Registra en las evidencias únicamente las pruebas que hayas realizado.

## Seguridad y consideraciones

- Desconecta la alimentación USB antes de modificar el cableado.
- Verifica la polaridad del LED y utiliza siempre una resistencia en serie.
- No conectes motores, relés ni cargas de alta corriente directamente a un GPIO.
- El servidor HTTP local de esta práctica es apropiado para una demostración aislada; no debe exponerse directamente a Internet como sistema de control seguro.
- Mantén las credenciales Wi-Fi privadas si personalizaste la red.

## Referencias

- [Documentación de instalación de Arduino-ESP32](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html)
- [Documentación Wi-Fi de Arduino-ESP32](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html)
- [ESP32-DevKitC V4 – documentación de hardware](https://docs.espressif.com/projects/esp-idf/en/v5.3.1/esp32/hw-reference/esp32/get-started-devkitc.html)
