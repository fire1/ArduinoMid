/* 
 Horhomun
 pavel@vasilenok.by
 Viber: +375 29 3487042
 
 6 марта 2015 года
 
 Бортовой компьютер для автомобиля на arduino + bluetooth hc-05 + elm327
 
 Ссылка на статью: http://habrahabr.ru/post/252207/
 */

#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>

#define RxD 7                //Arduino pin connected to Tx of HC-05
#define TxD 8                //Arduino pin connected to Rx of HC-05

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // LiquidCrystal(rs, enable, d4, d5, d6, d7)

SoftwareSerial blueToothSerial(RxD,TxD);

// ТРЕБУТ КОРРЕКТИРОВКИ----------------------------------------------------------------------------------------
double tcorrect=1.014;         // ВНИМАНИЕ!!! Корректировка коэффициента времени arduino uno, у каждого свой.
double speed_korrect_val=1.0;  // ВНИМАНИЕ!!! Корректировка коэффициента скорости, посмотреть по GPS
double VE_correct=1.0;         // ВНИМАНИЕ!!! Корректировка Объёмного КПД ДВС:  добиваемся чтобы мгновенный расход на холостых оборотах был в половину объема двигателя
double ED = 1.998;             // ВНИМАНИЕ!!! Объем двигателя в литрах (пример: 1.398)
byte delay_var=65;             // ВНИМАНИЕ!!! Поправьте задержку между запросами если ЭБУ не выдает показания правильно, 50-300 мс (пример: duster - 65, cirrus - 235)
//-------------------------------------------------------------------------------------------------------------


// НЕ НАДО ИЗМЕНЯТЬ -------------------------------------------------------------------------------------------
byte count_display=20; // Количество экранов бк
byte pin=10, pin2=9;  // пины кнопок
int engine_on_rpm=400; // обороты при которых считать двигатель заведенным
double AirFuelRatio = 14.70;  // константа расхода 14,7 воздуха к 1 литра бензина, у дизеля своя, у газа своя
double FuelDensityGramsPerLiter = 750.0;   // константа - грамм бензина в 1 литре бензина
//-------------------------------------------------------------------------------------------------------------


boolean off2=true;

char v1, v2, v3, v4, v5; 
char rxData[20];
char rxIndex=0;

byte selmon, off, value, value2, t1, kol_check_engine_error_val, fss_val;

int speed_error, tmp_error, rpm_error, dvk_error, iat_error, rnd_error, dts_error, uoz_error, maf_error, pdz_error, ut_error, tm_error;

long dvk_var, intake_air_temp_var, davlenie_topliva_var, tmp_masla_var, speed_var, tmp_var, t2, check_engine_km, check_engine_flash;

unsigned long time_new, time_old, time_old_gurnal;

double VE, rpm_var, uoz_var, raschet_nagruzka_dvigatelya_var, maf_var, polozh_dross_zaslon, uroven_topliva_var, IMAP, MAF, FuelFlowGramsPerSecond,FuelFlowLitersPerSecond,time,
odometr, benz_potracheno, LPH, LP100, odometr_add, benz_add, odometr_add_gurnal, benz_add_gurnal, long_term_val, short_term_val, b1s1_val, b1s2_val, ls_term_val;

byte grad[8] =    // символ градуса
{
  B01100,
  B10010,
  B10010,
  B01100,
  B00000,
  B00000,
  B00000,
  B00000,
};

void setup()
{
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);  
  lcd.begin(20, 4);
  pinMode(pin,INPUT);
  digitalWrite(pin,HIGH);
  pinMode(pin2,INPUT);
  digitalWrite(pin2,HIGH);

  lcd.setCursor(0,0);
  lcd.print("Connecting...");
  delay(2000);

  value=digitalRead(pin); // Бутон за проверка, за да въведете recovery

  if (value==LOW) {
    off=222;
    count_display=count_display+3;  // 3 технология скрит екран
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("|----[recovery]----|");   //забраните дърпа данни от ELM327 чрез натискане на бутона при писане Connecting...
    tone(13, 3000, 500);
    delay(2000);
    value=digitalRead(pin); // Бутон за проверка, за да въведете admin
    if (value==LOW) {
      off2=false; // отключаем функцию getResponse, а то ничего не будет показывать без связи с блютуз
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("|----[all off]----|");   //отключаем вытаскивание данных из elm327
      tone(13, 3000, 500);
      delay(2000);
    }  
  } 

  lcd.clear();
  lcd.print("Connecting...[ok]");
  delay(1000);
  blueToothSerial.flush();  
  blueToothSerial.begin(38400); //default communication baud rate of HC-05 is 38400
  delay(delay_var); 
  blueToothSerial.println("ATZ"); //rezet
  delay(900); 
  blueToothSerial.println("ATSP0"); // протокол ATSP0 - авто, cirrus ATSP3
  delay(100); 
  lcd.clear();

  SearchErrorEngine(); 

  if (kol_check_engine_error_val>0) {

    tone(13, 3000, 10000);

    lcd.setCursor(0,0);
    lcd.print("Check Engine Error!!");   //выводим сообщение что есть ошибки двигателя
    lcd.setCursor(0,1);
    lcd.print("Kolichestvo oshibok:");
    lcd.setCursor(0,2);
    lcd.print(kol_check_engine_error_val);

    delay(delay_var); 
    blueToothSerial.flush();
    blueToothSerial.print("03");  //  	Error codes view
    blueToothSerial.print("\r");
    getResponse();
    getResponse();  
    if (strtol(&rxData[0],0,16)==67) {
      lcd.setCursor(0,3); 
      lcd.print("Kod: P");
      lcd.setCursor(7,3);
      lcd.print(&rxData[3]);    // можно и так String(strtol(&rxData[0],0,16), HEX) выводит hex число
      lcd.setCursor(9,3);
      lcd.print(&rxData[6]);
    }
    delay(10000);
    lcd.clear();
  }

  lcd.createChar(1, grad);
}

