#include <HTTPClient.h>
#include <WiFi.h>

/* this can be run with an emulated server on host:
        cd esp8266-core-root-dir
        cd tests/host
        make ../../libraries/ESP8266WebServer/examples/PostServer/PostServer
        bin/PostServer/PostServer
   then put your PC's IP address in SERVER_IP below, port 9080 (instead of default 80):
*/
//#define SERVER_IP "10.0.1.7:9080" // PC address with emulation on host
#define SERVER_IP "http://api-irrigacao.herokuapp.com/sensor"

#define STASSID "Theodoro_2.4G"
#define STAPSK  "20011999"

int idSensor = 2;
int valorSensor = 851;
#define apiKey "\"Copanga7\""

/*void jsonSensor(int id, int value)
{
  String jsonPayload = "{\"key\":"apiKey",\"idSensor\":" + String(id) + ",\"valorSensor\":" + String(value) + "}";
  
}*/

String json(int id, int value)
{
  String jsonPayload = "{\"key\":"apiKey",\"idSensor\":" + String(id) + ",\"valorSensor\":" + String(value) + "}";
  return jsonPayload;
}

void postHTTP(String endereco, String payload)
{
  WiFiClient client;
  HTTPClient http;

  Serial.print("Requisição iniciada...\n");
  //Inicia a comunicação http e envia o header na requisição
  http.begin(client, endereco); //HTTP
  http.addHeader("Content-Type", "application/json");

  Serial.print("POST...\n");
  //httpCode é a variável que recebe o valor da requisição, se deu certo o valor é 200, responsável também por chamar a função que faz um POST na API.
  int httpCode = http.POST(payload);

  //caso o código seja menor que 0, significa que o POST deu erro
  if (httpCode > 0) {
    // Mostra no monitor serial qual o código da requisição
    Serial.printf("Código HTTP: %d\n", httpCode);

    // Caso a requisição seja compreendida pela API, mostra no monitor serial o retorno da API
    if (httpCode == HTTP_CODE_OK) {
      const String& bodyGET = http.getString();
      Serial.println("Pacote recebido:\n<<");
      Serial.println(bodyGET);
      Serial.println(">>");
    }
    } else {
      Serial.printf("Ocorreu erro ao enviar a requisição POST, erro: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();  
}


void setup() {

  Serial.begin(115200);

  Serial.println();
  Serial.println();
  Serial.println();

  WiFi.begin(STASSID, STAPSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {
    postHTTP("http://api-irrigacao.herokuapp.com/sensor", json(3, 1021));
  }
  delay(20000);
}
