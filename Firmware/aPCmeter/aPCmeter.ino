/*!
 * @file aPCmeter.ino
 * @brief aPCmeter Project, source code of Arduino firmware.
 * @details This single file contains all source code of aPCmeter Arduino Nano firmware. It is tested by using Arduino IDE v1.6.7 on Windows 7 x64.
 * @author Alper Yazar
 * @version s1
 * @date 2016-05-08
 * @copyright CC BY-NC-SA 4.0 (http://creativecommons.org/licenses/by-nc-sa/4.0/)
 * @bug Visit: https://github.com/alperyazar/aPCmeter/issues to check existing bugs or to report a new one.
 * @warning NO WARRANTY! AS IS!
 * @see The Official Project Page: http://www.alperyazar.com/r/aPCmeter
 */

 /*!
  * @brief Determines the maximum trials between two consecutive command reception in active state before reset.
  * @details When aPCmeter is in active state, it waits commands from PC. PC software should send commands with period less than #_UART_TIMEOUT_ms to avoid time out. If no valid message is received within #_UART_TIMEOUT_ms msec after the last successfully received command, a timeout counter is incremented by one. If value of this counter exceeds #_NO_CMD_TIMEOUT, aPCmeter resets itself.
  * @see loop()
  * @verbatim Maximum duration between two successful message before reset in msec =  
(#_NO_CMD_TIMEOUT + 1) x #_UART_TIMEOUT_ms @endverbatim
  * @warning It should be a non-negative integer.
  */
#define _NO_CMD_TIMEOUT 29

/*!
 * @brief Determines the duration of wait state after reset.
 * @details After reset, aPCmeter sends "aPCmeter" message periodically determined by #_UART_TIMEOUT_ms. After each message it waits a valid message from PC for the first connection. At each time "aPCmeter" message is sent, a pseudo random color is selected for both CPU and RAM gauges and both gauges are illuminated with the selected color. This is used to indiciate that aPCmeter is looking for a PC connection and to provide pseudo random test pattern sequence for all three channels (R, G, B) of both LEDs. If the number of trials exceeds the threshold value determined by #_NO_PC_TIMEOUT, aPCmeter enters to the silent wait state. In that state, G and B LEDs of both gauges become off. Only brightness of R LEDs of both gauges is changed in a pseudo random manner. This is done to avoid disturbing color changes when all R, G, B LEDs are illuminated pseudo randomly. This may be the case when aPCmeter is left connected to the PC or another USB host device. However brightness of red LEDs continues to change and aPCmeter looks a PC connection for ever. One may still be able to notice that aPCmeter is active since brightness of red LEDs is changed periodically.
 * @note Duration of wait state can be calculated as follow
 * @verbatim Duration of wait state in msec =  (#_NO_PC_TIMEOUT + 1) x #_UART_TIMEOUT_ms @endverbatim
 * @warning It should be a non-negative integer.
 * @see wait_for_connection()
 */
#define _NO_PC_TIMEOUT 29

/*!
 * @brief Receive timeout value in msec for serial channel receive function.
 * @details aPCmeter waits at most #_UART_TIMEOUT_ms msec when serial read functions are called. If no message is received from PC in #_UART_TIMEOUT_ms msec, serial receive functions are timed out and code continues. Actually this is basic "tic" source for this code.
 * @warning It should be a non-negative integer.
 * @see setup()
 */
#define _UART_TIMEOUT_ms 2000

/*!
 * @brief Baud rate for serial communication in bps
 * @see setup()
 */
#define _UART_BAUD_RATE 9600

/*!
 * @brief Pin number of red LED of CPU gauge
 * @see http://playground.arduino.cc/Learning/Pins
 */
const int redPinCPU = 3;

/*!
 * @brief Pin number of green LED of CPU gauge
 * @see http://playground.arduino.cc/Learning/Pins
 */
const int greenPinCPU = 5;

/*!
 * @brief Pin number of blue LED of CPU gauge
 * @see http://playground.arduino.cc/Learning/Pins
 */
const int bluePinCPU = 2;

/*!
 * @brief Pin number of CPU gauge
 * @see http://playground.arduino.cc/Learning/Pins
 */
const int gaugePinCPU = 6;

/*!
 * @brief Pin number of red LED of RAM gauge
 * @see http://playground.arduino.cc/Learning/Pins
 */
const int redPinRAM = 9;

/*!
 * @brief Pin number of green LED of RAM gauge
 * @see http://playground.arduino.cc/Learning/Pins
 */
const int greenPinRAM = 10;

/*!
 * @brief Pin number of blue LED of RAM gauge
 * @see http://playground.arduino.cc/Learning/Pins
 */
const int bluePinRAM = 19;

