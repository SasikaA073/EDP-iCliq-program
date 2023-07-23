// #include <TFT_eSPI.h>
// #include <SPI.h>
// #include "BluetoothSerial.h"

// // fonts, icons
// #include "dec14.h"
// #include "icons.h"


// String blue_text = ""; //bluetooth message
// int coin = 0;
// int motstat = 0;
// int blstat = 1;

// #if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
// #error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
// #endif

// BluetoothSerial SerialBT;

// TFT_eSPI tft = TFT_eSPI();  // Invoke Library

// #define chgsense 12 // lets know if the watch is charging (connect to CHG_STAT
//                     // from MCP7383

// // Rotary encoder:
// // Interchange ROT_DT and ROT_CL to invert the direction of increment 
// #define ROT_SW 14
// #define ROT_DT 27
// #define ROT_CL 13  

// #define MOT_OUT 19

// TaskHandle_t Task1;
// TaskHandle_t Task2;
// TaskHandle_t Task3;

// void setup() {
//   // put your setup code here, to run once:
//   Serial.begin(115200);
//   SerialBT.begin("MetroBand"); //Bluetooth device name
//   tft.init();
//   tft.fillScreen(TFT_BLACK);
//   tft.setTextColor(TFT_WHITE, TFT_BLACK);

//   pinMode(ROT_SW, INPUT);
//   pinMode(ROT_DT, INPUT);
//   pinMode(ROT_CL, INPUT);
//   pinMode(MOT_OUT, OUTPUT);

//   pinMode(22, OUTPUT);
//   digitalWrite(22,HIGH);
//   wordtrain("- METROBAND -",300);


//   // Tasks
//   xTaskCreatePinnedToCore(
//                     Task1code,   /* Task function. */
//                     "Task1",     /* name of task. */
//                     10000,       /* Stack size of task */
//                     NULL,        /* parameter of the task */
//                     1,           /* priority of the task */
//                     &Task1,      /* Task handle to keep track of created task */
//                     1);          /* pin task to core 1 */                  
//   delay(500);

//   xTaskCreatePinnedToCore(
//                     Task2code,   /* Task function. */
//                     "Task2",     /* name of task. */
//                     10000,       /* Stack size of task */
//                     NULL,        /* parameter of the task */
//                     1,           /* priority of the task */
//                     &Task2,      /* Task handle to keep track of created task */
//                     1);          /* pin task to core 0 */         //(0)
//     delay(500);

//   xTaskCreatePinnedToCore(
//                     Task3code,   /* Task function. */
//                     "Task3",     /* name of task. */
//                     10000,       /* Stack size of task */
//                     NULL,        /* parameter of the task */
//                     1,           /* priority of the task */     //(2)
//                     &Task3,      /* Task handle to keep track of created task */
//                     1);          /* pin task to core 1 */
//     delay(500);
// }


// int lastClk = HIGH;
// int BPM = 100;
// float HIGH_T = 0.1;
// float LOW_T = 0.0;


// //Task1code: Calculating BPM and other tasks
// void Task1code( void * pvParameters ){
//   for(;;){

//     if (SerialBT.available() > 0) {
//       blue_text = SerialBT.readStringUntil('\n');
//       if(blue_text.toInt() != 0){
//         BPM = blue_text.toInt();
//       } else {
//         coin = 1;
//       }
//     }
  
//     int newClk = digitalRead(ROT_CL);

//     if (newClk != lastClk) {
//       // There was a change on the CLK pin
//       lastClk = newClk;
//       int dtValue = digitalRead(ROT_DT);

//       if (newClk == LOW && dtValue == HIGH) {
//         BPM += 1;
//       }
//       if (newClk == LOW && dtValue == LOW) {
//         BPM -= 1;
//       }
//     }
//     LOW_T = 60.0/BPM - HIGH_T;
//   }
// }


// //Task2code: blinks the LED to the given BPM
// void Task2code( void * pvParameters ){

