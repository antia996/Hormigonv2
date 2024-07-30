# Hormigonv2
Interacción de neutrones con una columna de hormigón

**DetectorConstruction.cc**
Columna de hormigón de 80 x 110 x 80 cm, situada en el centro del *World volume*

**EventAction.cc**
Rellena los histogramas si las partículas que llegan son neutrones. 
1-> Histograma 1D de la energía depositada
2-> Histograma 1D de la longitud recorrida en el detector
3-> Histograma 1D de la trayectoria efectiva de los neutrones en el hormigón.

**PrimaryGeneratorAction.cc**
Se define la fuente: neutrones en la dirección "+z" con una energía por defecto de 2.5 MeV. Se lanzan todos desde el mismo punto

**RunAction.cc**
Se crean los histogramas y tuplas , que posteriormente se rellenan en *EventAction.cc*

**SteppingAction.cc**
Se recoge el *track* de cada partícula, la posición inicial cuando entra en el volumen de hormigón y *step by step* se va comprobando si la partícula sale del volumen o se detiene y cuando esto ocurre se registra la posición final de la partícula, se resta con respecto a la inicial y se obtiene el rango efectivo, que se utilizará para el histograma 3.
