/*PM01Lite v0.1.0 Firmware*/

unsigned int duty = 130,offsetmin = 50, offsetmax = 80;
unsigned int left_motor_pin = 3, right_motor = 11;
unsigned int left_Tracer1 = 4, left_Tracer2 = 5, right_Tracer1 = 6, right_Tracer2 = 7;
char buffer[128];

/*********循迹探头分布***********
***O—————O—————O—————O—————O****
*  ↑     ↑     ↑     ↑     ↑   *
* 左二   左一  停用   右一  右二 *
-------------------------------*/


// 假设白色为高电平

void setup() {
  // 把你的启动代码放在这里，它们将在启动时运行一次:

  // 初始化PWM
  pinMode(left_motor_pin, OUTPUT);
  pinMode(right_motor, OUTPUT);
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21) | _BV(CS20);
  OCR2A = duty;
  OCR2B = duty;

  // 初始化四路循迹
  pinMode(left_Tracer1, INPUT);pinMode(left_Tracer2, INPUT);
  pinMode(right_Tracer1, INPUT);pinMode(right_Tracer2, INPUT);

  // 初始化串口
  Serial.begin(115200);
  sprintf(buffer, "AutoCar Start.PWM Frequency 2kHz,Duty %d %%", (unsigned int)((duty / 255.0 )* 100.0));
  Serial.println(buffer);

}

//Arduino Delay us



void loop() {
  // 把你的主程序代码放在这里，它们将循环持续运行
  if (digitalRead(left_Tracer1) && !digitalRead(right_Tracer1) && (digitalRead(left_Tracer2)) && (digitalRead(right_Tracer2))) {OCR2B = duty;OCR2A = duty-offsetmin;} // 右偏小幅左转
  if (!digitalRead(left_Tracer1) && digitalRead(right_Tracer1) && (digitalRead(left_Tracer2)) && (digitalRead(right_Tracer2))) {OCR2A = duty;OCR2B = duty-offsetmin;} // 左偏小幅右转
  if (digitalRead(left_Tracer2) && !digitalRead(right_Tracer2) && (digitalRead(left_Tracer1)) && (digitalRead(right_Tracer1))) {OCR2B = duty+offsetmax;OCR2A = duty-offsetmax;} // 右偏大幅左转
  if (!digitalRead(left_Tracer2) && digitalRead(right_Tracer2) && (digitalRead(left_Tracer1)) && (digitalRead(right_Tracer1))) {OCR2A = duty+offsetmax;OCR2B = duty-offsetmax;} // 左偏大幅右转
  if (!digitalRead(left_Tracer1) && !digitalRead(right_Tracer1) && (digitalRead(left_Tracer2)) && (digitalRead(right_Tracer2))) {OCR2B = duty;OCR2A = duty;} //直行
  // if (!digitalRead(left_Tracer1) && !digitalRead(right_Tracer1) && !(digitalRead(left_Tracer2)) && !(digitalRead(right_Tracer2))) {OCR2B = 0;OCR2A = 0;} //停车
  
   delay(1); // 延时
}
