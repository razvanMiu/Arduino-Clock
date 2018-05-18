# Incarcator de baterie
## Schema incarcare
![alt text](https://github.com/razvanMiu/Arduino-Clock/blob/master/charger.png)
## Materiale folosite
* Baterie NiMH R6 AA (1.2V)
* Rezistenta putere 10 Ohm (min 5 Watts) 
* Condensator 1uF
* Tranzistor MOSFET IRF510
## Notiuni teoretice
### Divizor de tensiune
![alt text](https://github.com/razvanMiu/Arduino-Clock/blob/master/divizor.png)
```
V = (R2/(R1 + R2)) * E
```
### Incarcarea bateriei
Am folosit o rata de incarcare egala cu C / 10, unde C este capacitatea bateriei.

Circuitul este alimentat la 5V, incarcand bateria printr-o rezistenta de putere de 10 Ohm si un tranzistor MOSFET. Tranzistorul seteaza cat curent este permis sa circule in baterie. Curentul este masurat prin conectarea capetelor rezistentei la pinii analog A1 si A2 si masurant voltajul pentru fiecare capat.

Tranzistorul MOSFET este controlat prin pinul digital D9 (PWM). Pulsatiile semnalului pinului D9 sunt mai apoi filtrate intr-un semnal de tensiune stabil printr-o rezistenta de 1 MOhm si un condensator de 1 uF.

# Descarcare baterie
## Schema incarcare
![alt text](https://github.com/razvanMiu/Arduino-Clock/blob/master/discharger.png)
## Materiale folosite
* Rezistente 2 x 5K1 Ohm
* Rezistenta de putere 10 Ohm
* Tranzistor MOSFET IRLZ44N
* OLED
## Notiuni teoretice
### Descarcarea bateriei
Arduino verifica conditia bateriei, daca bateria este in regula, da comanda de activare a tranzistorului MOSFET. Acesta permite trecerea curentului de la borna pozitiva a bateriei, prin rezistenta si MOSFET si apoi completeaza circuitul inapoi la borna negativa a bateriei. Astfel bateria se descarca intr-o perioada de timp. Arduino masoara tensiunea pe rezistenta printr-un divizor de tensiune calculand mai apoi curentul de descarcare.
```
I = (V - Vm) / R
I => curentul de descarcare
V => tensiunea pe rezistenta de putere
R => 10 Ohm
```
Pentru masurarea capacitatii:
```
Q = I * T
Q => capacitate
T => timp
```

### Demo discharger
![alt text](https://github.com/razvanMiu/Arduino-Clock/blob/master/battary_discharge.png)
