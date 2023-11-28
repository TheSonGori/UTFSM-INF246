import threading
from datetime import datetime
import time
import random

# Lock para el registro de actividades
registro_lock = threading.Lock()

def registrar_actividad(archivo, mensaje):
    with registro_lock:
        with open(archivo, 'a') as f:
            f.write(f"{mensaje}\n")


class Departamento:
    def __init__(self, nombre, capacidad_fila, capacidad_departamento, duracion_consulta):
        self.nombre = nombre
        self.capacidad_fila = capacidad_fila
        self.capacidad_departamento = capacidad_departamento
        self.duracion_consulta = duracion_consulta
        self.fila = threading.Semaphore(capacidad_fila)
        self.departamento = threading.Semaphore(capacidad_departamento)

    def procesar_consulta(self, id_persona):
        time.sleep(self.duracion_consulta)



class Persona(threading.Thread):
    def __init__(self, id, departamentos):
        threading.Thread.__init__(self)
        self.id = id
        self.departamentos = departamentos

    def run(self):
        # Registro de ingreso a la universidad
        hora_actual = datetime.now()
        hora_ingreso_universidad = hora_actual.strftime("%H:%M:%S.%f")[:-3]
        departamentos_a_visitar = random.sample(self.departamentos, 2)
        registro_universidad = f"Persona{self.id}, {hora_ingreso_universidad}"

        for i, dep in enumerate(departamentos_a_visitar, start=1):

            with dep.fila:
                # Ha entrado a la fila
                hora_actual = datetime.now()
                hora_ingreso_fila = hora_actual.strftime("%H:%M:%S.%f")[:-3]
                registro_universidad += f", {dep.nombre}, {hora_ingreso_fila}"

                with dep.departamento:
                    # Entrada al departamento
                    hora_actual = datetime.now()
                    hora_entrada_dep = hora_actual.strftime("%H:%M:%S.%f")[:-3]
                    registro_departamento = f"Persona{self.id}, {hora_ingreso_fila}, {hora_entrada_dep}, {i}"
                    if(dep.nombre == "DEFIDER"):
                        registrar_actividad(f"{dep.nombre}.txt", registro_departamento)
                    else:
                        registrar_actividad(f"Departamento_de_{dep.nombre}.txt", registro_departamento)
                    

                    # Procesar consulta
                    dep.procesar_consulta(self.id)

        # Registrar en PdLamparas.txt
        registrar_actividad("PdLamparas.txt", registro_universidad)

        # Registro de salida de la universidad
        hora_actual = datetime.now()
        hora_salida_universidad = hora_actual.strftime("%H:%M:%S.%f")[:-3]
        registrar_actividad("Salida.txt", f"Persona{self.id}, {hora_salida_universidad}")




departamentos = [
    Departamento("Matematicas", 20, 10, 9),
    Departamento("Fisica", 15, 5, 7),
    Departamento("Informatica", 8, 2, 5),
    Departamento("Quimica", 6, 3, 4),
    Departamento("DEFIDER", 6, 5, 1),
    Departamento("Mecanica", 9, 4 , 4),
    Departamento("Minas", 7, 2, 5)
]

personas = [Persona(i, departamentos) for i in range(500)] 

for persona in personas:
    persona.start()

for persona in personas:
    persona.join()

print("Simulación completada")