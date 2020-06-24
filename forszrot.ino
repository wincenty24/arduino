#include <JC_Button.h>
//#include <Button2.h>
#include <Bounce2.h>
#include <SPI.h>
//#include <PCF8574.h>
#include <EEPROM.h>
#include <Wire.h>
#include <Timers.h>
#include <FT_VM800P43_50.h>
#include <EasyTransfer.h>
Button prz_gora(A1, 50);
Button prz_dol(A2, 50);
EasyTransfer ET;
//PCF8574 expander;
Button myBtn(A4, 50);
#define czuj_heder A3
#define czuj_predkosc A5
#define przycisknr1 7
#define przycisknr2 8
#define kogutpin 6
// Instantiate a Bounce object :
Bounce predkosc = Bounce();
Bounce przycisk_1 = Bounce();
Bounce przycisk_2 = Bounce();
//Bounce czuj_mlocarnia = Bounce();
Bounce mlocarnia_wlacznik = Bounce();

unsigned long long buttonPressTimeStamp = 0;;


unsigned   long long currentMillis = 0;
unsigned   long long buttonPushedMillis;
unsigned  long long ledTurnedOnAt;
int turnOnDelay = 3000;
bool ledReady = false;

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
boolean wlacz_ostrzeganie=false;
long poslisk = 0;
int liczenie_zbiornik = 0;
int liczenie_zbiornik_100 = 0;
boolean kogut = 1;
boolean stan_kogut_zbiornik = 0;
boolean stan_kogut_zbiornik_100 = 0;
boolean zbiornik_boolean_75 = false;
boolean zbiornik_boolean_100 = false;
int rmp = 0;
const float R = 0.47;////////////////////////////////////////////////////////////////////////////
float w = 0;
boolean czujnik_sieczka = 1;
int  szerokosc_heder_1 = 5;
boolean menu_heder = 0;
int ilosc_rzedow = 5;
boolean rodzaj_hederu = 0;
int do_jasnosci = 0;
int jasnosc = 20;
int stan_heder = 0;
float metry_hektary = 0;
float f_val = 1.23;
static char char_V[15];
static char char_droga[15];
int hektary_liczenie = 0;
float dlugosc_hektary_480 = 0;
float dlugosc_hektary_240 = 0;
float dlugosc_hektary_120 = 0;
float dlugosc_hektary_160 = 0;
float dlugosc_hektary_360 = 0;
float dlugosc_hektary_375 = 0;
float dlugosc_hektary_300 = 0;
float dlugosc_hektary_225 = 0;
float dlugosc_hektary_150 = 0;
float dlugosc_hektary_75 = 0;
float droga = 0;
DATA_STRUCTURE myData;
int szerokosc_heder = 480;
float hektary = 0;
float hektary_480 = 0;
float hektary_240 = 0;
float hektary_120 = 0;
float hektary_160 = 0;
float hektary_360 = 0;
float hektary_375 = 0;
float hektary_300 = 0;
float hektary_225 = 0;
float hektary_150 = 0;
float hektary_75 = 0;
static char char_hektary[15];
double i = 0;
float r = 3.14 * 2 * 0.76;/////////////////////////////////////////////////tutaj
float V = 0;
int tagval = 0;
int zmienna_1 = 0;
boolean wlaczanieprzekaznikow = 0;
boolean buzzer = 0;
boolean buzzer_1 = 0;
boolean buzzer_2 = 0;
boolean buzzer_3 = 0;
boolean buzzer_4 = 0;
boolean buzzer_5 = 0;
boolean buzzer_6 = 0;
boolean buzzer_7 = 0;
boolean buzzer_mlocarnia = 1;
boolean buzzer_wialni = 1;
boolean buzzer_ziarnowy = 1;
boolean buzzer_klosowy = 1;
boolean buzzer_pochyly = 1;
boolean buzzer_wytrzasacz = 1;
boolean buzzer_sieczkarnia = 1;
boolean buzzer_sieczka = 1;
boolean czujnik_od_mlocarni = 0;
boolean czas_1 = 0;
long h = 0;
int adres = 0;
int adres_1 = 1;
int droga_liczenie = 0;
int obroty_x = 0;
int obroty_y = 0;
int obroty_mlucarni = 0;
int obroty_wialnia = 0;
unsigned long czas_mlucarni = 0;
unsigned long czas_mlucarni_1 = 0;
//HCSR04 sensor;
//HCSR04 sensor_1;
//Button b1(czuj1);
Timer buzzerTimer;
Timer uwaga;
Timer zbiornik_timer;
Timer zbiornik_timer_100;
//Timer plusjasnoscTimer;
//Timer czas_wialniaTimer;


FT800IMPL_SPI FTImpl(FT_CS_PIN, FT_PDN_PIN, FT_INT_PIN);
int16_t BootupConfigure()
{
  uint32_t chipid = 0;
  FTImpl.Init(FT_DISPLAY_RESOLUTION);

  delay(20);
  chipid = FTImpl.Read32(FT_ROM_CHIPID);


  if (FT800_CHIPID != chipid)
  {
    Serial.print("Error in chip id read ");
    Serial.println(chipid, HEX);
    return 1;
  }


  FTImpl.SetDisplayEnablePin(FT_DISPENABLE_PIN);
  FTImpl.SetAudioEnablePin(FT_AUDIOENABLE_PIN);
  FTImpl.DisplayOn();
  FTImpl.AudioOn();
  return 0;
}
int zmienna = 1;
void Calibrate()
{
  FTImpl.DLStart();
  FTImpl.ClearColorRGB(64, 64, 0xFF);
  FTImpl.Clear(1, 1, 1);
  FTImpl.ColorRGB(0xff, 0xff, 0xff);
  FTImpl.Cmd_Text((FT_DISPLAYWIDTH / 2), (FT_DISPLAYHEIGHT / 2), 27, FT_OPT_CENTER, "konfiguracja");
  FTImpl.Cmd_Calibrate(0);
  FTImpl.Finish();
}