//   for(;;){
//     if(digitalRead(ROT_SW)==LOW || coin == 1) {
//     coin = 0;
//     motstat = 1;
//     for (;;) {
//       digitalWrite(MOT_OUT, HIGH);
//       delay(HIGH_T*1000);
//       digitalWrite(MOT_OUT, LOW);
//       delay(LOW_T*1000);
//       if(digitalRead(ROT_SW)==LOW || coin == 1) {
//         coin = 0;
//         motstat = 0;
//         break;
//       }
//     }
//     delay(1000);
//     }
//   }
// }

// //Task3code: display
// void Task3code(void * pvParameters){
//   for(;;){
//     tft.fillScreen(TFT_BLACK);
//     tft.setTextColor(TFT_WHITE);

//     float voltage  = 2096*3.3/4095;   //analogread(34)
//     int percentage = round(voltage/3.3*100);

//     String BPMtxt;
//     if (BPM>=100){
//       BPMtxt = String(BPM);
//     } else if (BPM>=10){
//       BPMtxt = " " + String(BPM);
//     } else {
//       BPMtxt = "  " + String(BPM);
//     }

//     if (motstat==0) {
//       tft.drawBitmap(6,18,ch_ps,50,13,TFT_CYAN);
//     } else {
//       tft.drawBitmap(6,33,ch_pl,50,13,TFT_YELLOW);
//     }

//     bignum(BPMtxt);

//     tft.setTextColor(TFT_BLACK,TFT_RED);
//     tft.fillRect(78, 108, 42, 20, TFT_RED);
//     tft.fillTriangle(71, 108, 78, 108, 78, 128, TFT_RED);
//     displayPrint("BPM",82, 111, 2); //83,74
//     tft.setTextColor(TFT_WHITE);

//     if (blstat==0){
//       tft.drawBitmap(0,0,ch_btsc,33,15,TFT_CYAN);
//     } else {
//       tft.drawBitmap(0,0,ch_btsc,33,15,TFT_CYAN);
//     }
//     int right = 128;
//     if (digitalRead(chgsense)) {
//       right -= 10;
//       tft.drawBitmap(51,0,ch_chg,12,15,TFT_CYAN);
//     }
//     if (percentage>75) {
//       tft.drawBitmap(98,0,ch_b100,30,15,TFT_CYAN);
//     } else if (percentage>50) {
//       tft.drawBitmap(98,0,ch_b75,30,15,TFT_CYAN);
//     } else if (percentage>25) {
//       tft.drawBitmap(98,0,ch_b50,30,15,TFT_CYAN);
//     } else {
//       tft.drawBitmap(98,0,ch_b25,30,15,TFT_CYAN);
//     }
//     int col = 70;
//     if (percentage==100) {
//       col -= 6;
//     } else if (percentage<10) {
//       col += 6;
//     }
//     tft.setTextColor(TFT_CYAN);
//     displayPrint((String(percentage)+"%"),col, 4, 1);
//     tft.setTextColor(TFT_WHITE);

//     delay(1000); 
//   }
// }

// void loop() {}

// // Dipslays text on TFT
// void displayPrint(String text,int column,int row,int size){
//   tft.setCursor(column, row); // (column, row)
//   tft.setTextSize(size);
//   tft.println(text);
// }

// // Displays on main 14seg 
// void bignum(String bignumtxt){
//   int vfd = 30;
//   tft.drawBitmap( 3,60,bn_ALL,39,42,tft.color565(vfd,vfd,vfd));
//   tft.drawBitmap(42,60,bn_ALL,39,42,tft.color565(vfd,vfd,vfd));
//   tft.drawBitmap(81,60,bn_ALL,39,42,tft.color565(vfd,vfd,vfd));

//   char bigArray[3];
//   bignumtxt.toCharArray(bigArray, bignumtxt.length()+1);

//   charcall(bigArray[0],0);
//   charcall(bigArray[1],1);
//   charcall(bigArray[2],2);
// }

// //Sorts char from libraries
// void charcall(char letter, int pos){
//   int posx;
//   if (pos==0) {
//     posx = 3;
//   } else if (pos==1) {
//     posx = 42;
//   } else if (pos==2) {
//     posx = 81;
//   }

//   int posy = 60;

