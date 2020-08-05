#ifndef DHT11_H
#define DHT11_H

class DHT11{
  public:
  DHT11(char pin);
  void start();
  void get_responce();
  char read();

  private:
  char data_pin;
};

#endif