void setup()
{
  //expander.begin(0x20);
  pinMode(przycisknr1, INPUT_PULLUP);
  przycisk_1.attach(przycisknr1);
  przycisk_1.interval(25);


  pinMode(przycisknr2, INPUT_PULLUP);
  przycisk_2.attach(przycisknr2);
  przycisk_2.interval(25);

  pinMode(czuj_heder, INPUT_PULLUP);

  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);

  pinMode(czuj_predkosc, INPUT_PULLUP);
  predkosc.attach(czuj_predkosc);
  predkosc.interval(2);
  ET.begin(details(myData), &Serial);
  uwaga.begin(SECS(1));
  buzzerTimer.begin(SECS(1));
  zbiornik_timer.begin(SECS(1));
  zbiornik_timer_100.begin(SECS(1));
  // plusjasnoscTimer.begin(SECS(1));

  pinMode(5, OUTPUT);//buzzer
  pinMode(kogutpin, OUTPUT);///kogut
  /*
    expander.pinMode(0, OUTPUT);
    expander.pinMode(1, OUTPUT);
    expander.pinMode(2, OUTPUT);
    expander.pinMode(3, OUTPUT);
    expander.digitalWrite(0, HIGH);
    expander.digitalWrite(1, HIGH);
    expander.digitalWrite(2, HIGH);
    expander.digitalWrite(3, HIGH);
  */
  pinMode(A4, INPUT_PULLUP);//czuj mlocarnia
  // mlocarnia_wlacznik.attach(A4);
  //mlocarnia_wlacznik.interval(25);

  myBtn.begin();
  prz_gora.begin();
  prz_dol.begin();

  {

    Serial.begin(9600);


    Serial.println("--Start Application--");
    if (BootupConfigure())
    {

    }
    else
    {
      Calibrate();

    }
    Serial.println("--End Application--");
  }
}
uint8_t *pbuff;
uint32_t NumBytesGen = 0;
uint16_t CurrWriteOffset = 0;
void loop() {


  
 
   // poslisk = myData.poslisk_na_kole;
  
  Serial.println(digitalRead(5));
  myBtn.read();

  FTImpl.DLStart();
  currentMillis = millis();
  mlocarnia_wlacznik.update();
  if (myBtn.wasPressed()) {
    buttonPushedMillis = currentMillis;
    ledReady = true;
  }
  if (ledReady) {
    if ((currentMillis - buttonPushedMillis) >= turnOnDelay) {
      ledTurnedOnAt = currentMillis;
      ledReady = false;
      czujnik_od_mlocarni = 1;
    }
  }
  if (digitalRead(A4) == HIGH) {
    czujnik_od_mlocarni = 0;
  }




  menu_heder = EEPROM.read(adres);
  rodzaj_hederu = menu_heder;
  do_jasnosci = jasnosc * 6.4;
  // Serial.print(i / 1000);
  //Serial.print("      ");
  // Serial.print(V);
  // Serial.print("      ");


  if (jasnosc > 20) {
    jasnosc = 20;
  }
  if (jasnosc < 1) {
    jasnosc = 1;
  }
  //jasnosc = map(jasnosc, 0, 10, 0, 128);
  FTImpl.Write(REG_PWM_DUTY, do_jasnosci); ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  dtostrf(V, 7, 2, char_V);
  dtostrf(hektary, 7, 2, char_hektary);
  dtostrf(droga, 7, 1, char_droga);
  // Call code if Bounce fell (transition from HIGH to LOW) :

  if (szerokosc_heder == 480) {
    hektary_480 = dlugosc_hektary_480 * (szerokosc_heder / 100) * 0.0001;
  }
  if (szerokosc_heder == 240) {
    hektary_240 = dlugosc_hektary_240 * (szerokosc_heder / 100) * 0.0001;
  }
  if (szerokosc_heder == 120) {
    hektary_120 = dlugosc_hektary_120 * (szerokosc_heder / 100) * 0.0001;
  }
  if (szerokosc_heder == 160) {
    hektary_160 = dlugosc_hektary_160 * (szerokosc_heder / 100) * 0.0001;
  }
  if (szerokosc_heder == 360) {
    hektary_360 = dlugosc_hektary_360 * (szerokosc_heder / 100) * 0.0001;
  }
  if (szerokosc_heder == 375) {
    hektary_375 = dlugosc_hektary_375 * (szerokosc_heder / 100) * 0.0001;
  }
  if (szerokosc_heder == 300) {
    hektary_300 = dlugosc_hektary_300 * (szerokosc_heder / 100) * 0.0001;
  }
  if (szerokosc_heder == 225) {
    hektary_225 = dlugosc_hektary_225 * (szerokosc_heder / 100) * 0.0001;
  }
  if (szerokosc_heder == 150) {
    hektary_150 = dlugosc_hektary_150 * (szerokosc_heder / 100) * 0.0001;
  }
  if (szerokosc_heder == 75) {
    hektary_75 = dlugosc_hektary_75 * (szerokosc_heder / 100) * 0.0001;
  }




  hektary = hektary_480 + hektary_240 + hektary_120 + hektary_160 + hektary_360 + hektary_375 + hektary_300 + hektary_225 + hektary_150 + hektary_75;









  {

    //b1.read();
    uint16_t scrollval = 0;

    FTImpl.Cmd_Track( (FT_DISPLAYWIDTH - 40), 40, 8, (FT_DISPLAYHEIGHT - 80), 12); //do trackowania
    FTImpl.Finish();       /// jak szybko zamyka sie petla




    tagval = FTImpl.Read32(REG_TRACKER);







    if (1 == tagval)
    {

      zmienna = 1;


    }

    if (2 == tagval)
    {

      zmienna = 2;


    }
    if (tagval == 3) {
      zmienna = 3;
    }

    if (tagval == 4) {
      zmienna = 4;
    }
    if (tagval == 5) {
      zmienna = 5;
    }
    if (tagval == 8) {
      buzzer_mlocarnia = 0;
    }

    if (tagval == 9) {
      buzzer_wialni = 0;
    }
    if (tagval == 14) {

      jasnosc++;
    }
    if (tagval == 15) {
      rodzaj_hederu = 1;
      menu_heder = 1;
      EEPROM.write(adres, menu_heder);
    }
    if (tagval == 16) {
      menu_heder = 0;
      rodzaj_hederu = 0;
      EEPROM.write(adres, menu_heder);
    }
    if (tagval == 21) {
      ilosc_rzedow = 5;

    }
    if (tagval == 20) {
      ilosc_rzedow = 4;

    }
    if (tagval == 19) {
      ilosc_rzedow = 3;

    }
    if (tagval == 18) {
      ilosc_rzedow = 2;

    }
    if (tagval == 17) {
      ilosc_rzedow = 1;

    }
    if (ilosc_rzedow == 5  && rodzaj_hederu == 1) {
      szerokosc_heder = 375;
      szerokosc_heder_1 = 5;
    }
    if (ilosc_rzedow == 4  && rodzaj_hederu == 1) {
      szerokosc_heder = 300;
      szerokosc_heder_1 = 4;
    }
    if (ilosc_rzedow == 3  && rodzaj_hederu == 1) {
      szerokosc_heder = 225;
      szerokosc_heder_1 = 3;
    }
    if (ilosc_rzedow == 2  && rodzaj_hederu == 1) {
      szerokosc_heder = 150 ;
      szerokosc_heder_1 = 2;
    }
    if (ilosc_rzedow == 1  && rodzaj_hederu == 1) {
      szerokosc_heder = 75;
      szerokosc_heder_1 = 1;
    }
    if (tagval == 22) {
      buzzer_ziarnowy = 0;
    }
    if (tagval == 23) {
      buzzer_klosowy = 0;
    }
    if (tagval == 24) {
      buzzer_pochyly = 0;
    }
    if (tagval == 25) {
      buzzer_wytrzasacz = 0;
    }
    if (tagval == 26) {
      buzzer_sieczkarnia = 0;
    }


    if (tagval == 27) {
      szerokosc_heder = 120;
    }
    if (tagval == 28) {
      szerokosc_heder = 160;
    }
    if (tagval == 29) {
      szerokosc_heder = 240;
    }
    if (tagval == 30) {
      szerokosc_heder = 360;
    }
    if (tagval == 31) {
      szerokosc_heder = 480;
    }

    if (tagval == 32) {
      jasnosc--;
    }
    if (tagval == 33) {
      kogut = 0;
    }
    if (tagval == 34) {
      kogut = 1;
    }
    digitalWrite(kogutpin, kogut);
    FTImpl.DLStart();
    {
      int32_t tmpval2;
    }

    FTImpl.Clear(1, 1, 1);
    /*
        if (zmienna == 1 || zmienna == 2 || zmienna == 3 || zmienna == 4 || zmienna == 5) {
          wlaczanieprzekaznikow = 0;
          expander.digitalWrite(0, HIGH);
          expander.digitalWrite(1, HIGH);
          expander.digitalWrite(2, HIGH);
          expander.digitalWrite(3, HIGH);
    */





    if (zmienna == 1) {/////////////////////////////////////////////////////


      FTImpl.Cmd_Text(225, 120, 24, 0, "zbiornik:");
      if (digitalRead(A2) == LOW && digitalRead(A1) == HIGH) //tutaj sadddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddlhlkjhlkasjhdfkljhasdf
      {
        FTImpl.Cmd_Text(225, 150, 24, 0, "75 %");

      }
      if (digitalRead(A2) == LOW  && digitalRead(A1) == LOW) //tutaj sadddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddlhlkjhlkasjhdfkljhasdf
      {
        FTImpl.Cmd_Text(225, 150, 24, 0, "100 %");

      }



      if (digitalRead(czuj_heder) == HIGH) {
        FTImpl.Cmd_Text(430, 210, 30, 0, "^");
      }

      if (czujnik_od_mlocarni == 1) {
        FTImpl.Cmd_Text(420, 240, 30, 0, "<=>");
      }
      FTImpl.Begin(FT_LINE_STRIP);
      FTImpl.ColorRGB(255, 255, 127);
      FTImpl.Vertex2ii(357, 0, 0, 0);
      FTImpl.Vertex2ii(357, 190, 0, 0);
      FTImpl.Vertex2ii(480, 190, 0, 0);
      FTImpl.End();
      FTImpl.Begin(FT_LINE_STRIP);
      FTImpl.ColorRGB(153   , 102  , 204);
      FTImpl.Vertex2ii(260, 0, 0, 0);
      FTImpl.Vertex2ii(260, 50, 0, 0);
      FTImpl.Vertex2ii(220, 50, 0, 0);
      FTImpl.Vertex2ii(220, 190, 0, 0);
      FTImpl.Vertex2ii(357, 190, 0, 0);
      FTImpl.End();
      FTImpl.ColorRGB(255, 255, 255);
      FTImpl.Cmd_Text(150, 0, 31, 0, "KM/H");
      FTImpl.Cmd_Text(0, 0, 31, 0, char_V );
      FTImpl.Cmd_Text(0, 130, 23, 0, "Szerokosc hederu:");
      FTImpl.Cmd_Number(180, 130, 23, 0, szerokosc_heder );
      FTImpl.ColorRGB(252, 3, 0);
      FTImpl.Cmd_Text(270, 0, 25, 0, "RMP:");
      FTImpl.Cmd_Number(270, 30, 25, 0, myData.obrotysilnik);
      //if (poslisk >= 80) {
        FTImpl.ColorRGB(255, 255, 255);
        FTImpl.Cmd_Text(225, 60, 24, 0, "poslisk %: ");
        FTImpl.Cmd_Number(225, 90, 24, 0, myData.poslisk_na_kole);
        FTImpl.Cmd_Number(320, 90, 22, 0, myData.obroty_na_pasku);
      //}
      //else if (poslisk < 80) {
        //FTImpl.ColorRGB(255, 0, 0);
        //FTImpl.Cmd_Text(225, 60, 24, 0, "poslisk %: ");
        //FTImpl.Cmd_Number(225, 90, 24, 0, poslisk);
        //


      //}

      if (rodzaj_hederu == 0) {
        FTImpl.ColorRGB(91, 93, 116);
        FTImpl.Cmd_Progress(10, 190, 200, 12, 0, szerokosc_heder, 480);
      } else {
        FTImpl.ColorRGB(91, 93, 116);
        FTImpl.Cmd_Progress(10, 190, 200, 12, 0, szerokosc_heder_1, 5);
      }

      if (stan_heder == 1) {
        FTImpl.ColorRGB(255  , 200   , 50 );
        FTImpl.Cmd_Text(0, 90, 30, 0, "Hektary:");
        FTImpl.Cmd_Text(130, 90, 30, 0, char_hektary);

      }

      if (rodzaj_hederu == 1) {
        FTImpl.ColorRGB(255, 191, 0) ;

        FTImpl.Cmd_Text(0, 160, 23, 0, "Ilosc rzedow:");
        FTImpl.Cmd_Number(130, 160, 23, 0, ilosc_rzedow);
      }
      if (droga_liczenie == 1) {

        FTImpl.ColorRGB(0, 255, 128);
        FTImpl.Cmd_Text(0, 50, 30, 0, "Dlugosc:");
        FTImpl.Cmd_Text(130, 50, 30, 0, char_droga);
      }
      FTImpl.ColorRGB(204, 119, 34);
      FTImpl.Cmd_Text(370, 0, 24, 0, "Mlocarnia");
      FTImpl.Cmd_Number(380, 30, 24, 0, myData.int_obrotymlocarni);
      FTImpl.ColorRGB(255, 0 , 255 );
      FTImpl.Cmd_Text(370, 60, 24, 0, "Wialnia");
      FTImpl.Cmd_Number(380, 90, 24, 0, myData.int_obrotywialni );
      FTImpl.ColorRGB(128, 128, 0 );
      FTImpl.Cmd_Text(370, 120, 24, 0, "Sieczkar");
      FTImpl.Cmd_Number(380, 150, 24, 0,  myData.sieczkarnia);


    }

    if (zmienna == 3) {///////////////////////////////////////////////////////////33333333333333333333333333333333333
      FTImpl.Cmd_Text(0, 0, 30, 0, "Rodzaj hederu:");
      if (rodzaj_hederu == 1) {

        FTImpl.ColorRGB(255, 191, 0) ;
        FTImpl.Cmd_Text(230, 0, 30, 0, "QQrydza");
        FTImpl.Cmd_Text(0, 70, 24, 0, "Ilosc rzedow:");
        FTImpl.Cmd_Number(150, 70, 24, 0, ilosc_rzedow);
        FTImpl.ColorRGB(255 , 255, 255);
        FTImpl.Cmd_Text(0, 40, 24, 0, "Szerokosc hederu:");

        FTImpl.Cmd_Number(220, 40, 24, 0, szerokosc_heder );
        FTImpl.Cmd_Text(270, 40, 24, 0, "(Cm)");
        FTImpl.ColorRGB(255, 191, 0) ;
        FTImpl.Cmd_FGColor(0x0027C2);
        FTImpl.Tag(16);
        FTImpl.Cmd_Button(370, 0, 110, 60, 27, FT_OPT_FLAT, "do ZBOZA");
        //FTImpl.ColorRGB(255 , 255, 255);


        FTImpl.Cmd_FGColor(0x0027C2);
        FTImpl.Tag(17);
        FTImpl.Cmd_Button(0, 120, 80, 50, 27, FT_OPT_FLAT, "1");
        FTImpl.Tag(18);
        FTImpl.Cmd_Button(90, 120, 80, 50, 27, FT_OPT_FLAT, "2");
        FTImpl.Tag(19);
        FTImpl.Cmd_Button(180, 120, 80, 50, 27, FT_OPT_FLAT, "3");
        FTImpl.Tag(20);
        FTImpl.Cmd_Button(270, 120, 80, 50, 27, FT_OPT_FLAT, "4");
        FTImpl.Tag(21);
        FTImpl.Cmd_Button(360, 120, 80, 50, 27, FT_OPT_FLAT, "5");


      }



      if (rodzaj_hederu == 0) {//zboze

        FTImpl.ColorRGB(172, 122, 51 );
        FTImpl.Cmd_Text(0, 40, 24, 0, "Szerokosc hederu:");

        FTImpl.Cmd_Number(220, 40, 24, 0, szerokosc_heder );
        FTImpl.Cmd_Text(270, 40, 24, 0, "(Cm)");
        FTImpl.Cmd_Text(230, 0, 30, 0, "Zboze");
        FTImpl.ColorRGB(255 , 255, 255);
        FTImpl.Cmd_FGColor(0x0027C2);
        FTImpl.Tag(15);
        FTImpl.Cmd_Button(370, 0, 110, 60, 27, FT_OPT_FLAT, "do QQRYDZY");


        /*
            przycisk_1.update();

            if (przycisk_1.fell() && zmienna == 3 && rodzaj_hederu == 0 ) {
              szerokosc_heder = szerokosc_heder + 5;
            }

            przycisk_2.update();
            if (przycisk_2.fell() && zmienna == 3 && rodzaj_hederu == 0 ) {
              szerokosc_heder = szerokosc_heder - 5;
            }
        */


        FTImpl.Cmd_FGColor(0x0027C2);
        FTImpl.Tag(27);
        FTImpl.Cmd_Button(0, 120, 80, 50, 27, FT_OPT_FLAT,  "1/4");
        FTImpl.Tag(28);
        FTImpl.Cmd_Button(90, 120, 80, 50, 27, FT_OPT_FLAT, "1/3");
        FTImpl.Tag(29);
        FTImpl.Cmd_Button(180, 120, 80, 50, 27, FT_OPT_FLAT, "1/2");
        FTImpl.Tag(30);
        FTImpl.Cmd_Button(270, 120, 80, 50, 27, FT_OPT_FLAT, "3/4");
        FTImpl.Tag(31);
        FTImpl.Cmd_Button(360, 120, 80, 50, 27, FT_OPT_FLAT, "Caly");
      }
    }





    if (zmienna == 5) {////////////////////////////////////////////555555555555555555555555555555555555

      FTImpl.Cmd_Text(0, 0, 30, 0, "Jasnosc ekranu:");

      FTImpl.Cmd_Text(275, 0, 30, 0, "/20");
      przycisk_1.update();

      if (przycisk_1.fell() && zmienna == 5) {
        jasnosc++;
      }

      przycisk_2.update();
      if (przycisk_2.fell() && zmienna == 5) {
        jasnosc--;
      }


      FTImpl.Cmd_Progress(10, 80, 180, 12, 0, jasnosc, 20);

      FTImpl.Cmd_Number(240, 0, 30, 0, jasnosc );
      FTImpl.Tag(14);
      FTImpl.Cmd_Button(250, 55, 75, 60, 27, FT_OPT_FLAT, "+");
      FTImpl.Tag(32);
      FTImpl.Cmd_Button(350, 55, 75, 60, 27, FT_OPT_FLAT, "-");
      FTImpl.ColorRGB(255, 255, 255);
      FTImpl.Cmd_FGColor(0xF23B1C );
      FTImpl.Tag(33);
      FTImpl.Cmd_Button(250, 120, 75, 60, 27, FT_OPT_FLAT, "ON");
      FTImpl.Tag(34);
      FTImpl.Cmd_Button(350, 120, 75, 60, 27, FT_OPT_FLAT, "OFF");
      FTImpl.ColorRGB(242 ,  59 , 28  );
      FTImpl.Cmd_Text(0, 135, 24, 0, "Swiatla ostrzegawcze" );
      if (kogut == 0) {
        FTImpl.ColorRGB(242 ,  59 , 28  );
        FTImpl.Cmd_Text(0, 160, 23, 0, "wlaczone" );
      }
      if (kogut == 1) {
        FTImpl.ColorRGB(242 ,  59 , 28  );
        FTImpl.Cmd_Text(0, 160, 23, 0, "wylaczone" );
      }
    }


    /*/////////////////////////////////////////////////////////////////////////////////////???!11111111111111111111111!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!dodaj żeby nie dało się zmieniac obrotow w innym menu
      if (zmienna == 6) { //////////////////////////////////////////////////////6666666666666666666666666666666666666666666666666666666666666
      FTImpl.ColorRGB(204, 119, 34);
      FTImpl.Cmd_Text(0, 0, 24, 0, "Mlucarnia:");
      FTImpl.Cmd_Number(170, 0, 24, 0, myData.int_obrotymlocarni);

      if (wlaczanieprzekaznikow == 1) {
        if (digitalRead(przycisknr1) == LOW) {
          expander.digitalWrite(0, LOW);
          expander.digitalWrite(2, LOW);
        } else {
          expander.digitalWrite(0, HIGH);
          expander.digitalWrite(2, HIGH);

        }

        if (digitalRead(przycisknr2) == LOW) {
          expander.digitalWrite(1, LOW);
          expander.digitalWrite(3, LOW);
        } else {
          expander.digitalWrite(1, HIGH);
          expander.digitalWrite(3, HIGH);
        }

      }


      if (wlaczanieprzekaznikow == 0) {
        FTImpl.Cmd_FGColor(0xff0000);
        FTImpl.ColorRGB(255, 255, 255);
        FTImpl.Tag(34);
        FTImpl.Cmd_Button(0, 100, 300, 40, 27, FT_OPT_FLAT, "Nacisnij aby ustawic obrory!!");
      }
      }
    */
    if (zmienna == 4) {/////////////////////////////////////////////////////////////////



      FTImpl.ColorRGB(204, 119, 34);
      FTImpl.Cmd_Text(0, 0, 24, 0, "Mlocarnia:");
      FTImpl.Cmd_Number(170, 0, 24, 0, myData.int_obrotymlocarni);
      FTImpl.ColorRGB(255 , 0 , 255 );
      FTImpl.Cmd_Text(0, 35, 24, 0, "Wialnia:");
      FTImpl.Cmd_Number(170, 35, 24, 0, myData.int_obrotywialni);
      FTImpl.ColorRGB(0 , 255, 255);
      FTImpl.Cmd_Text(0, 75, 24, 0, "P.ziarnowy:");
      FTImpl.Cmd_Number(170, 75, 24, 0,  myData.podajnik_ziarnowy);
      FTImpl.ColorRGB(128, 128, 0 );
      FTImpl.Cmd_Text(0, 115, 24, 0, "Sieczkarnia:");
      FTImpl.Cmd_Number(170, 115, 24, 0,  myData.sieczkarnia);
      FTImpl.ColorRGB(255, 255, 0);
      FTImpl.Cmd_Text(250, 0, 24, 0, "P.klosowy:");
      FTImpl.Cmd_Number(410, 0, 24, 0,  myData.podajnik_klosowy);
      FTImpl.ColorRGB(0, 0, 255);
      FTImpl.Cmd_Text(250, 35, 24, 0, "P.pochyly:");
      FTImpl.Cmd_Number(410, 35, 24, 0,  myData.podajnik_pochyly);
      FTImpl.ColorRGB(150, 150, 150);
      FTImpl.Cmd_Text(250, 75, 24, 0, "Wytrzasacz:");
      FTImpl.Cmd_Number(410, 75, 24, 0,  myData.wytrzasacze);

      if (czujnik_sieczka == 0) {

        FTImpl.ColorRGB(255, 0, 0);
        FTImpl.Cmd_Text(0, 165, 24, 0, "Zapchane");
        FTImpl.Cmd_Text(0, 185, 24, 0, "Wytrzasacze");

      }
      FTImpl.ColorRGB(255, 255, 255);
      if (buzzer == 1) {
        FTImpl.Cmd_FGColor(0xff0000);
        FTImpl.Tag(8);
        FTImpl.Cmd_Button(200, 160, 100, 40, 27, FT_OPT_FLAT, "MLOCARNIA");
      }

      if (buzzer_1 == 1) {
        FTImpl.Cmd_FGColor(0xff0000);
        FTImpl.Tag(9);
        FTImpl.Cmd_Button(390, 120, 90, 40, 27, FT_OPT_FLAT, "Wialnia");// tutaj podmienilem z pochylym
      }
      if (buzzer_2 == 1) {
        FTImpl.Cmd_FGColor(0xff0000);
        FTImpl.Tag(22);
        FTImpl.Cmd_Button(200, 120, 100, 40, 27, FT_OPT_FLAT, "P.ziarnowy");
      }
      if (buzzer_3 == 1) {
        FTImpl.Cmd_FGColor(0xff0000);
        FTImpl.Tag(23);
        FTImpl.Cmd_Button(300, 120, 90, 40, 27, FT_OPT_FLAT, "P.klosowy");
      }
      /*
        if (buzzer_4 == 1) {
        FTImpl.Cmd_FGColor(0xff0000);
        FTImpl.Tag(24);
        FTImpl.Cmd_Button(390, 120, 90, 40, 27, FT_OPT_FLAT, "P.pochyly");
        }
      */
      if (buzzer_5 == 1) {
        FTImpl.Cmd_FGColor(0xff0000);
        FTImpl.Tag(25);
        FTImpl.Cmd_Button(390, 160, 90, 40, 27, FT_OPT_FLAT, "Wytrzasacz");
      }
      if (buzzer_6 == 1) {
        FTImpl.Cmd_FGColor(0xff0000);
        FTImpl.Tag(26);
        FTImpl.Cmd_Button(300, 160, 90, 40, 27, FT_OPT_FLAT, "Sieczkarnia");
      }
    }

    if (zmienna == 2) {//////////////////////////////////////////////////////////////////////
      FTImpl.ColorRGB(255  , 200   , 50 );
      if (stan_heder == 2) {

        FTImpl.Cmd_Text(400, 125, 24, 0, "Reset");
        dlugosc_hektary_480 = 0;
        dlugosc_hektary_240 = 0;
        dlugosc_hektary_120 = 0;
        dlugosc_hektary_160 = 0;
        dlugosc_hektary_360 = 0;
        dlugosc_hektary_375 = 0;
        dlugosc_hektary_300 = 0;
        dlugosc_hektary_225 = 0;
        dlugosc_hektary_150 = 0;
        dlugosc_hektary_75 = 0;
        hektary = 0;
        hektary_480 = 0;
        hektary_240 = 0;
        hektary_120 = 0;
        hektary_160 = 0;
        hektary_360 = 0;
        hektary_375 = 0;
        hektary_300 = 0;
        hektary_225 = 0;
        hektary_150 = 0;
        hektary_75 = 0;

      }
      if (stan_heder == 1) {

        FTImpl.Cmd_Text(400, 125, 24, 0, "Licze");
      }
      if (stan_heder == 0) {
        FTImpl.Cmd_Text(400, 125, 24, 0, "Stop");
      }
      FTImpl.Cmd_Text(10, 135, 30, 0, char_hektary);
      FTImpl.Cmd_Text(0, 115, 23, 0, "hektary:");
      FTImpl.ColorRGB(0, 255, 128  );

      FTImpl.Cmd_Text(10, 35, 30, 0, char_droga);

      FTImpl.Cmd_Text(0, 0, 23 , 0, "dlugosc (m):");
      if (droga_liczenie == 1) {
        FTImpl.Cmd_Text(400, 25, 24, 0, "Licze");
      } if (droga_liczenie == 0) {
        FTImpl.Cmd_Text(400, 25, 24, 0, "Stop");
      }//F4C430
      if (droga_liczenie == 2) {
        droga = 0;
        FTImpl.Cmd_Text(400, 25, 24, 0, "Reset");
      }
      FTImpl.ColorRGB(255  , 255   , 255 );
      FTImpl.Cmd_FGColor(0x808080  );
      FTImpl.Tag(6);
      FTImpl.Cmd_Button(150, 25, 75, 50, 27, FT_OPT_FLAT, "start");
      FTImpl.Tag(7);
      FTImpl.Cmd_Button(230, 25, 75, 50, 27, FT_OPT_FLAT, "stop");
      FTImpl.Tag(10);
      FTImpl.Cmd_Button(310, 25, 75, 50, 27, FT_OPT_FLAT, "reset");

      FTImpl.Tag(13);
      FTImpl.Cmd_Button(310, 120, 75, 50, 27, FT_OPT_FLAT, "reset");
      FTImpl.Tag(11);
      FTImpl.Cmd_Button(150, 120, 75, 50, 27, FT_OPT_FLAT, "start");
      FTImpl.Tag(12);
      FTImpl.Cmd_Button(230, 120, 75, 50, 27, FT_OPT_FLAT, "stop");


      if (tagval == 11) {

        stan_heder = 1;
      }
      if (tagval == 12) {
        stan_heder = 0;

      }

      if (tagval == 13) {
        stan_heder = 2;

      }


      if (tagval == 6) {
        droga_liczenie = 1;

      }
      if (tagval == 7) {
        droga_liczenie = 0;
      }
      if (tagval == 10) {
        droga_liczenie = 2;

      }





    }

    FTImpl.ColorRGB(0xff, 0xff, 0xff);
    FTImpl.Cmd_FGColor(0x00a000);


    FTImpl.Cmd_FGColor(0x964B00);
    FTImpl.Tag(1);
    FTImpl.Cmd_Button(0, 210, 75, 60, 27, FT_OPT_FLAT, "MENU");
    FTImpl.Cmd_FGColor(0x808080  );
    FTImpl.Tag(2);
    FTImpl.Cmd_Button(80, 210, 75, 60, 27, FT_OPT_FLAT, "<AA>");
    FTImpl.Cmd_FGColor(0x0027C2);
    FTImpl.Tag(3);
    FTImpl.Cmd_Button(160, 210, 75, 60, 27, FT_OPT_FLAT, "HEDER");
    FTImpl.Cmd_FGColor(0x618358);
    FTImpl.Tag(4);
    FTImpl.Cmd_Button(240, 210, 75, 60, 27, FT_OPT_FLAT, "CZUJNIKI");
    FTImpl.Cmd_FGColor(0xCFB53B );
    FTImpl.Tag(5);
    FTImpl.Cmd_Button(320, 210, 75, 60, 27, FT_OPT_FLAT, "EKRAN");


    FTImpl.DLEnd();
    FTImpl.Finish();



    FTImpl.Cmd_Track( 183, 119, 0, 0, 12);


    FTImpl.Finish();
  }


  if (ET.receiveData())
  {


  }


  liczenie();

}

