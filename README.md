# robotic-lasercat
Código em C++ para Arduino IDE, a partir de um curso que fiz na Alura, que controla um dispositivo que aponta um laser para o chão em posições aleatórias, como um brinquedo para gatos. Também é possível monitorar o funcionamento pela internet via MQTT, assim como acionar ou desligar o dispositivo.

O dispositivo foi feito com:
- uma placa NodeMCU ESP8266
- sensor de presença PIR
- módulo laser 5v
- 2 servo motores 9g
- suporte pan/tilt para os servos
- 2 mini protoboards
- jumpers MxM e MxF
- uma caixa velha de iPhone 6s (na falta de uma caixa mais resistente de plástico, por exemplo)

O sensor, localizado na parte de baixo da caixa, detecta movimento e aciona o laser e os motores, que se movem com o laser durante um minuto apontando para posições aleatórias.

No momento em que ele é acionado, o dispositivo envia uma mensagem para um broker MQTT que sabe quando ele está acionado ou não (veja a imagem do dashboard abaixo). Da mesma forma, pelo app no celular que se conecta com o broker MQTT, é possível enviar uma mensagem para acionar ou desligar o dispositivo.

Site do curso: https://cursos.alura.com.br/course/robotica-brinquedo-interativo


<img src="https://raw.githubusercontent.com/marciocoelho31/robotic-lasercat/main/lasercat1.jpg" height="300">&nbsp;&nbsp;&nbsp;<img src="https://raw.githubusercontent.com/marciocoelho31/robotic-lasercat/main/lasercat2.jpg" height="300"><br><br><img src="https://raw.githubusercontent.com/marciocoelho31/robotic-lasercat/main/lasercat3.jpg" height="300">&nbsp;&nbsp;&nbsp;
<img src="https://raw.githubusercontent.com/marciocoelho31/robotic-lasercat/main/lasercat4.jpg" height="300">&nbsp;&nbsp;&nbsp;<img src="https://raw.githubusercontent.com/marciocoelho31/robotic-lasercat/main/lasercat5.jpg" height="300"><br><br><img src="https://raw.githubusercontent.com/marciocoelho31/robotic-lasercat/main/mqttdash1.png" height="300">&nbsp;&nbsp;&nbsp;<img src="https://raw.githubusercontent.com/marciocoelho31/robotic-lasercat/main/mqttdash2.png" height="300">


