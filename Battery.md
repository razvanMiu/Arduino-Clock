# Incarcator de baterie
## Schema
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