void liczenie() {

  //r=3.14*0.7*2;
  w = myData.predkosctyl;
  V = w ;

  predkosc.update();


  if (predkosc.fell()) {
    if ((digitalRead(czuj_heder) == HIGH && stan_heder == 1) || (digitalRead(czuj_heder) == LOW && stan_heder == 0) || (digitalRead(czuj_heder) == HIGH && stan_heder == 0)) {
      dlugosc_hektary_480 = dlugosc_hektary_480;
      dlugosc_hektary_240 = dlugosc_hektary_240;
      dlugosc_hektary_160 = dlugosc_hektary_160;
      dlugosc_hektary_120 = dlugosc_hektary_120;
      dlugosc_hektary_360 = dlugosc_hektary_360;
      dlugosc_hektary_375 = dlugosc_hektary_375;
      dlugosc_hektary_300 = dlugosc_hektary_300;
      dlugosc_hektary_225 = dlugosc_hektary_225;
      dlugosc_hektary_150 = dlugosc_hektary_150;
      dlugosc_hektary_75 = dlugosc_hektary_75;

    }
    if (digitalRead(czuj_heder) == LOW && stan_heder == 1 ) {
      if (szerokosc_heder == 480) {
        dlugosc_hektary_480 += r;
      }
      if (szerokosc_heder == 240) {
        dlugosc_hektary_240 += r;
      }
      if (szerokosc_heder == 120) {
        dlugosc_hektary_120 += r;
      }
      if (szerokosc_heder == 160) {
        dlugosc_hektary_160 += r;
      }
      if (szerokosc_heder == 360) {
        dlugosc_hektary_360 += r;
      }
      if (szerokosc_heder == 375) {
        dlugosc_hektary_375 += r;
      }
      if (szerokosc_heder == 300) {
        dlugosc_hektary_300 += r;
      }
      if (szerokosc_heder == 225) {
        dlugosc_hektary_225 += r;
      }
      if (szerokosc_heder == 150) {
        dlugosc_hektary_150 += r;
      }
      if (szerokosc_heder == 75) {
        dlugosc_hektary_75 += r;
      }
    }


    if (droga_liczenie == 1) {

      droga += r;
    }
    if (droga_liczenie == 0) {
      droga = droga;
    }

    metry_hektary = metry_hektary + r;
    i = millis() - buttonPressTimeStamp;
    buttonPressTimeStamp = millis();

  }


    prz_dol.read();
    if (prz_dol.wasPressed()) {
      //kogut = 0;
      zbiornik_boolean_75 = true;


    }


    if (zbiornik_boolean_75 == true ) {

      if (zbiornik_timer.available()) {
        liczenie_zbiornik++;
        
        zbiornik_timer.restart();
      }
     


      if (liczenie_zbiornik >= 1) {

        zbiornik_boolean_75 = false;
        liczenie_zbiornik = 0;
        //kogut = 1;
      }
    }
    /*
      else {
      if (zbiornik_boolean_75 == false) {

        if (buzzer == 0 && buzzer_1 == 0 && buzzer_2 == 0 && buzzer_3 == 0 && buzzer_4 == 0 && buzzer_5 == 0 && buzzer_6 == 0 ) {

          digitalWrite(5, HIGH);
        }
      }

      }
    */
    prz_gora.read();
    if (prz_gora.wasPressed()) {
      zbiornik_boolean_100 = true;
      kogut = 0;

    }
    if (prz_gora.wasReleased()) {
      kogut = 1;

    }

    if (zbiornik_boolean_100 == true ) {

      if (zbiornik_timer_100.available()) {
        liczenie_zbiornik_100++;

        zbiornik_timer_100.restart();
      }


      if (liczenie_zbiornik_100 >= 1) {
        zbiornik_boolean_100 = false;
        liczenie_zbiornik_100 = 0;
      }
    }



  if (myData.czuj_sieczkarnia == 0) {
    zmienna = 4;
    czujnik_sieczka = 0;
  }
  if (myData.czuj_sieczkarnia == 1) {
    czujnik_sieczka = 1;

  }


//if(myData.obrotysilnik>2000){
  if (myData.int_obrotymlocarni > 800 ) {
    buzzer_mlocarnia = 1;
  }
  if (myData.int_obrotymlocarni < 800  && buzzer_mlocarnia == 1 && czujnik_od_mlocarni == 1) {
    zmienna = 4;
    buzzer = 1;

  } else if (myData.int_obrotymlocarni > 800 || buzzer_mlocarnia == 0) {
    buzzer = 0;

  }




  if (myData.int_obrotywialni > 60 ) {
    buzzer_wialni = 1;
  }
  if (myData.int_obrotywialni < 60 && buzzer_wialni == 1 && czujnik_od_mlocarni == 1 ) {
    zmienna = 4;
    buzzer_1 = 1;

  } else if (myData.int_obrotywialni > 60 || buzzer_wialni == 0) {
    buzzer_1 = 0;

  }


  if ( myData.podajnik_ziarnowy > 60 ) {
    buzzer_ziarnowy = 1;
  }
  if (myData.podajnik_ziarnowy < 60 && buzzer_ziarnowy == 1 && czujnik_od_mlocarni == 1) {
    zmienna = 4;
    buzzer_2 = 1;

  } else if (myData.podajnik_ziarnowy > 60 || buzzer_ziarnowy == 0) {
    buzzer_2 = 0;

  }



  if ( myData.podajnik_klosowy > 60 ) {
    buzzer_klosowy = 1;
  }
  if (myData.podajnik_klosowy < 60 && buzzer_klosowy == 1 && czujnik_od_mlocarni == 1) {
    zmienna = 4;
    buzzer_3 = 1;

  } else if (myData.podajnik_klosowy > 60 || buzzer_klosowy == 0) {
    buzzer_3 = 0;

  }




  if ( myData.podajnik_pochyly > 60 ) {
    buzzer_pochyly = 1;
  }
  if (myData.podajnik_pochyly < 60 && buzzer_pochyly == 1 && czujnik_od_mlocarni == 1) {
    //zmienna = 4;
    // buzzer_4 = 1;

  } else if (myData.podajnik_pochyly > 60 || buzzer_pochyly == 0) {
    //buzzer_4 = 0;

  }


  if ( myData.wytrzasacze > 60 ) {
    buzzer_wytrzasacz = 1;
  }
  if (myData.wytrzasacze < 60 && buzzer_wytrzasacz == 1 && czujnik_od_mlocarni == 1 ) {
    zmienna = 4;
    buzzer_5 = 1;

  } else if (myData.wytrzasacze > 60 || buzzer_wytrzasacz == 0) {
    buzzer_5 = 0;

  }

  if ( myData.sieczkarnia > 60 ) {
    buzzer_sieczkarnia = 1;
  }
  if (myData.sieczkarnia < 60 && buzzer_sieczkarnia == 1 && czujnik_od_mlocarni == 1) {
    zmienna = 4;
    buzzer_6 = 1;

  } else if (myData.sieczkarnia > 60 || buzzer_sieczkarnia == 0 ) {
    buzzer_6 = 0;

  }
//}
  if (buzzer == 1 || buzzer_1 == 1 || buzzer_2 == 1 || buzzer_3 == 1 || buzzer_5 == 1  || buzzer_6 == 1  || zbiornik_boolean_75 == 1 || zbiornik_boolean_100 == 1) {
    //if (buzzerTimer.available()) {
   
      digitalWrite(5, LOW);
      // buzzerTimer.restart();
    

  } if (buzzer == 0 && buzzer_1 == 0 && buzzer_2 == 0 && buzzer_3 == 0 /*&& buzzer_4 == 0 */ && buzzer_5 == 0 && buzzer_6 == 0 && zbiornik_boolean_75 == 0 && zbiornik_boolean_100 == 0 ) {
    digitalWrite(5, HIGH);

  }
}
