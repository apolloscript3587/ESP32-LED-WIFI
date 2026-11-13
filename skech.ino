#include <WiFi.h>
#include <WebServer.h>

// Datos de la red Wi-Fi que creara el ESP32.
const char* NOMBRE_RED = "ESP32_LED_AULA";
const char* CLAVE_RED = "Mecatronica2026";

// Para ESP32 clasico tipo DevKit / WROOM-32.
const int PIN_LED = 23;

WebServer servidor(80);
bool ledEncendido = false;

// Genera la pagina que se ve en el navegador.
void mostrarPagina() {
  String pagina = R"HTML(
<!DOCTYPE html>
<html lang="es">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Control LED ESP32</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin: 40px; background: #f5f7fb; }
    button { font-size: 20px; padding: 15px 25px; margin: 8px; cursor: pointer; }
  </style>
</head>
<body>
  <h1>Control de LED por Wi-Fi</h1>
  <p>Estado actual: <strong>%ESTADO%</strong></p>
  <form action="/encender" method="POST">
    <button type="submit">Encender</button>
  </form>
  <form action="/apagar" method="POST">
    <button type="submit">Apagar</button>
  </form>
</body>
</html>
)HTML";

  pagina.replace("%ESTADO%", ledEncendido ? "ENCENDIDO" : "APAGADO");
  servidor.send(200, "text/html; charset=utf-8", pagina);
}

// Atiende la solicitud de encendido.
void encenderLED() {
  digitalWrite(PIN_LED, HIGH);
  ledEncendido = true;
  Serial.println("Orden recibida: encender LED");
  servidor.sendHeader("Location", "/");
  servidor.send(303, "text/plain", "");
}

// Atiende la solicitud de apagado.
void apagarLED() {
  digitalWrite(PIN_LED, LOW);
  ledEncendido = false;
  Serial.println("Orden recibida: apagar LED");
  servidor.sendHeader("Location", "/");
  servidor.send(303, "text/plain", "");
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  WiFi.mode(WIFI_AP);
  bool redCreada = WiFi.softAP(NOMBRE_RED, CLAVE_RED);

  if (!redCreada) {
    Serial.println("ERROR: no se pudo crear la red Wi-Fi");
    while (true) {
      delay(1000);
    }
  }

  Serial.println("Red Wi-Fi creada correctamente");
  Serial.print("Nombre: ");
  Serial.println(NOMBRE_RED);
  Serial.print("Direccion IP: ");
  Serial.println(WiFi.softAPIP());

  servidor.on("/", HTTP_GET, mostrarPagina);
  servidor.on("/encender", HTTP_POST, encenderLED);
  servidor.on("/apagar", HTTP_POST, apagarLED);
  servidor.begin();
  Serial.println("Servidor HTTP listo");
}

void loop() {
  servidor.handleClient();
}