//   if (letter==48) {
//     tft.drawBitmap(posx,posy,bn_0,39,42,TFT_WHITE); //0
//   } else if (letter==49) {
//     tft.drawBitmap(posx,posy,bn_1,39,42,TFT_WHITE); //1
//   } else if (letter==50) {
//     tft.drawBitmap(posx,posy,bn_2,39,42,TFT_WHITE); //2
//   } else if (letter==51) {
//     tft.drawBitmap(posx,posy,bn_3,39,42,TFT_WHITE); //3
//   } else if (letter==52) {
//     tft.drawBitmap(posx,posy,bn_4,39,42,TFT_WHITE); //4
//   } else if (letter==53) {
//     tft.drawBitmap(posx,posy,bn_5,39,42,TFT_WHITE); //5
//   } else if (letter==54) {
//     tft.drawBitmap(posx,posy,bn_6,39,42,TFT_WHITE); //6
//   } else if (letter==55) {
//     tft.drawBitmap(posx,posy,bn_7,39,42,TFT_WHITE); //7
//   } else if (letter==56) {
//     tft.drawBitmap(posx,posy,bn_8,39,42,TFT_WHITE); //8
//   } else if (letter==57) {
//     tft.drawBitmap(posx,posy,bn_9,39,42,TFT_WHITE); //9
//   } else if (letter==84) {
//     tft.drawBitmap(posx,posy,bn_T,39,42,TFT_WHITE); //T
//   } else if (letter==69) {
//     tft.drawBitmap(posx,posy,bn_E,39,42,TFT_WHITE); //E
//   } else if (letter==66) {
//     tft.drawBitmap(posx,posy,bn_B,39,42,TFT_WHITE); //B
//   } else if (letter==79) {
//     tft.drawBitmap(posx,posy,bn_O,39,42,TFT_WHITE); //O
//   } else if (letter==78) {
//     tft.drawBitmap(posx,posy,bn_N,39,42,TFT_WHITE); //N
//   } else if (letter==45) {
//     tft.drawBitmap(posx,posy,bn__,39,42,TFT_WHITE); //-
//   } else if (letter==65) {
//     tft.drawBitmap(posx,posy,bn_A,39,42,TFT_WHITE); //A
//   } else if (letter==82) {
//     tft.drawBitmap(posx,posy,bn_R,39,42,TFT_WHITE); //R
//   } else if (letter==68) {
//     tft.drawBitmap(posx,posy,bn_D,39,42,TFT_WHITE); //D
//   } else if (letter==67) {
//     tft.drawBitmap(posx,posy,bn_C,39,42,TFT_WHITE); //C
//   } else if (letter==89) {
//     tft.drawBitmap(posx,posy,bn_Y,39,42,TFT_WHITE); //Y
//   } else if (letter==76) {
//     tft.drawBitmap(posx,posy,bn_L,39,42,TFT_WHITE); //L
//   } else if (letter==87) {
//     tft.drawBitmap(posx,posy,bn_W,39,42,TFT_WHITE); //W
//   } else if (letter==77) {
//     tft.drawBitmap(posx,posy,bn_M,39,42,TFT_WHITE); //M
//   } 
// }

// // Wordtrain on the main 14seg
// void wordtrain(String str, int delaytime) {
//   int vfd = 30;

//   char ltgp[str.length()+7];
//   str.toCharArray(ltgp, str.length()+6);
//   for (int i=0; i<str.length(); i++) {
//     ltgp[str.length()+2-i] = ltgp[str.length()-1-i];
//   }

//   ltgp[0] = 32;
//   ltgp[1] = 32;
//   ltgp[2] = 32;
//   ltgp[str.length()+3] = 32;
//   ltgp[str.length()+4] = 32;
//   ltgp[str.length()+5] = 32;

//   for (int i = 0; i<(str.length()+3); i++) {
//     charcall(ltgp[i],0);
//     charcall(ltgp[i+1],1);
//     charcall(ltgp[i+2],2);
//     delay(delaytime);
//     tft.drawBitmap( 3,60,bn_ALL,39,42,tft.color565(vfd,vfd,vfd));
//     tft.drawBitmap(42,60,bn_ALL,39,42,tft.color565(vfd,vfd,vfd));
//     tft.drawBitmap(81,60,bn_ALL,39,42,tft.color565(vfd,vfd,vfd));
//   }

//   delay(1000-delaytime);
// }