/*!
 * @brief Pin number of RAM gauge
 * @see http://playground.arduino.cc/Learning/Pins
 */
const int gaugePinRAM = 11;

/*!
 * @brief Receive buffer for serial channel
 * @details More than enough actually...
 */
char RX_buf[64];

/*!
 * @brief Tricky reset function. Just set PC to 0.
 */
void(* resetFunc) (void) = 0;

/*!
 * @brief This function is called after startup. Like a well known main() function in C.
 * @see https://www.arduino.cc/en/Reference/Setup
 */
void setup()
{
  pinMode(gaugePinCPU, INPUT);
  pinMode(gaugePinRAM, INPUT);

  digitalWrite(redPinCPU, HIGH);
  digitalWrite(greenPinCPU, HIGH);
  digitalWrite(bluePinCPU, HIGH);

  digitalWrite(redPinRAM, HIGH);
  digitalWrite(greenPinRAM, HIGH);
  digitalWrite(bluePinRAM, HIGH);

  pinMode(redPinCPU, OUTPUT);
  pinMode(greenPinCPU, OUTPUT);
  pinMode(bluePinCPU, OUTPUT);

  pinMode(redPinRAM, OUTPUT);
  pinMode(greenPinRAM, OUTPUT);
  pinMode(bluePinRAM, OUTPUT);

  //All LEDs are off. Gauges are off.
  
  // Set properties of Serial's.
  Serial.setTimeout(_UART_TIMEOUT_ms);
  Serial.begin(_UART_BAUD_RATE);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  wait_for_connection();
}

/*!
 * @brief Wait proper connection initialization from PC
 */
void wait_for_connection()
{
  int timeout = 0;

  // Try to randomize pseudo-random sequences used by random() funtion.
  randomSeed(analogRead(0));
  while (1)
  {
    Serial.println("aPCmeter");
    if ( Serial.readBytesUntil('E', RX_buf, 64) == 0)
    {
      analogWrite(redPinCPU, random(256));
      analogWrite(redPinRAM, random(256));
      analogWrite(greenPinRAM, random(256));
      analogWrite(greenPinCPU, random(256));
      digitalWrite(bluePinCPU, random(2));
      digitalWrite(bluePinRAM, random(2));
      timeout++;
      if (timeout > _NO_PC_TIMEOUT)
      {
        timeout--;
        digitalWrite(greenPinCPU, HIGH);
        digitalWrite(bluePinCPU, HIGH);
        digitalWrite(greenPinRAM, HIGH);
        digitalWrite(bluePinRAM, HIGH);
        analogWrite(redPinRAM, 128 + random(128));
        analogWrite(redPinCPU, 128 + random(128));
      }
    }
    else
    {
      if ((RX_buf[0] == 'S') && (RX_buf[1] == 's'))
      {
        timeout = 0;
        digitalWrite(redPinCPU, HIGH);
        digitalWrite(redPinRAM, HIGH);
        digitalWrite(bluePinCPU, HIGH);
        digitalWrite(bluePinRAM, HIGH);
        digitalWrite(greenPinCPU, LOW);
        digitalWrite(greenPinRAM, LOW);
		// Both gauges are fully green.
        Serial.println("OK");
        return;
      }
      memset(RX_buf, ' ', 64); //Clear all buffer
    }
  }
}

/*!
 * @brief This function is automatically after setup(). Loops forever...
 * @see https://www.arduino.cc/en/Reference/Loop
 */
void loop()
{
  int timeout_ctr = 0;
  while (1)
  {
    if ( Serial.readBytesUntil('E', RX_buf, 64) == 0)
    {
      timeout_ctr++;
      if (timeout_ctr > _NO_CMD_TIMEOUT)
      {
        resetFunc();
      }
    }
    else
    {
      switch (RX_buf[0])
      {
        case 'P':
          if (RX_buf[19] == 'p')
          {
            analogWrite(redPinCPU, str2duty(&(RX_buf[1])));
            analogWrite(greenPinCPU, str2duty(&(RX_buf[4])));
            analogWrite(gaugePinCPU, str2duty(&(RX_buf[7])));
            analogWrite(redPinRAM, str2duty(&(RX_buf[10])));
            analogWrite(greenPinRAM, str2duty(&(RX_buf[13])));
            analogWrite(gaugePinRAM, str2duty(&(RX_buf[16])));
            Serial.println("OK");
            timeout_ctr = 0;
          }
          break;
        case 'T':
          if (RX_buf[1] == 't')
          {
            self_test();
            Serial.println("OK");
            timeout_ctr = 0;
          }
          break;
        case 'B':
          if (RX_buf[3] == 'b')
          {
            if (RX_buf[1] == '1')
            {
              digitalWrite(bluePinCPU, LOW);
            }
            if (RX_buf[1] == '0')
            {
              digitalWrite(bluePinCPU, HIGH);
            }
            if (RX_buf[2] == '1')
            {
              digitalWrite(bluePinRAM, LOW);
            }
            if (RX_buf[2] == '0')
            {
              digitalWrite(bluePinRAM, HIGH);
            }
            Serial.println("OK");
            timeout_ctr = 0;
          }
          break;
        case 'R':
          if (RX_buf[1] == 'r')
          {
            Serial.println("OK");
            timeout_ctr = 0;
            Serial.flush();
            resetFunc();
          }
          break;
		case 'K':
		  if (RX_buf[1] == 'k')
          {
            Serial.println("OK");
            timeout_ctr = 0;
          }
          break;
        default:
			timeout_ctr++;
          break;
      }
      memset(RX_buf, ' ', 64);
    }

  }
}

