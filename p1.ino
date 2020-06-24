#include <EasyTransfer.h>
#include <JC_Button.h>
EasyTransfer ET;
#include <Timers.h>

Timer heszkeTimer;
Timer popekTimer;
unsigned long czas_millis;
unsigned long czas_minus;
unsigned long czas_sekunda;
float sekunda;
float V = 0;


int d = 0;
int x = 0;
int c = 0;
int v = 0;
int b = 0;
int n = 0;
int m = 0;
int l = 0;
int k = 0;
int q = 0;
int obrotynapaskuposlisk = 0;
boolean a = 0;
struct DATA_STRUCTURE {
  int int_obrotymlocarni;
  int int_obrotywialni;
  int podajnik_ziarnowy;
  int podajnik_klosowy;
  int podajnik_pochyly;
  int wytrzasacze;
  int sieczkarnia;
  int czuj_sieczkarnia;
  float predkosctyl;
  int obrotysilnik;
  int obroty_na_pasku;
  int poslisk_na_kole;
};


Button myBtn(7, 1);
Button p(6, 1);
Button p1(A5, 1);
Button p2(A4, 1);
Button p3(A3, 1);
Button p4(A2, 1);
Button p5(A1, 1);
Button p6(8, 1);
Button p7(A0, 1);
Button p8(9, 1);


long czas = 0;
long czas_1 = 0;
DATA_STRUCTURE myData;


void setup()
{
  heszkeTimer.begin(SECS(1));
  popekTimer.begin(SECS(2));
  p.begin();
  p1.begin();
  p2.begin();
  p3.begin();
  p4.begin();
  p5.begin();
  p6.begin();
  p7.begin();
  p8.begin();
  myBtn.begin();
  //Initalizing EasyTransfer Serial Ports
  Serial.begin(9600);
  ET.begin(details(myData), &Serial);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A0, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);//czuj zapch
  pinMode(8, INPUT_PULLUP);////predkosc tyl
  pinMode(2, INPUT_PULLUP);

}

void loop() {
  czas_millis = millis();

  if (digitalRead(2) == LOW) {
    a = 0;
  } else {
    a = 1;
  }

  myBtn.read();               // read the button

  if (myBtn.wasReleased())    // if the button was released, change the LED state
  {
    d++;

  }
  p.read();
  if (p.wasPressed())    // if the button was released, change the LED state
  {
    x++;
  }
  p1.read();
  if (p1.wasPressed())    // if the button was released, change the LED state
  {
    c++;
  }
  p2.read();
  if (p2.wasPressed())    // if the button was released, change the LED state
  {
    v++;
  }

  p4.read();
  if (p4.wasPressed())    // if the button was released, change the LED state
  {
    n++;
  }
  p5.read();
  if (p5.wasReleased())    // if the button was released, change the LED state
  {
    m++;
  }

  p3.read();
  if (p3.wasPressed())    // if the button was released, change the LED state
  {
    b++;
  }
  p6.read();
  if (p6.wasPressed())    // if the button was released, change the LED state//l++
  {
    czas_sekunda = czas_millis - czas_minus;
  }
  if (p6.wasReleased())    // if the button was released, change the LED state
  {
    czas_minus = czas_millis;
  }

  p7.read();
  if (p7.wasPressed())    // if the button was released, change the LED state
  {
    k++;
  }
  p8.read();
  if (p8.wasPressed()) {
    q++;
  }
  //Serial.println(obrotynapaskuposlisk);
  if (heszkeTimer.available())
  {
    myData.obroty_na_pasku = q * 20;
    myData.int_obrotymlocarni = d * 15;
    myData.int_obrotywialni = x * 15;
    myData.podajnik_ziarnowy = c * 15;
    myData.podajnik_klosowy = v * 15;
    myData.podajnik_pochyly = b * 15; //b * 60;
    myData.wytrzasacze = n * 20;
    myData.sieczkarnia = m * 10;
    myData.czuj_sieczkarnia = a;
    myData.obrotysilnik = k * 30;
    if (q == 0 || k == 0) {
      myData.poslisk_na_kole = 0;
    }
    if (q != 0 && k != 0) {
      myData.poslisk_na_kole = ( myData.obroty_na_pasku / myData.obrotysilnik) * 100;
    }
    /*if (myData.obrotysilnik > 0) {
      myData.obroty_na_pasku = (obrotynapaskuposlisk / myData.obrotysilnik) * 100;///  q/k
      //Serial.println(obrotynapaskuposlisk);
      }
      if (myData.obrotysilnik == 0) {
      myData.obroty_na_pasku = 0;
      //Serial.println(obrotynapaskuposlisk);
      }
    */
    ET.sendData();

    d = 0;
    x = 0;
    c = 0;
    v = 0;
    b = 0;
    n = 0;
    m = 0;
    k = 0;
    q = 0;
    heszkeTimer.restart();
  }

  if (popekTimer.available()) {
    sekunda = czas_sekunda;
    sekunda = sekunda / 10;
    sekunda = sekunda / 100;
    
        if (czas_millis - czas_minus > 3000 ) {
          //sekunda = 0;
          V = 0;
        } else {

           V = (0.49 / sekunda) * 3.6;
        }
    
   

    myData.predkosctyl = V;
    ET.sendData();
    popekTimer.restart();
  }
  /*
  Serial.print(czas_sekunda);
  Serial.print("    ");
  Serial.println(sekunda);

*/

}