void loop()
{ 
  IMAP=0;
  MAF=0;
  FuelFlowGramsPerSecond=0;
  FuelFlowLitersPerSecond=0;
  LPH=0;
  odometr_add=0;
  benz_add=0;
  ls_term_val=0;


  value=digitalRead(pin);  // проверка состояния кнопки для переключения экрана
  value2=digitalRead(pin2); 

  if (value==LOW) {
    if (selmon==count_display-3) {
      lcd.clear();
      selmon=0; 
    }  
    else {
      lcd.clear();                                                                                                                 
      selmon++;  
    }
    tone(13, 3000, 100);
    delay(150);
  }

  if (value2==LOW) {
    if (selmon==0) {
      lcd.clear();
      selmon=count_display-3; 
    }  
    else {
      lcd.clear();                                                                                                                 
      selmon--;  
    }
    delay(150);
    tone(13, 3000, 100);
  }

  /*  IMAP = RPM * MAP / IAT
   MAF = (IMAP/120)*(VE/100)*(ED)*(MM)/(R)
   	
   MAP - Manifold Absolute Pressure in kPa
   IAT - Intake Air Temperature in Kelvin
   R  -  Specific Gas Constant (8.314472 J/(mol.K)
   MM -  Average molecular mass of air (28.9644 g/mol)
   VE -  volumetric efficiency measured in percent, let's say 80%
   ED - Engine Displacement in liters   */

  if (off2)
  { 

    if ((t1==60) or (t2==10)) { // чтение температуры и вольтажа каждые 100 циклов работы (примерно 30 секунд), первый раз чтение выполниться через примерно 5 секунд со старта
      Tmp();                                    
      Voltage(); 
      LongTerm();
      t1=0;
      t2++;
    }  
    else {  
      t1++; 
      t2++;
    }

    Rpm();

  } 
  else {

    delay(300); 

  }

  if (rpm_var>engine_on_rpm) {     // если обороты больше engine_on_rpm то считаем расход и расстояние (машина заведена)

    FuelSystemStatus(); 

    Rnd(); // вытягиваем информацию о нагрузка двигателя, если нету такго параметра в ЭБУ то закомментировать строчку символами //

    if (raschet_nagruzka_dvigatelya_var>5.0)  {  // Проверка торможения двигателем по нагрузке двигателя, если информации о нагрузке в ЭБУ нету то поменять на ((fss_val==2) or (speed_var>70))

      ShortTerm();  
      IntakeAirTemp();
      DavlenVpuskKoll();



if (rpm_var < 800) {
  VE = 64;
}
else {
  if (rpm_var < 1001) {
    VE = 66;
  }
  else {
    if (rpm_var < 1251) {
      VE = 70;
    }
    else {

      if (rpm_var < 1501) {
        VE = 70;
      }
      else {

        if (rpm_var < 1751) {
          VE = 74;
        }
        else {

          if (rpm_var < 2001) {
            VE = 74;
          }
          else {

            if (rpm_var < 2126) {
              VE = 76;
            }
            else {

              if (rpm_var < 2251) {
                VE = 77;
              }
              else {

                if (rpm_var < 2376) {
                  VE = 79;
                }
                else {

                  if (rpm_var < 2501) {
                    VE = 80;
                  }
                  else {

                    if (rpm_var < 2626) {
                      VE = 82;
                    }
                    else {

                      if (rpm_var < 2751) {
                        VE = 82;
                      }
                      else {

                        if (rpm_var < 2876) {
                          VE = 83;
                        }
                        else {

                          if (rpm_var < 3001) {
                            VE = 84;
                          }
                          else {

                            if (rpm_var < 3501) {
                              VE = 85;
                            }
                            else {

                              if (rpm_var < 4001) {
                                VE = 86;
                              }
                              else {

                                if (rpm_var < 4501) {
                                  VE = 88;
                                }
                                else {

                                  if (rpm_var < 5001) {
                                    VE = 90;
                                  }
                                  else {

                                    if (rpm_var < 5501) {
                                      VE = 93;
                                    }
                                    else {

                                      if (rpm_var < 6001) {
                                        VE = 95;
                                      }
                                      else {

                                        if (rpm_var < 6501) {
                                          VE = 91;
                                        }
                                        else {

                                          if (rpm_var < 7000) {
                                            VE = 85;
                                          }
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}



      IMAP=double(rpm_var*dvk_var)/double(intake_air_temp_var+273.15);

      MAF=double(IMAP/120.0)*double(double(VE*VE_correct)/100.0)*ED*28.9644/8.314472;      


      if (fss_val==2) {   // если замкнутая обратная связь  - Closed Loop
        ls_term_val=double(100.0+(long_term_val+short_term_val))/100.0; // коэффициент корректировки расхода по ShortTerm и LongTerm    
      } 
      else {
        ls_term_val=double(100.0+long_term_val)/100.0; // коэффициент корректировки расхода по LongTerm  
      }

      FuelFlowGramsPerSecond = double(MAF/AirFuelRatio)*ls_term_val;   // Получаем расход грамм бензина в секунду в соотношении 14,7 воздуха/к 1 литра бензина, корректировка ls_term_val
      FuelFlowLitersPerSecond = FuelFlowGramsPerSecond / FuelDensityGramsPerLiter;  // Переводим граммы бензина в литры
      LPH = FuelFlowLitersPerSecond * 3600.0;       // Ковертирование литров в час
    }

    Speed(); // получить скорость 

    if (time_old==0) { 
      time_old = millis();  // выполнится один раз при появлении оборотов   
    }  

    time_new = millis();   // время со старта программы в мс
    time=(double(time_new-time_old)/1000.0)*tcorrect;     // прошло время с последнего расчета скорости, расхода  - в сек
     if (time>10) {	time=0; }
    time_old = time_new;  // записать новое время для сравнения в следующем цикле

    if (speed_var>0) { 
      odometr_add=double((double(speed_var*1000.0)/3600.0)*time)/1000.0;
      odometr=odometr+odometr_add;    //обший пробег в км
    }
    benz_add=FuelFlowLitersPerSecond*time;
    benz_potracheno=benz_potracheno+benz_add;  // общий расход в литрах

    if (((speed_var>1) and (speed_var<10) and ((time_new-time_old_gurnal)>30000)) or ((speed_var==0) and ((time_new-time_old_gurnal)>10000))) {
      double odometr_eeprom=EEPROM_float_read(111)+odometr_add_gurnal+odometr_add;
      double benz_eeprom=EEPROM_float_read(122)+benz_add_gurnal+benz_add;
      EEPROM_float_write(111, (odometr_eeprom)); // записываем в энергонезависимую память журнала расстояние каждые 5 секунд... в памяти занимаются ячейкм 111,112,113,114
      EEPROM_float_write(122, (benz_eeprom));  // записываем бензин
      odometr_add_gurnal=0;
      benz_add_gurnal=0;
      time_old_gurnal=time_new;
    }  
    else {    
      odometr_add_gurnal=odometr_add_gurnal+odometr_add;
      benz_add_gurnal=benz_add_gurnal+benz_add;    
    }
  }

  if (odometr>0) {                                                            
    LP100=(benz_potracheno/odometr)*100.0;                                          //расход бензина на 100 км (в литрах)
  } 

  double full_odometr_gurnal=EEPROM_float_read(111);
  double full_benz_gurnal=EEPROM_float_read(122);

  switch (selmon) {
  case 0:

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(speed_var); // Скорость 
    lcd.print(" km/h");
    lcd.setCursor(12,0);
    lcd.print(int(rpm_var)); //Обороты
    lcd.print(" rpm");
    lcd.setCursor(0,1); 
    lcd.print(LPH); // л/час
    lcd.print(" L/h");
    lcd.setCursor(12,1); 
    if (v1!=49) {
      lcd.print("-"); //Напряжение
      lcd.print("-"); 
      lcd.print("."); 
      lcd.print("-");
    }   
    else {
      lcd.print(v1); //Напряжение
      lcd.print(v2); 
      lcd.print(v3); 
      lcd.print(v4); 
    }
    lcd.print(" V");
    lcd.setCursor(0,2); 
    if (odometr>0.1) {   // отображать расход на 100 км только после 100 метров пробега
      lcd.print(LP100); 
    }  
    else { 
      lcd.print("-.--"); 
    } 
    lcd.print(" L/100");
    lcd.setCursor(12,2); 
    lcd.print(tmp_var);  //Температура    
    lcd.print(" \1");
    lcd.print("C");
    lcd.setCursor(0,3); 
    lcd.print(odometr); // пройденный путь с заводки авто
    lcd.print(" km");
    lcd.setCursor(12,3); 
    lcd.print(benz_potracheno); // бензина потрачено с заводки авто
    lcd.print(" L");

    break;

  case 1:

    value=digitalRead(pin);  // проверка состояния кнопки для переключения экрана
    value2=digitalRead(pin2); 

    if ((value==LOW) and (value2==LOW)) {
      float odometr0=0.00, benz0=0.00;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("  Ochistka jurnala! ");
      lcd.setCursor(0,1);
      lcd.print("--------------------");
      lcd.setCursor(0,2);
      lcd.print(" Gdi 3 secundy dlya ");
      lcd.setCursor(0,3);
      lcd.print("     ochistki..     ");
      delay(3000);
      lcd.clear();
      value=digitalRead(pin);  // проверка состояния кнопки для переключения экрана
      value2=digitalRead(pin2); 

      if ((value==LOW) and (value2==LOW)) {
        EEPROM_float_write(111, odometr0);
        EEPROM_float_write(122, benz0);
        lcd.setCursor(0,1);
        lcd.print("|----[Clear OK]----|");
        delay(2000);
      }   
      else {
        lcd.setCursor(0,1);
        lcd.print("|----[Otmeneno]----|");
        delay(2000);
      }
    }

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(full_odometr_gurnal); // Полное расстояние 
    lcd.print(" km");
    lcd.setCursor(12,0);
    lcd.print(full_benz_gurnal); // Весь потраченный бензин
    lcd.print(" L");

    if (full_odometr_gurnal>0) {
      lcd.setCursor(0,1);
      lcd.print((full_benz_gurnal/full_odometr_gurnal)*100.0); // Полный средний расход
      lcd.print(" L/100");

    }


    break;

  case 2:
    lcd.setCursor(0,0); 
    lcd.print(selmon);
    lcd.print(".");
    lcd.print("Nagruzka dvigatela");
    lcd.setCursor(0,3);
    lcd.print(raschet_nagruzka_dvigatelya_var); 
    lcd.print(" % ");

    if (off==222) {         

      lcd.setCursor(0,1); 
      lcd.print(&rxData[0]);
      lcd.setCursor(3,1);
      lcd.print(&rxData[3]);
      lcd.setCursor(6,1);
      lcd.print(&rxData[6]);    
      lcd.setCursor(9,1);
      lcd.print("-");
      lcd.print(rnd_error);
    }

    break;

  case 3:

    Tmp();   
    lcd.setCursor(0,0); 
    lcd.print(selmon);
    lcd.print(".");
    lcd.print("Temperatura");
    lcd.setCursor(0,1); 
    lcd.print("ohlazhdaiushchej");
    lcd.setCursor(0,2); 
    lcd.print("zhidkosti");
    lcd.setCursor(0,3);
    lcd.print(tmp_var); 
    lcd.print(" \1");
    lcd.print("C  ");

    if (off==222) {        
      lcd.setCursor(9,3); 
      lcd.print(&rxData[0]);
      lcd.setCursor(12,3);
      lcd.print(&rxData[3]);
      lcd.setCursor(15,3);
      lcd.print(&rxData[6]);
      lcd.setCursor(18,3);
      lcd.print("-");
      lcd.print(tmp_error);
    }

    break;

  case 4:

    Pdz();
    lcd.setCursor(0,0); 
    lcd.print(selmon);
    lcd.print(".");
    lcd.print("Polozhenie");
    lcd.setCursor(0,1); 
    lcd.print("drosselnoj zaslonki");
    lcd.setCursor(0,3);
    lcd.print(polozh_dross_zaslon); 
    lcd.print(" %  ");

    if (off==222) {         
      lcd.setCursor(0,2); 
      lcd.print(&rxData[0]);
      lcd.setCursor(3,2);
      lcd.print(&rxData[3]);
      lcd.setCursor(6,2);
      lcd.print(&rxData[6]);
      lcd.setCursor(9,2);
      lcd.print("-");
      lcd.print(pdz_error);
    }

    break;

  case 5:

    if (rpm_var<engine_on_rpm) {  
      DavlenVpuskKoll();
    }     
    lcd.setCursor(0,0); 
    lcd.print(selmon);
    lcd.print(".");
    lcd.print("Davlenie");
    lcd.setCursor(0,1); 
    lcd.print("vpusknogo kollektora");
    lcd.setCursor(0,3);
    lcd.print(dvk_var); 
    lcd.print(" kPa  ");

    if (off==222) { 
      DavlenVpuskKoll();    
      lcd.setCursor(0,2); 
      lcd.print(&rxData[0]);
      lcd.setCursor(3,2);
      lcd.print(&rxData[3]);
      lcd.setCursor(6,2);
      lcd.print(&rxData[6]);
      lcd.setCursor(9,2);
      lcd.print("-");
      lcd.print(dvk_error);
    }

    break;

  case 6:

    lcd.setCursor(0,0); 
    lcd.print(selmon);
    lcd.print(".");
    lcd.print("Obototy dvigatelya");
    lcd.setCursor(0,3);
    lcd.print(rpm_var); 
    lcd.print(" rpm   ");

    if (off==222) {     
      Rpm();    
      lcd.setCursor(0,1); 
      lcd.print(&rxData[0]);
      lcd.setCursor(3,1);
      lcd.print(&rxData[3]);
      lcd.setCursor(6,1);
      lcd.print(&rxData[6]);
      lcd.setCursor(9,1);
      lcd.print(&rxData[9]);
      lcd.setCursor(12,1);
      lcd.print("-");
      lcd.print(rpm_error);   
    }

    break;

  case 7:

    lcd.setCursor(0,0); 
    lcd.print(selmon);
    lcd.print(".");
    lcd.print("Skorost avto");
    lcd.setCursor(0,3);
    lcd.print(speed_var); 
    lcd.print(" km/h  ");

    if (off==222) {     
      Speed();    
      lcd.setCursor(0,1); 
      lcd.print(&rxData[0]);
      lcd.setCursor(3,1);
      lcd.print(&rxData[3]);
      lcd.setCursor(6,1);
      lcd.print(&rxData[6]);
      lcd.setCursor(9,1);
      lcd.print("-");
      lcd.print(speed_error);
    }   

    break;

  case 8:

    Uoz();
    lcd.setCursor(0,0); 
    lcd.print(selmon);
    lcd.print(".");
    lcd.print("UOZ");
    lcd.setCursor(0,3);
    lcd.print(uoz_var); 
    lcd.print(" \1  ");

    if (off==222) {         
      lcd.setCursor(0,1); 
      lcd.print(&rxData[0]);
      lcd.setCursor(3,1);
      lcd.print(&rxData[3]);
      lcd.setCursor(6,1);
      lcd.print(&rxData[6]);
      lcd.setCursor(9,1);
      lcd.print("-");
      lcd.print(uoz_error);
    }

    break;

  case 9:

    if (rpm_var<engine_on_rpm) {  
      IntakeAirTemp();
    }      

    lcd.setCursor(0,0); 
    lcd.print(selmon);
    lcd.print(".");
    lcd.print("Temperatura");
    lcd.setCursor(0,1); 
    lcd.print("vsasyvaemogo vozduha");
    lcd.setCursor(0,3);
    lcd.print(intake_air_temp_var); 
    lcd.print(" \1");
    lcd.print("C  ");

    if (off==222) {         
      lcd.setCursor(0,2); 
      lcd.print(&rxData[0]);
      lcd.setCursor(3,2);
      lcd.print(&rxData[3]);
      lcd.setCursor(6,2);
      lcd.print(&rxData[6]);
      lcd.setCursor(9,2);
      lcd.print("-");
      lcd.print(iat_error);
    }

    break;

  case 10:

    Voltage();
    lcd.setCursor(0,0); 
    lcd.print(selmon);
    lcd.print(".");
    lcd.print("Napryazhenie");
    lcd.setCursor(0,3);    
    if (v1!=49) {
      lcd.print("-"); //Напряжение
      lcd.print("-"); 
      lcd.print("."); 
      lcd.print("-");
    }  
    else {
      lcd.print(v1); //Напряжение
      lcd.print(v2); 
      lcd.print(v3); 
      lcd.print(v4); 
    }
    lcd.print(" V   ");

    break;

  case 11:

    B1S1();
    B1S2();   
    lcd.setCursor(0,0); 
    lcd.print(selmon);
    lcd.print(".");
    lcd.print("Oxygen B1S1:");
    lcd.setCursor(0,1); 
    lcd.print(b1s1_val);
    lcd.print(" V    "); 
    lcd.setCursor(0,2); 
    lcd.print("Oxygen B1S2:"); 
    lcd.setCursor(0,3); 
    lcd.print(b1s2_val);
    lcd.print(" V    ");  

    break;

  case 12:

    lcd.setCursor(0,0); 
    lcd.print(selmon);
    lcd.print(".");
    lcd.print("Short fuel trim");
    lcd.setCursor(0,1); 
    lcd.print(short_term_val);
    lcd.print(" %    "); 
    lcd.setCursor(0,2); 
    lcd.print("Long fuel trim"); 
    lcd.setCursor(0,3); 
    lcd.print(long_term_val);
    lcd.print(" %    "); 

    break;

  case 13:

    Dts();
    lcd.setCursor(0,0); 
    lcd.print(selmon);
    lcd.print(".");
    lcd.print("Davlenie topliva");
    lcd.setCursor(0,3);
    lcd.print(davlenie_topliva_var); 
    lcd.print(" kPa  ");

    if (off==222) {        
      lcd.setCursor(0,1); 
      lcd.print(&rxData[0]);
      lcd.setCursor(3,1);
      lcd.print(&rxData[3]);
      lcd.setCursor(6,1);
      lcd.print(&rxData[6]);
      lcd.setCursor(9,1);
      lcd.print("-");
      lcd.print(dts_error);
    }   

    break;


  case 14:

    Uroventopliva();
    lcd.setCursor(0,0); 
    lcd.print(selmon);
    lcd.print(".");
    lcd.print("Uroven topliva");
    lcd.setCursor(0,3);
    lcd.print(uroven_topliva_var); 
    lcd.print(" %  ");

    if (off==222) {         
      lcd.setCursor(0,1); 
      lcd.print(&rxData[0]);
      lcd.setCursor(3,1);
      lcd.print(&rxData[3]);
      lcd.setCursor(6,1);
      lcd.print(&rxData[6]);
      lcd.setCursor(9,1);
      lcd.print("-");
      lcd.print(ut_error);
    }

    break;

  case 15:

    Maf();
    lcd.setCursor(0,0); 
    lcd.print(selmon);
    lcd.print(".");
    lcd.print("Massovyj rasshod");
    lcd.setCursor(0,1); 
    lcd.print("vozduha");
    lcd.setCursor(0,3);
    lcd.print(maf_var); 
    lcd.print(" gramm/sec  ");

    if (off==222) {         
      lcd.setCursor(0,2); 
      lcd.print(&rxData[0]);
      lcd.setCursor(3,2);
      lcd.print(&rxData[3]);
      lcd.setCursor(6,2);
      lcd.print(&rxData[6]);
      lcd.setCursor(9,2);
      lcd.print(&rxData[9]);   
      lcd.setCursor(12,2);
      lcd.print("-");
      lcd.print(maf_error);
    }

    break;

  case 16:

    Tempmasla();
    lcd.setCursor(0,0); 
    lcd.print(selmon);
    lcd.print(".");
    lcd.print("Temperatura masla");
    lcd.setCursor(0,1); 
    lcd.print("dvigatelya");
    lcd.setCursor(0,3);
    lcd.print(tmp_masla_var); 
    lcd.print(" \1");
    lcd.print("C  ");

    if (off==222) {         
      lcd.setCursor(0,2); 
      lcd.print(&rxData[0]);
      lcd.setCursor(3,2);
      lcd.print(&rxData[3]);
      lcd.setCursor(6,2);
      lcd.print(&rxData[6]);
      lcd.setCursor(9,2);
      lcd.print("-");
      lcd.print(tm_error);
    } 

    break;

  case 17:
    if (rpm_var<engine_on_rpm) {  
      FuelSystemStatus();
    }
    lcd.setCursor(0,0); 
    lcd.print(selmon);
    lcd.print(".");
    lcd.print("FuelSystemStatus:"); 
    lcd.setCursor(0,3);
    lcd.print(fss_val); // если форсунки отключены то 1, если работают то 2

    if (off==222) {         
      lcd.setCursor(0,1); 
      lcd.print(&rxData[0]);
      lcd.setCursor(3,1);
      lcd.print(&rxData[3]);
      lcd.setCursor(6,1);
      lcd.print(&rxData[6]);
    } 

    break;

  case 18:   // экран технологический 1

    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("IMAP:");
    lcd.print(IMAP);
    lcd.setCursor(12,0); 
    lcd.print("VE: ");
    lcd.print(VE);
    lcd.setCursor(0,1); 
    lcd.print("MAF: ");
    lcd.print(MAF);
    lcd.setCursor(0,2); 
    lcd.print("Benzin: ");
    lcd.print(String(benz_potracheno,9));
    lcd.setCursor(0,3); 
    lcd.print("AddMgn: ");
    lcd.print(String(benz_add,9));
    break;

  case 19:  // экран технологический 2

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("time_start: ");
    lcd.print(millis()/1000);
    lcd.setCursor(0,1); 
    lcd.print("LS_kor: ");   
    lcd.print(String(ls_term_val,6)); 
    lcd.setCursor(0,2); 
    lcd.print("FSS: ");   
    lcd.print(fss_val); 
    lcd.setCursor(8,2); 
    lcd.print("Load: ");   
    lcd.print(raschet_nagruzka_dvigatelya_var);    
    lcd.setCursor(0,3); 
    lcd.print("time_loop: ");   
    lcd.print(time);

    break;

  case 20:
    CheckEngineOchistka();
    CheckEngineKm();
    lcd.clear();    
    value=digitalRead(pin); 
    value2=digitalRead(pin2); 

    if ((value==LOW) and (value2==LOW)) {  // проверка состояния кнопки для очистки ошибок
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("  Ochistka oshibok! ");
      lcd.setCursor(0,1);
      lcd.print("--------------------");
      lcd.setCursor(0,2);
      lcd.print(" Gdi 4 secundy dlya ");
      lcd.setCursor(0,3);
      lcd.print("     ochistki..     ");
      delay(4000);
      lcd.clear();
      value=digitalRead(pin);  // проверка состояния кнопки для очистки ошибок
      value2=digitalRead(pin2); 

      if ((value==LOW) and (value2==LOW)) {
        blueToothSerial.flush();
        blueToothSerial.print("04");  // очистка ошибок
        blueToothSerial.print("\r");
        lcd.setCursor(0,1);
        lcd.print("|----[Clear OK]----|");
        delay(2000);
      }  
      else {
        lcd.setCursor(0,1);
        lcd.print("|----[Otmeneno]----|");
        delay(2000);
      }
    }



    lcd.setCursor(0,0); 
    lcd.print("S ochistki oshibok: "); 
    lcd.setCursor(0,1); 
    lcd.print(check_engine_km);
    lcd.print(" km    "); 
    lcd.setCursor(0,2); 
    lcd.print("Check Engine gorit: ");
    lcd.setCursor(0,3); 
    lcd.print(check_engine_flash);
    lcd.print(" km    "); 

    break;

  }

}

void Speed(void) {
  delay(delay_var); 
  blueToothSerial.flush();
  blueToothSerial.print("010D");  // скорость
  blueToothSerial.print("\r");
  getResponse();
  getResponse();  
  if ((strtol(&rxData[0],0,16)==65) and (strtol(&rxData[3],0,16)==13)) {   
    speed_var = strtol(&rxData[6],0,16)*speed_korrect_val;
  } 
  else {
    speed_error++;   // считаем кол-во ошибок чтения
  }
}

void Tmp(void) {
  delay(delay_var);
  blueToothSerial.flush();
  blueToothSerial.print("0105");  // температура
  blueToothSerial.print("\r");
  getResponse();
  getResponse();  
  if ((strtol(&rxData[0],0,16)==65) and (strtol(&rxData[3],0,16)==5)) {   
    tmp_var = strtol(&rxData[6],0,16)-40; 
  }  
  else {
    tmp_error++; // считаем кол-во ошибок чтения
  }
}

void Rpm(void) {  
  delay(delay_var);
  blueToothSerial.flush();
  blueToothSerial.print("010C");  // обороты
  blueToothSerial.print("\r");
  getResponse();
  getResponse();
  if ((strtol(&rxData[0],0,16)==65) and (strtol(&rxData[3],0,16)==12)) {    //запрос приходит от ЭБУ в виде "41 0С XX XX", где XX XX это обороты, а 41 0С это проверка, где 0С это запрос оборотов  
    rpm_var = double((strtol(&rxData[6], 0,16) * 256) + strtol(&rxData[9], 0,16)) / 4.0;  // но мы проверяем в десятичном формате, а запрос в 16чном, итого 41 это 65, а 0С - 12.
    if (rpm_var<399) { 
      rpm_var=0;
    }
  } 
  else {
    rpm_error++; // считаем кол-во ошибок чтения
  }
}  

void DavlenVpuskKoll(void) {
  delay(delay_var);
  blueToothSerial.flush();
  blueToothSerial.print("010B");  // давление впускного коллектора
  blueToothSerial.print("\r");
  getResponse();
  getResponse();  
  if ((strtol(&rxData[0],0,16)==65) and (strtol(&rxData[3],0,16)==11)) {  
    dvk_var = strtol(&rxData[6],0,16); 
  }  
  else {
    dvk_error++; // считаем кол-во ошибок чтения
  }
}

void IntakeAirTemp(void) {
  delay(delay_var);
  blueToothSerial.flush();
  blueToothSerial.print("010F");  // Температура всасываемого воздуха
  blueToothSerial.print("\r");
  getResponse();
  getResponse();  
  if ((strtol(&rxData[0],0,16)==65) and (strtol(&rxData[3],0,16)==15)) {  
    intake_air_temp_var = strtol(&rxData[6],0,16)-40; 
  }  
  else {
    iat_error++; // считаем кол-во ошибок чтения
  }
}

void Voltage(void) {
  delay(delay_var);
  blueToothSerial.flush();
  blueToothSerial.print("ATRV");  // Вольтаж
  blueToothSerial.print("\r");
  getResponse();
  getResponse();
  v1=rxData[0];
  v2=rxData[1];
  v3=rxData[2];
  v4=rxData[3];
}


void Rnd(void) {

  delay(delay_var);
  blueToothSerial.flush();
  blueToothSerial.print("0104");  // Расчетное значение нагрузки на двигатель
  blueToothSerial.print("\r");
  getResponse();
  getResponse();  
  if ((strtol(&rxData[0],0,16)==65) and (strtol(&rxData[3],0,16)==4)) {  
    raschet_nagruzka_dvigatelya_var = double(strtol(&rxData[6],0,16)*100)/255.0; 
  }  
  else {
    rnd_error++;
    ; // считаем кол-во ошибок чтения
  }
}

void Dts(void) {
  delay(delay_var);
  blueToothSerial.flush();
  blueToothSerial.print("010A");  // 	Давление топлива 
  blueToothSerial.print("\r");
  getResponse();
  getResponse();  
  if ((strtol(&rxData[0],0,16)==65) and (strtol(&rxData[3],0,16)==10)) { 
    davlenie_topliva_var = strtol(&rxData[6],0,16)*3; 
  } 
  else {
    dts_error++;
    ; // считаем кол-во ошибок чтения
  }
}

void Uoz(void) { 
  delay(delay_var);
  blueToothSerial.flush();
  blueToothSerial.print("010E");  // Угол опережения зажигания 
  blueToothSerial.print("\r");
  getResponse();
  getResponse();  
  if ((strtol(&rxData[0],0,16)==65) and (strtol(&rxData[3],0,16)==14)) {  
    uoz_var = double(strtol(&rxData[6],0,16)/2.0)-64.0; 
  } 
  else {
    uoz_error++;
    ; // считаем кол-во ошибок чтения
  }
}

void Maf(void) {
  delay(delay_var);
  blueToothSerial.flush();
  blueToothSerial.print("0110");  // Массовый расход воздуха
  blueToothSerial.print("\r");
  getResponse();
  getResponse();  
  if ((strtol(&rxData[0],0,16)==65) and (strtol(&rxData[3],0,16)==16)) {   
    maf_var = double((strtol(&rxData[6], 0,16) * 256) + strtol(&rxData[9], 0,16)) / 100.0;
  } 
  else { 
    maf_error++;
    ; // считаем кол-во ошибок чтения
  }
}

void Pdz(void) {
  delay(delay_var);
  blueToothSerial.flush();
  blueToothSerial.print("0111");  // Положение дроссельной заслонки  
  blueToothSerial.print("\r");
  getResponse();
  getResponse();  
  if ((strtol(&rxData[0],0,16)==65) and (strtol(&rxData[3],0,16)==17)) {    
    polozh_dross_zaslon = double(strtol(&rxData[6],0,16)*100)/255.0; 
  } 
  else {
    pdz_error++;
    ; // считаем кол-во ошибок чтения
  }
}

void Uroventopliva(void) {
  delay(delay_var); 
  blueToothSerial.flush();
  blueToothSerial.print("012F");  //  Уровень топлива 
  blueToothSerial.print("\r");
  getResponse();
  getResponse();  
  if ((strtol(&rxData[0],0,16)==65) and (strtol(&rxData[3],0,16)==47)) {  
    uroven_topliva_var = double(strtol(&rxData[6],0,16)*100)/255.0; 
  } 
  else {
    ut_error++;
    ; // считаем кол-во ошибок чтения
  }
}

void Tempmasla(void) {
  delay(delay_var); 
  blueToothSerial.flush();
  blueToothSerial.print("015C");  // Температура масла двигателя 
  blueToothSerial.print("\r");
  getResponse();
  getResponse();  
  if ((strtol(&rxData[0],0,16)==65) and (strtol(&rxData[3],0,16)==92)) {  
    tmp_masla_var = strtol(&rxData[6],0,16)-40; 
  } 
  else {
    tm_error++;
    ; // считаем кол-во ошибок чтения
  }
}

void CheckEngineKm(void) {
  delay(delay_var); 
  blueToothSerial.flush();
  blueToothSerial.print("0131");  // Км пройдено с очистки кодов
  blueToothSerial.print("\r");
  getResponse();
  getResponse();  
  if ((strtol(&rxData[0],0,16)==65) and (strtol(&rxData[3],0,16)==49)) {
    check_engine_km=(strtol(&rxData[6], 0,16)*256)+strtol(&rxData[9], 0,16);
  }
}

void CheckEngineOchistka(void) {
  delay(delay_var); 
  blueToothSerial.flush();
  blueToothSerial.print("0121");  // Км пройдено с горящей лампой
  blueToothSerial.print("\r");
  getResponse();
  getResponse();  
  if ((strtol(&rxData[0],0,16)==65) and (strtol(&rxData[3],0,16)==33)) {
    check_engine_flash=(strtol(&rxData[6], 0,16)*256) + strtol(&rxData[9], 0,16);
  } 
}

void ShortTerm(void) {

  delay(delay_var); 
  blueToothSerial.flush();
  blueToothSerial.print("0106");  //  	Short term fuel % trim—Bank 1
  blueToothSerial.print("\r");
  getResponse();
  getResponse();  
  if ((strtol(&rxData[0],0,16)==65) and (strtol(&rxData[3],0,16)==6)) {
    short_term_val=double(strtol(&rxData[6], 0,16)-128)*0.78125;
  } 
}

void LongTerm(void) {   
  delay(delay_var); 
  blueToothSerial.flush();
  blueToothSerial.print("0107");  //  	Long term fuel % trim—Bank 1
  blueToothSerial.print("\r");
  getResponse();
  getResponse();  
  if ((strtol(&rxData[0],0,16)==65) and (strtol(&rxData[3],0,16)==7)) {
    long_term_val=double(strtol(&rxData[6], 0,16)-128)*0.78125;
  } 
}

void SearchErrorEngine(void) {   
  delay(delay_var); 
  blueToothSerial.flush();
  blueToothSerial.print("0101");  //  	Проверка ошибок двигателя
  blueToothSerial.print("\r");
  getResponse();
  getResponse();  
  if ((strtol(&rxData[0],0,16)==65) and (strtol(&rxData[3],0,16)==1)) {
    kol_check_engine_error_val=strtol(&rxData[6], 0,16)-1;
  } 
}

void B1S1(void) {   
  delay(delay_var); 
  blueToothSerial.flush();
  blueToothSerial.print("0114");  //  	Проверка кислорода 1
  blueToothSerial.print("\r");
  getResponse();
  getResponse();  
  if ((strtol(&rxData[0],0,16)==65) and (strtol(&rxData[3],0,16)==20)) {
    b1s1_val=double(strtol(&rxData[6], 0,16))/200.0;
  } 
}

void B1S2(void) {   
  delay(delay_var); 
  blueToothSerial.flush();
  blueToothSerial.print("0115");  //  	Проверка кислорода 2
  blueToothSerial.print("\r");
  getResponse();
  getResponse();  
  if ((strtol(&rxData[0],0,16)==65) and (strtol(&rxData[3],0,16)==21)) {
    b1s2_val=double(strtol(&rxData[6], 0,16))/200.0;
  } 
}

void FuelSystemStatus(void) {   
  delay(delay_var); 
  blueToothSerial.flush();
  blueToothSerial.print("0103");  //  	Проверка, если форсунки отключены то 1, если работают то 2
  blueToothSerial.print("\r");
  getResponse();
  getResponse();  
  if ((strtol(&rxData[0],0,16)==65) and (strtol(&rxData[3],0,16)==3)) {
      fss_val = strtol (&rxData[6], 0, 16);
  } 
}



void getResponse(void){
  char inChar=0;
  if (off2)
  {  
    while(inChar != '\r'){
      if(blueToothSerial.available() > 0){
        if(blueToothSerial.peek() == '\r'){
          inChar=blueToothSerial.read();
          rxData[rxIndex]='\0';
          rxIndex=0;
        } 
        else {
          inChar = blueToothSerial.read();
          rxData[rxIndex++]=inChar;
        }
      }
    } 
  } 
}

void EEPROM_float_write(int addr, float val) // запись в ЕЕПРОМ
{  
  byte *x = (byte *)&val;
  for(byte i = 0; i < 4; i++) EEPROM.write(i+addr, x[i]);
}

float EEPROM_float_read(int addr) // чтение из ЕЕПРОМ
{    
  byte x[4];
  for(byte i = 0; i < 4; i++) x[i] = EEPROM.read(i+addr);
  float *y = (float *)&x;
  return y[0];
}