/*!
 * @brief Converted string to integer. (Ex: "105" -> 105)
 * @details PWM information for LEDs and gauges are sent as a text formatted as 3 digit decimal number like "000", "104", "205". This function is used to 3 digit decimal number to its corresponding integere value.
 * @param[in] str It is a pointer to the string that is converted to integer. It should be 3 digit unsigned decimal value. Null termination doesn't affect the behaviour.
 * @retval Integer value is returned. Value is truncated in between 0 and 255.
 */
 
int str2duty(const char *str)
{
  int temp = 0;

  temp = 100 * (str[0] - 0x30);
  temp += 10 * (str[1] - 0x30);
  temp += 1 * (str[2] - 0x30);

  return (constrain(temp, 0, 255));

}

/*!
 * @brief Set duty cycle for red, green LED and gauge for a channel
 * @details It is used by BIST (built-in self-test)
 * @param[in] channel 0 means CPU gauge, 1 means RAM gauge
 * @param[in] duty It should be between 0 and 255 (including both). Otherwise, it will be truncated into this range. It is the PWM value for analogWrite() function. 0 corresponds to 0% for gauge, 100% for green and 0% for red.
 */
void set_duty(int channel, int duty)
{
  // channel 0-> CPU, 1-> RAM

  duty = constrain(duty, 0, 255);
  if (channel == 0)
  {
    analogWrite(gaugePinCPU, duty);
    analogWrite(greenPinCPU, duty);
    analogWrite(redPinCPU, 255 - duty);
  }
  else if (channel == 1)
  {
    analogWrite(gaugePinRAM, duty);
    analogWrite(greenPinRAM, duty);
    analogWrite(redPinRAM, 255 - duty);
  }
  else
  {
  }

}

/*!
 * @brief BIST (built-in self-test)
 * @details Sweep all LEDs and gauges to make sure that everything is working well.
 */
void self_test()
{
  int duty = 0;
  pinMode(gaugePinCPU, INPUT);
  pinMode(gaugePinRAM, INPUT);
  delay(3000); //Gauges will relax in 3 seconds.
  //Just sweep
  for (duty = 0; duty < 255; duty++)
  {
    analogWrite(gaugePinCPU, duty);
    analogWrite(gaugePinRAM, duty);
    if ( (0 <= duty) && (duty < 85))
    {
      digitalWrite(redPinCPU, HIGH);
      digitalWrite(greenPinCPU, HIGH);
      digitalWrite(bluePinCPU, LOW);

      digitalWrite(redPinRAM, HIGH);
      digitalWrite(greenPinRAM, HIGH);
      digitalWrite(bluePinRAM, LOW);
    }
    if ( (85 <= duty) && (duty < 170))
    {
      digitalWrite(redPinCPU, HIGH);
      digitalWrite(bluePinCPU, HIGH);
      digitalWrite(greenPinCPU, LOW);

      digitalWrite(redPinRAM, HIGH);
      digitalWrite(bluePinRAM, HIGH);
      digitalWrite(greenPinRAM, LOW);

    }
    if ( (170 <= duty) && (duty < 255))
    {
      digitalWrite(greenPinCPU, HIGH);
      digitalWrite(bluePinCPU, HIGH);
      digitalWrite(redPinCPU, LOW);

      digitalWrite(bluePinRAM, HIGH);
      digitalWrite(greenPinRAM, HIGH);
      digitalWrite(redPinRAM, LOW);

    }
    delay(30);
  }
  for (duty = 255; duty > 0; duty--)
  {
    set_duty(0, duty);
    set_duty(1, duty);
    delay(10);
  }
  pinMode(gaugePinCPU, INPUT);
  pinMode(gaugePinRAM, INPUT);
  delay(1000);
}

