#include <TroykaTextLCD.h>
#include <AmperkaKB.h>

// Пищалка
int buzzPin = 10;

// Светодиоды
int redLed = 12;
int greenLed = 13;
int blueLed = 11;

// Конфигурация дисплея
TroykaTextLCD lcd;
const int numRows = 2;
const int numCols = 16;
int thisCol = 0;
String code = "4591#";
String number = "";

// настройка кейпада
AmperkaKB KB(9, 8, 7, 6, 5, 4, 3, 2);

// функция очистки отдельной строки
void clearLine(int line) {
  lcd.setCursor(0, line);
  for(int n = 0; n < numCols; n++) {
    lcd.print(" ");
  }
}

// функция звукового оповещения для неправильного кода
void wrongSound() {
  tone(buzzPin, 4500, 1000);
}

// функция оповещения для правильного пароля
void correctSound() {
  tone(buzzPin, 3500, 80);
  delay(200);
  tone(buzzPin, 3500, 80);
  delay(300);
  tone(buzzPin, 4500, 100);
}

void setup() {
  Serial.begin(9600);
  
  //* * *  ЭКРАН * * *//
  // устанавливаем количество столбцов и строк экрана
  lcd.begin(numCols, numRows);
  lcd.setContrast(20); // контраст
  lcd.setBrightness(255); // яркость
  lcd.setCursor(0, 0); // курсор в колонку 0, строку 0
  lcd.print("PASSWORD:");
  //* * * --- * * *//

  //* * * ПИЩАЛКА * * *//
  pinMode(buzzPin, OUTPUT);
  //* * * --- * * *//

  //* * * КЛАВИАТУРА * * *//
  KB.begin(KB4x4);
  //* * * --- * * *//

  //* * * ЛАМПОЧКИ * * *//
  pinMode(redLed, OUTPUT);
  digitalWrite(redLed, LOW);

  pinMode(greenLed, OUTPUT);
  digitalWrite(greenLed, LOW);

  pinMode(blueLed, OUTPUT);
  digitalWrite(blueLed, HIGH);
  //* * * --- * * *//
}
 
void loop() {
  KB.read(); // отслеживаем все нажатия кнопок 
  if (KB.justPressed()){
    tone(buzzPin, 2500, 80);  // каждое нажатие - писк баззера
    lcd.setCursor(thisCol, 1); // установка курсора в начало второй строки
    char num = KB.getChar; // записываем в буфер нажатую клавишу
    number = number + num; // содержимое буфера записываем в строку
    lcd.print(num); // печатаем содержимое буфера 
    thisCol += 1; // сдвигаем курсор на одну позицию

    // обработка нажатия клавиши "*" - очистка поля ввода
    if (num == '*') {
      thisCol = 0; // сдвигаем курсор в начало строки
      number = ""; // очищаем строку
      clearLine(1); // очищаем дисплей
      digitalWrite(greenLed, LOW); // выключаем зеленый светодиод, если он включен
    }

    // обработка нажатия клавиши "#" - ввод
    if (num == '#') {
      // если введен правильный пароль
      if (number == code) {
        digitalWrite(greenLed, HIGH); // включаем зеленый светодиод
        correctSound(); // воспроизводим звук правильного ввода
        clearLine(1); // очищаем дисплей
        lcd.setCursor(0, 1); // переводим курсор в начало строки 
        lcd.print("That's it!"); // печатаем сообщение
        digitalWrite(greenLed, LOW); // выключаем зеленый светодиод
      } else { // если пароль не подошел
        clearLine(1);
        lcd.setCursor(0, 1); // переводим курсор в начало строки
        lcd.print("Try again."); // печатаем сообщение
        wrongSound(); // воспроизводим звук
        // включаем и выключаем светодиод
        digitalWrite(redLed, HIGH);
        delay(2000);
        digitalWrite(redLed, LOW);
        // очищаем строку
        clearLine(1);
        // сдвигаем счетчик строки в начало 
        thisCol = 0;
        // очищаем переменную
        number = "";
      }
    }
  } 
}
